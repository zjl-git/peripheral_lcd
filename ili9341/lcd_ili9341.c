#include "lcd_ili9341.h"
#include "lcd_ili9341_utils.h"
#include "lcd_ili9341_port.h"
#include "lcd_ili9341_config.h"

#include <stdio.h>

static unsigned short g_back_color = LCD_WHITE, g_point_color = LCD_BLACK;

static void lcd_ili9341_write(unsigned char data, unsigned char mode)
{
    lcd_ili9341_port_write_data(data, mode);
}

static void lcd_ili9341_set_address(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{
    lcd_ili9341_write(0x2a, LCD_WRITE_CMD);
    lcd_ili9341_write(x1>>8, LCD_WRITE_DAT);
    lcd_ili9341_write(x1, LCD_WRITE_DAT);
    lcd_ili9341_write(x2>>8, LCD_WRITE_DAT);
    lcd_ili9341_write(x2, LCD_WRITE_DAT);

    lcd_ili9341_write(0x2b, LCD_WRITE_CMD);
    lcd_ili9341_write(y1>>8, LCD_WRITE_DAT);
    lcd_ili9341_write(y1, LCD_WRITE_DAT);
    lcd_ili9341_write(y2>>8, LCD_WRITE_DAT);
    lcd_ili9341_write(y2, LCD_WRITE_DAT);

    lcd_ili9341_write(0x2C, LCD_WRITE_CMD);
}

static void lcd_ili9341_fill(unsigned short xsta, unsigned short ysta, unsigned short xend, unsigned short yend, unsigned short color)
{
    unsigned short i, j;

    lcd_ili9341_set_address(xsta, ysta, xend, yend);
    for(i = ysta; i <= yend; i++) {													   	 	
        for(j = xsta; j <= xend; j++) {
            lcd_ili9341_write(color >> 8, LCD_WRITE_DAT);
            lcd_ili9341_write(color, LCD_WRITE_DAT);
        } 
    } 	
}

void lcd_ili9341_clear(unsigned short color) 
{
    unsigned short i, j;

    lcd_ili9341_set_address(0, 0, LCD_W - 1, LCD_H - 1);
    for (i = 0; i < LCD_W; i++) {
        for (j = 0; j < LCD_H; j++) {
            lcd_ili9341_write(color >> 8, LCD_WRITE_DAT);
            lcd_ili9341_write(color, LCD_WRITE_DAT);
        }
    }
    
}

void lcd_ili9341_draw_point(unsigned int x,unsigned int y)
{
    lcd_ili9341_set_address(x, y, x, y);
    lcd_ili9341_write(g_point_color >> 8, LCD_WRITE_DAT);
    lcd_ili9341_write(g_point_color, LCD_WRITE_DAT);
}

void lcd_ili9341_draw_big_point(unsigned int x,unsigned int y)
{
    lcd_ili9341_fill(x -1, y - 1, x + 1, y + 1, g_point_color);
}

void lcd_ili9341_draw_line(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{
    unsigned short t;
    int xerr = 0, yerr = 0;
    int delta_x = x2 - x1, delta_y = y2 - y1;
    int distance;
    int incx, incy;
    int urow = x1, ucol = y1;

    if (delta_x > 0) {
        incx = 1;
    } else if (delta_x == 0) {
        incx = 0;
    } else {
        incx = -1;
        delta_x = -delta_x;
    }

    if (delta_y > 0) {
        incy = 1;
    } else if (delta_y == 0) {
        incy = 0;
    } else {
        incy = -1;
        delta_y = -delta_y;
    }

    if (delta_x > delta_y) {
        distance = delta_x;
    } else {
        distance = delta_y;
    }

    for (t = 0; t <= distance + 1; t++) {
        lcd_ili9341_draw_point(urow, ucol);
        xerr += delta_x;
        if (xerr > distance) {
            xerr -= distance;
            urow += incx;
        }

        yerr += delta_y;
        if (yerr > distance) {
            yerr -= distance;
            ucol += incy;
        }
    }
}

void lcd_ili9341_draw_rectangle(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{
    lcd_ili9341_draw_line(x1, y1, x2, y1);
    lcd_ili9341_draw_line(x1, y1, x1, y2);
    lcd_ili9341_draw_line(x1, y2, x2, y2);
    lcd_ili9341_draw_line(x2, y1, x2, y2);
}

void lcd_ili9341_draw_circle(unsigned int x0,unsigned int y0,unsigned int r)
{
    int a = 0, b = r;
    int di = 3 - (r << 1);

    while (a <= b) {
        lcd_ili9341_draw_point(x0-b,y0-a);             /*3*/        
        lcd_ili9341_draw_point(x0+b,y0-a);             /*0*/         
        lcd_ili9341_draw_point(x0-a,y0+b);             /*1*/     
        lcd_ili9341_draw_point(x0-b,y0-a);             /*7*/         
        lcd_ili9341_draw_point(x0-a,y0-b);             /*2*/            
        lcd_ili9341_draw_point(x0+b,y0+a);             /*4*/             
        lcd_ili9341_draw_point(x0+a,y0-b);             /*5*/
        lcd_ili9341_draw_point(x0+a,y0+b);             /*6*/
        lcd_ili9341_draw_point(x0-b,y0+a);

        a += 1;
        /*bresenham*/
        if (di < 0) {
            di += 4 * a + 6;	  
        } else {
            di += 10 + 4 * (a - b); 
        }
        lcd_ili9341_draw_point(x0 + a, y0 + b);
    }
}

/*mode: superposition(1) or not superposition(0)*/
void lcd_ili9341_show_char(unsigned short x, unsigned short y, unsigned char num, unsigned char mode)
{
    unsigned char temp;
    unsigned char pos, t;
    unsigned short x0 = x;
    unsigned short colortemp = g_point_color;

    if (x > LCD_W - 16 || y > LCD_H - 16) {
        return ;
    }

    num = num - ' ';
    lcd_ili9341_set_address(x, y, x + 8 - 1, y + 16 - 1);
    if (!mode) {
        for (pos = 0; pos < 16; pos++) {
            temp = asc2_1608[num * 16 + pos];
            for (t = 0; t < 8; t ++) {
                if (temp & 0x01) {
                    g_point_color = colortemp;
                } else {
                    g_point_color = g_back_color;
                }
                lcd_ili9341_write(g_point_color, LCD_WRITE_DAT);
                temp >>= 1;
                x++;
            }
            x = x0;
            y++;
        }
    } else {
        for (pos = 0; pos < 16; pos++) {
            temp = asc2_1608[num * 16 + pos];
            for (t = 0; t < 8; t ++) {
                if (temp & 0x01) {
                    lcd_ili9341_draw_point(x + t, y + pos);
                }
                temp >>= 1;
            }
        }
    }
    g_point_color = colortemp;
}

void lcd_ili9341_show_string(unsigned short x, unsigned short y, char *chr)
{
    while (*chr != '\0') {
        if (x > LCD_W - 16) {
            x = 0;
            y += 16;
        }

        if (y > LCD_H - 16) {
            y = x = 0;
            lcd_ili9341_clear(LCD_RED);
        }

        lcd_ili9341_show_char(x, y, *chr, 0);
        x += 8;
        chr++;
    }
}

void lcd_ili9341_show_hex(unsigned char x, unsigned char y, unsigned char *buf, unsigned char len)
{
    unsigned int i = 0, j = 0;
    char *str = lcd_ili9341_malloc((len * 3) + 1);
    if (str == NULL) {
        return ;
    }
    
    while (i < len) {
        if ((i + 1) < len) {
            sprintf(str + j, "%02X ", buf[i]);
            j += 3;
        } else {
            sprintf(str + j, "%02X", buf[i]);
        }
        i += 1;
    }
    lcd_ili9341_show_string(x, y, str);
    lcd_ili9341_free(str);
}



void lcd_ili9341_init(void)
{
    lcd_ili9341_port_init();

    lcd_ili9341_write(0xCF, LCD_WRITE_CMD);  
    lcd_ili9341_write(0x00, LCD_WRITE_DAT); 
    lcd_ili9341_write(0xD9, LCD_WRITE_DAT); 
    lcd_ili9341_write(0X30, LCD_WRITE_DAT); 
 
    lcd_ili9341_write(0xED, LCD_WRITE_CMD);  
    lcd_ili9341_write(0x64, LCD_WRITE_DAT); 
    lcd_ili9341_write(0x03, LCD_WRITE_DAT); 
    lcd_ili9341_write(0X12, LCD_WRITE_DAT); 
    lcd_ili9341_write(0X81, LCD_WRITE_DAT); 
 
    lcd_ili9341_write(0xE8, LCD_WRITE_CMD);  
    lcd_ili9341_write(0x85, LCD_WRITE_DAT); 
    lcd_ili9341_write(0x10, LCD_WRITE_DAT); 
    lcd_ili9341_write(0x78, LCD_WRITE_DAT); 
 
    lcd_ili9341_write(0xCB, LCD_WRITE_CMD);  
    lcd_ili9341_write(0x39, LCD_WRITE_DAT); 
    lcd_ili9341_write(0x2C, LCD_WRITE_DAT); 
    lcd_ili9341_write(0x00, LCD_WRITE_DAT); 
    lcd_ili9341_write(0x34, LCD_WRITE_DAT); 
    lcd_ili9341_write(0x02, LCD_WRITE_DAT); 
 
    lcd_ili9341_write(0xF7, LCD_WRITE_CMD);  
    lcd_ili9341_write(0x20, LCD_WRITE_DAT); 
 
    lcd_ili9341_write(0xEA, LCD_WRITE_CMD);  
    lcd_ili9341_write(0x00, LCD_WRITE_DAT); 
    lcd_ili9341_write(0x00, LCD_WRITE_DAT); 
 
    lcd_ili9341_write(0xC0, LCD_WRITE_CMD);          /*Power control*/
    lcd_ili9341_write(0x21, LCD_WRITE_DAT);          /* VRH[5:0] */
 
    lcd_ili9341_write(0xC1, LCD_WRITE_CMD);          /* Power control */
    lcd_ili9341_write(0x12, LCD_WRITE_DAT);          /* SAP[2:0];BT[3:0] */
 
    lcd_ili9341_write(0xC5, LCD_WRITE_CMD);          /* VCM control */
    lcd_ili9341_write(0x32, LCD_WRITE_DAT); 
    lcd_ili9341_write(0x3C, LCD_WRITE_DAT); 
 
    lcd_ili9341_write(0xC7, LCD_WRITE_CMD);          /* VCM control2 */
    lcd_ili9341_write(0XC1, LCD_WRITE_DAT); 
 
    lcd_ili9341_write(0x36, LCD_WRITE_CMD);          /* Memory Access Control */
    lcd_ili9341_write(0x08, LCD_WRITE_DAT); 
 
    lcd_ili9341_write(0x3A, LCD_WRITE_CMD);   
    lcd_ili9341_write(0x55, LCD_WRITE_DAT); 

    lcd_ili9341_write(0xB1, LCD_WRITE_CMD);   
    lcd_ili9341_write(0x00, LCD_WRITE_DAT);   
    lcd_ili9341_write(0x18, LCD_WRITE_DAT); 
 
    lcd_ili9341_write(0xB6, LCD_WRITE_CMD);          /* Display Function Control */
    lcd_ili9341_write(0x0A, LCD_WRITE_DAT); 
    lcd_ili9341_write(0xA2, LCD_WRITE_DAT); 

    lcd_ili9341_write(0xF2, LCD_WRITE_CMD);          /* 3Gamma Function Disable */
    lcd_ili9341_write(0x00, LCD_WRITE_DAT); 
 
    lcd_ili9341_write(0x26, LCD_WRITE_CMD);          /* Gamma curve selected */
    lcd_ili9341_write(0x01, LCD_WRITE_DAT); 
 
    lcd_ili9341_write(0xE0, LCD_WRITE_CMD);          /* Set Gamma */
    lcd_ili9341_write(0x0F, LCD_WRITE_DAT); 
    lcd_ili9341_write(0x20, LCD_WRITE_DAT); 
    lcd_ili9341_write(0x1E, LCD_WRITE_DAT); 
    lcd_ili9341_write(0x09, LCD_WRITE_DAT); 
    lcd_ili9341_write(0x12, LCD_WRITE_DAT); 
    lcd_ili9341_write(0x0B, LCD_WRITE_DAT); 
    lcd_ili9341_write(0x50, LCD_WRITE_DAT); 
    lcd_ili9341_write(0XBA, LCD_WRITE_DAT); 
    lcd_ili9341_write(0x44, LCD_WRITE_DAT); 
    lcd_ili9341_write(0x09, LCD_WRITE_DAT); 
    lcd_ili9341_write(0x14, LCD_WRITE_DAT); 
    lcd_ili9341_write(0x05, LCD_WRITE_DAT); 
    lcd_ili9341_write(0x23, LCD_WRITE_DAT); 
    lcd_ili9341_write(0x21, LCD_WRITE_DAT); 
    lcd_ili9341_write(0x00, LCD_WRITE_DAT); 
 
    lcd_ili9341_write(0XE1, LCD_WRITE_CMD);          /* Set Gamma */
    lcd_ili9341_write(0x00, LCD_WRITE_DAT); 
    lcd_ili9341_write(0x19, LCD_WRITE_DAT); 
    lcd_ili9341_write(0x19, LCD_WRITE_DAT); 
    lcd_ili9341_write(0x00, LCD_WRITE_DAT); 
    lcd_ili9341_write(0x12, LCD_WRITE_DAT); 
    lcd_ili9341_write(0x07, LCD_WRITE_DAT); 
    lcd_ili9341_write(0x2D, LCD_WRITE_DAT); 
    lcd_ili9341_write(0x28, LCD_WRITE_DAT); 
    lcd_ili9341_write(0x3F, LCD_WRITE_DAT); 
    lcd_ili9341_write(0x02, LCD_WRITE_DAT); 
    lcd_ili9341_write(0x0A, LCD_WRITE_DAT); 
    lcd_ili9341_write(0x08, LCD_WRITE_DAT); 
    lcd_ili9341_write(0x25, LCD_WRITE_DAT); 
    lcd_ili9341_write(0x2D, LCD_WRITE_DAT); 
    lcd_ili9341_write(0x0F, LCD_WRITE_DAT); 
 
    lcd_ili9341_write(0x11, LCD_WRITE_CMD);          /* Exit Sleep */
    lcd_ili9341_port_delay(120); 
    lcd_ili9341_write(0x29, LCD_WRITE_CMD);          /* Display on */
}
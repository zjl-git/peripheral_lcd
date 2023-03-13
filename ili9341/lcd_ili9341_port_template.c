#include "lcd_ili9341.h"
#include "lcd_ili9341_port.h"

#include <stddef.h>

static void lcd_ili9341_port_dc_set(void)
{

}

static void lcd_ili9341_port_dc_clr(void)
{

}

static void lcd_ili9341_port_cs_set(void)
{

}

static void lcd_ili9341_port_cs_clr(void)
{

}

static void lcd_ili9341_port_res_set(void)
{

}

static void lcd_ili9341_port_res_clr(void)
{

}

static void lcd_ili9341_port_blk_set(void)
{

}

static void lcd_ili9341_port_blk_clr(void)
{

}


void *lcd_ili9341_malloc(unsigned int num) 
{
    return NULL;
}

void lcd_ili9341_free(void *mem) 
{

}


void lcd_ili9341_port_delay(unsigned int ms)
{

}

void lcd_ili9341_port_init(void)
{
    lcd_ili9341_port_res_clr();
    lcd_ili9341_port_delay(20);
    lcd_ili9341_port_res_set();
    lcd_ili9341_port_delay(20);
    lcd_ili9341_port_blk_set();
}

void lcd_ili9341_port_write_data(unsigned char data, unsigned char mode)
{
    lcd_ili9341_port_cs_clr();

    if (mode == LCD_WRITE_CMD) {
        lcd_ili9341_port_dc_set();
    } else if (mode == LCD_WRITE_DAT) {
        lcd_ili9341_port_dc_clr();
    } else {
        lcd_ili9341_port_cs_set();
        return ;
    }

    //send data.
    lcd_ili9341_port_cs_set();
}
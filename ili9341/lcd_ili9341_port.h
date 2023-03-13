#ifndef _LCD_ILI9341_PORT__
#define _LCD_ILI9341_PORT__

#ifdef __cplusplus
extern "C"
{
#endif

void *lcd_ili9341_malloc(unsigned int num);

void lcd_ili9341_free(void *mem);

void lcd_ili9341_port_delay(unsigned int ms);

void lcd_ili9341_port_init(void);

void lcd_ili9341_port_write_data(unsigned char data, unsigned char mode);

#ifdef __cplusplus
}
#endif

#endif

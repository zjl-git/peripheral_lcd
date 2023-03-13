#ifndef _LCD_ILI9341__
#define _LCD_ILI9341__

#ifdef __cplusplus
extern "C"
{
#endif

#define LCD_WRITE_CMD          0
#define LCD_WRITE_DAT          1

/*point color*/
#define LCD_WHITE         	 0xFFFF
#define LCD_BLACK         	 0x0000	  
#define LCD_BLUE         	 0x001F  
#define LCD_BRED             0XF81F
#define LCD_GRED 			 0XFFE0
#define LCD_GBLUE			 0X07FF
#define LCD_RED           	 0xF800
#define LCD_MAGENTA       	 0xF81F
#define LCD_GREEN         	 0x07E0
#define LCD_CYAN          	 0x7FFF
#define LCD_YELLOW        	 0xFFE0
#define LCD_BROWN 			 0XBC40
#define LCD_BRRED 			 0XFC07
#define LCD_GRAY  			 0X8430

/*back color*/
#define LCD_DARKBLUE      	 0X01CF
#define LCD_LIGHTBLUE      	 0X7D7C
#define LCD_GRAYBLUE       	 0X5458
 
#define LCD_LIGHTGREEN     	 0X841F
#define LCD_LGRAY 			 0XC618

#define LCD_LGRAYBLUE        0XA651
#define LCD_LBBLUE           0X2B12

#ifdef __cplusplus
}
#endif

#endif

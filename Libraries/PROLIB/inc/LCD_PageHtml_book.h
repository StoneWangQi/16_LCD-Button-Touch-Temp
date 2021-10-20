#ifndef __LCD_PAGEHTML_BOOK_H
#define __LCD_PAGEHTML_BOOK_H       

#include "stm32f10x.h"

void Display_Char_test(void);
void Display_LCD_clear(void);
uint8_t XPT2046_Touch_Calibrate_Page (void);
void  fn_Lcd_Page_Init(void);
void  fn_Lcd_Page1(void);
void  fn_Lcd_Page2(void);
void  fn_Lcd_Page3(void);
#endif 

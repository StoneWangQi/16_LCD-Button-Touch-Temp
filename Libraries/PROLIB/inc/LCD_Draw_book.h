#ifndef      __LCD_DRAW_BOOK_H
#define	     __LCD_DRAW_BOOK_H


#include "stm32f10x.h"
#include "LCD_book.h"
#include "fonts.h" 
#include "USART_book.h"
/******************************* 定义 ILI934 显示屏常用颜色 ********************************/
#define      BACKGROUND		                BLACK   //默认背景颜色

#define      WHITE		 		                  0xFFFF	   //白色
#define      BLACK                         0x0000	   //黑色 
#define      GREY                          0xF7DE	   //灰色 
#define      BLUE                          0x001F	   //蓝色 
#define      BLUE2                         0x051F	   //浅蓝色 
#define      RED                           0xF800	   //红色 
#define      MAGENTA                       0xF81F	   //红紫色，洋红色 
#define      GREEN                         0x07E0	   //绿色 
#define      CYAN                          0x7FFF	   //蓝绿色，青色 
#define      YELLOW                        0xFFE0	   //黄色 
#define      BRED                          0xF81F
#define      GRED                          0xFFE0
#define      GBLUE                         0x07FF

//1110 1100 1101 0101 1010 1111
//11101 110101 10101
//GB565(Rh,Gh,Bh)       (((Rh&0xf8)<<8)|((Gh&0xfc)<<3)|((Bh&0xf8)>>3)) 
//GB888(RGB)            ((((RGB&0xf80000)) | ((RGB&0x00fc00)<<3)| ((RGB&0x0000f8)<<5))>>8)
#define      GB565(Rh,Gh,Bh)       (((Rh&0xf8)<<8)|((Gh&0xfc)<<3)|((Bh&0xf8)>>3)) 
#define      GB888(RGB)            ((((RGB&0xf80000)) | ((RGB&0x00fc00)<<3)| ((RGB&0x0000f8)<<5))>>8)

extern  sFONT *LCD_Currentfonts  ;  //英文字体
extern uint16_t CurrentForecolor ;//前景色
extern uint16_t CurrentBackColor ;//背景色

//================================ 接口函数=======================================

void ILI9341_OpenWindow ( 
  uint16_t usX,     //usX ：在特定扫描方向下窗口的起点X坐标
  uint16_t usY,     //usY ：在特定扫描方向下窗口的起点Y坐标
  uint16_t usWidth, //usWidth ：窗口的宽度 
  uint16_t usHeight //usHeight ：窗口的高度
);
 
static void ILI9341_SetCursor ( 
  uint16_t usX, // usX ：在特定扫描方向下光标的X坐标
  uint16_t usY, // usY ：在特定扫描方向下光标的Y坐标
  uint16_t usW
);
 
static __inline void ILI9341_FillColor ( 
  uint32_t ulAmout_Point,// ulAmout_Point ：要填充颜色的像素点的总数目
  uint16_t usColor       // usColor ：颜色
);
 
void ILI9341_Clear ( 
  uint16_t usX,     //usX ：在特定扫描方向下窗口的起点X坐标
  uint16_t usY,     //usY ：在特定扫描方向下窗口的起点Y坐标
  uint16_t usWidth, //usWidth ：窗口的宽度
  uint16_t usHeight //usHeight ：窗口的高度 
);

void LCD_ClearLine(uint16_t Line);
//================================ 颜色状态函数=======================================
void LCD_SetColors(uint16_t TextColor, uint16_t BackColor); //设置LCD的前景(字体)及背景颜色,RGB565
void LCD_SetForeColor(uint16_t Color);//设置LCD的前景(字体)颜色,RGB565
void LCD_SetBackColor(uint16_t Color); //设置LCD的背景颜色,RGB565
 
//================================ 形状绘制应用函数=======================================
 
void ILI9341_SetPointPixel (
  uint16_t usX,  //usX ：在特定扫描方向下该点的X坐标
  uint16_t usY,  //usY ：在特定扫描方向下该点的Y坐标
  uint16_t usW   //usW ：在特定扫描宽度
);
 
void ILI9341_DrawLine ( //在 ILI9341 显示器上使用 Bresenham 算法画线段
  uint16_t usX1,  //usX1 ：在特定扫描方向下线段的一个端点X坐标
  uint16_t usY1,  //usY1 ：在特定扫描方向下线段的一个端点Y坐标
  uint16_t usX2,  //usX2 ：在特定扫描方向下线段的另一个端点X坐标
  uint16_t usY2,  //usY2 ：在特定扫描方向下线段的另一个端点Y坐标 
  uint16_t usW
);
 
void ILI9341_DrawRectangle ( 
  uint16_t usX_Start,     //X坐标
  uint16_t usY_Start,     //Y坐标 
  uint16_t usWidth,       //X宽度
  uint16_t usHeight,      //Y宽度
  uint8_t ucFilled ,      //填充类型
  uint16_t usW            //填充线宽度
);

 
void ILI9341_DrawCircle (
  uint16_t usX_Center, //usX_Center ：在特定扫描方向下圆心的X坐标
  uint16_t usY_Center, //usY_Center ：在特定扫描方向下圆心的Y坐标
  uint16_t usRadius,   //usRadius：圆的半径（单位：像素）
  uint8_t ucFilled ,  //ucFilled ：选择是否填充该圆
  uint16_t usW
);

void ILI9341_DrawCross ( uint16_t usX, uint16_t usY );

void LCD_DrawUniLineCircle(
  uint16_t x1, 
  uint16_t y1, 
  uint16_t x2, 
  uint16_t y2,
  uint8_t thick 
);

//================================ 文字颜色状态绘制应用函数=======================================
void LCD_SetTextColor(uint16_t Color);
void LCD_SetFont(sFONT *fonts);
sFONT *LCD_GetFont(void);
//================================ 文字绘制应用函数=======================================

//================================ 英文文字绘制应用函数==================================== 
 
void ILI9341_DispChar_EN ( 
  uint16_t usX, 
  uint16_t usY, 
  const char cChar );
 
void ILI9341_DispStringLine_EN (  
  uint16_t line,  
  char * pStr 
);

void ILI9341_DispString_EN ( 
	uint16_t usX ,
  uint16_t usY,  
  char * pStr 
);
 
void ILI9341_DispString_EN_YDir (	 
  uint16_t usX,uint16_t usY ,  
  char * pStr 
);

//================================ 中文文字绘制应用函数==================================== 

void ILI9341_DispChar_CH ( //在 ILI9341 显示器上显示一个中文字符
  uint16_t usX, 
  uint16_t usY, 
  uint16_t usChar 
);
 
void ILI9341_DispString_CH (  //在 ILI9341 显示器上显示中文字符串	
  uint16_t usX ,   
  uint16_t usY, 
  char * pStr 
);
 
void ILI9341_DispStringLine_EN_CH (  
  uint16_t line,    //line ：在特定扫描方向下字符串的起始Y坐标
  char * pStr       //pStr ：要显示的字符串的首地址
);
 
void ILI9341_DispString_EN_CH ( 	
  uint16_t usX ,  //在特定扫描方向下字符的起始X坐标
  uint16_t usY,   //在特定扫描方向下字符的起始Y坐标
  char * pStr     //要显示的字符串的首地址
);
 
void ILI9341_DispString_EN_CH_YDir (  
  uint16_t usX,   //usX ：在特定扫描方向下字符的起始X坐标
  uint16_t usY,   //usY ：在特定扫描方向下字符的起始Y坐标 
  char * pStr     //要显示的中英文字符串的首地址
);
  
/***********************缩放字体****************************/
 
void ILI9341_zoomChar(
  uint16_t in_width,	//原始字符宽度
  uint16_t in_heig,		//原始字符高度
  uint16_t out_width,	//缩放后的字符宽度
  uint16_t out_heig,	//缩放后的字符高度
  uint8_t *in_ptr,	  //字库输入指针	注意：1pixel 1bit
  uint8_t *out_ptr,   //缩放后的字符输出指针 注意: 1pixel 8bit
  uint8_t en_cn		    //0为英文，1为中文	
); 	

 
void ILI9341_DrawChar_Ex(
  uint16_t usX,         //字符显示位置x
  uint16_t usY,         //字符显示位置y
  uint16_t Font_width, //字符宽度
  uint16_t Font_Height,  //字符高度 
  uint8_t *c,						//字模数据
  uint16_t DrawModel		//是否反色显示
);
 
void ILI9341_DisplayStringEx(
  uint16_t x, 		      //字符显示位置x
  uint16_t y, 				  //字符显示位置y
  uint16_t Font_width,	//要显示的字体宽度，英文字符在此基础上/2。注意为偶数
  uint16_t Font_Height,	//要显示的字体高度，注意为偶数
  uint8_t *ptr,					//显示的字符内容
  uint16_t DrawModel    //是否反色显示
);
 
void ILI9341_DisplayStringEx_YDir(
  uint16_t x, 		//字符显示位置x
  uint16_t y, 				//字符显示位置y
  uint16_t Font_width,	//要显示的字体宽度，英文字符在此基础上/2。注意为偶数
  uint16_t Font_Height,	//要显示的字体高度，注意为偶数
  uint8_t *ptr,					//显示的字符内容
  uint16_t DrawModel  //是否反色显示
);

void Lcd_display_String(uint8_t *ptr);
  
#endif /* __BSP_ILI9341_ILI9341_H */


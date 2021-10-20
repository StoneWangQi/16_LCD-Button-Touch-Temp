#include "LCD_PageHtml_book.h" 
#include "LCD_book.h"
#include "LCD_Draw_book.h"
#include "XPT2046_LCD_Function_book.h"
#include "USART_book.h"
#include "XPT2046_LCD_Device_book.h"
#include "fonts.h" 
 

 
#include <stdio.h> 
#include <string.h>

/// 自模 设计 字宽 16 字高 16 
//  阴码  逐行 24 24 顺向 16进制  
uint8_t test_modu[] ={ 
 0x00,0x00,0x7F,0xFC,0x01,0x00,0x01,0x00,0x01,0x00,0x01,
 0x00,0x01,0x00,0x3F,0xF8,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00 ,
 0x01,0x00,0x01,0x00,0xFF,0xFE,0x00,0x00};/*"王",0*/

//#define _row_count  16
//#define _byte_count  2
//#define _bit_count   8
 
/// 自模 设计 字宽 32 字高 32 
//  阴码  逐行 24 24 顺向 16进制  
uint8_t test_modu2[2][32*32] ={
 
 { 
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 ,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xFF,0xFF,0xC0,0x07,0xFF,0xFF,0xC0 ,
 0x04,0x03,0xC0,0x00,0x00,0x03,0xC0,0x00,0x00,0x03,0xC0,0x00,0x00,0x03,0xC0,0x00,0x01,0xFF,0xFF,0x00,0x03,0xFF,0xFF,0x00 ,
 0x03,0x03,0xC0,0x00,0x00,0x03,0xC0,0x00,0x00,0x03,0xC0,0x00,0x00,0x03,0xC0,0x00,0x1F,0xFF,0xFC,0x00,0x3F,0xFF,0xFF,0xFE ,
 0x7F,0x80,0x7F,0xFE,0x60,0x00,0x03,0xFC,0x00,0x00,0x00,0x78,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 ,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"王",0*/ 


{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 ,
 0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x03,0x87,0x00,0x00,0x03,0x87,0x00,0x18,0x07,0x87,0xC0,0x3F,0xEF,0xFF,0xC0 ,
 0x3F,0xCF,0x87,0x00,0x03,0x83,0x87,0x00,0x03,0x83,0xFF,0x00,0x1F,0x83,0x87,0x00,0x3F,0xE3,0x87,0x00,0x3F,0x83,0xFF,0x00 ,
 0x03,0x83,0x87,0x00,0x03,0xB3,0x87,0x00,0x03,0xFF,0xFF,0xFE,0x3F,0xDF,0xFF,0xFC,0x7E,0x18,0x00,0xFC,0x78,0x01,0x87,0x38 ,
 0x40,0x03,0x87,0x80,0x00,0x0F,0x03,0xC0,0x00,0x1C,0x00,0xE0,0x00,0x20,0x00,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 ,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}/*"琪",0*/
};

#define _row_count   32
#define _byte_count  4
#define _bit_count   8
void Display_Char_test(void){
  
  uint8_t row_count , byte_count , bit_count ;
  uint8_t i;
  for(i=0 ;i<2 ;i++ ){
      for(row_count = 0 ;row_count< _row_count; row_count++){
        
        printf("\n");
        
        for(byte_count = 0 ;byte_count< _byte_count ; byte_count++){
          
          for( bit_count = 0;bit_count < _bit_count ; bit_count++){
            
            if(test_modu2[i][row_count*_byte_count+byte_count] &(0x80 >>bit_count)){
               printf("*");
            }else{
              printf(" ");
            }
          }
        }
      }
  }
  
}
void Display_LCD_clear(void){
  LCD_SetColors(GB888(0x000000), GB888(0xFFFFFF));
	ILI9341_Clear ( 0, 0, LCD_X_LENGTH, LCD_Y_LENGTH ); 
}
//================================ 页面设计函数=======================================
//================================ 页面设计函数=======================================

uint8_t XPT2046_Touch_Calibrate_Page (void){
		uint8_t i;
		char cStr [ 100 ];	
	  char * pStr = 0;
 
		LCD_SetFont(&Font8x16);
		LCD_SetColors(GB888(0xFE5632),GB888(0xCFDA00));
	  
    XPT2046_Touch_Calibrate_SetStand4Pint();
    
		for ( i = 0; i < 4; i ++ ){ 
			ILI9341_Clear ( 0, 0, LCD_X_LENGTH, LCD_Y_LENGTH );       		
			pStr = "Touch Calibrate 触摸校验...";		
			//插入空格，居中显示
			sprintf(cStr,"%*c%s",((LCD_X_LENGTH/(((sFONT *)LCD_GetFont())->Width))-strlen(pStr))/2,' ',pStr)	;	
      ILI9341_DispStringLine_EN_CH (LCD_Y_LENGTH >> 1, cStr );			
		
			//插入空格，居中显示
			sprintf ( cStr, "%*c%d",((LCD_X_LENGTH/(((sFONT *)LCD_GetFont())->Width)) -1)/2,' ',i + 1 );
			ILI9341_DispStringLine_EN_CH (( LCD_Y_LENGTH >> 1 ) - (((sFONT *)LCD_GetFont())->Height), cStr ); 
		
			ILI9341_Delay ( 0xFFFFF );		                     //适当的延时很有必要
			
      LCD_SetForeColor( GB888(0x8C78EB)); 
      ILI9341_DrawCircle(strCrossCoordinate[i] .x, strCrossCoordinate[i].y , 20 , 1, 1);
      LCD_SetForeColor( GB888(0xDA44A6)); 
			ILI9341_DrawCross ( strCrossCoordinate[i] .x, strCrossCoordinate[i].y );  //显示校正用的“十”字

			while ( ! XPT2046_ReadAdc_Smooth_XY ( & strScreenSample [i] ) );               //读取XPT2046数据到变量pCoordinate，当ptr为空时表示没有触点被按下

		}
		
    if( XPT2046_Touch_Calibrate(_LCD_SCAN_MODE)==1){
      goto XPT2046_Touch_Calibrate_SUCCESS ;
    }else{
      goto XPT2046_Touch_Calibrate_Failure ;
    }
    
XPT2046_Touch_Calibrate_SUCCESS:
     
	ILI9341_Clear ( 0, 0, LCD_X_LENGTH, LCD_Y_LENGTH );
	LCD_SetColors(GB888(0x331B93),GB888(0xE4C294)); 
	LCD_SetTextColor(GB888(0xFF5B93));
	
	pStr = "Calibrate Succed";
	//插入空格，居中显示	
	sprintf(cStr,"%*c%s",((LCD_X_LENGTH/(((sFONT *)LCD_GetFont())->Width))-strlen(pStr))/2,' ',pStr)	;	
  ILI9341_DispStringLine_EN (LCD_Y_LENGTH >> 1, cStr );	

  ILI9341_Delay ( 0xFFFFFF );
	return 1;    
	

XPT2046_Touch_Calibrate_Failure:
	
	ILI9341_Clear ( 0, 0, LCD_X_LENGTH, LCD_Y_LENGTH ); 
	LCD_SetColors(GB888(0x331B93),GB888(0xFF5B93)); 
	LCD_SetTextColor(RED);
	
	pStr = "Calibrate fail";	
	//插入空格，居中显示	
	sprintf(cStr,"%*c%s",((LCD_X_LENGTH/(((sFONT *)LCD_GetFont())->Width))-strlen(pStr))/2,' ',pStr)	;	
  ILI9341_DispStringLine_EN (LCD_Y_LENGTH >> 1, cStr );	

	pStr = "try again";
	//插入空格，居中显示		
	sprintf(cStr,"%*c%s",((LCD_X_LENGTH/(((sFONT *)LCD_GetFont())->Width))-strlen(pStr))/2,' ',pStr)	;	
	ILI9341_DispStringLine_EN ( ( LCD_Y_LENGTH >> 1 ) + (((sFONT *)LCD_GetFont())->Height), cStr );				

	ILI9341_Delay ( 0xFFFFFF );		
	return 0; 
		
		
}


//================================ 页面设计函数=======================================
void  fn_Lcd_Page_Init(void){
  fn_Lcd_Page1();
}
//================================ 页面设计函数=======================================

/**
 * @brief  在ILI9341显示器上开辟一个窗口
 * @param  usX ：在特定扫描方向下窗口的起点X坐标
 * @param  usY ：在特定扫描方向下窗口的起点Y坐标
 * @param  usWidth ：窗口的宽度
 * @param  usHeight ：窗口的高度
 * @retval 无
 */
void  fn_Lcd_Page1(void){
//    printf("\n-->LCD %x",GB565(255, 193 ,98));
//    printf("\n-->LCD %x",GB888(0xFFC162));
      LCD_SetColors( GB888(0x589447), GB888(0xFFC162));
      ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* 清屏，显示 黄色的界面 */
      
      LCD_SetForeColor(GB888(0x589447)); //标题绿色的框
      ILI9341_DrawRectangle( 0,0,LCD_X_LENGTH,25,1,1);
      LCD_SetForeColor(GB888(0x797979));//阴影框
      ILI9341_DrawLine(0,25,LCD_X_LENGTH,25,2);
      LCD_SetForeColor(GB888(0xCDCDCD));
      ILI9341_DrawLine(0,27,LCD_X_LENGTH,27,1);
      
      LCD_SetForeColor(WHITE);//Wifi设计
      ILI9341_DrawLine(LCD_X_LENGTH-20,19,LCD_X_LENGTH-15,19,3);
      ILI9341_DrawLine(LCD_X_LENGTH-25,12,LCD_X_LENGTH-10,12,3);
      ILI9341_DrawLine(LCD_X_LENGTH-28,5,LCD_X_LENGTH-7,5,3);
            
      LCD_SetFont(&Font8x16);
      LCD_SetForeColor(GB888(0xE8E5C4)); //白色文字，绿色背景
      /********显示字符串示例*******/
      ILI9341_DispStringLine_EN(LINE(0),"Wangqi Telephone:");
      
      LCD_SetFont(&Font8x16);  //制作对话框
      LCD_SetForeColor( GB888(0x000000));
      ILI9341_DispStringLine_EN(LINE(3),"  Personnal News:");
      
      LCD_SetForeColor(GB888(0x589447)); //灰色的框
      ILI9341_DrawRectangle( 10,65,(LCD_X_LENGTH-20),30,0,1);
      LCD_SetForeColor(GB888(0xC3C09C)); //里面的
      ILI9341_DrawRectangle( 11,66,(LCD_X_LENGTH-20-1),30-1,1,1);
           
      LCD_SetFont(&Font8x16);
      LCD_SetColors(GB888(0xE8E5C4), GB888(0x589447));
      LCD_SetForeColor(GB888(0x589447)); //绿色的按钮
      ILI9341_DrawRectangle( 180,110,(50),30,1,1);
      LCD_SetForeColor(WHITE); //白色阴影
      ILI9341_DrawRectangle( 180,110,(50),30,0,1);
           
      //-------------------------
      LCD_SetFont(&Font8x16);  //制作对话框
      LCD_SetForeColor( GB888(0xC90000)); //红色文字
      ILI9341_DispStringLine_EN(LINE(9)," Have Girlfriend?");
      //-------------------------
      LCD_SetColors( GB888(0xF0A0A1), GB888(0xFFC162)); //圆圈
      ILI9341_DrawCircle(210,180,10,1,1);
      LCD_SetForeColor(GB888(0x797979));//阴影框
      ILI9341_DrawCircle(210,180,10,0,1);
      LCD_SetColors( GB888(0x34BDD5), GB888(0xFFC162)); //圆圈
      ILI9341_DrawCircle(210,205,10,1,1);
       LCD_SetForeColor(GB888(0x797979));//阴影框
      ILI9341_DrawCircle(210,205,10,0,1);
      //-------------------------
      LCD_SetFont(&Font16x24);  //制作对话框
      LCD_SetForeColor( GB888(0xBB3756)); //红色文字
      ILI9341_DispStringLine_EN(LINE(7),"        Yes!");
      LCD_SetForeColor( GB888(0x310F29)); //红色文字
      ILI9341_DispStringLine_EN(LINE(8),"         NO!");
      
      //-------------------------
      LCD_SetForeColor(GB888(0xA6A6A6));//阴影框
      ILI9341_DrawLine(0,230,LCD_X_LENGTH,230,1);
      
      LCD_SetForeColor( GB888(0xBF5BBA)); //红色文字
      ILI9341_DispStringLine_EN(LINE(10),"   My LOVE ");
      LCD_SetForeColor(  GB888(0x2369FC)); //白色文字
      ILI9341_DispStringLine_EN(LINE(12)," Hi 2021-8-14 ");
}

//=============================================================================

/**
 * @brief  在ILI9341显示器上开辟一个窗口
 * @param  usX ：在特定扫描方向下窗口的起点X坐标
 * @param  usY ：在特定扫描方向下窗口的起点Y坐标
 * @param  usWidth ：窗口的宽度
 * @param  usHeight ：窗口的高度
 * @retval 无
 */
void  fn_Lcd_Page2(void){

//      printf("\n-->LCD %x",GB565(255, 193 ,98));
//      printf("\n-->LCD %x",GB888(0xFFC162));
  
      LCD_SetColors( GB888(0xF09387), GB888(0xF09387));
      ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* 清屏，显示 粉色的界面 */
      
      LCD_SetForeColor(GB888(0xFFDC78)); //标题绿色的框
      ILI9341_DrawRectangle( 0,0,LCD_X_LENGTH,25,1,1);
      LCD_SetForeColor(GB888(0x797979));//阴影框
      ILI9341_DrawLine(0,25,LCD_X_LENGTH,25,2);
      LCD_SetForeColor(GB888(0xCDCDCD));
      ILI9341_DrawLine(0,27,LCD_X_LENGTH,27,1);
      
      LCD_SetForeColor(GB888(0xA700A7));//Wifi设计
      ILI9341_DrawLine(LCD_X_LENGTH-20,19,LCD_X_LENGTH-15,19,3);
      ILI9341_DrawLine(LCD_X_LENGTH-25,12,LCD_X_LENGTH-10,12,3);
      ILI9341_DrawLine(LCD_X_LENGTH-28,5,LCD_X_LENGTH-7,5,3);
            
      LCD_SetFont(&Font8x16);
      LCD_SetForeColor(GB888(0xA700A7)); //白色文字，绿色背景
      /********显示字符串示例*******/
      ILI9341_DispString_EN_CH(10,5,"输入 Telephone!");
      
      LCD_SetFont(&Font8x16);  //制作对话框
      LCD_SetForeColor( GB888(0x000000));
      ILI9341_DispStringLine_EN_CH(LINE(3),"  信息:");
      
      LCD_SetForeColor(GB888(0x589447)); //灰色的框
      ILI9341_DrawRectangle( 10,65,(LCD_X_LENGTH-20),30,0,1);
      LCD_SetForeColor(GB888(0xC3C09C)); //里面的
      ILI9341_DrawRectangle( 11,66,(LCD_X_LENGTH-20-1),30-1,1,1);
           
      LCD_SetFont(&Font8x16);  //制作对话框
      LCD_SetForeColor( GB888(0x000000));
      ILI9341_DispString_CH(13,72,"不顾一切的去看你");     
           
      LCD_SetFont(&Font8x16);
      LCD_SetColors(GB888(0xE8E5C4), GB888(0x589447));
      LCD_SetForeColor(GB888(0x589447)); //绿色的按钮
      ILI9341_DrawRectangle( 180,110,(50),30,1,1);
      LCD_SetForeColor(WHITE); //白色阴影
      ILI9341_DrawRectangle( 180,110,(50),30,0,1);
           
      LCD_SetFont(&Font8x16);  //制作对话框
      LCD_SetForeColor( GB888(0x000000));
      ILI9341_DispStringLine_EN_CH(LINE(7),"                        确定");     
           
      //-------------------------
      LCD_SetFont(&Font8x16);  //制作对话框
      LCD_SetForeColor( GB888(0xC90000)); //红色文字
      ILI9341_DispStringLine_EN_CH(LINE(9)," 回忆化成蝶?");
      //-------------------------
      LCD_SetColors( GB888(0xF0A0A1), GB888(0xFFC162)); //圆圈
      ILI9341_DrawCircle(210,180,10,1,1);
      LCD_SetForeColor(GB888(0x797979));//阴影框
      ILI9341_DrawCircle(210,180,10,0,1);
      LCD_SetColors( GB888(0x34BDD5), GB888(0xFFC162)); //圆圈
      ILI9341_DrawCircle(210,205,10,1,1);
       LCD_SetForeColor(GB888(0x797979));//阴影框
      ILI9341_DrawCircle(210,205,10,0,1);
      //-------------------------
      LCD_SetFont(&Font24x32);  //制作对话框
      LCD_SetForeColor( GB888(0xBB3756)); //红色文字
      ILI9341_DisplayStringEx(0,200,Font24x32.Width  , Font24x32.Height , "        Yes!",0);
      LCD_SetForeColor( GB888(0x310F29)); //红色文字
      ILI9341_DisplayStringEx(0,230,Font24x32.Width  , Font24x32.Height ,"         NO!",0);
      
      //-------------------------
      LCD_SetForeColor(GB888(0xA6A6A6));//阴影框
      ILI9341_DrawLine(0,230,LCD_X_LENGTH,230,1);
      
      LCD_SetForeColor( GB888(0xBF5BBA)); //红色文字
      ILI9341_DisplayStringEx(0,260,Font24x32.Width  , Font24x32.Height ,"   My LOVE ",0);
      LCD_SetForeColor(  GB888(0x2369FC)); //白色文字
      LCD_SetFont(&Font16x24);  //制作对话框
      ILI9341_DispStringLine_EN(LINE(12)," Hi 2021-8-16 ");
 
}

//=============================================================================

/**
 * @brief  在ILI9341显示器上开辟一个窗口
 * @param  usX ：在特定扫描方向下窗口的起点X坐标
 * @param  usY ：在特定扫描方向下窗口的起点Y坐标
 * @param  usWidth ：窗口的宽度
 * @param  usHeight ：窗口的高度
 * @retval 无
 */
void  fn_Lcd_Page3(void){
  //绘制触摸画板界面
	Palette_Init(_LCD_SCAN_MODE ,CL_WHITE , CL_BLACK);
   
}





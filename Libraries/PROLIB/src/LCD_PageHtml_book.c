#include "LCD_PageHtml_book.h" 
#include "LCD_book.h"
#include "LCD_Draw_book.h"
#include "XPT2046_LCD_Function_book.h"
#include "USART_book.h"
#include "XPT2046_LCD_Device_book.h"
#include "fonts.h" 
 

 
#include <stdio.h> 
#include <string.h>

/// ��ģ ��� �ֿ� 16 �ָ� 16 
//  ����  ���� 24 24 ˳�� 16����  
uint8_t test_modu[] ={ 
 0x00,0x00,0x7F,0xFC,0x01,0x00,0x01,0x00,0x01,0x00,0x01,
 0x00,0x01,0x00,0x3F,0xF8,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00 ,
 0x01,0x00,0x01,0x00,0xFF,0xFE,0x00,0x00};/*"��",0*/

//#define _row_count  16
//#define _byte_count  2
//#define _bit_count   8
 
/// ��ģ ��� �ֿ� 32 �ָ� 32 
//  ����  ���� 24 24 ˳�� 16����  
uint8_t test_modu2[2][32*32] ={
 
 { 
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 ,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xFF,0xFF,0xC0,0x07,0xFF,0xFF,0xC0 ,
 0x04,0x03,0xC0,0x00,0x00,0x03,0xC0,0x00,0x00,0x03,0xC0,0x00,0x00,0x03,0xC0,0x00,0x01,0xFF,0xFF,0x00,0x03,0xFF,0xFF,0x00 ,
 0x03,0x03,0xC0,0x00,0x00,0x03,0xC0,0x00,0x00,0x03,0xC0,0x00,0x00,0x03,0xC0,0x00,0x1F,0xFF,0xFC,0x00,0x3F,0xFF,0xFF,0xFE ,
 0x7F,0x80,0x7F,0xFE,0x60,0x00,0x03,0xFC,0x00,0x00,0x00,0x78,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 ,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"��",0*/ 


{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 ,
 0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x03,0x87,0x00,0x00,0x03,0x87,0x00,0x18,0x07,0x87,0xC0,0x3F,0xEF,0xFF,0xC0 ,
 0x3F,0xCF,0x87,0x00,0x03,0x83,0x87,0x00,0x03,0x83,0xFF,0x00,0x1F,0x83,0x87,0x00,0x3F,0xE3,0x87,0x00,0x3F,0x83,0xFF,0x00 ,
 0x03,0x83,0x87,0x00,0x03,0xB3,0x87,0x00,0x03,0xFF,0xFF,0xFE,0x3F,0xDF,0xFF,0xFC,0x7E,0x18,0x00,0xFC,0x78,0x01,0x87,0x38 ,
 0x40,0x03,0x87,0x80,0x00,0x0F,0x03,0xC0,0x00,0x1C,0x00,0xE0,0x00,0x20,0x00,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 ,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}/*"��",0*/
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
//================================ ҳ����ƺ���=======================================
//================================ ҳ����ƺ���=======================================

uint8_t XPT2046_Touch_Calibrate_Page (void){
		uint8_t i;
		char cStr [ 100 ];	
	  char * pStr = 0;
 
		LCD_SetFont(&Font8x16);
		LCD_SetColors(GB888(0xFE5632),GB888(0xCFDA00));
	  
    XPT2046_Touch_Calibrate_SetStand4Pint();
    
		for ( i = 0; i < 4; i ++ ){ 
			ILI9341_Clear ( 0, 0, LCD_X_LENGTH, LCD_Y_LENGTH );       		
			pStr = "Touch Calibrate ����У��...";		
			//����ո񣬾�����ʾ
			sprintf(cStr,"%*c%s",((LCD_X_LENGTH/(((sFONT *)LCD_GetFont())->Width))-strlen(pStr))/2,' ',pStr)	;	
      ILI9341_DispStringLine_EN_CH (LCD_Y_LENGTH >> 1, cStr );			
		
			//����ո񣬾�����ʾ
			sprintf ( cStr, "%*c%d",((LCD_X_LENGTH/(((sFONT *)LCD_GetFont())->Width)) -1)/2,' ',i + 1 );
			ILI9341_DispStringLine_EN_CH (( LCD_Y_LENGTH >> 1 ) - (((sFONT *)LCD_GetFont())->Height), cStr ); 
		
			ILI9341_Delay ( 0xFFFFF );		                     //�ʵ�����ʱ���б�Ҫ
			
      LCD_SetForeColor( GB888(0x8C78EB)); 
      ILI9341_DrawCircle(strCrossCoordinate[i] .x, strCrossCoordinate[i].y , 20 , 1, 1);
      LCD_SetForeColor( GB888(0xDA44A6)); 
			ILI9341_DrawCross ( strCrossCoordinate[i] .x, strCrossCoordinate[i].y );  //��ʾУ���õġ�ʮ����

			while ( ! XPT2046_ReadAdc_Smooth_XY ( & strScreenSample [i] ) );               //��ȡXPT2046���ݵ�����pCoordinate����ptrΪ��ʱ��ʾû�д��㱻����

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
	//����ո񣬾�����ʾ	
	sprintf(cStr,"%*c%s",((LCD_X_LENGTH/(((sFONT *)LCD_GetFont())->Width))-strlen(pStr))/2,' ',pStr)	;	
  ILI9341_DispStringLine_EN (LCD_Y_LENGTH >> 1, cStr );	

  ILI9341_Delay ( 0xFFFFFF );
	return 1;    
	

XPT2046_Touch_Calibrate_Failure:
	
	ILI9341_Clear ( 0, 0, LCD_X_LENGTH, LCD_Y_LENGTH ); 
	LCD_SetColors(GB888(0x331B93),GB888(0xFF5B93)); 
	LCD_SetTextColor(RED);
	
	pStr = "Calibrate fail";	
	//����ո񣬾�����ʾ	
	sprintf(cStr,"%*c%s",((LCD_X_LENGTH/(((sFONT *)LCD_GetFont())->Width))-strlen(pStr))/2,' ',pStr)	;	
  ILI9341_DispStringLine_EN (LCD_Y_LENGTH >> 1, cStr );	

	pStr = "try again";
	//����ո񣬾�����ʾ		
	sprintf(cStr,"%*c%s",((LCD_X_LENGTH/(((sFONT *)LCD_GetFont())->Width))-strlen(pStr))/2,' ',pStr)	;	
	ILI9341_DispStringLine_EN ( ( LCD_Y_LENGTH >> 1 ) + (((sFONT *)LCD_GetFont())->Height), cStr );				

	ILI9341_Delay ( 0xFFFFFF );		
	return 0; 
		
		
}


//================================ ҳ����ƺ���=======================================
void  fn_Lcd_Page_Init(void){
  fn_Lcd_Page1();
}
//================================ ҳ����ƺ���=======================================

/**
 * @brief  ��ILI9341��ʾ���Ͽ���һ������
 * @param  usX �����ض�ɨ�跽���´��ڵ����X����
 * @param  usY �����ض�ɨ�跽���´��ڵ����Y����
 * @param  usWidth �����ڵĿ��
 * @param  usHeight �����ڵĸ߶�
 * @retval ��
 */
void  fn_Lcd_Page1(void){
//    printf("\n-->LCD %x",GB565(255, 193 ,98));
//    printf("\n-->LCD %x",GB888(0xFFC162));
      LCD_SetColors( GB888(0x589447), GB888(0xFFC162));
      ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* ��������ʾ ��ɫ�Ľ��� */
      
      LCD_SetForeColor(GB888(0x589447)); //������ɫ�Ŀ�
      ILI9341_DrawRectangle( 0,0,LCD_X_LENGTH,25,1,1);
      LCD_SetForeColor(GB888(0x797979));//��Ӱ��
      ILI9341_DrawLine(0,25,LCD_X_LENGTH,25,2);
      LCD_SetForeColor(GB888(0xCDCDCD));
      ILI9341_DrawLine(0,27,LCD_X_LENGTH,27,1);
      
      LCD_SetForeColor(WHITE);//Wifi���
      ILI9341_DrawLine(LCD_X_LENGTH-20,19,LCD_X_LENGTH-15,19,3);
      ILI9341_DrawLine(LCD_X_LENGTH-25,12,LCD_X_LENGTH-10,12,3);
      ILI9341_DrawLine(LCD_X_LENGTH-28,5,LCD_X_LENGTH-7,5,3);
            
      LCD_SetFont(&Font8x16);
      LCD_SetForeColor(GB888(0xE8E5C4)); //��ɫ���֣���ɫ����
      /********��ʾ�ַ���ʾ��*******/
      ILI9341_DispStringLine_EN(LINE(0),"Wangqi Telephone:");
      
      LCD_SetFont(&Font8x16);  //�����Ի���
      LCD_SetForeColor( GB888(0x000000));
      ILI9341_DispStringLine_EN(LINE(3),"  Personnal News:");
      
      LCD_SetForeColor(GB888(0x589447)); //��ɫ�Ŀ�
      ILI9341_DrawRectangle( 10,65,(LCD_X_LENGTH-20),30,0,1);
      LCD_SetForeColor(GB888(0xC3C09C)); //�����
      ILI9341_DrawRectangle( 11,66,(LCD_X_LENGTH-20-1),30-1,1,1);
           
      LCD_SetFont(&Font8x16);
      LCD_SetColors(GB888(0xE8E5C4), GB888(0x589447));
      LCD_SetForeColor(GB888(0x589447)); //��ɫ�İ�ť
      ILI9341_DrawRectangle( 180,110,(50),30,1,1);
      LCD_SetForeColor(WHITE); //��ɫ��Ӱ
      ILI9341_DrawRectangle( 180,110,(50),30,0,1);
           
      //-------------------------
      LCD_SetFont(&Font8x16);  //�����Ի���
      LCD_SetForeColor( GB888(0xC90000)); //��ɫ����
      ILI9341_DispStringLine_EN(LINE(9)," Have Girlfriend?");
      //-------------------------
      LCD_SetColors( GB888(0xF0A0A1), GB888(0xFFC162)); //ԲȦ
      ILI9341_DrawCircle(210,180,10,1,1);
      LCD_SetForeColor(GB888(0x797979));//��Ӱ��
      ILI9341_DrawCircle(210,180,10,0,1);
      LCD_SetColors( GB888(0x34BDD5), GB888(0xFFC162)); //ԲȦ
      ILI9341_DrawCircle(210,205,10,1,1);
       LCD_SetForeColor(GB888(0x797979));//��Ӱ��
      ILI9341_DrawCircle(210,205,10,0,1);
      //-------------------------
      LCD_SetFont(&Font16x24);  //�����Ի���
      LCD_SetForeColor( GB888(0xBB3756)); //��ɫ����
      ILI9341_DispStringLine_EN(LINE(7),"        Yes!");
      LCD_SetForeColor( GB888(0x310F29)); //��ɫ����
      ILI9341_DispStringLine_EN(LINE(8),"         NO!");
      
      //-------------------------
      LCD_SetForeColor(GB888(0xA6A6A6));//��Ӱ��
      ILI9341_DrawLine(0,230,LCD_X_LENGTH,230,1);
      
      LCD_SetForeColor( GB888(0xBF5BBA)); //��ɫ����
      ILI9341_DispStringLine_EN(LINE(10),"   My LOVE ");
      LCD_SetForeColor(  GB888(0x2369FC)); //��ɫ����
      ILI9341_DispStringLine_EN(LINE(12)," Hi 2021-8-14 ");
}

//=============================================================================

/**
 * @brief  ��ILI9341��ʾ���Ͽ���һ������
 * @param  usX �����ض�ɨ�跽���´��ڵ����X����
 * @param  usY �����ض�ɨ�跽���´��ڵ����Y����
 * @param  usWidth �����ڵĿ��
 * @param  usHeight �����ڵĸ߶�
 * @retval ��
 */
void  fn_Lcd_Page2(void){

//      printf("\n-->LCD %x",GB565(255, 193 ,98));
//      printf("\n-->LCD %x",GB888(0xFFC162));
  
      LCD_SetColors( GB888(0xF09387), GB888(0xF09387));
      ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* ��������ʾ ��ɫ�Ľ��� */
      
      LCD_SetForeColor(GB888(0xFFDC78)); //������ɫ�Ŀ�
      ILI9341_DrawRectangle( 0,0,LCD_X_LENGTH,25,1,1);
      LCD_SetForeColor(GB888(0x797979));//��Ӱ��
      ILI9341_DrawLine(0,25,LCD_X_LENGTH,25,2);
      LCD_SetForeColor(GB888(0xCDCDCD));
      ILI9341_DrawLine(0,27,LCD_X_LENGTH,27,1);
      
      LCD_SetForeColor(GB888(0xA700A7));//Wifi���
      ILI9341_DrawLine(LCD_X_LENGTH-20,19,LCD_X_LENGTH-15,19,3);
      ILI9341_DrawLine(LCD_X_LENGTH-25,12,LCD_X_LENGTH-10,12,3);
      ILI9341_DrawLine(LCD_X_LENGTH-28,5,LCD_X_LENGTH-7,5,3);
            
      LCD_SetFont(&Font8x16);
      LCD_SetForeColor(GB888(0xA700A7)); //��ɫ���֣���ɫ����
      /********��ʾ�ַ���ʾ��*******/
      ILI9341_DispString_EN_CH(10,5,"���� Telephone!");
      
      LCD_SetFont(&Font8x16);  //�����Ի���
      LCD_SetForeColor( GB888(0x000000));
      ILI9341_DispStringLine_EN_CH(LINE(3),"  ��Ϣ:");
      
      LCD_SetForeColor(GB888(0x589447)); //��ɫ�Ŀ�
      ILI9341_DrawRectangle( 10,65,(LCD_X_LENGTH-20),30,0,1);
      LCD_SetForeColor(GB888(0xC3C09C)); //�����
      ILI9341_DrawRectangle( 11,66,(LCD_X_LENGTH-20-1),30-1,1,1);
           
      LCD_SetFont(&Font8x16);  //�����Ի���
      LCD_SetForeColor( GB888(0x000000));
      ILI9341_DispString_CH(13,72,"����һ�е�ȥ����");     
           
      LCD_SetFont(&Font8x16);
      LCD_SetColors(GB888(0xE8E5C4), GB888(0x589447));
      LCD_SetForeColor(GB888(0x589447)); //��ɫ�İ�ť
      ILI9341_DrawRectangle( 180,110,(50),30,1,1);
      LCD_SetForeColor(WHITE); //��ɫ��Ӱ
      ILI9341_DrawRectangle( 180,110,(50),30,0,1);
           
      LCD_SetFont(&Font8x16);  //�����Ի���
      LCD_SetForeColor( GB888(0x000000));
      ILI9341_DispStringLine_EN_CH(LINE(7),"                        ȷ��");     
           
      //-------------------------
      LCD_SetFont(&Font8x16);  //�����Ի���
      LCD_SetForeColor( GB888(0xC90000)); //��ɫ����
      ILI9341_DispStringLine_EN_CH(LINE(9)," ���仯�ɵ�?");
      //-------------------------
      LCD_SetColors( GB888(0xF0A0A1), GB888(0xFFC162)); //ԲȦ
      ILI9341_DrawCircle(210,180,10,1,1);
      LCD_SetForeColor(GB888(0x797979));//��Ӱ��
      ILI9341_DrawCircle(210,180,10,0,1);
      LCD_SetColors( GB888(0x34BDD5), GB888(0xFFC162)); //ԲȦ
      ILI9341_DrawCircle(210,205,10,1,1);
       LCD_SetForeColor(GB888(0x797979));//��Ӱ��
      ILI9341_DrawCircle(210,205,10,0,1);
      //-------------------------
      LCD_SetFont(&Font24x32);  //�����Ի���
      LCD_SetForeColor( GB888(0xBB3756)); //��ɫ����
      ILI9341_DisplayStringEx(0,200,Font24x32.Width  , Font24x32.Height , "        Yes!",0);
      LCD_SetForeColor( GB888(0x310F29)); //��ɫ����
      ILI9341_DisplayStringEx(0,230,Font24x32.Width  , Font24x32.Height ,"         NO!",0);
      
      //-------------------------
      LCD_SetForeColor(GB888(0xA6A6A6));//��Ӱ��
      ILI9341_DrawLine(0,230,LCD_X_LENGTH,230,1);
      
      LCD_SetForeColor( GB888(0xBF5BBA)); //��ɫ����
      ILI9341_DisplayStringEx(0,260,Font24x32.Width  , Font24x32.Height ,"   My LOVE ",0);
      LCD_SetForeColor(  GB888(0x2369FC)); //��ɫ����
      LCD_SetFont(&Font16x24);  //�����Ի���
      ILI9341_DispStringLine_EN(LINE(12)," Hi 2021-8-16 ");
 
}

//=============================================================================

/**
 * @brief  ��ILI9341��ʾ���Ͽ���һ������
 * @param  usX �����ض�ɨ�跽���´��ڵ����X����
 * @param  usY �����ض�ɨ�跽���´��ڵ����Y����
 * @param  usWidth �����ڵĿ��
 * @param  usHeight �����ڵĸ߶�
 * @retval ��
 */
void  fn_Lcd_Page3(void){
  //���ƴ����������
	Palette_Init(_LCD_SCAN_MODE ,CL_WHITE , CL_BLACK);
   
}





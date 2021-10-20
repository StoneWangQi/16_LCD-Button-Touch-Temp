#ifndef      __LCD_DRAW_BOOK_H
#define	     __LCD_DRAW_BOOK_H


#include "stm32f10x.h"
#include "LCD_book.h"
#include "fonts.h" 
#include "USART_book.h"
/******************************* ���� ILI934 ��ʾ��������ɫ ********************************/
#define      BACKGROUND		                BLACK   //Ĭ�ϱ�����ɫ

#define      WHITE		 		                  0xFFFF	   //��ɫ
#define      BLACK                         0x0000	   //��ɫ 
#define      GREY                          0xF7DE	   //��ɫ 
#define      BLUE                          0x001F	   //��ɫ 
#define      BLUE2                         0x051F	   //ǳ��ɫ 
#define      RED                           0xF800	   //��ɫ 
#define      MAGENTA                       0xF81F	   //����ɫ�����ɫ 
#define      GREEN                         0x07E0	   //��ɫ 
#define      CYAN                          0x7FFF	   //����ɫ����ɫ 
#define      YELLOW                        0xFFE0	   //��ɫ 
#define      BRED                          0xF81F
#define      GRED                          0xFFE0
#define      GBLUE                         0x07FF

//1110 1100 1101 0101 1010 1111
//11101 110101 10101
//GB565(Rh,Gh,Bh)       (((Rh&0xf8)<<8)|((Gh&0xfc)<<3)|((Bh&0xf8)>>3)) 
//GB888(RGB)            ((((RGB&0xf80000)) | ((RGB&0x00fc00)<<3)| ((RGB&0x0000f8)<<5))>>8)
#define      GB565(Rh,Gh,Bh)       (((Rh&0xf8)<<8)|((Gh&0xfc)<<3)|((Bh&0xf8)>>3)) 
#define      GB888(RGB)            ((((RGB&0xf80000)) | ((RGB&0x00fc00)<<3)| ((RGB&0x0000f8)<<5))>>8)

extern  sFONT *LCD_Currentfonts  ;  //Ӣ������
extern uint16_t CurrentForecolor ;//ǰ��ɫ
extern uint16_t CurrentBackColor ;//����ɫ

//================================ �ӿں���=======================================

void ILI9341_OpenWindow ( 
  uint16_t usX,     //usX �����ض�ɨ�跽���´��ڵ����X����
  uint16_t usY,     //usY �����ض�ɨ�跽���´��ڵ����Y����
  uint16_t usWidth, //usWidth �����ڵĿ�� 
  uint16_t usHeight //usHeight �����ڵĸ߶�
);
 
static void ILI9341_SetCursor ( 
  uint16_t usX, // usX �����ض�ɨ�跽���¹���X����
  uint16_t usY, // usY �����ض�ɨ�跽���¹���Y����
  uint16_t usW
);
 
static __inline void ILI9341_FillColor ( 
  uint32_t ulAmout_Point,// ulAmout_Point ��Ҫ�����ɫ�����ص������Ŀ
  uint16_t usColor       // usColor ����ɫ
);
 
void ILI9341_Clear ( 
  uint16_t usX,     //usX �����ض�ɨ�跽���´��ڵ����X����
  uint16_t usY,     //usY �����ض�ɨ�跽���´��ڵ����Y����
  uint16_t usWidth, //usWidth �����ڵĿ��
  uint16_t usHeight //usHeight �����ڵĸ߶� 
);

void LCD_ClearLine(uint16_t Line);
//================================ ��ɫ״̬����=======================================
void LCD_SetColors(uint16_t TextColor, uint16_t BackColor); //����LCD��ǰ��(����)��������ɫ,RGB565
void LCD_SetForeColor(uint16_t Color);//����LCD��ǰ��(����)��ɫ,RGB565
void LCD_SetBackColor(uint16_t Color); //����LCD�ı�����ɫ,RGB565
 
//================================ ��״����Ӧ�ú���=======================================
 
void ILI9341_SetPointPixel (
  uint16_t usX,  //usX �����ض�ɨ�跽���¸õ��X����
  uint16_t usY,  //usY �����ض�ɨ�跽���¸õ��Y����
  uint16_t usW   //usW �����ض�ɨ����
);
 
void ILI9341_DrawLine ( //�� ILI9341 ��ʾ����ʹ�� Bresenham �㷨���߶�
  uint16_t usX1,  //usX1 �����ض�ɨ�跽�����߶ε�һ���˵�X����
  uint16_t usY1,  //usY1 �����ض�ɨ�跽�����߶ε�һ���˵�Y����
  uint16_t usX2,  //usX2 �����ض�ɨ�跽�����߶ε���һ���˵�X����
  uint16_t usY2,  //usY2 �����ض�ɨ�跽�����߶ε���һ���˵�Y���� 
  uint16_t usW
);
 
void ILI9341_DrawRectangle ( 
  uint16_t usX_Start,     //X����
  uint16_t usY_Start,     //Y���� 
  uint16_t usWidth,       //X���
  uint16_t usHeight,      //Y���
  uint8_t ucFilled ,      //�������
  uint16_t usW            //����߿��
);

 
void ILI9341_DrawCircle (
  uint16_t usX_Center, //usX_Center �����ض�ɨ�跽����Բ�ĵ�X����
  uint16_t usY_Center, //usY_Center �����ض�ɨ�跽����Բ�ĵ�Y����
  uint16_t usRadius,   //usRadius��Բ�İ뾶����λ�����أ�
  uint8_t ucFilled ,  //ucFilled ��ѡ���Ƿ�����Բ
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

//================================ ������ɫ״̬����Ӧ�ú���=======================================
void LCD_SetTextColor(uint16_t Color);
void LCD_SetFont(sFONT *fonts);
sFONT *LCD_GetFont(void);
//================================ ���ֻ���Ӧ�ú���=======================================

//================================ Ӣ�����ֻ���Ӧ�ú���==================================== 
 
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

//================================ �������ֻ���Ӧ�ú���==================================== 

void ILI9341_DispChar_CH ( //�� ILI9341 ��ʾ������ʾһ�������ַ�
  uint16_t usX, 
  uint16_t usY, 
  uint16_t usChar 
);
 
void ILI9341_DispString_CH (  //�� ILI9341 ��ʾ������ʾ�����ַ���	
  uint16_t usX ,   
  uint16_t usY, 
  char * pStr 
);
 
void ILI9341_DispStringLine_EN_CH (  
  uint16_t line,    //line �����ض�ɨ�跽�����ַ�������ʼY����
  char * pStr       //pStr ��Ҫ��ʾ���ַ������׵�ַ
);
 
void ILI9341_DispString_EN_CH ( 	
  uint16_t usX ,  //���ض�ɨ�跽�����ַ�����ʼX����
  uint16_t usY,   //���ض�ɨ�跽�����ַ�����ʼY����
  char * pStr     //Ҫ��ʾ���ַ������׵�ַ
);
 
void ILI9341_DispString_EN_CH_YDir (  
  uint16_t usX,   //usX �����ض�ɨ�跽�����ַ�����ʼX����
  uint16_t usY,   //usY �����ض�ɨ�跽�����ַ�����ʼY���� 
  char * pStr     //Ҫ��ʾ����Ӣ���ַ������׵�ַ
);
  
/***********************��������****************************/
 
void ILI9341_zoomChar(
  uint16_t in_width,	//ԭʼ�ַ����
  uint16_t in_heig,		//ԭʼ�ַ��߶�
  uint16_t out_width,	//���ź���ַ����
  uint16_t out_heig,	//���ź���ַ��߶�
  uint8_t *in_ptr,	  //�ֿ�����ָ��	ע�⣺1pixel 1bit
  uint8_t *out_ptr,   //���ź���ַ����ָ�� ע��: 1pixel 8bit
  uint8_t en_cn		    //0ΪӢ�ģ�1Ϊ����	
); 	

 
void ILI9341_DrawChar_Ex(
  uint16_t usX,         //�ַ���ʾλ��x
  uint16_t usY,         //�ַ���ʾλ��y
  uint16_t Font_width, //�ַ����
  uint16_t Font_Height,  //�ַ��߶� 
  uint8_t *c,						//��ģ����
  uint16_t DrawModel		//�Ƿ�ɫ��ʾ
);
 
void ILI9341_DisplayStringEx(
  uint16_t x, 		      //�ַ���ʾλ��x
  uint16_t y, 				  //�ַ���ʾλ��y
  uint16_t Font_width,	//Ҫ��ʾ�������ȣ�Ӣ���ַ��ڴ˻�����/2��ע��Ϊż��
  uint16_t Font_Height,	//Ҫ��ʾ������߶ȣ�ע��Ϊż��
  uint8_t *ptr,					//��ʾ���ַ�����
  uint16_t DrawModel    //�Ƿ�ɫ��ʾ
);
 
void ILI9341_DisplayStringEx_YDir(
  uint16_t x, 		//�ַ���ʾλ��x
  uint16_t y, 				//�ַ���ʾλ��y
  uint16_t Font_width,	//Ҫ��ʾ�������ȣ�Ӣ���ַ��ڴ˻�����/2��ע��Ϊż��
  uint16_t Font_Height,	//Ҫ��ʾ������߶ȣ�ע��Ϊż��
  uint8_t *ptr,					//��ʾ���ַ�����
  uint16_t DrawModel  //�Ƿ�ɫ��ʾ
);

void Lcd_display_String(uint8_t *ptr);
  
#endif /* __BSP_ILI9341_ILI9341_H */


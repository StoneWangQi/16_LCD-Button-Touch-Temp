#ifndef __XPT2046_LCD_DEVICE_BOOK_H__
#define	__XPT2046_LCD_DEVICE_BOOK_H__

#include "stm32f10x.h"

#include "XPT2046_LCD_Function_book.h"
#include "XPT2046_LCD_book.h"
#include "LCD_book.h"
#include "LCD_Draw_book.h" 
#include "USART_book.h"



//======================================================��ɫ����=====================================================
/*
	LCD ��ɫ���룬CL_��Color�ļ�д
	16Bit�ɸ�λ����λ�� RRRR RGGG GGGB BBBB

	�����RGB �꽫24λ��RGBֵת��Ϊ16λ��ʽ��
	����windows�Ļ��ʳ��򣬵���༭��ɫ��ѡ���Զ�����ɫ�����Ի�õ�RGBֵ��

	�Ƽ�ʹ������ȡɫ���������㿴���Ľ�����ɫ��
*/
#define LCD_RGB_565 1 
#if LCD_RGB_565
  #define      RGB(Rh,Gh,Bh)       (((Rh&0xf8)<<8)|((Gh&0xfc)<<3)|((Bh&0xf8)>>3)) 
#else 
  #define      RGB(RGB)            ((((RGB&0xf80000)) | ((RGB&0x00fc00)<<3)| ((RGB&0x0000f8)<<5))>>8)
#endif


enum{
	CL_WHITE    = RGB(255,255,255),	/* ��ɫ */
	CL_BLACK    = RGB(  0,  0,  0),	/* ��ɫ */
	CL_RED      = RGB(255,	0,  0),	/* ��ɫ */
	CL_GREEN    = RGB(  0,255,  0),	/* ��ɫ */
	CL_BLUE     = RGB(  0,	0,255),	/* ��ɫ */
	CL_YELLOW   = RGB(255,255,  0),	/* ��ɫ */

	CL_GREY    = RGB( 98, 98, 98), 	/* ���ɫ */
	CL_GREY1		= RGB( 150, 150, 150), 	/* ǳ��ɫ */
	CL_GREY2		= RGB( 180, 180, 180), 	/* ǳ��ɫ */
	CL_GREY3		= RGB( 200, 200, 200), 	/* ��ǳ��ɫ */
	CL_GREY4		= RGB( 230, 230, 230), 	/* ��ǳ��ɫ */

	CL_BUTTON_GREY	= RGB( 195, 195, 195), /* WINDOWS ��ť�����ɫ */

	CL_MAGENTA      = RGB(255, 0, 255),	/* ����ɫ�����ɫ */
	CL_CYAN         = RGB( 0, 255, 255),	/* ����ɫ����ɫ */

	CL_BLUE1        = RGB(  0,  0, 240),		/* ����ɫ */
	CL_BLUE2        = RGB(  0,  0, 128),		/* ����ɫ */
	CL_BLUE3        = RGB(  68, 68, 255),		/* ǳ��ɫ1 */
	CL_BLUE4        = RGB(  0, 64, 128),		/* ǳ��ɫ1 */

	/* UI ���� Windows�ؼ�����ɫ */
	CL_BTN_FACE		  = RGB(236, 233, 216),	/* ��ť������ɫ(��) */
	CL_BOX_BORDER1	= RGB(172, 168,153),	/* �����������ɫ */
	CL_BOX_BORDER2	= RGB(255, 255,255),	/* �������Ӱ����ɫ */

	CL_MASK			    = 0x7FFF	/* RGB565��ɫ���룬�������ֱ���͸�� */
};
//======================================================��ʽ��Ĭ�ϻ��Ƕ��ƻ�=====================================================
 
typedef enum{
  Type_default = 0,
  Type_particularly ,
  Type_especially,           //��Ƥ
}Type_Style_Device;
//======================================================������ť�Ĳ���=====================================================
 
typedef struct{
  uint16_t start_x;   //������x��ʼ����  
  uint16_t start_y;   //������y��ʼ����
  uint16_t end_x;     //������x�������� 
  uint16_t end_y;     //������y��������
  uint32_t para;      //��ɫ��ť�б�ʾѡ�����ɫ���ʼ���״��ť�б�ʾѡ��Ļ�ˢ
  uint8_t touch_flag; //�������µı�־
  
  void (*draw_btn)(void * btn);     //������溯��
  void (*btn_command)(void * btn);  //��������ִ�к����������л���ɫ����ˢ
  
}Touch_Button;

//======================================================������ͼ���ܵĲ���=====================================================
 
typedef enum{
  LINE_SING_PIXCEL = 0 ,//��������
  LINE_2_PIXCEL,  //2������
  
  LINE_4_PIXCEL,  //4������
  
  LINE_6_PIXCEL,  //6������
  
  LINE_8_PIXCEL,  //8������
  
  LINE_16_PIXCEL, //16������
  
  LINE_20_PIXCEL, //20������
  
  LINE_WITH_CIRCLE,  //��������
    
  RUBBER,           //��Ƥ
  
  Draw_CLEAR,
  
}SHAPE;


/*��ˢ����*/
typedef struct
{
  uint32_t color;  //��ɫ����
  
  SHAPE  shape;  // ��״����
  
}Brush_Style;

extern Brush_Style brush;


//======================================================���ܲ���=====================================================
void Palette_Init(  
  uint8_t  _LCD_GramScan_Mode ,//��Ļ����
  uint16_t _Page_Color ,  //������ɫ
  uint16_t _Brush_Color   //������ɫ
);
 
void Touch_Button_Down(uint16_t x,uint16_t y);
void Touch_Button_Up(uint16_t x,uint16_t y);
void Draw_Trail(int16_t pre_x,int16_t pre_y,int16_t x,int16_t y,Brush_Style* brush);


//======================================================���ܲ���=====================================================
#define COLOR_BLOCK_WIDTH   40  //Ĭ�ϰ�ť�Ĵ�С
#define COLOR_BLOCK_HEIGHT  28  //Ĭ�ϰ�ť�ĸ߶� 

#define BUTTON_NUM  18          //Ĭ�ϰ�ť������ 
#define PALETTE_START_Y   0     //Ĭ�ϰ�ť������˳��
#define PALETTE_END_Y     LCD_Y_LENGTH  //�ò���������LCD_BOOK ����չ����

#define PALETT_Left 1
#if  PALETT_Left   //��ť�������
  #define BUTTON_START_X      0
  #define PALETTE_START_X   (COLOR_BLOCK_WIDTH*2+1)
  #define PALETTE_END_X     LCD_X_LENGTH

#else     //��ť�����ұߣ�(���ڴ�������ʱҲ���bug�����ڲ��Դ�������߽�)
  #define BUTTON_START_X      (LCD_X_LENGTH-2*COLOR_BLOCK_WIDTH)
  #define PALETTE_START_X   	0
  #define PALETTE_END_X     	(LCD_X_LENGTH-2*COLOR_BLOCK_WIDTH)

#endif



#endif /* __XPT2046_LCD_DEVICE_BOOK_H__ */


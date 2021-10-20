#ifndef __XPT2046_LCD_FUNCTION_BOOK_H__
#define	__XPT2046_LCD_FUNCTION_BOOK_H__


#include "stm32f10x.h"


typedef union {
  struct{
    unsigned char BIT0:1;unsigned char BIT1:1;unsigned char BIT2:1;unsigned char BIT3:1;
    unsigned char BIT4:1;unsigned char BIT5:1;unsigned char BIT6:1;unsigned char BIT7:1;
    //unsigned char BIT8:1;unsigned char BIT9:1;unsigned char BIT10:1;unsigned char BIT11:1;
    //unsigned char BIT12:1;unsigned char BIT13:1;unsigned char BIT14:1;unsigned char BIT15:1;
  }DATA_BIT;
  uint8_t DATA_BYTE;
}Per_XPT2046_LCD_type;


/*******************************����״̬�����*************************************/
typedef enum
{
	XPT2046_STATE_RELEASE  = 0,	//�����ͷ�
	XPT2046_STATE_WAITING,			//��������
	XPT2046_STATE_PRESSED,			//��������
}enumTouchState	;
 
#define TOUCH_NOT 		        0
#define TOUCH_PRESSED 				1
#define TOUCH_NOT_PRESSED			2


//����������ֵ
#define DURIATION_TIME				3
//�����ź���Ч��ƽ
#define             XPT2046_PENIRQ_ActiveLevel                     0
#define             XPT2046_PENIRQ_Read()                          GPIO_ReadInputDataBit ( XPT2046_PENIRQ_GPIO_PORT, XPT2046_PENIRQ_GPIO_PIN )

/// ���� XPT2046 ��ص��ⲿȫ�ֱ��� 
extern volatile uint8_t               ucXPT2046_TouchFlag;
extern volatile                       Per_XPT2046_LCD_type  XPT2046_LCD_flag;
    #define bXPT2046_LCD_10ms         XPT2046_LCD_flag.DATA_BIT.BIT0


/*******************************���������ж� *************************************/

typedef	struct{          //Һ������ṹ�� 		
	/*����ֵ��ʾ��������*/
   int16_t x;			//��¼���µĴ�������ֵ
   int16_t y; 	
	/*���ڼ�¼��������ʱ(����)����һ�δ���λ��*/
	 int16_t pre_x;		
   int16_t pre_y;	
} strType_XPT2046_Coordinate;   

typedef struct{         //У׼���ӽṹ�� 
	 float  An,  		 //ע:sizeof(long double) = 8
					Bn,     
					Cn,   
					Dn,    
					En,    
					Fn,     
					Divider;
} strType_XPT2046_Calibration;

typedef struct{         //У׼ϵ���ṹ�壨����ʹ�ã�
	 float dX_X,  			 
					dX_Y,     
					dX,   
					dY_X,    
					dY_Y,    
					dY;
} strType_XPT2046_TouchPara;

/// ���� XPT2046 ��ص��ⲿȫ�ֱ��� 
extern strType_XPT2046_TouchPara      strXPT2046_TouchPara[];
//  ����У��4��У���
extern strType_XPT2046_Coordinate     strCrossCoordinate[4];
extern strType_XPT2046_Coordinate     strScreenSample[4];


 
//======================================================================
////======================== ����״̬����ͨѶ ========================== 
//======================================================================
/** ״̬����
  * @brief  ���������״̬��
  * @retval ����״̬
	* �÷���ֵΪ����ֵ֮һ��
  * @arg TOUCH_PRESSED :��������
  * @arg TOUCH_NOT_PRESSED :�޴���
  */
uint8_t XPT2046_TouchDetect(void);
uint8_t XPT2046_ReadAdc_Smooth_XY ( strType_XPT2046_Coordinate * pScreenCoordinate );
/**
  * @brief  XPT2046 ������У׼��ʼ��
	* @param	LCD_Mode��ָ��ҪУ������Һ��ɨ��ģʽ�Ĳ���
  * @note  ���������ú���Һ��ģʽ����ΪLCD_Mode
  * @retval У׼���
  *   �÷���ֵΪ����ֵ֮һ:
  *     @arg 1 :У׼�ɹ�
  *     @arg 0 :У׼ʧ��
  */
 
void XPT2046_Touch_Calibrate_SetStand4Pint(void);
/**
* @brief  XPT2046 ������У׼
* @param	LCD_Mode��ָ��ҪУ������Һ��ɨ��ģʽ�Ĳ���
* @note  ���������ú���Һ��ģʽ����ΪLCD_Mode
* @retval У׼���
*   �÷���ֵΪ����ֵ֮һ��
*     @arg 1 :У׼�ɹ�
*     @arg 0 :У׼ʧ��
*/

uint8_t XPT2046_Touch_Calibrate ( uint8_t LCD_Mode );
/**
  * @brief  ��ȡ XPT2046 �����㣨У׼�󣩵�����
  * @param  pDisplayCoordinate ����ָ���Ż�ȡ���Ĵ���������
  * @param  pTouchPara������У׼ϵ��
  * @retval ��ȡ���
	*   �÷���ֵΪ����ֵ֮һ��
  *     @arg 1 :��ȡ�ɹ�
  *     @arg 0 :��ȡʧ��
  */
uint8_t XPT2046_Get_TouchedPoint ( strType_XPT2046_Coordinate * pDisplayCoordinate, strType_XPT2046_TouchPara * pTouchPara );
/**
  * @brief   �����������µ�ʱ�����ñ�����
  * @param  touch������������Ľṹ��
  * @note  ���ڱ������б�д�Լ��Ĵ������´���Ӧ��
  * @retval ��
  */
void XPT2046_TouchDown(strType_XPT2046_Coordinate * touch);
/**
  * @brief   �������ͷŵ�ʱ�����ñ�����
  * @param  touch������������Ľṹ��
  * @note  ���ڱ������б�д�Լ��Ĵ����ͷŴ���Ӧ��
  * @retval ��
  */
void XPT2046_TouchUp(strType_XPT2046_Coordinate * touch);
/**
	* @brief   ��⵽�����ж�ʱ���õĴ�����,ͨ��������tp_down ��tp_up�㱨������
	*	@note 	 ��������Ҫ��whileѭ���ﱻ���ã�Ҳ��ʹ�ö�ʱ����ʱ����
	*			���磬����ÿ��5ms����һ�Σ�������ֵ��DURIATION_TIME������Ϊ2������ÿ�������Լ��100���㡣
	*						����XPT2046_TouchDown��XPT2046_TouchUp�����б�д�Լ��Ĵ���Ӧ��
	* @param   none
	* @retval  none
	*/
void XPT2046_TouchEvenHandler(void );
#endif /* __BSP_TOUCH_H */

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


/*******************************触摸状态机相关*************************************/
typedef enum
{
	XPT2046_STATE_RELEASE  = 0,	//触摸释放
	XPT2046_STATE_WAITING,			//触摸按下
	XPT2046_STATE_PRESSED,			//触摸按下
}enumTouchState	;
 
#define TOUCH_NOT 		        0
#define TOUCH_PRESSED 				1
#define TOUCH_NOT_PRESSED			2


//触摸消抖阈值
#define DURIATION_TIME				3
//触屏信号有效电平
#define             XPT2046_PENIRQ_ActiveLevel                     0
#define             XPT2046_PENIRQ_Read()                          GPIO_ReadInputDataBit ( XPT2046_PENIRQ_GPIO_PORT, XPT2046_PENIRQ_GPIO_PIN )

/// 声明 XPT2046 相关的外部全局变量 
extern volatile uint8_t               ucXPT2046_TouchFlag;
extern volatile                       Per_XPT2046_LCD_type  XPT2046_LCD_flag;
    #define bXPT2046_LCD_10ms         XPT2046_LCD_flag.DATA_BIT.BIT0


/*******************************触摸坐标判断 *************************************/

typedef	struct{          //液晶坐标结构体 		
	/*负数值表示无新数据*/
   int16_t x;			//记录最新的触摸参数值
   int16_t y; 	
	/*用于记录连续触摸时(长按)的上一次触摸位置*/
	 int16_t pre_x;		
   int16_t pre_y;	
} strType_XPT2046_Coordinate;   

typedef struct{         //校准因子结构体 
	 float  An,  		 //注:sizeof(long double) = 8
					Bn,     
					Cn,   
					Dn,    
					En,    
					Fn,     
					Divider;
} strType_XPT2046_Calibration;

typedef struct{         //校准系数结构体（最终使用）
	 float dX_X,  			 
					dX_Y,     
					dX,   
					dY_X,    
					dY_Y,    
					dY;
} strType_XPT2046_TouchPara;

/// 声明 XPT2046 相关的外部全局变量 
extern strType_XPT2046_TouchPara      strXPT2046_TouchPara[];
//  触摸校验4点校验点
extern strType_XPT2046_Coordinate     strCrossCoordinate[4];
extern strType_XPT2046_Coordinate     strScreenSample[4];


 
//======================================================================
////======================== 触摸状态机制通讯 ========================== 
//======================================================================
/** 状态机制
  * @brief  触摸屏检测状态机
  * @retval 触摸状态
	* 该返回值为以下值之一：
  * @arg TOUCH_PRESSED :触摸按下
  * @arg TOUCH_NOT_PRESSED :无触摸
  */
uint8_t XPT2046_TouchDetect(void);
uint8_t XPT2046_ReadAdc_Smooth_XY ( strType_XPT2046_Coordinate * pScreenCoordinate );
/**
  * @brief  XPT2046 触摸屏校准初始化
	* @param	LCD_Mode：指定要校正哪种液晶扫描模式的参数
  * @note  本函数调用后会把液晶模式设置为LCD_Mode
  * @retval 校准结果
  *   该返回值为以下值之一:
  *     @arg 1 :校准成功
  *     @arg 0 :校准失败
  */
 
void XPT2046_Touch_Calibrate_SetStand4Pint(void);
/**
* @brief  XPT2046 触摸屏校准
* @param	LCD_Mode：指定要校正哪种液晶扫描模式的参数
* @note  本函数调用后会把液晶模式设置为LCD_Mode
* @retval 校准结果
*   该返回值为以下值之一：
*     @arg 1 :校准成功
*     @arg 0 :校准失败
*/

uint8_t XPT2046_Touch_Calibrate ( uint8_t LCD_Mode );
/**
  * @brief  获取 XPT2046 触摸点（校准后）的坐标
  * @param  pDisplayCoordinate ：该指针存放获取到的触摸点坐标
  * @param  pTouchPara：坐标校准系数
  * @retval 获取情况
	*   该返回值为以下值之一：
  *     @arg 1 :获取成功
  *     @arg 0 :获取失败
  */
uint8_t XPT2046_Get_TouchedPoint ( strType_XPT2046_Coordinate * pDisplayCoordinate, strType_XPT2046_TouchPara * pTouchPara );
/**
  * @brief   触摸屏被按下的时候会调用本函数
  * @param  touch包含触摸坐标的结构体
  * @note  请在本函数中编写自己的触摸按下处理应用
  * @retval 无
  */
void XPT2046_TouchDown(strType_XPT2046_Coordinate * touch);
/**
  * @brief   触摸屏释放的时候会调用本函数
  * @param  touch包含触摸坐标的结构体
  * @note  请在本函数中编写自己的触摸释放处理应用
  * @retval 无
  */
void XPT2046_TouchUp(strType_XPT2046_Coordinate * touch);
/**
	* @brief   检测到触摸中断时调用的处理函数,通过它调用tp_down 和tp_up汇报触摸点
	*	@note 	 本函数需要在while循环里被调用，也可使用定时器定时调用
	*			例如，可以每隔5ms调用一次，消抖阈值宏DURIATION_TIME可设置为2，这样每秒最多可以检测100个点。
	*						可在XPT2046_TouchDown及XPT2046_TouchUp函数中编写自己的触摸应用
	* @param   none
	* @retval  none
	*/
void XPT2046_TouchEvenHandler(void );
#endif /* __BSP_TOUCH_H */

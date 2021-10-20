#include "XPT2046_LCD_Function_book.h"
#include "XPT2046_LCD_book.h"
#include "LCD_book.h"
#include "XPT2046_LCD_Device_book.h"
#include "USART_book.h"


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
  
volatile          uint8_t      ucXPT2046_TouchFlag;
volatile                       Per_XPT2046_LCD_type  XPT2046_LCD_flag;

uint8_t XPT2046_TouchDetect(void){
  if(bXPT2046_LCD_10ms == 0){return TOUCH_NOT ;}
  bXPT2046_LCD_10ms = 0;
  static enumTouchState touch_state = XPT2046_STATE_RELEASE;
  static uint8_t count_Touch = 0;
  uint8_t detectResult = TOUCH_NOT_PRESSED;
  switch(touch_state){
    case XPT2046_STATE_RELEASE: 
      if(XPT2046_PENIRQ_Read() == XPT2046_PENIRQ_ActiveLevel){
        touch_state = XPT2046_STATE_WAITING;
        detectResult = TOUCH_NOT_PRESSED;
      }else{
        touch_state = XPT2046_STATE_RELEASE;
        detectResult = TOUCH_NOT_PRESSED;
      }
      break;
    case XPT2046_STATE_WAITING:
      if(XPT2046_PENIRQ_Read() == XPT2046_PENIRQ_ActiveLevel){
        if(count_Touch++>DURIATION_TIME){
          count_Touch = 0;
          touch_state = XPT2046_STATE_PRESSED;
          detectResult = TOUCH_PRESSED;
        }else{
          touch_state = XPT2046_STATE_WAITING;
					detectResult =	 TOUCH_NOT_PRESSED;	
        }
      }else{
        count_Touch = 0;
        touch_state = XPT2046_STATE_RELEASE; 
				detectResult = TOUCH_NOT_PRESSED;
      }
      break;
    case XPT2046_STATE_PRESSED:
      if(XPT2046_PENIRQ_Read() == XPT2046_PENIRQ_ActiveLevel){
        touch_state = XPT2046_STATE_PRESSED;
        detectResult = TOUCH_PRESSED;
      }else{
        touch_state = XPT2046_STATE_RELEASE;
			  detectResult = TOUCH_NOT_PRESSED;
      }
      break;  
  }
  return detectResult;
}

//======================================================================
////======================== 触摸坐标采集 ===============================
//======================================================================
 
#if   0                 //注意：校正较精准，但是相对复杂，速度较慢
/**
  * @brief  在触摸 XPT2046 屏幕时获取一组坐标的AD值，并对该坐标进行滤波
  * @param  无
  * @retval 滤波之后的坐标AD值
  */

static uint8_t XPT2046_ReadAdc_Smooth_XY ( strType_XPT2046_Coordinate * pScreenCoordinate ){
	uint8_t ucCount = 0;
	int16_t sAD_X, sAD_Y;
	int16_t sBufferArray [ 2 ] [ 9 ] = { { 0 }, { 0 } };  //坐标X和Y进行9次采样
	int32_t lAverage  [ 3 ], lDifference [ 3 ];
	do{		   
		XPT2046_ReadAdc_XY ( & sAD_X, & sAD_Y );
		sBufferArray [ 0 ] [ ucCount ] = sAD_X;  
		sBufferArray [ 1 ] [ ucCount ] = sAD_Y;		
		ucCount ++; 			 
	}while ( ( XPT2046_EXTI_Read() == XPT2046_EXTI_ActiveLevel ) && ( ucCount < 9 ) ); 	//用户点击触摸屏时即TP_INT_IN信号为低 并且 ucCount<9*/
	 
	/*如果触笔弹起*/
	if ( XPT2046_EXTI_Read() != XPT2046_EXTI_ActiveLevel )
		ucXPT2046_TouchFlag = 0;			//触摸中断标志复位		
	/* 如果成功采样9次,进行滤波 */ 
	if ( ucCount == 9 ){  
		/* 为减少运算量,分别分3组取平均值 */
		lAverage  [ 0 ] = ( sBufferArray [ 0 ] [ 0 ] + sBufferArray [ 0 ] [ 1 ] + sBufferArray [ 0 ] [ 2 ] ) / 3;
		lAverage  [ 1 ] = ( sBufferArray [ 0 ] [ 3 ] + sBufferArray [ 0 ] [ 4 ] + sBufferArray [ 0 ] [ 5 ] ) / 3;
		lAverage  [ 2 ] = ( sBufferArray [ 0 ] [ 6 ] + sBufferArray [ 0 ] [ 7 ] + sBufferArray [ 0 ] [ 8 ] ) / 3;		
		/* 计算3组数据的差值 */
		lDifference [ 0 ] = lAverage  [ 0 ]-lAverage  [ 1 ];
		lDifference [ 1 ] = lAverage  [ 1 ]-lAverage  [ 2 ];
		lDifference [ 2 ] = lAverage  [ 2 ]-lAverage  [ 0 ];
		/* 对上述差值取绝对值 */
		lDifference [ 0 ] = lDifference [ 0 ]>0?lDifference [ 0 ]: ( -lDifference [ 0 ] );
		lDifference [ 1 ] = lDifference [ 1 ]>0?lDifference [ 1 ]: ( -lDifference [ 1 ] );
		lDifference [ 2 ] = lDifference [ 2 ]>0?lDifference [ 2 ]: ( -lDifference [ 2 ] );	
		/* 判断绝对差值是否都超过差值门限，如果这3个绝对差值都超过门限值，则判定这次采样点为野点,抛弃采样点，差值门限取为2 */
		if (  lDifference [ 0 ] > XPT2046_THRESHOLD_CalDiff  &&  lDifference [ 1 ] > XPT2046_THRESHOLD_CalDiff  &&  lDifference [ 2 ] > XPT2046_THRESHOLD_CalDiff  ) 
			return 0;
			
		/* 计算它们的平均值，同时赋值给strScreenCoordinate */ 
		if ( lDifference [ 0 ] < lDifference [ 1 ] ){
			if ( lDifference [ 2 ] < lDifference [ 0 ] ) 
				pScreenCoordinate ->x = ( lAverage  [ 0 ] + lAverage  [ 2 ] ) / 2;
			else 
				pScreenCoordinate ->x = ( lAverage  [ 0 ] + lAverage  [ 1 ] ) / 2;	
		}
		else if ( lDifference [ 2 ] < lDifference [ 1 ] ) 
			pScreenCoordinate -> x = ( lAverage  [ 0 ] + lAverage  [ 2 ] ) / 2;
		else 
			pScreenCoordinate ->x = ( lAverage  [ 1 ] + lAverage  [ 2 ] ) / 2;

		/* 同上，计算Y的平均值 */
		lAverage  [ 0 ] = ( sBufferArray [ 1 ] [ 0 ] + sBufferArray [ 1 ] [ 1 ] + sBufferArray [ 1 ] [ 2 ] ) / 3;
		lAverage  [ 1 ] = ( sBufferArray [ 1 ] [ 3 ] + sBufferArray [ 1 ] [ 4 ] + sBufferArray [ 1 ] [ 5 ] ) / 3;
		lAverage  [ 2 ] = ( sBufferArray [ 1 ] [ 6 ] + sBufferArray [ 1 ] [ 7 ] + sBufferArray [ 1 ] [ 8 ] ) / 3;
		
		lDifference [ 0 ] = lAverage  [ 0 ] - lAverage  [ 1 ];
		lDifference [ 1 ] = lAverage  [ 1 ] - lAverage  [ 2 ];
		lDifference [ 2 ] = lAverage  [ 2 ] - lAverage  [ 0 ];
		
		/* 取绝对值 */
		lDifference [ 0 ] = lDifference [ 0 ] > 0 ? lDifference [ 0 ] : ( - lDifference [ 0 ] );
		lDifference [ 1 ] = lDifference [ 1 ] > 0 ? lDifference [ 1 ] : ( - lDifference [ 1 ] );
		lDifference [ 2 ] = lDifference [ 2 ] > 0 ? lDifference [ 2 ] : ( - lDifference [ 2 ] );
			
		if ( lDifference [ 0 ] > XPT2046_THRESHOLD_CalDiff && lDifference [ 1 ] > XPT2046_THRESHOLD_CalDiff && lDifference [ 2 ] > XPT2046_THRESHOLD_CalDiff ) 
			return 0;     
		if ( lDifference [ 0 ] < lDifference [ 1 ] ){
			if ( lDifference [ 2 ] < lDifference [ 0 ] ) 
				pScreenCoordinate ->y =  ( lAverage  [ 0 ] + lAverage  [ 2 ] ) / 2;
			else 
				pScreenCoordinate ->y =  ( lAverage  [ 0 ] + lAverage  [ 1 ] ) / 2;	
		}
		else if ( lDifference [ 2 ] < lDifference [ 1 ] ) 
			pScreenCoordinate ->y =  ( lAverage  [ 0 ] + lAverage  [ 2 ] ) / 2;
		else
			pScreenCoordinate ->y =  ( lAverage  [ 1 ] + lAverage  [ 2 ] ) / 2;		
		return 1;		
	}
	else if ( ucCount > 1 ){
		pScreenCoordinate ->x = sBufferArray [ 0 ] [ 0 ];
		pScreenCoordinate ->y = sBufferArray [ 1 ] [ 0 ];	
		return 0;		
	}  	
	return 0; 	
}


#else     //注意：画板应用实例专用,不是很精准，但是简单，速度比较快   
uint8_t XPT2046_ReadAdc_Smooth_XY ( strType_XPT2046_Coordinate * pScreenCoordinate ){
	uint8_t ucCount = 0, i;
	int16_t sAD_X, sAD_Y;
	int16_t sBufferArray [ 2 ] [ 10 ] = { { 0 },{ 0 } };  //坐标X和Y进行多次采样
	//存储采样中的最小值、最大值
	int32_t lX_Min, lX_Max, lY_Min, lY_Max;
  
  if ( XPT2046_PENIRQ_Read() != XPT2046_PENIRQ_ActiveLevel ){return 0;}
	/* 循环采样10次 */ 
	do{		  
		XPT2046_ReadAdc_XY ( & sAD_X, & sAD_Y );  	
		sBufferArray [ 0 ] [ ucCount ] = sAD_X;  
		sBufferArray [ 1 ] [ ucCount ] = sAD_Y;		
		ucCount ++;  		
	}while ( ( XPT2046_PENIRQ_Read() == XPT2046_PENIRQ_ActiveLevel ) && ( ucCount < 10 ) );
  //用户点击触摸屏时即TP_INT_IN信号为低 并且 ucCount<10
	
	/*如果触笔弹起*/
	if ( XPT2046_PENIRQ_Read() != XPT2046_PENIRQ_ActiveLevel )
		ucXPT2046_TouchFlag = 0;			//中断标志复位
	
	/*如果成功采样10个样本*/
	if ( ucCount ==10 ){
		lX_Max = lX_Min = sBufferArray [ 0 ] [ 0 ];
		lY_Max = lY_Min = sBufferArray [ 1 ] [ 0 ];       	
		for ( i = 1; i < 10; i ++ ){
			if ( sBufferArray[ 0 ] [ i ] < lX_Min )
				lX_Min = sBufferArray [ 0 ] [ i ];
			else if ( sBufferArray [ 0 ] [ i ] > lX_Max )
				lX_Max = sBufferArray [ 0 ] [ i ];
		}	
		for ( i = 1; i < 10; i ++ ){
			if ( sBufferArray [ 1 ] [ i ] < lY_Min )
				lY_Min = sBufferArray [ 1 ] [ i ];			
			else if ( sBufferArray [ 1 ] [ i ] > lY_Max )
				lY_Max = sBufferArray [ 1 ] [ i ];
		}
		
		/*去除最小值和最大值之后求平均值*/
		pScreenCoordinate ->x =  ( sBufferArray [ 0 ] [ 0 ] + sBufferArray [ 0 ] [ 1 ] + sBufferArray [ 0 ] [ 2 ] + sBufferArray [ 0 ] [ 3 ] + sBufferArray [ 0 ] [ 4 ] + 
		                           sBufferArray [ 0 ] [ 5 ] + sBufferArray [ 0 ] [ 6 ] + sBufferArray [ 0 ] [ 7 ] + sBufferArray [ 0 ] [ 8 ] + sBufferArray [ 0 ] [ 9 ] - lX_Min-lX_Max ) >> 3;		
		pScreenCoordinate ->y =  ( sBufferArray [ 1 ] [ 0 ] + sBufferArray [ 1 ] [ 1 ] + sBufferArray [ 1 ] [ 2 ] + sBufferArray [ 1 ] [ 3 ] + sBufferArray [ 1 ] [ 4 ] + 
		                           sBufferArray [ 1 ] [ 5 ] + sBufferArray [ 1 ] [ 6 ] + sBufferArray [ 1 ] [ 7 ] + sBufferArray [ 1 ] [ 8 ] + sBufferArray [ 1 ] [ 9 ] - lY_Min-lY_Max ) >> 3; 
		return 1;			
	}   	
	return 0;    	
}

#endif


//======================================================================
////======================== 触摸坐标校准算法 ===============================
//======================================================================
 
/**
  * @brief  计算 XPT2046 触摸坐标校正系数（注意：只有在LCD和触摸屏间的误差角度非常小时,才能运用下面公式）
  * @param  pDisplayCoordinate ：屏幕人为显示的已知坐标
  * @param  pstrScreenSample ：对已知坐标点触摸时 XPT2046 产生的坐标
  * @param  pCalibrationFactor ：根据人为设定坐标和采样回来的坐标计算出来的屏幕触摸校正系数
  * @retval 计算状态
	*   该返回值为以下值之一：
  *     @arg 1 :计算成功
  *     @arg 0 :计算失败
  */

/******************************* 定义 XPT2046 全局变量 ***************************/
//默认触摸参数，不同的屏幕稍有差异，可重新调用触摸校准函数获取
strType_XPT2046_TouchPara strXPT2046_TouchPara[] = { 	
 -0.006464,   -0.073259,  280.358032,    0.074878,    0.002052,   -6.545977,//扫描方式0
	0.086314,    0.001891,  -12.836658,   -0.003722,   -0.065799,  254.715714,//扫描方式1
	0.002782,    0.061522,  -11.595689,    0.083393,    0.005159,  -15.650089,//扫描方式2
	0.089743,   -0.000289,  -20.612209,   -0.001374,    0.064451,  -16.054003,//扫描方式3
	0.000767,   -0.068258,  250.891769,   -0.085559,   -0.000195,  334.747650,//扫描方式4
 -0.084744,    0.000047,  323.163147,   -0.002109,   -0.066371,  260.985809,//扫描方式5
 -0.001848,    0.066984,  -12.807136,   -0.084858,   -0.000805,  333.395386,//扫描方式6
 -0.085470,   -0.000876,  334.023163,   -0.003390,    0.064725,   -6.211169,//扫描方式7
};

static uint8_t XPT2046_Calculate_CalibrationFactor ( 
  strType_XPT2046_Coordinate * pDisplayCoordinate, 
  strType_XPT2046_Coordinate * pScreenSample, 
  strType_XPT2046_Calibration * pCalibrationFactor 
){
	uint8_t ucRet = 1;
	/* K＝ ( X0－X2 )  ( Y1－Y2 )－ ( X1－X2 )  ( Y0－Y2 ) */
	pCalibrationFactor -> Divider =  ( ( pScreenSample [ 0 ] .x - pScreenSample [ 2 ] .x ) *  ( pScreenSample [ 1 ] .y - pScreenSample [ 2 ] .y ) ) - 
									                 ( ( pScreenSample [ 1 ] .x - pScreenSample [ 2 ] .x ) *  ( pScreenSample [ 0 ] .y - pScreenSample [ 2 ] .y ) ) ;
		
	if (  pCalibrationFactor -> Divider == 0  )
		ucRet = 0;
	else{
		/* A＝ (  ( XD0－XD2 )  ( Y1－Y2 )－ ( XD1－XD2 )  ( Y0－Y2 ) )／K	*/
		pCalibrationFactor -> An =  ( ( pDisplayCoordinate [ 0 ] .x - pDisplayCoordinate [ 2 ] .x ) *  ( pScreenSample [ 1 ] .y - pScreenSample [ 2 ] .y ) ) - 
								                ( ( pDisplayCoordinate [ 1 ] .x - pDisplayCoordinate [ 2 ] .x ) *  ( pScreenSample [ 0 ] .y - pScreenSample [ 2 ] .y ) );
		
		/* B＝ (  ( X0－X2 )  ( XD1－XD2 )－ ( XD0－XD2 )  ( X1－X2 ) )／K	*/
		pCalibrationFactor -> Bn =  ( ( pScreenSample [ 0 ] .x - pScreenSample [ 2 ] .x ) *  ( pDisplayCoordinate [ 1 ] .x - pDisplayCoordinate [ 2 ] .x ) ) - 
								                ( ( pDisplayCoordinate [ 0 ] .x - pDisplayCoordinate [ 2 ] .x ) *  ( pScreenSample [ 1 ] .x - pScreenSample [ 2 ] .x ) );
		
		/* C＝ ( Y0 ( X2XD1－X1XD2 )+Y1 ( X0XD2－X2XD0 )+Y2 ( X1XD0－X0XD1 ) )／K */
		pCalibrationFactor -> Cn =  ( pScreenSample [ 2 ] .x * pDisplayCoordinate [ 1 ] .x - pScreenSample [ 1 ] .x * pDisplayCoordinate [ 2 ] .x ) * pScreenSample [ 0 ] .y +
								                ( pScreenSample [ 0 ] .x * pDisplayCoordinate [ 2 ] .x - pScreenSample [ 2 ] .x * pDisplayCoordinate [ 0 ] .x ) * pScreenSample [ 1 ] .y +
								                ( pScreenSample [ 1 ] .x * pDisplayCoordinate [ 0 ] .x - pScreenSample [ 0 ] .x * pDisplayCoordinate [ 1 ] .x ) * pScreenSample [ 2 ] .y ;
		
		/* D＝ (  ( YD0－YD2 )  ( Y1－Y2 )－ ( YD1－YD2 )  ( Y0－Y2 ) )／K	*/
		pCalibrationFactor -> Dn =  ( ( pDisplayCoordinate [ 0 ] .y - pDisplayCoordinate [ 2 ] .y ) *  ( pScreenSample [ 1 ] .y - pScreenSample [ 2 ] .y ) ) - 
								                ( ( pDisplayCoordinate [ 1 ] .y - pDisplayCoordinate [ 2 ] .y ) *  ( pScreenSample [ 0 ] .y - pScreenSample [ 2 ] .y ) ) ;
		
		/* E＝ (  ( X0－X2 )  ( YD1－YD2 )－ ( YD0－YD2 )  ( X1－X2 ) )／K	*/
		pCalibrationFactor -> En =  ( ( pScreenSample [ 0 ] .x - pScreenSample [ 2 ] .x ) *  ( pDisplayCoordinate [ 1 ] .y - pDisplayCoordinate [ 2 ] .y ) ) - 
								                ( ( pDisplayCoordinate [ 0 ] .y - pDisplayCoordinate [ 2 ] .y ) *  ( pScreenSample [ 1 ] .x - pScreenSample [ 2 ] .x ) ) ;
		
		
		/* F＝ ( Y0 ( X2YD1－X1YD2 )+Y1 ( X0YD2－X2YD0 )+Y2 ( X1YD0－X0YD1 ) )／K */
		pCalibrationFactor -> Fn =  ( pScreenSample [ 2 ] .x * pDisplayCoordinate [ 1 ] .y - pScreenSample [ 1 ] .x * pDisplayCoordinate [ 2 ] .y ) * pScreenSample [ 0 ] .y +
								                ( pScreenSample [ 0 ] .x * pDisplayCoordinate [ 2 ] .y - pScreenSample [ 2 ] .x * pDisplayCoordinate [ 0 ] .y ) * pScreenSample [ 1 ] .y +
								                ( pScreenSample [ 1 ] .x * pDisplayCoordinate [ 0 ] .y - pScreenSample [ 0 ] .x * pDisplayCoordinate [ 1 ] .y ) * pScreenSample [ 2 ] .y;
			
	}
	return ucRet;
}


//======================================================================
////======================== 触摸坐标校准执行 ===============================
//======================================================================
/**
  * @brief  XPT2046 触摸屏校准初始化
	* @param	LCD_Mode：指定要校正哪种液晶扫描模式的参数
  * @note  本函数调用后会把液晶模式设置为LCD_Mode
  * @retval 校准结果
  *   该返回值为以下值之一:
  *     @arg 1 :校准成功
  *     @arg 0 :校准失败
  */
strType_XPT2046_Coordinate     strCrossCoordinate[4];
strType_XPT2046_Coordinate     strScreenSample[4];

void XPT2046_Touch_Calibrate_SetStand4Pint(void){ 
   /* 设定“十”字交叉点的坐标 */ 
		strCrossCoordinate [0].x = LCD_X_LENGTH >> 2;
		strCrossCoordinate[0].y = LCD_Y_LENGTH >> 2;
		
		strCrossCoordinate[1].x = strCrossCoordinate[0].x;
		strCrossCoordinate[1].y = ( LCD_Y_LENGTH * 3 ) >> 2;
		
		strCrossCoordinate[2].x = ( LCD_X_LENGTH * 3 ) >> 2;
		strCrossCoordinate[2].y = strCrossCoordinate[1].y;
		
		strCrossCoordinate[3].x = strCrossCoordinate[2].x;
		strCrossCoordinate[3].y = strCrossCoordinate[0].y;	
  
}


/**
* @brief  XPT2046 触摸屏校准
* @param	LCD_Mode：指定要校正哪种液晶扫描模式的参数
* @note  本函数调用后会把液晶模式设置为LCD_Mode
* @retval 校准结果
*   该返回值为以下值之一：
*     @arg 1 :校准成功
*     @arg 0 :校准失败
*/

uint8_t XPT2046_Touch_Calibrate ( uint8_t LCD_Mode ){
		uint8_t i;
		uint16_t usTest_x = 0, usTest_y = 0, usGap_x = 0, usGap_y = 0;	
 
	  strType_XPT2046_Calibration CalibrationFactor; 
		
		XPT2046_Calculate_CalibrationFactor ( strCrossCoordinate, strScreenSample, & CalibrationFactor ) ;  	 //用原始参数计算出 原始参数与坐标的转换系数
		
		if ( CalibrationFactor.Divider == 0 ) goto Failure; //进行校验完成
		
		usTest_x = ( ( CalibrationFactor.An * strScreenSample[3].x ) + ( CalibrationFactor.Bn * strScreenSample[3].y ) + CalibrationFactor.Cn ) / CalibrationFactor.Divider;		//取一个点计算X值	 
		usTest_y = ( ( CalibrationFactor.Dn * strScreenSample[3].x ) + ( CalibrationFactor.En * strScreenSample[3].y ) + CalibrationFactor.Fn ) / CalibrationFactor.Divider;    //取一个点计算Y值
		
		usGap_x = ( usTest_x > strCrossCoordinate[3].x ) ? ( usTest_x - strCrossCoordinate[3].x ) : ( strCrossCoordinate[3].x - usTest_x );   //实际X坐标与计算坐标的绝对差
		usGap_y = ( usTest_y > strCrossCoordinate[3].y ) ? ( usTest_y - strCrossCoordinate[3].y ) : ( strCrossCoordinate[3].y - usTest_y );   //实际Y坐标与计算坐标的绝对差
		
    if ( ( usGap_x > 15 ) || ( usGap_y > 15 ) ) goto Failure;       //可以通过修改这两个值的大小来调整精度    
		

    /* 校准系数为全局变量 */ 
		strXPT2046_TouchPara[LCD_Mode].dX_X = ( CalibrationFactor.An * 1.0 ) / CalibrationFactor.Divider;
		strXPT2046_TouchPara[LCD_Mode].dX_Y = ( CalibrationFactor.Bn * 1.0 ) / CalibrationFactor.Divider;
		strXPT2046_TouchPara[LCD_Mode].dX   = ( CalibrationFactor.Cn * 1.0 ) / CalibrationFactor.Divider;
		
		strXPT2046_TouchPara[LCD_Mode].dY_X = ( CalibrationFactor.Dn * 1.0 ) / CalibrationFactor.Divider;
		strXPT2046_TouchPara[LCD_Mode].dY_Y = ( CalibrationFactor.En * 1.0 ) / CalibrationFactor.Divider;
		strXPT2046_TouchPara[LCD_Mode].dY   = ( CalibrationFactor.Fn * 1.0 ) / CalibrationFactor.Divider;
		
		#if 1	//输出调试信息，注意要先初始化串口
		{
					float * ulHeadAddres ;

					/* 打印校校准系数 */ 
					printf ( "显示模式【%d】校准系数如下：", LCD_Mode);
					
					ulHeadAddres = ( float* ) ( & strXPT2046_TouchPara[LCD_Mode] );
					
					for ( i = 0; i < 6; i ++ )
					{					
						printf ( "%12f,", *ulHeadAddres++  );			
					}	
					printf("\r\n");
			}
		#endif	
	
	return 1;    
Failure:
	return 0; 	
}

/**
  * @brief  获取 XPT2046 触摸点（校准后）的坐标
  * @param  pDisplayCoordinate ：该指针存放获取到的触摸点坐标
  * @param  pTouchPara：坐标校准系数
  * @retval 获取情况
	*   该返回值为以下值之一：
  *     @arg 1 :获取成功
  *     @arg 0 :获取失败
  */
uint8_t XPT2046_Get_TouchedPoint ( strType_XPT2046_Coordinate * pDisplayCoordinate, strType_XPT2046_TouchPara * pTouchPara )
{
	uint8_t ucRet = 1;           //若正常，则返回0
	
	strType_XPT2046_Coordinate strScreenCoordinate; 
	

  if ( XPT2046_ReadAdc_Smooth_XY ( & strScreenCoordinate ) )
  {    
		pDisplayCoordinate ->x = ( ( pTouchPara[LCD_SCAN_MODE].dX_X * strScreenCoordinate.x ) + ( pTouchPara[LCD_SCAN_MODE].dX_Y * strScreenCoordinate.y ) + pTouchPara[LCD_SCAN_MODE].dX );        
		pDisplayCoordinate ->y = ( ( pTouchPara[LCD_SCAN_MODE].dY_X * strScreenCoordinate.x ) + ( pTouchPara[LCD_SCAN_MODE].dY_Y * strScreenCoordinate.y ) + pTouchPara[LCD_SCAN_MODE].dY );

  }
	 
	else ucRet = 0;            //如果获取的触点信息有误，则返回0
		
  return ucRet;
} 

//--------------------------------按键动作------------------------------------
//----------------------------------------------------------------------------

/**
  * @brief   触摸屏被按下的时候会调用本函数
  * @param  touch包含触摸坐标的结构体
  * @note  请在本函数中编写自己的触摸按下处理应用
  * @retval 无
  */
void XPT2046_TouchDown(strType_XPT2046_Coordinate * touch){
  //如果是负数 表示之前已经处理过完毕
  if((touch->pre_x == -1)&&(touch->pre_y==-1)){
    return;
  }
  /***在此处编写自己的触摸按下处理应用***/
   Touch_Button_Down(touch->x,touch->y);
  
  /*处理描绘轨迹*/
  Draw_Trail(touch->pre_x,touch->pre_y,touch->x,touch->y,&brush);
  
  /***在上面编写自己的触摸按下处理应用***/
}


/**
  * @brief   触摸屏释放的时候会调用本函数
  * @param  touch包含触摸坐标的结构体
  * @note  请在本函数中编写自己的触摸释放处理应用
  * @retval 无
  */
void XPT2046_TouchUp(strType_XPT2046_Coordinate * touch) 
{
	//若为负值表示之前已处理过
	if(touch->pre_x == -1 && touch->pre_x == -1)
		return;
		
	/***在此处编写自己的触摸释放处理应用***/
  
	/*处理触摸画板的选择按钮*/
  Touch_Button_Up(touch->pre_x,touch->pre_y);	
	
	/***在上面编写自己的触摸释放处理应用***/
}


/**
	* @brief   检测到触摸中断时调用的处理函数,通过它调用tp_down 和tp_up汇报触摸点
	*	@note 	 本函数需要在while循环里被调用，也可使用定时器定时调用
	*			例如，可以每隔5ms调用一次，消抖阈值宏DURIATION_TIME可设置为2，这样每秒最多可以检测100个点。
	*						可在XPT2046_TouchDown及XPT2046_TouchUp函数中编写自己的触摸应用
	* @param   none
	* @retval  none
	*/
void XPT2046_TouchEvenHandler(void ){
	  static strType_XPT2046_Coordinate cinfo={-1,-1,-1,-1};
		if(XPT2046_TouchDetect() == TOUCH_PRESSED){
			printf("\nIdentify the screen by pressing\n" );
			
			//获取触摸坐标
			XPT2046_Get_TouchedPoint(&cinfo,strXPT2046_TouchPara);
			
			//输出调试信息到串口
			printf("x=%d,y=%d",cinfo.x,cinfo.y);
			
			//调用触摸被按下时的处理函数，可在该函数编写自己的触摸按下处理过程
			XPT2046_TouchDown(&cinfo);
			
			/*更新触摸信息到pre xy*/
			cinfo.pre_x = cinfo.x; cinfo.pre_y = cinfo.y;  

		}
		else {
        if(
            (cinfo.x != -1)||
            (cinfo.y != -1)||
            (cinfo.pre_x != -1)||
            (cinfo.pre_y != -1) 
          ){
            printf(" Identify the screen by loosen\n" );
            
            //调用触摸被释放时的处理函数，可在该函数编写自己的触摸释放处理过程
            XPT2046_TouchUp(&cinfo); 
            
            /*触笔释放，把 xy 重置为负*/
            cinfo.x = -1;
            cinfo.y = -1; 
            cinfo.pre_x = -1;
            cinfo.pre_y = -1;
        }
		}

}




#include "XPT2046_LCD_Function_book.h"
#include "XPT2046_LCD_book.h"
#include "LCD_book.h"
#include "XPT2046_LCD_Device_book.h"
#include "USART_book.h"


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
////======================== ��������ɼ� ===============================
//======================================================================
 
#if   0                 //ע�⣺У���Ͼ�׼��������Ը��ӣ��ٶȽ���
/**
  * @brief  �ڴ��� XPT2046 ��Ļʱ��ȡһ�������ADֵ�����Ը���������˲�
  * @param  ��
  * @retval �˲�֮�������ADֵ
  */

static uint8_t XPT2046_ReadAdc_Smooth_XY ( strType_XPT2046_Coordinate * pScreenCoordinate ){
	uint8_t ucCount = 0;
	int16_t sAD_X, sAD_Y;
	int16_t sBufferArray [ 2 ] [ 9 ] = { { 0 }, { 0 } };  //����X��Y����9�β���
	int32_t lAverage  [ 3 ], lDifference [ 3 ];
	do{		   
		XPT2046_ReadAdc_XY ( & sAD_X, & sAD_Y );
		sBufferArray [ 0 ] [ ucCount ] = sAD_X;  
		sBufferArray [ 1 ] [ ucCount ] = sAD_Y;		
		ucCount ++; 			 
	}while ( ( XPT2046_EXTI_Read() == XPT2046_EXTI_ActiveLevel ) && ( ucCount < 9 ) ); 	//�û����������ʱ��TP_INT_IN�ź�Ϊ�� ���� ucCount<9*/
	 
	/*������ʵ���*/
	if ( XPT2046_EXTI_Read() != XPT2046_EXTI_ActiveLevel )
		ucXPT2046_TouchFlag = 0;			//�����жϱ�־��λ		
	/* ����ɹ�����9��,�����˲� */ 
	if ( ucCount == 9 ){  
		/* Ϊ����������,�ֱ��3��ȡƽ��ֵ */
		lAverage  [ 0 ] = ( sBufferArray [ 0 ] [ 0 ] + sBufferArray [ 0 ] [ 1 ] + sBufferArray [ 0 ] [ 2 ] ) / 3;
		lAverage  [ 1 ] = ( sBufferArray [ 0 ] [ 3 ] + sBufferArray [ 0 ] [ 4 ] + sBufferArray [ 0 ] [ 5 ] ) / 3;
		lAverage  [ 2 ] = ( sBufferArray [ 0 ] [ 6 ] + sBufferArray [ 0 ] [ 7 ] + sBufferArray [ 0 ] [ 8 ] ) / 3;		
		/* ����3�����ݵĲ�ֵ */
		lDifference [ 0 ] = lAverage  [ 0 ]-lAverage  [ 1 ];
		lDifference [ 1 ] = lAverage  [ 1 ]-lAverage  [ 2 ];
		lDifference [ 2 ] = lAverage  [ 2 ]-lAverage  [ 0 ];
		/* ��������ֵȡ����ֵ */
		lDifference [ 0 ] = lDifference [ 0 ]>0?lDifference [ 0 ]: ( -lDifference [ 0 ] );
		lDifference [ 1 ] = lDifference [ 1 ]>0?lDifference [ 1 ]: ( -lDifference [ 1 ] );
		lDifference [ 2 ] = lDifference [ 2 ]>0?lDifference [ 2 ]: ( -lDifference [ 2 ] );	
		/* �жϾ��Բ�ֵ�Ƿ񶼳�����ֵ���ޣ������3�����Բ�ֵ����������ֵ�����ж���β�����ΪҰ��,���������㣬��ֵ����ȡΪ2 */
		if (  lDifference [ 0 ] > XPT2046_THRESHOLD_CalDiff  &&  lDifference [ 1 ] > XPT2046_THRESHOLD_CalDiff  &&  lDifference [ 2 ] > XPT2046_THRESHOLD_CalDiff  ) 
			return 0;
			
		/* �������ǵ�ƽ��ֵ��ͬʱ��ֵ��strScreenCoordinate */ 
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

		/* ͬ�ϣ�����Y��ƽ��ֵ */
		lAverage  [ 0 ] = ( sBufferArray [ 1 ] [ 0 ] + sBufferArray [ 1 ] [ 1 ] + sBufferArray [ 1 ] [ 2 ] ) / 3;
		lAverage  [ 1 ] = ( sBufferArray [ 1 ] [ 3 ] + sBufferArray [ 1 ] [ 4 ] + sBufferArray [ 1 ] [ 5 ] ) / 3;
		lAverage  [ 2 ] = ( sBufferArray [ 1 ] [ 6 ] + sBufferArray [ 1 ] [ 7 ] + sBufferArray [ 1 ] [ 8 ] ) / 3;
		
		lDifference [ 0 ] = lAverage  [ 0 ] - lAverage  [ 1 ];
		lDifference [ 1 ] = lAverage  [ 1 ] - lAverage  [ 2 ];
		lDifference [ 2 ] = lAverage  [ 2 ] - lAverage  [ 0 ];
		
		/* ȡ����ֵ */
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


#else     //ע�⣺����Ӧ��ʵ��ר��,���Ǻܾ�׼�����Ǽ򵥣��ٶȱȽϿ�   
uint8_t XPT2046_ReadAdc_Smooth_XY ( strType_XPT2046_Coordinate * pScreenCoordinate ){
	uint8_t ucCount = 0, i;
	int16_t sAD_X, sAD_Y;
	int16_t sBufferArray [ 2 ] [ 10 ] = { { 0 },{ 0 } };  //����X��Y���ж�β���
	//�洢�����е���Сֵ�����ֵ
	int32_t lX_Min, lX_Max, lY_Min, lY_Max;
  
  if ( XPT2046_PENIRQ_Read() != XPT2046_PENIRQ_ActiveLevel ){return 0;}
	/* ѭ������10�� */ 
	do{		  
		XPT2046_ReadAdc_XY ( & sAD_X, & sAD_Y );  	
		sBufferArray [ 0 ] [ ucCount ] = sAD_X;  
		sBufferArray [ 1 ] [ ucCount ] = sAD_Y;		
		ucCount ++;  		
	}while ( ( XPT2046_PENIRQ_Read() == XPT2046_PENIRQ_ActiveLevel ) && ( ucCount < 10 ) );
  //�û����������ʱ��TP_INT_IN�ź�Ϊ�� ���� ucCount<10
	
	/*������ʵ���*/
	if ( XPT2046_PENIRQ_Read() != XPT2046_PENIRQ_ActiveLevel )
		ucXPT2046_TouchFlag = 0;			//�жϱ�־��λ
	
	/*����ɹ�����10������*/
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
		
		/*ȥ����Сֵ�����ֵ֮����ƽ��ֵ*/
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
////======================== ��������У׼�㷨 ===============================
//======================================================================
 
/**
  * @brief  ���� XPT2046 ��������У��ϵ����ע�⣺ֻ����LCD�ʹ�����������Ƕȷǳ�Сʱ,�����������湫ʽ��
  * @param  pDisplayCoordinate ����Ļ��Ϊ��ʾ����֪����
  * @param  pstrScreenSample ������֪����㴥��ʱ XPT2046 ����������
  * @param  pCalibrationFactor ��������Ϊ�趨����Ͳ�����������������������Ļ����У��ϵ��
  * @retval ����״̬
	*   �÷���ֵΪ����ֵ֮һ��
  *     @arg 1 :����ɹ�
  *     @arg 0 :����ʧ��
  */

/******************************* ���� XPT2046 ȫ�ֱ��� ***************************/
//Ĭ�ϴ�����������ͬ����Ļ���в��죬�����µ��ô���У׼������ȡ
strType_XPT2046_TouchPara strXPT2046_TouchPara[] = { 	
 -0.006464,   -0.073259,  280.358032,    0.074878,    0.002052,   -6.545977,//ɨ�跽ʽ0
	0.086314,    0.001891,  -12.836658,   -0.003722,   -0.065799,  254.715714,//ɨ�跽ʽ1
	0.002782,    0.061522,  -11.595689,    0.083393,    0.005159,  -15.650089,//ɨ�跽ʽ2
	0.089743,   -0.000289,  -20.612209,   -0.001374,    0.064451,  -16.054003,//ɨ�跽ʽ3
	0.000767,   -0.068258,  250.891769,   -0.085559,   -0.000195,  334.747650,//ɨ�跽ʽ4
 -0.084744,    0.000047,  323.163147,   -0.002109,   -0.066371,  260.985809,//ɨ�跽ʽ5
 -0.001848,    0.066984,  -12.807136,   -0.084858,   -0.000805,  333.395386,//ɨ�跽ʽ6
 -0.085470,   -0.000876,  334.023163,   -0.003390,    0.064725,   -6.211169,//ɨ�跽ʽ7
};

static uint8_t XPT2046_Calculate_CalibrationFactor ( 
  strType_XPT2046_Coordinate * pDisplayCoordinate, 
  strType_XPT2046_Coordinate * pScreenSample, 
  strType_XPT2046_Calibration * pCalibrationFactor 
){
	uint8_t ucRet = 1;
	/* K�� ( X0��X2 )  ( Y1��Y2 )�� ( X1��X2 )  ( Y0��Y2 ) */
	pCalibrationFactor -> Divider =  ( ( pScreenSample [ 0 ] .x - pScreenSample [ 2 ] .x ) *  ( pScreenSample [ 1 ] .y - pScreenSample [ 2 ] .y ) ) - 
									                 ( ( pScreenSample [ 1 ] .x - pScreenSample [ 2 ] .x ) *  ( pScreenSample [ 0 ] .y - pScreenSample [ 2 ] .y ) ) ;
		
	if (  pCalibrationFactor -> Divider == 0  )
		ucRet = 0;
	else{
		/* A�� (  ( XD0��XD2 )  ( Y1��Y2 )�� ( XD1��XD2 )  ( Y0��Y2 ) )��K	*/
		pCalibrationFactor -> An =  ( ( pDisplayCoordinate [ 0 ] .x - pDisplayCoordinate [ 2 ] .x ) *  ( pScreenSample [ 1 ] .y - pScreenSample [ 2 ] .y ) ) - 
								                ( ( pDisplayCoordinate [ 1 ] .x - pDisplayCoordinate [ 2 ] .x ) *  ( pScreenSample [ 0 ] .y - pScreenSample [ 2 ] .y ) );
		
		/* B�� (  ( X0��X2 )  ( XD1��XD2 )�� ( XD0��XD2 )  ( X1��X2 ) )��K	*/
		pCalibrationFactor -> Bn =  ( ( pScreenSample [ 0 ] .x - pScreenSample [ 2 ] .x ) *  ( pDisplayCoordinate [ 1 ] .x - pDisplayCoordinate [ 2 ] .x ) ) - 
								                ( ( pDisplayCoordinate [ 0 ] .x - pDisplayCoordinate [ 2 ] .x ) *  ( pScreenSample [ 1 ] .x - pScreenSample [ 2 ] .x ) );
		
		/* C�� ( Y0 ( X2XD1��X1XD2 )+Y1 ( X0XD2��X2XD0 )+Y2 ( X1XD0��X0XD1 ) )��K */
		pCalibrationFactor -> Cn =  ( pScreenSample [ 2 ] .x * pDisplayCoordinate [ 1 ] .x - pScreenSample [ 1 ] .x * pDisplayCoordinate [ 2 ] .x ) * pScreenSample [ 0 ] .y +
								                ( pScreenSample [ 0 ] .x * pDisplayCoordinate [ 2 ] .x - pScreenSample [ 2 ] .x * pDisplayCoordinate [ 0 ] .x ) * pScreenSample [ 1 ] .y +
								                ( pScreenSample [ 1 ] .x * pDisplayCoordinate [ 0 ] .x - pScreenSample [ 0 ] .x * pDisplayCoordinate [ 1 ] .x ) * pScreenSample [ 2 ] .y ;
		
		/* D�� (  ( YD0��YD2 )  ( Y1��Y2 )�� ( YD1��YD2 )  ( Y0��Y2 ) )��K	*/
		pCalibrationFactor -> Dn =  ( ( pDisplayCoordinate [ 0 ] .y - pDisplayCoordinate [ 2 ] .y ) *  ( pScreenSample [ 1 ] .y - pScreenSample [ 2 ] .y ) ) - 
								                ( ( pDisplayCoordinate [ 1 ] .y - pDisplayCoordinate [ 2 ] .y ) *  ( pScreenSample [ 0 ] .y - pScreenSample [ 2 ] .y ) ) ;
		
		/* E�� (  ( X0��X2 )  ( YD1��YD2 )�� ( YD0��YD2 )  ( X1��X2 ) )��K	*/
		pCalibrationFactor -> En =  ( ( pScreenSample [ 0 ] .x - pScreenSample [ 2 ] .x ) *  ( pDisplayCoordinate [ 1 ] .y - pDisplayCoordinate [ 2 ] .y ) ) - 
								                ( ( pDisplayCoordinate [ 0 ] .y - pDisplayCoordinate [ 2 ] .y ) *  ( pScreenSample [ 1 ] .x - pScreenSample [ 2 ] .x ) ) ;
		
		
		/* F�� ( Y0 ( X2YD1��X1YD2 )+Y1 ( X0YD2��X2YD0 )+Y2 ( X1YD0��X0YD1 ) )��K */
		pCalibrationFactor -> Fn =  ( pScreenSample [ 2 ] .x * pDisplayCoordinate [ 1 ] .y - pScreenSample [ 1 ] .x * pDisplayCoordinate [ 2 ] .y ) * pScreenSample [ 0 ] .y +
								                ( pScreenSample [ 0 ] .x * pDisplayCoordinate [ 2 ] .y - pScreenSample [ 2 ] .x * pDisplayCoordinate [ 0 ] .y ) * pScreenSample [ 1 ] .y +
								                ( pScreenSample [ 1 ] .x * pDisplayCoordinate [ 0 ] .y - pScreenSample [ 0 ] .x * pDisplayCoordinate [ 1 ] .y ) * pScreenSample [ 2 ] .y;
			
	}
	return ucRet;
}


//======================================================================
////======================== ��������У׼ִ�� ===============================
//======================================================================
/**
  * @brief  XPT2046 ������У׼��ʼ��
	* @param	LCD_Mode��ָ��ҪУ������Һ��ɨ��ģʽ�Ĳ���
  * @note  ���������ú���Һ��ģʽ����ΪLCD_Mode
  * @retval У׼���
  *   �÷���ֵΪ����ֵ֮һ:
  *     @arg 1 :У׼�ɹ�
  *     @arg 0 :У׼ʧ��
  */
strType_XPT2046_Coordinate     strCrossCoordinate[4];
strType_XPT2046_Coordinate     strScreenSample[4];

void XPT2046_Touch_Calibrate_SetStand4Pint(void){ 
   /* �趨��ʮ���ֽ��������� */ 
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
* @brief  XPT2046 ������У׼
* @param	LCD_Mode��ָ��ҪУ������Һ��ɨ��ģʽ�Ĳ���
* @note  ���������ú���Һ��ģʽ����ΪLCD_Mode
* @retval У׼���
*   �÷���ֵΪ����ֵ֮һ��
*     @arg 1 :У׼�ɹ�
*     @arg 0 :У׼ʧ��
*/

uint8_t XPT2046_Touch_Calibrate ( uint8_t LCD_Mode ){
		uint8_t i;
		uint16_t usTest_x = 0, usTest_y = 0, usGap_x = 0, usGap_y = 0;	
 
	  strType_XPT2046_Calibration CalibrationFactor; 
		
		XPT2046_Calculate_CalibrationFactor ( strCrossCoordinate, strScreenSample, & CalibrationFactor ) ;  	 //��ԭʼ��������� ԭʼ�����������ת��ϵ��
		
		if ( CalibrationFactor.Divider == 0 ) goto Failure; //����У�����
		
		usTest_x = ( ( CalibrationFactor.An * strScreenSample[3].x ) + ( CalibrationFactor.Bn * strScreenSample[3].y ) + CalibrationFactor.Cn ) / CalibrationFactor.Divider;		//ȡһ�������Xֵ	 
		usTest_y = ( ( CalibrationFactor.Dn * strScreenSample[3].x ) + ( CalibrationFactor.En * strScreenSample[3].y ) + CalibrationFactor.Fn ) / CalibrationFactor.Divider;    //ȡһ�������Yֵ
		
		usGap_x = ( usTest_x > strCrossCoordinate[3].x ) ? ( usTest_x - strCrossCoordinate[3].x ) : ( strCrossCoordinate[3].x - usTest_x );   //ʵ��X�������������ľ��Բ�
		usGap_y = ( usTest_y > strCrossCoordinate[3].y ) ? ( usTest_y - strCrossCoordinate[3].y ) : ( strCrossCoordinate[3].y - usTest_y );   //ʵ��Y�������������ľ��Բ�
		
    if ( ( usGap_x > 15 ) || ( usGap_y > 15 ) ) goto Failure;       //����ͨ���޸�������ֵ�Ĵ�С����������    
		

    /* У׼ϵ��Ϊȫ�ֱ��� */ 
		strXPT2046_TouchPara[LCD_Mode].dX_X = ( CalibrationFactor.An * 1.0 ) / CalibrationFactor.Divider;
		strXPT2046_TouchPara[LCD_Mode].dX_Y = ( CalibrationFactor.Bn * 1.0 ) / CalibrationFactor.Divider;
		strXPT2046_TouchPara[LCD_Mode].dX   = ( CalibrationFactor.Cn * 1.0 ) / CalibrationFactor.Divider;
		
		strXPT2046_TouchPara[LCD_Mode].dY_X = ( CalibrationFactor.Dn * 1.0 ) / CalibrationFactor.Divider;
		strXPT2046_TouchPara[LCD_Mode].dY_Y = ( CalibrationFactor.En * 1.0 ) / CalibrationFactor.Divider;
		strXPT2046_TouchPara[LCD_Mode].dY   = ( CalibrationFactor.Fn * 1.0 ) / CalibrationFactor.Divider;
		
		#if 1	//���������Ϣ��ע��Ҫ�ȳ�ʼ������
		{
					float * ulHeadAddres ;

					/* ��ӡУУ׼ϵ�� */ 
					printf ( "��ʾģʽ��%d��У׼ϵ�����£�", LCD_Mode);
					
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
  * @brief  ��ȡ XPT2046 �����㣨У׼�󣩵�����
  * @param  pDisplayCoordinate ����ָ���Ż�ȡ���Ĵ���������
  * @param  pTouchPara������У׼ϵ��
  * @retval ��ȡ���
	*   �÷���ֵΪ����ֵ֮һ��
  *     @arg 1 :��ȡ�ɹ�
  *     @arg 0 :��ȡʧ��
  */
uint8_t XPT2046_Get_TouchedPoint ( strType_XPT2046_Coordinate * pDisplayCoordinate, strType_XPT2046_TouchPara * pTouchPara )
{
	uint8_t ucRet = 1;           //���������򷵻�0
	
	strType_XPT2046_Coordinate strScreenCoordinate; 
	

  if ( XPT2046_ReadAdc_Smooth_XY ( & strScreenCoordinate ) )
  {    
		pDisplayCoordinate ->x = ( ( pTouchPara[LCD_SCAN_MODE].dX_X * strScreenCoordinate.x ) + ( pTouchPara[LCD_SCAN_MODE].dX_Y * strScreenCoordinate.y ) + pTouchPara[LCD_SCAN_MODE].dX );        
		pDisplayCoordinate ->y = ( ( pTouchPara[LCD_SCAN_MODE].dY_X * strScreenCoordinate.x ) + ( pTouchPara[LCD_SCAN_MODE].dY_Y * strScreenCoordinate.y ) + pTouchPara[LCD_SCAN_MODE].dY );

  }
	 
	else ucRet = 0;            //�����ȡ�Ĵ�����Ϣ�����򷵻�0
		
  return ucRet;
} 

//--------------------------------��������------------------------------------
//----------------------------------------------------------------------------

/**
  * @brief   �����������µ�ʱ�����ñ�����
  * @param  touch������������Ľṹ��
  * @note  ���ڱ������б�д�Լ��Ĵ������´���Ӧ��
  * @retval ��
  */
void XPT2046_TouchDown(strType_XPT2046_Coordinate * touch){
  //����Ǹ��� ��ʾ֮ǰ�Ѿ����������
  if((touch->pre_x == -1)&&(touch->pre_y==-1)){
    return;
  }
  /***�ڴ˴���д�Լ��Ĵ������´���Ӧ��***/
   Touch_Button_Down(touch->x,touch->y);
  
  /*�������켣*/
  Draw_Trail(touch->pre_x,touch->pre_y,touch->x,touch->y,&brush);
  
  /***�������д�Լ��Ĵ������´���Ӧ��***/
}


/**
  * @brief   �������ͷŵ�ʱ�����ñ�����
  * @param  touch������������Ľṹ��
  * @note  ���ڱ������б�д�Լ��Ĵ����ͷŴ���Ӧ��
  * @retval ��
  */
void XPT2046_TouchUp(strType_XPT2046_Coordinate * touch) 
{
	//��Ϊ��ֵ��ʾ֮ǰ�Ѵ�����
	if(touch->pre_x == -1 && touch->pre_x == -1)
		return;
		
	/***�ڴ˴���д�Լ��Ĵ����ͷŴ���Ӧ��***/
  
	/*�������������ѡ��ť*/
  Touch_Button_Up(touch->pre_x,touch->pre_y);	
	
	/***�������д�Լ��Ĵ����ͷŴ���Ӧ��***/
}


/**
	* @brief   ��⵽�����ж�ʱ���õĴ�������,ͨ��������tp_down ��tp_up�㱨������
	*	@note 	 ��������Ҫ��whileѭ���ﱻ���ã�Ҳ��ʹ�ö�ʱ����ʱ����
	*			���磬����ÿ��5ms����һ�Σ�������ֵ��DURIATION_TIME������Ϊ2������ÿ�������Լ��100���㡣
	*						����XPT2046_TouchDown��XPT2046_TouchUp�����б�д�Լ��Ĵ���Ӧ��
	* @param   none
	* @retval  none
	*/
void XPT2046_TouchEvenHandler(void ){
	  static strType_XPT2046_Coordinate cinfo={-1,-1,-1,-1};
		if(XPT2046_TouchDetect() == TOUCH_PRESSED){
			printf("\nIdentify the screen by pressing\n" );
			
			//��ȡ��������
			XPT2046_Get_TouchedPoint(&cinfo,strXPT2046_TouchPara);
			
			//���������Ϣ������
			printf("x=%d,y=%d",cinfo.x,cinfo.y);
			
			//���ô���������ʱ�Ĵ������������ڸú�����д�Լ��Ĵ������´�������
			XPT2046_TouchDown(&cinfo);
			
			/*���´�����Ϣ��pre xy*/
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
            
            //���ô������ͷ�ʱ�Ĵ������������ڸú�����д�Լ��Ĵ����ͷŴ�������
            XPT2046_TouchUp(&cinfo); 
            
            /*�����ͷţ��� xy ����Ϊ��*/
            cinfo.x = -1;
            cinfo.y = -1; 
            cinfo.pre_x = -1;
            cinfo.pre_y = -1;
        }
		}

}



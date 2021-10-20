#include "XPT2046_LCD_book.h"
#include "LCD_Draw_book.h" 
#include "USART_book.h"
#include "SPI_book.h"

//======================================================================
////======================== 模拟SPI通讯 ===============================
//======================================================================
 
/**
  * @brief  XPT2046_GPIO_Init 初始化函数
  * @param  无
  * @retval 无
  */	
void  XPT2046_GPIO_Init(void){
   GPIO_InitTypeDef  GPIO_InitStructure;
	

  /* 开启GPIO时钟 */
  RCC_APB2PeriphClockCmd ( XPT2046_SPI_GPIO_CLK|XPT2046_PENIRQ_GPIO_CLK, ENABLE );

  /* 模拟SPI GPIO初始化 */          
  GPIO_InitStructure.GPIO_Pin=XPT2046_SPI_CLK_PIN;
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz ;	  
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
  GPIO_Init(XPT2046_SPI_CLK_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = XPT2046_SPI_MOSI_PIN;
  GPIO_Init(XPT2046_SPI_MOSI_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = XPT2046_SPI_MISO_PIN; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;      
  GPIO_Init(XPT2046_SPI_MISO_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = XPT2046_SPI_CS_PIN; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      
  GPIO_Init(XPT2046_SPI_CS_PORT, &GPIO_InitStructure); 
   
  /* 拉低片选，选择XPT2046 */
  XPT2046_CS_ENABLE();		
								
	//触摸屏触摸信号指示引脚，不使用中断
  GPIO_InitStructure.GPIO_Pin = XPT2046_PENIRQ_GPIO_PIN;       
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 // 上拉输入
  GPIO_Init(XPT2046_PENIRQ_GPIO_PORT, &GPIO_InitStructure);

  
}

/**
  * @brief  用于 XPT2046 的简单微秒级延时函数
  * @param  nCount ：延时计数值，单位为微妙
  * @retval 无
  */	
static void XPT2046_DelayUS ( __IO uint32_t ulCount ){
	uint32_t i;
	for ( i = 0; i < ulCount; i ++ )	{
		uint8_t uc = 12;     //设置值为12，大约延1微秒  
	  while ( uc -- );     //延1微秒	
	}
}

/**
  * @brief  XPT2046 的写入命令
  * @param  ucCmd ：命令
  *   该参数为以下值之一：
  *     @arg 0x90 :通道Y+的选择控制字
  *     @arg 0xd0 :通道X+的选择控制字
  * @retval 无
  */
static void XPT2046_WriteCMD(uint8_t ucCmd){
  uint8_t i ;
  //片选开始 
  XPT2046_CS_ENABLE();//引脚的出示状态
  XPT2046_MOSI_0();
  XPT2046_CLK_LOW();
  //产生8个时钟发送数据 
  for(i=0 ;i<8 ;i++ ){
     //指令要提前准备好
     ((ucCmd &(0x80>>i)) ==0 ) ? XPT2046_MOSI_0() : XPT2046_MOSI_1();
     XPT2046_DelayUS(5);
     XPT2046_CLK_HIGH();//时钟准备
     XPT2046_DelayUS(5);   
     XPT2046_CLK_LOW();
  }
}


/**
  * @brief  XPT2046 的读取命令 
  * @param  无 
  * @retval 读取到的数据 
  */
//读取函数 AD　为１２位的
static uint16_t XPT2046_ReadCMD(void){
  uint8_t i ;
  uint16_t usTemp = 0; 
  XPT2046_MOSI_0();
	XPT2046_CLK_HIGH();
	for ( i=0;i<12;i++ ){
    usTemp<<=1;
		XPT2046_CLK_LOW();    	
    (XPT2046_MISO() == Bit_SET )?(usTemp |= 0x01):(usTemp |= 0x00);		
		XPT2046_CLK_HIGH();		
	}
  XPT2046_CS_DISABLE();
  return usTemp;
}

/**
  * @brief  对 XPT2046 选择一个模拟通道后，
  * 启动ADC，并返回ADC采样结果
  * @param  ucChannel
  *   该参数为以下值之一：
  *     @arg 0x90 :通道Y+的选择控制字
  *     @arg 0xd0 :通道X+的选择控制字
  * @retval 该通道的ADC采样结果
  */
static uint16_t XPT2046_ReadAdc( uint8_t ucChannel ){
  XPT2046_WriteCMD(ucChannel);
  return XPT2046_ReadCMD();
}

/**
  * @brief  读取 XPT2046 的X通道和Y通道的AD值（12 bit，最大是4096）
  * @param  sX_Ad ：存放X通道AD值的地址
  * @param  sY_Ad ：存放Y通道AD值的地址
  * @retval 无
  */
void XPT2046_ReadAdc_XY ( int16_t * sX_Ad, int16_t * sY_Ad ){ 
	*sX_Ad = XPT2046_ReadAdc(XPT2046_CHANNEL_X);
	XPT2046_DelayUS(1); 
	*sY_Ad = XPT2046_ReadAdc (XPT2046_CHANNEL_Y); 
  
  //#ifdef _USEUSARTSHOW //#endif
  printf("触摸点 X: 0x%x  Y:0x%x \n" ,*sX_Ad ,*sY_Ad );
  //#endif
  
}







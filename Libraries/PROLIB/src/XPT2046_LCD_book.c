#include "XPT2046_LCD_book.h"
#include "LCD_Draw_book.h" 
#include "USART_book.h"
#include "SPI_book.h"

//======================================================================
////======================== ģ��SPIͨѶ ===============================
//======================================================================
 
/**
  * @brief  XPT2046_GPIO_Init ��ʼ������
  * @param  ��
  * @retval ��
  */	
void  XPT2046_GPIO_Init(void){
   GPIO_InitTypeDef  GPIO_InitStructure;
	

  /* ����GPIOʱ�� */
  RCC_APB2PeriphClockCmd ( XPT2046_SPI_GPIO_CLK|XPT2046_PENIRQ_GPIO_CLK, ENABLE );

  /* ģ��SPI GPIO��ʼ�� */          
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
   
  /* ����Ƭѡ��ѡ��XPT2046 */
  XPT2046_CS_ENABLE();		
								
	//�����������ź�ָʾ���ţ���ʹ���ж�
  GPIO_InitStructure.GPIO_Pin = XPT2046_PENIRQ_GPIO_PIN;       
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 // ��������
  GPIO_Init(XPT2046_PENIRQ_GPIO_PORT, &GPIO_InitStructure);

  
}

/**
  * @brief  ���� XPT2046 �ļ�΢�뼶��ʱ����
  * @param  nCount ����ʱ����ֵ����λΪ΢��
  * @retval ��
  */	
static void XPT2046_DelayUS ( __IO uint32_t ulCount ){
	uint32_t i;
	for ( i = 0; i < ulCount; i ++ )	{
		uint8_t uc = 12;     //����ֵΪ12����Լ��1΢��  
	  while ( uc -- );     //��1΢��	
	}
}

/**
  * @brief  XPT2046 ��д������
  * @param  ucCmd ������
  *   �ò���Ϊ����ֵ֮һ��
  *     @arg 0x90 :ͨ��Y+��ѡ�������
  *     @arg 0xd0 :ͨ��X+��ѡ�������
  * @retval ��
  */
static void XPT2046_WriteCMD(uint8_t ucCmd){
  uint8_t i ;
  //Ƭѡ��ʼ 
  XPT2046_CS_ENABLE();//���ŵĳ�ʾ״̬
  XPT2046_MOSI_0();
  XPT2046_CLK_LOW();
  //����8��ʱ�ӷ������� 
  for(i=0 ;i<8 ;i++ ){
     //ָ��Ҫ��ǰ׼����
     ((ucCmd &(0x80>>i)) ==0 ) ? XPT2046_MOSI_0() : XPT2046_MOSI_1();
     XPT2046_DelayUS(5);
     XPT2046_CLK_HIGH();//ʱ��׼��
     XPT2046_DelayUS(5);   
     XPT2046_CLK_LOW();
  }
}


/**
  * @brief  XPT2046 �Ķ�ȡ���� 
  * @param  �� 
  * @retval ��ȡ�������� 
  */
//��ȡ���� AD��Ϊ����λ��
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
  * @brief  �� XPT2046 ѡ��һ��ģ��ͨ����
  * ����ADC��������ADC�������
  * @param  ucChannel
  *   �ò���Ϊ����ֵ֮һ��
  *     @arg 0x90 :ͨ��Y+��ѡ�������
  *     @arg 0xd0 :ͨ��X+��ѡ�������
  * @retval ��ͨ����ADC�������
  */
static uint16_t XPT2046_ReadAdc( uint8_t ucChannel ){
  XPT2046_WriteCMD(ucChannel);
  return XPT2046_ReadCMD();
}

/**
  * @brief  ��ȡ XPT2046 ��Xͨ����Yͨ����ADֵ��12 bit�������4096��
  * @param  sX_Ad �����Xͨ��ADֵ�ĵ�ַ
  * @param  sY_Ad �����Yͨ��ADֵ�ĵ�ַ
  * @retval ��
  */
void XPT2046_ReadAdc_XY ( int16_t * sX_Ad, int16_t * sY_Ad ){ 
	*sX_Ad = XPT2046_ReadAdc(XPT2046_CHANNEL_X);
	XPT2046_DelayUS(1); 
	*sY_Ad = XPT2046_ReadAdc (XPT2046_CHANNEL_Y); 
  
  //#ifdef _USEUSARTSHOW //#endif
  printf("������ X: 0x%x  Y:0x%x \n" ,*sX_Ad ,*sY_Ad );
  //#endif
  
}







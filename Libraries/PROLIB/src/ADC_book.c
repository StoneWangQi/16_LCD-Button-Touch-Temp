#include "ADC_book.h"

static void ADCx_GPIO_Config(void){
  GPIO_InitTypeDef  GPIO_InitStructure;
  //��ADC IO �ڵ�ʱ�� 
  ADC_GPIO_APBxClock_FUN(ADC_GPIO_CLK , ENABLE );
  //����ADC_IO���ŵ�ģʽ ����ģ������
  //����Ϊģ������
  GPIO_InitStructure.GPIO_Pin = ADC_PIN ; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  //��ʼ��ADC IO 
  GPIO_Init(ADC_PORT, &GPIO_InitStructure);
  
}

__IO uint16_t ADC_ConvertedValue;

static void ADCx_Mode_Config(void){
  
  
#ifdef __ADC_DMA_Mode
  DMA_InitTypeDef DMA_InitStructure ;
  RCC_AHBPeriphClockCmd(ADC_DMA_CLK ,ENABLE );
  
  DMA_DeInit(ADC_DMA_CHANNEL);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(ADC_x->DR));
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_ConvertedValue ;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = 1;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(ADC_DMA_CHANNEL , &DMA_InitStructure);
  DMA_Cmd(ADC_DMA_CHANNEL , ENABLE);
#endif 
  
  ADC_InitTypeDef ADC_InitStruct;
  // �� ADC ʱ��
  ADC_APBxClock_FUN(ADC_CLK , ENABLE);
  // ADC ģʽ���� ֻʹ��һ�� ADC�����ڶ���ģʽ
  ADC_InitStruct.ADC_Mode = ADC_Mode_Independent ; 
  /* ADC ɨ�裨��ͨ�������ߵ��Σ���ͨ����ģʽѡ�� */
  ADC_InitStruct.ADC_ScanConvMode = DISABLE;  
  // ����ת��ģʽ
  ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;
  // �����ⲿ����ת���������������
  ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  // ת������Ҷ���
  ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
  // ת��ͨ�� 1 ��
  ADC_InitStruct.ADC_NbrOfChannel = 1;
  // ��ʼ�� ADC
  ADC_Init(ADC_x , &ADC_InitStruct);
  // ���� ADC ʱ��Ϊ PCLK2 �� 8 ��Ƶ���� 9MHz
  RCC_ADCCLKConfig(RCC_PCLK2_Div8);
  // ���� ADC ͨ��ת��˳��Ͳ���ʱ��
  ADC_RegularChannelConfig(ADC_x ,ADC_CHANNEL , 1 ,ADC_SampleTime_55Cycles5 );
  // ADC ת�����������жϣ����жϷ�������ж�ȡת��ֵ
  ADC_ITConfig(ADC_x , ADC_IT_EOC , ENABLE);
  
#ifdef __ADC_DMA_Mode 
  ADC_Cmd(ADC_x , ENABLE);
#endif 
  // ���� ADC ������ʼת��
  ADC_Cmd(ADC_x , ENABLE);
  
  // ��ʼ�� ADC У׼�Ĵ���
  ADC_ResetCalibration(ADC_x);
  while(ADC_GetResetCalibrationStatus(ADC_x));
  
  // ADC ��ʼУ׼
  ADC_StartCalibration(ADC_x);
  while(ADC_GetCalibrationStatus(ADC_x));
  ADC_SoftwareStartConvCmd(ADC_x , ENABLE);
}

static void ADC_NVIC_Config(void){
  NVIC_InitTypeDef NVIC_InitStructure;
  //���ȼ�����
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  //�����ж����ȼ�
  NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQ ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1 ;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd =ENABLE ;
  NVIC_Init(&NVIC_InitStructure);
}
 

void ADx_Init(void){
#ifdef __ADC_DMA_Mode 
  ADCx_GPIO_Config();
  ADCx_Mode_Config();
#else
  ADC_NVIC_Config();
  ADCx_GPIO_Config();
  ADCx_Mode_Config();
#endif 
  
}

/**
  * @brief  This function handles ADC_IRQHandler 
  * @param  None
  * @retval None
  */
__IO uint16_t ADC_ConvertedValue;
void ADC_IRQHandler(void){
  if(ADC_GetITStatus(ADC_x , ADC_IT_EOC) == SET){
    ADC_ConvertedValue = ADC_GetConversionValue(ADC_x);
  }
  ADC_ClearITPendingBit(ADC_x , ADC_IT_EOC);
}



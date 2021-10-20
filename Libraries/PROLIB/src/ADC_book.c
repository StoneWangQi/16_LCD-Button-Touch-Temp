#include "ADC_book.h"

static void ADCx_GPIO_Config(void){
  GPIO_InitTypeDef  GPIO_InitStructure;
  //打开ADC IO 口的时钟 
  ADC_GPIO_APBxClock_FUN(ADC_GPIO_CLK , ENABLE );
  //配置ADC_IO引脚的模式 必须模拟输入
  //必须为模拟输入
  GPIO_InitStructure.GPIO_Pin = ADC_PIN ; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  //初始化ADC IO 
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
  // 打开 ADC 时钟
  ADC_APBxClock_FUN(ADC_CLK , ENABLE);
  // ADC 模式配置 只使用一个 ADC，属于独立模式
  ADC_InitStruct.ADC_Mode = ADC_Mode_Independent ; 
  /* ADC 扫描（多通道）或者单次（单通道）模式选择 */
  ADC_InitStruct.ADC_ScanConvMode = DISABLE;  
  // 连续转换模式
  ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;
  // 不用外部触发转换，软件开启即可
  ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  // 转换结果右对齐
  ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
  // 转换通道 1 个
  ADC_InitStruct.ADC_NbrOfChannel = 1;
  // 初始化 ADC
  ADC_Init(ADC_x , &ADC_InitStruct);
  // 配置 ADC 时钟为 PCLK2 的 8 分频，即 9MHz
  RCC_ADCCLKConfig(RCC_PCLK2_Div8);
  // 配置 ADC 通道转换顺序和采样时间
  ADC_RegularChannelConfig(ADC_x ,ADC_CHANNEL , 1 ,ADC_SampleTime_55Cycles5 );
  // ADC 转换结束产生中断，在中断服务程序中读取转换值
  ADC_ITConfig(ADC_x , ADC_IT_EOC , ENABLE);
  
#ifdef __ADC_DMA_Mode 
  ADC_Cmd(ADC_x , ENABLE);
#endif 
  // 开启 ADC ，并开始转换
  ADC_Cmd(ADC_x , ENABLE);
  
  // 初始化 ADC 校准寄存器
  ADC_ResetCalibration(ADC_x);
  while(ADC_GetResetCalibrationStatus(ADC_x));
  
  // ADC 开始校准
  ADC_StartCalibration(ADC_x);
  while(ADC_GetCalibrationStatus(ADC_x));
  ADC_SoftwareStartConvCmd(ADC_x , ENABLE);
}

static void ADC_NVIC_Config(void){
  NVIC_InitTypeDef NVIC_InitStructure;
  //优先级分组
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  //配置中断优先级
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



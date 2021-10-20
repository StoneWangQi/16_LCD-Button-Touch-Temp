#include "SRAM_book.h" 
#include "USART_book.h"

/**
  * @brief  ��ʼ������SRAM��IO
  * @param  ��
  * @retval ��
  */
static void SRAM_GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 
  /* ʹ��SRAM��ص�GPIOʱ�� */

                         /*��ַ�ź���*/
  RCC_APB2PeriphClockCmd(FSMC_A0_GPIO_CLK | FSMC_A1_GPIO_CLK | FSMC_A2_GPIO_CLK | 
                         FSMC_A3_GPIO_CLK | FSMC_A4_GPIO_CLK | FSMC_A5_GPIO_CLK |
                         FSMC_A6_GPIO_CLK | FSMC_A7_GPIO_CLK | FSMC_A8_GPIO_CLK |
                         FSMC_A9_GPIO_CLK | FSMC_A10_GPIO_CLK| FSMC_A11_GPIO_CLK| 
												 FSMC_A12_GPIO_CLK| FSMC_A13_GPIO_CLK|FSMC_A14_GPIO_CLK|
												 FSMC_A15_GPIO_CLK|FSMC_A16_GPIO_CLK|FSMC_A17_GPIO_CLK|FSMC_A18_GPIO_CLK|
                         /*�����ź���*/
                         FSMC_D0_GPIO_CLK | FSMC_D1_GPIO_CLK | FSMC_D2_GPIO_CLK | 
                         FSMC_D3_GPIO_CLK | FSMC_D4_GPIO_CLK | FSMC_D5_GPIO_CLK |
                         FSMC_D6_GPIO_CLK | FSMC_D7_GPIO_CLK | FSMC_D8_GPIO_CLK |
                         FSMC_D9_GPIO_CLK | FSMC_D10_GPIO_CLK| FSMC_D11_GPIO_CLK|
                         FSMC_D12_GPIO_CLK| FSMC_D13_GPIO_CLK| FSMC_D14_GPIO_CLK|
                         FSMC_D15_GPIO_CLK|  
                         /*�����ź���*/
                         FSMC_CS_GPIO_CLK  | FSMC_WE_GPIO_CLK | FSMC_OE_GPIO_CLK |
                         FSMC_UDQM_GPIO_CLK|FSMC_LDQM_GPIO_CLK, ENABLE);
												 //UDQM UB����


	 /*-- GPIO ���� -----------------------------------------------------*/

  /* ͨ�� GPIO ���� */
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;       //����Ϊ���ù���
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      //���Ǹ������깦����Ϊ����������ֲ�Ĺ涨
  
  /*A��ַ�ź��� �����������*/
  GPIO_InitStructure.GPIO_Pin = FSMC_A0_GPIO_PIN; 
  GPIO_Init(FSMC_A0_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = FSMC_A1_GPIO_PIN; 
  GPIO_Init(FSMC_A1_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = FSMC_A2_GPIO_PIN; 
  GPIO_Init(FSMC_A2_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = FSMC_A3_GPIO_PIN; 
  GPIO_Init(FSMC_A3_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = FSMC_A4_GPIO_PIN; 
  GPIO_Init(FSMC_A4_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = FSMC_A5_GPIO_PIN; 
  GPIO_Init(FSMC_A5_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = FSMC_A6_GPIO_PIN; 
  GPIO_Init(FSMC_A6_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = FSMC_A7_GPIO_PIN; 
  GPIO_Init(FSMC_A7_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = FSMC_A8_GPIO_PIN; 
  GPIO_Init(FSMC_A8_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = FSMC_A9_GPIO_PIN; 
  GPIO_Init(FSMC_A9_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = FSMC_A10_GPIO_PIN; 
  GPIO_Init(FSMC_A10_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = FSMC_A11_GPIO_PIN; 
  GPIO_Init(FSMC_A11_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = FSMC_A12_GPIO_PIN; 
  GPIO_Init(FSMC_A12_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = FSMC_A13_GPIO_PIN; 
  GPIO_Init(FSMC_A13_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = FSMC_A14_GPIO_PIN; 
  GPIO_Init(FSMC_A14_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = FSMC_A15_GPIO_PIN; 
  GPIO_Init(FSMC_A15_GPIO_PORT, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = FSMC_A16_GPIO_PIN; 
  GPIO_Init(FSMC_A16_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = FSMC_A17_GPIO_PIN; 
  GPIO_Init(FSMC_A17_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = FSMC_A18_GPIO_PIN; 
  GPIO_Init(FSMC_A18_GPIO_PORT, &GPIO_InitStructure);
    
  /*DQ�����ź��� �����������*/
  GPIO_InitStructure.GPIO_Pin = FSMC_D0_GPIO_PIN; 
  GPIO_Init(FSMC_D0_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = FSMC_D1_GPIO_PIN; 
  GPIO_Init(FSMC_D1_GPIO_PORT, &GPIO_InitStructure);
    
  GPIO_InitStructure.GPIO_Pin = FSMC_D2_GPIO_PIN; 
  GPIO_Init(FSMC_D2_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = FSMC_D3_GPIO_PIN; 
  GPIO_Init(FSMC_D3_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = FSMC_D4_GPIO_PIN; 
  GPIO_Init(FSMC_D4_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = FSMC_D5_GPIO_PIN; 
  GPIO_Init(FSMC_D5_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = FSMC_D6_GPIO_PIN; 
  GPIO_Init(FSMC_D6_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = FSMC_D7_GPIO_PIN; 
  GPIO_Init(FSMC_D7_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = FSMC_D8_GPIO_PIN; 
  GPIO_Init(FSMC_D8_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = FSMC_D9_GPIO_PIN; 
  GPIO_Init(FSMC_D9_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = FSMC_D10_GPIO_PIN; 
  GPIO_Init(FSMC_D10_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = FSMC_D11_GPIO_PIN; 
  GPIO_Init(FSMC_D11_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = FSMC_D12_GPIO_PIN; 
  GPIO_Init(FSMC_D12_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = FSMC_D13_GPIO_PIN; 
  GPIO_Init(FSMC_D13_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = FSMC_D14_GPIO_PIN; 
  GPIO_Init(FSMC_D14_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = FSMC_D15_GPIO_PIN; 
  GPIO_Init(FSMC_D15_GPIO_PORT, &GPIO_InitStructure);
  
  /*�����ź���*/
  GPIO_InitStructure.GPIO_Pin = FSMC_CS_GPIO_PIN; 
  GPIO_Init(FSMC_CS_GPIO_PORT, &GPIO_InitStructure);
    
  GPIO_InitStructure.GPIO_Pin = FSMC_WE_GPIO_PIN; 
  GPIO_Init(FSMC_WE_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = FSMC_OE_GPIO_PIN; 
  GPIO_Init(FSMC_OE_GPIO_PORT, &GPIO_InitStructure);    
  
  GPIO_InitStructure.GPIO_Pin = FSMC_UDQM_GPIO_PIN; 
  GPIO_Init(FSMC_UDQM_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = FSMC_LDQM_GPIO_PIN; 
  GPIO_Init(FSMC_LDQM_GPIO_PORT, &GPIO_InitStructure);	
}				  			


//  дʱ������ 
  //ADDSET +1 + DATAST +1 > 55ns;
  //DATAST +1 >40ns
  //ADDSET +1 > 0  ns
  //  ����ʱ��
  //ADDSET +1 > 25ns
  //DATAST +1 > 0 ns
  /////ʱ�ӵ�λ 1.38*10-8S = 13.8ns
  
  //дʱ��
  //ADDSET
  //DATAST
  //
  //��ʱ��
  //ADDSET = 0
  //DATAST = 2
  //
  //ADDSET +1 + DATAST +1 = (0+1+2+1)(13.8)=55.2> 55ns;
  //DATAST +1 >=2+1*(13.8 = 41.4)40ns
  //ADDSET +1  = 0+1*(13.8) = 13.8>0  ns
static void FSMC_ModleConfig(void){
  //�ԼĴ�����������
  FSMC_NORSRAMTimingInitTypeDef readTimingInitStruct ;  //��ʱ��
  FSMC_NORSRAMTimingInitTypeDef writeTimingInitStruct ; //дʱ��
  FSMC_NORSRAMInitTypeDef SRAMInitStruct;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC,ENABLE);     //ע��ʱ��ʹ��
  
  //��
  readTimingInitStruct.FSMC_AccessMode = FSMC_AccessMode_A ;/*���÷���ģʽ */
  readTimingInitStruct.FSMC_AddressHoldTime = 0; //SRAM û���õ�
  readTimingInitStruct.FSMC_BusTurnAroundDuration = 0;//SRAM û���õ�
  readTimingInitStruct.FSMC_CLKDivision = 0;//SRAM û���õ�
  readTimingInitStruct.FSMC_DataLatency = 0;//SRAM û���õ�
  readTimingInitStruct.FSMC_AddressSetupTime = 0; //����ADDSET��λ13.8ns
  readTimingInitStruct.FSMC_DataSetupTime =2;//����DATAST��λ13.8ns 
  //д
  writeTimingInitStruct.FSMC_AccessMode = FSMC_AccessMode_A ;/*���÷���ģʽ */
  writeTimingInitStruct.FSMC_AddressHoldTime = 0; //SRAM û���õ�
  writeTimingInitStruct.FSMC_BusTurnAroundDuration = 0;//SRAM û���õ�
  writeTimingInitStruct.FSMC_CLKDivision = 0;//SRAM û���õ�
  writeTimingInitStruct.FSMC_DataLatency = 0;//SRAM û���õ�
  writeTimingInitStruct.FSMC_AddressSetupTime = 0; //����ADDSET��λ13.8ns
  writeTimingInitStruct.FSMC_DataSetupTime =2;//����DATAST��λ13.8ns 
  //���˶�ʱ��ṹ�����
  
  SRAMInitStruct.FSMC_Bank = FSMC_Bank1_NORSRAM3;
  SRAMInitStruct.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable; //��дʱ��
  SRAMInitStruct.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
  SRAMInitStruct.FSMC_MemoryType = FSMC_MemoryType_SRAM;
  //SRAM û���õ�
  SRAMInitStruct.FSMC_BurstAccessMode =  FSMC_BurstAccessMode_Disable ;
  SRAMInitStruct.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
  SRAMInitStruct.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
  SRAMInitStruct.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
  SRAMInitStruct.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
  SRAMInitStruct.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
  SRAMInitStruct.FSMC_WrapMode = FSMC_WrapMode_Disable;
  SRAMInitStruct.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
  SRAMInitStruct.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
  //��д�����ṹ������
  SRAMInitStruct.FSMC_ReadWriteTimingStruct = &readTimingInitStruct;//��
  SRAMInitStruct.FSMC_WriteTimingStruct = &writeTimingInitStruct;//д 
  //д���� ע��FSMC_ExtendedMode ���ó� FSMC_ExtendedMode_Enable ʱ������
  
  // ������д��Ĵ���
  FSMC_NORSRAMInit(&SRAMInitStruct);
  //ʹ��FSMC
  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM3,ENABLE);
  
}



/**
  * @brief  ����SRAM�Ƿ����� 
  * @param  None
  * @retval ��������1���쳣����0
  */
uint8_t SRAM_Test(void){
  /*д�����ݼ�����*/
  uint32_t counter=0;
  
  /* 8λ������ */
  uint8_t ubWritedata_8b = 0, ubReaddata_8b = 0;  
  
  /* 16λ������ */
  uint16_t uhWritedata_16b = 0, uhReaddata_16b = 0; 
  
  printf("���ڼ��SRAM����8λ��16λ�ķ�ʽ��дsram...");


  /*��8λ��ʽ��д���ݣ���У��*/
  
  /* ��SRAM����ȫ������Ϊ0 ��IS62WV51216_SIZE����8λΪ��λ�� */
  for (counter = 0x00; counter < SRAM_SIZE; counter++)
  {
    *(__IO uint8_t*) (SRAM_BASE_ADDR + counter) = (uint8_t)0x0;
  }
  
  /* ������SRAMд������  8λ */
  for (counter = 0; counter < SRAM_SIZE; counter++)
  {
    *(__IO uint8_t*) (SRAM_BASE_ADDR + counter) = (uint8_t)(ubWritedata_8b + counter);
  }
  
  /* ��ȡ SRAM ���ݲ����*/
  for(counter = 0; counter<SRAM_SIZE;counter++ )
  {
    ubReaddata_8b = *(__IO uint8_t*)(SRAM_BASE_ADDR + counter);  //�Ӹõ�ַ��������
    
    if(ubReaddata_8b != (uint8_t)(ubWritedata_8b + counter))      //������ݣ�������ȣ���������,���ؼ��ʧ�ܽ����
    {
      printf("8λ���ݶ�д����");
      return 0;
    }
  }
	 
  /*��16λ��ʽ��д���ݣ������*/
  
  /* ��SRAM����ȫ������Ϊ0 */
  for (counter = 0x00; counter < SRAM_SIZE/2; counter++)
  {
    *(__IO uint16_t*) (SRAM_BASE_ADDR + 2*counter) = (uint16_t)0x00;
  }
  
  /* ������SRAMд������  16λ */
  for (counter = 0; counter < SRAM_SIZE/2; counter++)
  {
    *(__IO uint16_t*) (SRAM_BASE_ADDR + 2*counter) = (uint16_t)(uhWritedata_16b + counter);
  }
  
    /* ��ȡ SRAM ���ݲ����*/
  for(counter = 0; counter<SRAM_SIZE/2;counter++ )
  {
    uhReaddata_16b = *(__IO uint16_t*)(SRAM_BASE_ADDR + 2*counter);  //�Ӹõ�ַ��������
    
    if(uhReaddata_16b != (uint16_t)(uhWritedata_16b + counter))      //������ݣ�������ȣ���������,���ؼ��ʧ�ܽ����
    {
      printf("16λ���ݶ�д����\n");

      return 0;
    }
  }
  
  printf("SRAM��д����������\n"); 
  /*���������return 1 */
  return 1;

}



void SRAM_Init(void){
  SRAM_GPIO_Config();
  FSMC_ModleConfig();
}


static uint8_t testSRAM_Value __attribute__ ((at (SRAM_BASE_ADDR)));
  
void SRAM_Write(void){
  uint8_t* SRAM_P ;
  uint16_t* SRAM_P16 ;
  float* SRAM_Pf ;
  //---------------------------------
  SRAM_P = (uint8_t*)SRAM_BASE_ADDR;
  *SRAM_P = 0xA2;
  printf("����������Ϊ1: 0x%x\n",*SRAM_P); 
  //---------------------------------
  SRAM_P16 = (uint16_t*)SRAM_BASE_ADDR;
  *SRAM_P16 = 0x2AA2;
  printf("����������Ϊ2: 0x%x\n",*SRAM_P16);
  //---------------------------------
  SRAM_Pf = (float*)SRAM_BASE_ADDR;
  *SRAM_Pf = 83.65;
  printf("����������Ϊ3: 0x%.2f\n",*SRAM_Pf);
  //-----------���ó��������----------------------
  testSRAM_Value = 0x6A;
  printf("����������Ϊ1: 0x%x\n",testSRAM_Value); 
}





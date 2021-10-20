/*******************************************************************************
* @file    GPIO/JTAG_Remap/main.c 
* @author  MCD Application Team
* @version V3.5.0
* @date    08-April-2011
* @brief   Main program body
******************************************************************************
* @attention
*    
*    
******************************************************************************
*/ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "PROJ_book.h" 


//SD����flash�ĸ�Ŀ¼
#define SD_ROOT       "0:"
#define FLASH_ROOT    "1:"

/* ��Դ��SD���е�·�� */
#define RESOURCE_DIR         "0:/srcdata"
/* ��Դ��¼����FLASH����ַ��Ŀ¼��ַ�� */
#define RESOURCE_BASE_ADDR    (4096)
/* �洢��FLASH�е���ԴĿ¼��С */
#define CATALOG_SIZE           (8*1024)
  
  
/* ���ɵ���¼��Ϣ�ļ��� */
#define BURN_INFO_NAME        "burn_info.txt"
/* ������¼��Ϣ�ļ���*/  
#define BURN_INFO_NAME_FULL   (RESOURCE_DIR "/" BURN_INFO_NAME)
/* ��¼���Ե��ļ��� */
#define IGNORE_NAME           "ignore.txt"  
/* ���������ļ���*/    
#define IGNORE_NAME_FULL      (RESOURCE_DIR "/" IGNORE_NAME)
extern char src_dir[];
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void delay(int x);
void fn_LED_Flash_Init(void);
void fn_usart_show_Init(void);
void fn_DMA_show_Init(void);
void fn_I2C_EE_Init(void);
void fn_I2C_EE_Soft_Init(void);
void fn_SPI_FLASH_Soft_Init(void);
void fn_FatFs_Document_Init(void);
void fn_SRAM_Init(void);
void fn_LCD_Init(void);
void fn_XPT2046_Init(void);

#define countof(a)      (sizeof(a) / sizeof(*(a)))
  
#define  _I2C_BufferSize (countof(writeData)-1)
static uint8_t writeData[_I2C_PageSize]={4,5,6,7,8,9,10,11};
static uint8_t writeData2[_I2C_PageSize]={24,25,26,27,28,29,30,31};
static uint8_t ReadData[_I2C_BufferSize]={0};

#define  _SPI_BufferSize  SPI_PAGE_SIZE   //(countof(write_SPI_Data)-1)
static uint8_t write_SPI_Data[_SPI_BufferSize]={0};
static uint8_t Read_SPI_Data[_SPI_BufferSize]={0};

int main(void)
{ 
      
      int16_t sAD_X, sAD_Y ;
  
      //*************ϵͳ�ӿڳ�ʼ��**************
      fn_RCC_Init();              //CPU ��Ƶ   
      fn_Led_Init();              //LED �����ʼ��
      fn_Key_Init();              //���� �����ʼ��
      //*************����ͨѶϵͳ��ʼ��**************
      fn_USART_Init();            //���������ʼ��
      printf("\n\n\r\n********** ICϵͳ��ʼ���� **********\r\n");
      fn_Systick_Delay_Handler_set(10,_Systick_ms);
      //*************����ϵͳ��ʼ��*******************
      fn_LED_Flash_Init();        //RGB �������
      fn_usart_show_Init();       //�����������
      fn_EXTI_GPIO_Config();      //�ⲿ�ж����

      //*************����ϵͳ��ʼ��**************
           
      fn_DMA_show_Init();         //��ʼ��DMA������·  
      fn_I2C_EE_Init();           //��ʼ��Ӳ��I2C������·    
      fn_I2C_EE_Soft_Init();      //��ʼ�����I2C������·
      
      Display_Char_test();       
      fn_LCD_Init();
      
      fn_SPI_FLASH_Soft_Init();   //SPI����ͨѶ
      fn_FatFs_Document_Init();   //FatFs�ļ���ȡ����        
      fn_SRAM_Init();             //SRAM�ļ���ȡ���� 
      
      //*************LCDϵͳ��ʼ��************** 
      
      fn_XPT2046_Init(); 
      fn_Lcd_Page_Init();
      fn_Systick_Delay_Handler_set(10,_Systick_ms);
      while(1){
        fn_key_Check();
        XPT2046_TouchEvenHandler();
        if(bkey_change == 0){continue;}
        switch(Count_Modle_flag){
          case 0:
            fn_Lcd_Page1();
            break;
          case 1:
            fn_Lcd_Page2();
            break;
          case 2:
            fn_Lcd_Page3();
            //������⺯��������������10ms����һ��         
            break;
          default:
              break;
        }
        
        bkey_change = 0;
      }
}

//======================================================================
//======================================================================

  
void fn_LED_Flash_Init(void){
  uint16_t  count_Init = 2;
  printf("\n ---> LED��ʼ���� \n");
  while(count_Init-->0){
    fn_LED_ALL_OFF();
    __R_OUT__;
    fn_Systick_Delay(500,_Systick_ms);
    fn_LED_ALL_OFF();
    __G_OUT__;
    fn_Systick_Delay(500,_Systick_ms);
    fn_LED_ALL_OFF();
    __B_OUT__;
    fn_Systick_Delay(500,_Systick_ms);
    fn_LED_ALL_OFF();
    __R_OUT__;
  } 
}
//======================================================================
//======================================================================
void fn_usart_show_Init(void){ 
  fn_Usart_Send_Byte(_DEBUG_USARTx,'\r');
  printf("-->����ͨ��ָ������� \n");
  fn_Usart_SendString(_DEBUG_USARTx," : wangqi \n");
}
//======================================================================
//======================================================================
void fn_DMA_show_Init(void){
  printf("\n ---> DMA��ʼ���� \n");
  _DMA_ROM_TO_RAM(Map_BUFFER_SIZE ,aSRC_Cont_Buffer , aDST_Buffer);
  _DMA_RAM_TO_USART(Map_BUFFER_SIZE ,USART_Source_ADDR , aDST_Buffer); 
  printf("---> DMA������� \n");
}
//======================================================================
//======================================================================
void fn_I2C_EE_Init(void){
  printf("\n-->I2C_����д�뿪ʼ \n");
  _I2C_EE_Init();
  I2C_Write_fun(writeData ,EEP_Firstpage ,_I2C_BufferSize);
  I2C_Read_fun(ReadData ,EEP_Firstpage ,_I2C_BufferSize);
  printf("--->I2C_����д�����\n\r");
}
//======================================================================
//======================================================================
void fn_I2C_EE_Soft_Init(void){
  printf("\n-->I2C_�������д�뿪ʼ \n");
  I2C_Soft_Init();
  I2C_Soft_Write_fun(writeData2 ,EEP_Firstpage ,_I2C_BufferSize);
  I2C_Soft_Read_fun(ReadData ,EEP_Firstpage ,_I2C_BufferSize);
  printf("\n--->I2C_�������д�����\n\r");
}
//======================================================================
//======================================================================
void fn_SPI_FLASH_Soft_Init(void){
  uint16_t i,FlashID;
  printf("-->SPIͨ��ָ���Կ�ʼ \n");
  SPI_FLASH_Init(); 
  FlashID = SPI_Read_ID() ;
  if(FlashID == _SPI_FLASH_ID){
    printf("-->SPI  0x%x \n",FlashID);
  }
  
  SPI_Erase_Sector(0); //���һ��ҳ�Ŀռ� 
  printf("\n\n-->SPI��տ�ʼ \n");
  SPI_Read_Data(Read_SPI_Data , 0, SPI_PAGE_SIZE);
  SPI_Show_Data(Read_SPI_Data , SPI_PAGE_SIZE);
  printf("\n\n-->SPI������ \n");
   
  for(i=0 ; i < _SPI_BufferSize ; i++){
    write_SPI_Data[i] = 0xA7;
  }
  SPI_Show_Data(write_SPI_Data , SPI_PAGE_SIZE);
  
  SPI_BufferWrite_Data(write_SPI_Data ,0x000000,_SPI_BufferSize);
  
  
  printf("\n\n-->SPI������� \n");
  SPI_Read_Data(Read_SPI_Data , 0x000000, _SPI_BufferSize);
  SPI_Show_Data(Read_SPI_Data , _SPI_BufferSize);
  
  printf("-->SPIͨ��ָ������� \n");
}
//======================================================================
//======================================================================
void fn_FatFs_Document_Init(void){
  /*----------------------- ��ʽ������ -----------------*/ 
  FatFs_equipment_flash("1:");
  /*----------------------- �ļ�ϵͳ��д���� -------------------*/
  FatFs_document_Text(FileTest_ADDR_Buffer_Document, "Wangqi����!\n" );
   /*----------------------- �ļ���������ʶ�� -------------------*/
  file_check(FileTest_ADDR_Buffer_Document);
  /*----------------------- �ļ�Ŀ¼���Ĳ��� -------------------*/
  FatFs_document_set_content(FileTest_ADDR_Buffer_Document , FileTest_ADDR_Buffer_path , FileTest_ADDR_Buffer_NewDocument2);
  /*----------------------- �ļ���������ʶ�� -------------------*/
  file_check(FileTest_ADDR_Buffer_NewDocument2);
  /*----------------------- �ļ�Ŀ¼����ʶ�� -------------------*/
  Scan_file_Check("1:");
  /*----------------------- ��ɲ��� -------------------*/
  FatFs_Close_flash("1:");
}
//======================================================================
//======================================================================

void fn_SRAM_Init(void){             //SRAM�ļ���ȡ����
  printf("********** SRAM_��չ�ڴ���� ********** \n");  
  printf("-->�ⲿSRAM_�ڴ溯����ʼ����ʼ \n");
  SRAM_Init();
  printf("\n-->�ⲿSRAM_�ڴ溯�����Կ�ʼ \n");
  SRAM_Test();
  printf("\n-->�ⲿSRAM_�ڴ溯��д�뿪ʼ \n");
  SRAM_Write();
  
}

//======================================================================
//======================================================================

void fn_LCD_Init(void){             //LCD���в���
    __IO int16_t int_check;
    fn_Systick_Delay(500,_Systick_ms);
    int_check = (__IO int16_t)ILI9341_Init();
    Display_LCD_clear();
      switch(int_check){
        case 0x05:
            printf("\n-->LCD ��������");
            Lcd_display_String("  LCD ��������");     
            break;
        default:
            printf("\n-->LCD �����쳣");
            Lcd_display_String("   LCD �����쳣");
            
      }       
}

//======================================================================
//======================================================================

void fn_XPT2046_Init(void){             //XPT2046���в���
   printf("\n-->LCD ������ʼ��");
   Lcd_display_String("  LCD ������ʼ��");   
   XPT2046_GPIO_Init();  
   while(XPT2046_Touch_Calibrate_Page() == 0){;}
   Lcd_display_String("  LCD ������ʼ��ok");
}
//======================================================================
//======================================================================
void delay(int x){
	int y = 0xFFFFF;
	while((x--)>0){
		while((y--)>0){
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
		}
	}
}
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

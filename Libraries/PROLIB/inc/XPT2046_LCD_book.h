#ifndef __XPT2046_LCD_BOOK_H__
#define	__XPT2046_LCD_BOOK_H__

#include "stm32f10x.h"
 
/******************************* XPT2046 �����������ź�ָʾ���Ŷ���(��ʹ���ж�) ***************************/
#define             XPT2046_PENIRQ_GPIO_CLK                        RCC_APB2Periph_GPIOF   
#define             XPT2046_PENIRQ_GPIO_PORT                       GPIOF
#define             XPT2046_PENIRQ_GPIO_PIN                        GPIO_Pin_9

/******************************* XPT2046 ������ģ��SPI���Ŷ��� ***************************/
#define             XPT2046_SPI_GPIO_CLK                         RCC_APB2Periph_GPIOF| RCC_APB2Periph_GPIOG

#define             XPT2046_SPI_CS_PIN		                        GPIO_Pin_10
#define             XPT2046_SPI_CS_PORT		                      GPIOF

#define	            XPT2046_SPI_CLK_PIN	                        GPIO_Pin_7
#define             XPT2046_SPI_CLK_PORT	                        GPIOG

#define	            XPT2046_SPI_MOSI_PIN	                        GPIO_Pin_11
#define	            XPT2046_SPI_MOSI_PORT	                      GPIOF

#define	            XPT2046_SPI_MISO_PIN	                        GPIO_Pin_6
#define	            XPT2046_SPI_MISO_PORT	                      GPIOF


#define             XPT2046_CS_DISABLE()                          GPIO_SetBits ( XPT2046_SPI_CS_PORT, XPT2046_SPI_CS_PIN )    
#define             XPT2046_CS_ENABLE()                         GPIO_ResetBits ( XPT2046_SPI_CS_PORT, XPT2046_SPI_CS_PIN )  

#define             XPT2046_CLK_HIGH()                           GPIO_SetBits ( XPT2046_SPI_CLK_PORT, XPT2046_SPI_CLK_PIN )    
#define             XPT2046_CLK_LOW()                            GPIO_ResetBits ( XPT2046_SPI_CLK_PORT, XPT2046_SPI_CLK_PIN ) 

#define             XPT2046_MOSI_1()                             GPIO_SetBits ( XPT2046_SPI_MOSI_PORT, XPT2046_SPI_MOSI_PIN ) 
#define             XPT2046_MOSI_0()                             GPIO_ResetBits ( XPT2046_SPI_MOSI_PORT, XPT2046_SPI_MOSI_PIN )

#define             XPT2046_MISO()                               GPIO_ReadInputDataBit ( XPT2046_SPI_MISO_PORT, XPT2046_SPI_MISO_PIN )


/******************************* XPT2046 �������������� ***************************/
#define	            XPT2046_CHANNEL_X 	                          0x90 	          //ͨ��Y+��ѡ�������	
#define	            XPT2046_CHANNEL_Y 	                          0xd0	          //ͨ��X+��ѡ�������


void XPT2046_ReadAdc_XY ( int16_t * sX_Ad, int16_t * sY_Ad );

#endif /* __BSP_TOUCH_H */


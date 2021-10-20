/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2014        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
#include "ff.h"
#include "SPI_book.h"

//#include "usbdisk.h"	/* Example: Header file of existing USB MSD control module */
//#include "atadrive.h"	/* Example: Header file of existing ATA harddisk control module */
//#include "sdcard.h"		/* Example: Header file of existing MMC/SDC contorl module */

/* Definitions of physical drive number for each drive */
#define ATA		0	/* Example: Map ATA harddisk to physical drive 0 */
#define MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
#define USB		2	/* Example: Map USB MSD to physical drive 2 */

#define SD_CARD   0
#define SPI_FLASH 1

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat = STA_NOINIT;
//	int result;

	switch (pdrv) {
    case SD_CARD : break;
    
    case SPI_FLASH :
      if(SPI_Read_ID() == _SPI_FLASH_ID){
        stat = RES_OK;
      }else{
        stat = RES_ERROR;
      }
      break;
    default:
			stat = STA_NOINIT;  
	}
	return stat;
}

//disk_status ����ֻ��һ������ pdrv����ʾ�����š�һ�����Ƕ���ʹ�� switch ����ʵ
//�ֶ� pdrv �ķ�֧�жϡ����� SD ��ֻ��Ԥ���ӿڣ����ռ��ɡ����� SPI Flash оƬ������ֱ
//�ӵ����� SPI_FLASH_ReadID()��ȡ�豸 ID��

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat = STA_NOINIT;
//	int result;

	switch (pdrv) {
    case SD_CARD : break;
  
    case SPI_FLASH :
      SPI_FLASH_Init();
      SPI_Flash_WAKEUP();
      stat = disk_status(SPI_FLASH);
      break;
    default:
			stat = STA_NOINIT;  
	}
	return stat;
}

//disk_initialize ����Ҳ����һ������ pdrv������ָ���豸�����š����� SPI Flash оƬ
//���ǵ��� SPI_FLASH_Init()����ʵ�ֶ� SPI Flash оƬ���� GPIO ��ʼ�������Լ� SPI ͨ��
//�������á�SPI_Flash_WAKEUP()�������� SPI Flash оƬ���� SPI Flash оƬ����˯��ģʽ
//ʱ��Ҫ����оƬ�ſ��Խ��ж�д������

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res = RES_PARERR;
	//int result;

	switch (pdrv) {
    case SD_CARD : break;
  
    case SPI_FLASH :
      // ����ƫ��2MB  �ⲿFlash �ļ�ϵͳ�ռ���� SPI_FLAH�����6MB�ռ�
      sector += 512;
      SPI_Read_Data(buff,(sector << 12),(count << 12));
      res = RES_OK ; 
      break;
    default:
			res = RES_PARERR;  
	}

	return res;
}

//SPI Flash оƬ�ͺ�Ϊ W25Q64FV��ÿ��������СΪ 4096 ���ֽ�(4KB)��
//�ܹ��� 8M �ֽڿռ䣬Ϊ���ݺ���ʵ���������ֻ���󲿷� 6MB �ռ����� FatFs ʹ�ã�
//ǰ���� 2MB �ռ���������ʵ����Ҫ���� FatFs �Ǵ� 2MB �ռ俪ʼ��Ϊʵ�����Ч����Ҫ
//�����еĶ�д��ַ��ƫ�� 512 �������ռ�

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res = RES_PARERR;
	//int result;
  if(!count){return res;}
	switch (pdrv) {
	  case SD_CARD : break;
  
    case SPI_FLASH :
      // ����ƫ��2MB  �ⲿFlash �ļ�ϵͳ�ռ���� SPI_FLAH�����6MB�ռ�
      sector += 512;
      SPI_Erase_Sector(sector << 12);
      SPI_BufferWrite_Data((uint8_t *)buff ,sector << 12,count << 12);
      res = RES_OK ; 
      break;
    default:
			res = RES_PARERR;  
	}

	return res;
}
#endif
//disk_write �������ĸ��βΣ�pdrv Ϊ�豸�����š�buff ָ���д���������ݵ��׵�ַ��
//sector��ָ��Ҫд�����ݵ������׵�ַ��count ָ���������������� SPI Flash оƬ����д��
//����֮ǰ��Ҫ�Ȳ����������õ�������������(SPI_FLASH_SectorErase)��Ȼ������ڵ���
//����д�뺯��(SPI_FLASH_BufferWrite)������д�뵽ָ��λ���ڡ�

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res = RES_PARERR;
//	int result;

	switch (pdrv) {
    case SD_CARD : break;
  
    case SPI_FLASH :{
       switch(cmd){
          //���� �����ĸ��� 1280*4096/1024/1024 = 5MB
         case GET_SECTOR_COUNT:
           *(DWORD * )buff = 1280;
           break;
         //����ÿһ�������Ĵ�С
         case GET_SECTOR_SIZE:
           *(DWORD * )buff = 4096;
           break;
         case GET_BLOCK_SIZE:
           *(DWORD * )buff = 1;
           break;    
         default:
          return RES_PARERR; 
       }
       res = RES_OK;
       break;
    }
   default:
      res =  RES_PARERR;     
	}
	return res;
}
#endif

 DWORD get_fattime(void) {
	/* ���ص�ǰʱ��� */
	return	  ((DWORD)(2015 - 1980) << 25)	/* Year 2015 */
			| ((DWORD)1 << 21)				/* Month 1 */
			| ((DWORD)1 << 16)				/* Mday 1 */
			| ((DWORD)0 << 11)				/* Hour 0 */
			| ((DWORD)0 << 5)				  /* Min 0 */
			| ((DWORD)0 >> 1);				/* Sec 0 */
}


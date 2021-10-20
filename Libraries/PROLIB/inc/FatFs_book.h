#ifndef  __FATFS_BOOK_H_
#define  __FATFS_BOOK_H_

#include "ff.h"			/* Declarations of FatFs API */

extern FATFS     fs;                   /* FatFs �ļ�ϵͳ���� */
extern FIL       fnew;                 /* �ļ����� */
extern FRESULT   res_flash;            /* �ļ�������� */
extern UINT      fnum;                 /* �ļ��ɹ���д���� */
extern BYTE      FATFS_buffer[0];      /* �������� */
extern BYTE      textFile_Buffer[];
extern BYTE      textFile_Buffer2[];
extern BYTE      File_Read_Buffer[1024];
 
#define  textFile_Buffer         "��ӭ�Լ���ɵ�һ�׶λع��������񣬺������ �����Ǹ������ӣ��½��ļ�ϵͳ�����ļ�\r\n" 
 
#define  FileTest_ADDR_Buffer_Document        "1:Wangqi.txt" 
#define  FileTest_ADDR_Buffer_NewDocument     "1:Wangqi/Wangqi.txt" 
#define  FileTest_ADDR_Buffer_path            "1:test" 
#define  FileTest_ADDR_Buffer_NewDocument2    "1:test/testdir.txt" 
    
FRESULT FatFs_equipment_flash(
  const TCHAR* path
);
FRESULT FatFs_Close_flash(
  const TCHAR* path 
);
BYTE* FatFs_document_Text(
  const TCHAR* _File_ADDR ,
  BYTE* Text_Buffer 
);
  
void FatFs_document_set_content(
  const TCHAR* _File_ADDR , 
  const TCHAR* _File_OldPath , 
  const TCHAR* _File_NewPath 
);
  
FRESULT file_check(const TCHAR*  FileTest_ADDR); 
FRESULT Scan_file_Check(const TCHAR*  File_ADDR);
  
#endif

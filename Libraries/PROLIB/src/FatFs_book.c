#include "FatFs_book.h"
#include "Systick_book.h"
#include "USART_book.h"
#include "string.h"
FATFS     fs;                   /* FatFs �ļ�ϵͳ���� */
FIL       fnew;                 /* �ļ����� */
FRESULT   res_flash;            /* �ļ�������� */
UINT      fnum;                 /* �ļ��ɹ���д���� */
BYTE      FATFS_buffer[0];      /* �������� */
 
/**************************************************************
* @brief  
* FRESULT FatFs_equipment_flash(const TCHAR* path )
* @param  
* 
*FatFs �ĵ�һ����������ʹ�� f_mount �������ع�������f_mount �����������βΣ���
*һ��������ָ�� FATFS ����ָ�룬�����ֵΪ NULL ����ȡ�������豸���ء��ڶ�������
*Ϊ�߼��豸��ţ�ʹ���豸��·����ʾ���������豸��Źҹ����ڴ����嵥 26-1 �����Ƕ�
*�� SPI Flash оƬ������Ϊ 1����������ʹ�á�1������������������ѡ 0 �� 1��1 ��ʾ��
*�����أ�0 ��ʾ���������أ��ӳٹ��ء� f_mount �����᷵��һ�� FRESULT ����ֵ��ָʾ
*���������
*��� f_mount ��������ֵΪ FR_NO_FILESYSTEM��˵��û�� FAT �ļ�ϵͳ��������
*������ SPI Flash оƬ��û�� FAT �ļ�ϵͳ�����Ǿͱ���������豸���и�ʽ������ʹ��
*f_mkfs ��������ʵ�ָ�ʽ��������f_mkfs �����������βΣ���һ������Ϊ�߼��豸��ţ�
*�ڶ�������ѡ 0 ���� 1��0 ��ʾ�豸Ϊһ��Ӳ�̣�1 ��ʾ�豸Ϊ���̡�����������ָ������
*��С�����Ϊ 0����ʾͨ�������嵥 26-6 �� disk_ioctl ������ȡ����ʽ���ɹ�����Ҫ��ȡ��
*����ԭ���豸�������¹����豸��
*
*/
FRESULT FatFs_equipment_flash(
  const TCHAR* path 
){
  //���ⲿ SPI Flash �����ļ�ϵͳ���ļ�ϵͳ����ʱ��� SPI �豸��ʼ��
  //��ʼ������������������
  //f_mount()->find_volume()->disk_initialize->SPI_FLASH_Init()
  DIR     dir;
  FATFS   *pfs;
  DWORD   fre_clust , fre_sect , tot_sect ;
  printf("\n********** �ⲿSPIFlash�����ļ�ϵͳ **********\r\n");
  printf(" ���ⲿSPI Flash�����ļ�ϵͳ���ļ�ϵͳ����ʱ���SPI�豸��ʼ��\r\n");
  res_flash = f_mount(&fs ,path,1); //SPI Flash оƬ������Ϊ 1����������ʹ�á�1������
  /* Pointer to the file system object (NULL:unmount)*/
  /* Logical drive number to be mounted/unmounted */
  /* 0:Do not mount (delayed mount), 1:Mount immediately */
  /*----------------------- ��ʽ������ -----------------*/
  /* ���û���ļ�ϵͳ�͸�ʽ�����������ļ�ϵͳ */  
  if(res_flash ==  FR_NO_FILESYSTEM ){  
    printf("��FLASH ��û���ļ�ϵͳ����������Flash FatFS ��ʽ��...\r\n");
    /* ��ʽ�� */
    res_flash=f_mkfs(path,0,0);/* Create a file system on the volume */
     /* Logical drive number */
	   /* Partitioning rule 0:FDISK, 1:SFD */
	   /* Size of allocation unit in unit of byte or sector */
    //��ʽ��ɣ���ȡ������
    if(res_flash == FR_OK ){
      printf("��FLASH �ѳɹ���ʽ���ļ�ϵͳ��\r\n");
      res_flash = f_mount(NULL ,path ,1); 
      //���¹���
      res_flash = f_mount(&fs ,path ,1); 
    }else{
      printf("����ʽ��ʧ�ܡ� \r\n");
      return res_flash;
    } 
  }
  else if(res_flash != FR_OK){
    printf("���ⲿ Flash �����ļ�ϵͳʧ�ܡ�(%d)\r\n",res_flash);
    printf("������ԭ��SPI Flash ��ʼ�����ɹ���\r\n");
    return res_flash;
  }
  else{
    printf("���ļ�ϵͳ���سɹ������Խ��ж�д����\r\n");
  } 
  
  //��ȡ�豸��Ϣ�Ϳռ�Ĵ�С
  res_flash  = f_getfree(path ,&fre_clust ,&pfs );
  //����õ��������������Ϳ���������
  tot_sect = (pfs->n_fatent-2)*pfs->csize ;
  fre_sect = fre_clust * pfs->csize ;
  /* ��ӡ��Ϣ(4096 �ֽ�/����) */
  printf("���豸�ܿռ䣺%10lu KB��\n�����ÿռ䣺  %10lu KB��\r\n", tot_sect<<2, fre_sect<<2);
  return FR_OK;
}

/**************************************************************
* @brief  
* FRESULT FatFs_Close_flash( const TCHAR* path )
* @param  
*  �ر�Flash����
**************************************************************/
FRESULT FatFs_Close_flash(
  const TCHAR* path 
){
  res_flash=f_mkfs(path,0,0);
  /* ����ʹ���ļ�ϵͳ��ȡ�������ļ�ϵͳ */
 return f_mount(NULL ,path ,1);
}

/**************************************************************
* @brief  
* void FatFs_document_Text(
*  const TCHAR* _File_ADDR , �ļ���Ŀ¼
*  BYTE* Text_Buffer       ��Ҫд�������
*)
* @param  
*  �ر�Flash�ļ��Ķ�д����
**************************************************************/
BYTE* FatFs_document_Text(
  const TCHAR* _File_ADDR ,
  BYTE* Text_Buffer   
){
  BYTE  File_Read_Buffer[1024]={0}  ;
 
  printf("\n******** �ļ�%s��λ�͸�ʽ��д�빦�ܲ��� ********\r\n",File_Read_Buffer);
  res_flash = f_open(&fnew , _File_ADDR, FA_OPEN_ALWAYS|FA_WRITE|FA_READ );
  if( res_flash == FR_OK ){
    //res_flash = f_write(&fnew,Text_Buffer ,sizeof(Text_Buffer), &fnum);  ��������е�����:sizeof(Text_Buffer)
    //f_printf(&fnew ,Text_Buffer );
    printf("******** �ļ���λ׷������ ********\r\n");
    res_flash = f_lseek(&fnew , f_size(&fnew));	//�ļ���λ�� �ļ���ĩβ   Move file pointer of a file object  
    if( res_flash == FR_OK ){
      //��ʽ��д�� ������ʽ����printf����
      f_printf(&fnew ,Text_Buffer );  
      res_flash = f_lseek(&fnew,0);
      res_flash = f_read(&fnew , File_Read_Buffer , f_size(&fnew),&fnum );
      if(res_flash == FR_OK){
        printf("> �ļ����ݣ�\n%s\n" ,File_Read_Buffer );
      }
    }  
  }
  else{
    printf("!! ���ļ�ʧ�ܣ�%d\n",res_flash);
    printf("!! ������Ҫ�ٴ����С�FatFs��ֲ���д���ԡ�����\n");
  }
  f_close(&fnew);
  return File_Read_Buffer;
}
 

/**************************************************************
* @brief  
*void FatFs_document_set_content(
*  const TCHAR* _File_ADDR ,  //�ļ�ԭʼ����·��
*  const TCHAR* _File_OldPath ,//�����µ��ļ���Ŀ¼ 
*  const TCHAR* _File_NewPath  //�ļ��ƶ��Ժ���ļ�Ŀ¼
*)
* @param  
*  �ļ�Ŀ¼�ĸ���
**************************************************************/
void FatFs_document_set_content(
  const TCHAR* _File_ADDR , 
  const TCHAR* _File_OldPath , 
  const TCHAR* _File_NewPath 
){
   DIR     dir;
  //���Դ�Ŀ¼��
  printf("\n********** Ŀ¼���������������ܲ��� **********\r\n");
  res_flash = f_opendir(&dir,_File_OldPath);
  if(res_flash != FR_OK){
    printf("!! ��Ŀ¼%s ʧ�ܿ�ʼ���Դ����µ�Ŀ¼\n",_File_OldPath);
    res_flash = f_mkdir(_File_OldPath);
      res_flash = f_opendir(&dir,_File_OldPath);
      if(res_flash != FR_OK){
         printf("!! �����µ�Ŀ¼����ʧ��(%d) \n",res_flash);
         return;
      } 
  }
  else{
    printf("!! ���Ŀ¼�Ѿ����ڣ��ر���\n"); 
    res_flash = f_closedir(&dir);
    //ɾ���ļ�
    f_unlink(_File_NewPath);
  }
    //���������ƶ��ļ�
    res_flash=f_rename(_File_ADDR , _File_NewPath);
    printf(">  ���������ƶ��ļ�%d��� \n",res_flash);

 
}

/**************************************************************
* @brief  
* FRESULT file_check(const TCHAR*  File_ADDR)
* @param  
* �ļ����ԵĲ鿴
**************************************************************/
FRESULT file_check(const TCHAR*  File_ADDR){
  static FILINFO fno;
  printf("\n********** file_check ���� **********\r\n");
  /* ��ȡ�ļ���Ϣ������ȷ���ļ�����*/
  res_flash = f_stat(File_ADDR,&fno);
  if(res_flash==FR_OK){
    printf("%s �ļ���Ϣ��\n",File_ADDR);
    printf("���ļ���С: %ld(�ֽ�)\n", fno.fsize);
    printf("��ʱ���: %u/%02u/%02u, %02u:%02u\n",
      (fno.fdate >> 9) + 1980, fno.fdate >> 5 & 15, fno.fdate & 31,
       fno.ftime >> 11, fno.ftime >> 5 & 63);
    printf("������: %c%c%c%c%c\n\n",
      (fno.fattrib & AM_DIR) ? 'D' : '-', // ��һ��Ŀ¼
      (fno.fattrib & AM_RDO) ? 'R' : '-', // ֻ���ļ�
      (fno.fattrib & AM_HID) ? 'H' : '-', // �����ļ�
      (fno.fattrib & AM_SYS) ? 'S' : '-', // ϵͳ�ļ�
      (fno.fattrib & AM_ARC) ? 'A' : '-'); // �����ļ�
  }  
  else{
     printf("������ %ld(�ֽ�)\n",res_flash);
  }
  return res_flash;
} 

/**************************************************************
* @brief  
* FRESULT scan_file(char*  File_ADDR)
* @param  
* �ļ�Ŀ¼���ݵĲ鿴
**************************************************************/
static FRESULT scan_file(char*  File_ADDR){
  FRESULT res;  //�����ڵݹ���̱��޸ĵı���
  FILINFO fno;
  DIR     dir;
  int     i;
  TCHAR*  fn;
#if _USE_LFN  
  //���ļ���֧��
  //����������Ҫ2���ֽڱ���һ����
  static char lfn[_MAX_LFN*2 + 1];
  fno.lfname = lfn;
  fno.lfsize = sizeof(lfn);
#endif
  //��Ŀ¼ 
  res = f_opendir(&dir, File_ADDR);
  if(res == FR_OK){
    i = strlen(File_ADDR);
    while(1){
      //��ȡ Ŀ¼�µ����ݣ��ٶ����Զ���ȡ��һ���ļ�
      res = f_readdir(&dir , &fno);
      //Ϊ��ʱ���ʾ��ȡ���
      if((res != FR_OK) || (fno.fname[0] == 0 )){
        break;}
#if _USE_LFN  
      fn = *fno.lfname ?fno.lfname : fno.fname ;
#else
      fn = fno.fname;
#endif 
      //��ʾ��ǰĿ¼����
      if(*fn == '.'){continue;}
      //Ŀ¼ �ݹ��ȡ
      if( fno.fattrib & AM_DIR ){
        //�ϳ�����Ŀ¼��
        sprintf(&File_ADDR[i], "/%s", fn); 
        //�ݹ����
        res = scan_file(File_ADDR);
        //��ʧ�ܣ�����ѭ��
        File_ADDR[i] = 0;
        if(res != FR_OK ){break;}
      }else{
        printf( "%s/%s\r\n" , File_ADDR  , fn);
      }
    }
  }
  else{
    printf("������ %d \n",res);
  }
  return res;
}

/**************************************************************
* @brief  
* FRESULT Scan_file_Check(const TCHAR*  File_ADDR)
* @param  
* �ļ�Ŀ¼���ݵĲ鿴
**************************************************************/
FRESULT Scan_file_Check(const TCHAR*  File_ADDR){
  BYTE      fpath[100];                  /* ���浱ǰɨ��·�� */
  printf("\n********** file_·������ **********\r\n");
  strcpy(fpath,File_ADDR);
  return scan_file(fpath); 
}


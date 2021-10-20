/**
  ******************************************************************************
  * @file    palette.c
  * @author  fire
  * @version V1.0
  * @date     
  * @brief   ��������Ӧ�ú���
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

#include "XPT2046_LCD_Device_book.h"

//��ť�Ľṹ������
Touch_Button button[BUTTON_NUM];
//���ʲ���
Brush_Style brush;


static void Touch_Button_Init(void);
static void Draw_Color_Button(void *btn);
static void Draw_Shape_Button(void *btn);
static void Draw_RUBBER_Button(void *btn);
static void Draw_Clear_Button(void *btn);

static void Command_Select_Color(void *btn);
static void Command_Select_Brush(void *btn);
static void Command_RUBBER_Palette(void *btn);
static void Command_Clear_Palette(void *btn);

/**
* @brief  Palette_Init �����ʼ��
* @param  ��
* @retval ��
*/
void Palette_Init(
  uint8_t  _LCD_GramScan_Mode ,//��Ļ����
  uint16_t _Page_Color ,  //������ɫ
  uint16_t _Brush_Color   //������ɫ
){
  uint8_t i ;
  ILI9341_GramScan(_LCD_GramScan_Mode);
  
  //��ʼ��������ɫ
  LCD_SetBackColor(_Page_Color);
  ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);
  
  
  LCD_SetColors(GB888(0xEAC599) , CL_WHITE);
  ILI9341_DrawRectangle(0,
                        0,                    
                        LCD_X_LENGTH-(PALETTE_END_X-PALETTE_START_X), 
                        LCD_Y_LENGTH,1,1);
   
  //��ʼ����ť
  Touch_Button_Init();
  
  //��水ť
  for(i=0 ;i<BUTTON_NUM ;i++ ){
    button[i].draw_btn(&button[i]);
  }
  //��ʼ������
  brush.color = _Brush_Color;
  brush.shape = LINE_SING_PIXCEL ;//��������
  //��ʼ���ı���ɫ
  
  LCD_SetTextColor(brush.color);
}
  
  
/**
* @brief  Touch_Button_Init ��ʼ����ť����
* @param  ��
* #define COLOR_BLOCK_WIDTH   40  //Ĭ�ϰ�ť�Ĵ�С
* #define COLOR_BLOCK_HEIGHT  28  //Ĭ�ϰ�ť�ĸ߶� 
* 
* @retval ��
*/
static void Touch_Button_Init(void){
  int i;
  //��һ��
  for(i = 0 ;i<=8 ;i++ ){
    button[i].start_x = BUTTON_START_X; 
    button[i].end_x = button[i].start_x+COLOR_BLOCK_WIDTH ;
    button[i].start_y = COLOR_BLOCK_HEIGHT*(i+1);
    button[i].end_y = button[i].start_y + COLOR_BLOCK_HEIGHT;    
    button[i].touch_flag = 0;  
    button[i].draw_btn = Draw_Color_Button ;
    button[i].btn_command = Command_Select_Color ;
  }
  button[0].para = CL_BLACK; //������ť������
  button[1].para = CL_YELLOW;
  button[2].para = CL_BLUE ; 
  button[3].para = CL_CYAN;
  button[4].para = CL_RED;   
  button[5].para = CL_MAGENTA; 
  button[6].para = CL_GREEN;   
  button[7].para = CL_GREY1;
  button[8].para = CL_WHITE; 
 
  
  /*�ڶ��У���ҪΪ��ˢ��ť*/
  for(i = 0 ;i<7 ;i++ ){
    button[i+9].start_x = BUTTON_START_X + COLOR_BLOCK_WIDTH; 
    button[i+9].end_x = button[i+9].start_x + COLOR_BLOCK_WIDTH ;
    button[i+9].start_y = COLOR_BLOCK_HEIGHT*(i+1);
    button[i+9].end_y = button[i+9].start_y + COLOR_BLOCK_HEIGHT;    
    button[i+9].touch_flag = 0;  
    button[i+9].draw_btn = Draw_Shape_Button ;
    button[i+9].btn_command = Command_Select_Brush ;
  }
  
  button[9].para = LINE_SING_PIXCEL;
  button[10].para = LINE_2_PIXCEL;
  button[11].para = LINE_4_PIXCEL;
  button[12].para = LINE_6_PIXCEL;
  button[13].para = LINE_8_PIXCEL;
  button[14].para = LINE_16_PIXCEL;
  button[15].para = LINE_20_PIXCEL;
  
  
  button[16].start_x = BUTTON_START_X + COLOR_BLOCK_WIDTH; 
  button[16].end_x = button[16].start_x + COLOR_BLOCK_WIDTH ;
  button[16].start_y = COLOR_BLOCK_HEIGHT*(7+1);
  button[16].end_y = button[16].start_y + COLOR_BLOCK_HEIGHT;    
  button[16].touch_flag = 0;  
  button[16].draw_btn = Draw_RUBBER_Button ;
  button[16].btn_command = Command_RUBBER_Palette ;
  button[16].para = RUBBER;
  
  button[17].start_x = BUTTON_START_X + COLOR_BLOCK_WIDTH; 
  button[17].end_x = button[17].start_x + COLOR_BLOCK_WIDTH ;
  button[17].start_y = COLOR_BLOCK_HEIGHT*(8+1);
  button[17].end_y = button[17].start_y + COLOR_BLOCK_HEIGHT;    
  button[17].touch_flag = 0;  
  button[17].draw_btn = Draw_Clear_Button ;
  button[17].btn_command = Command_Clear_Palette ;
  button[17].para = Draw_CLEAR;
}


//==================================��ť����ʶ����=====================================
//=====================================================================================
/**
* @brief  Touch_Button_Down ����������ʱ���õĺ������ɴ���������
* @param  x ����λ�õ�x����
* @param  y ����λ�õ�y����
* @retval ��
*/
void Touch_Button_Down(uint16_t x,uint16_t y){  
  uint8_t  i ;
  for(i=0 ;i<BUTTON_NUM ;i++ ){
    //�������˰�ť
    if((x>=button[i].start_x)&&(y>=button[i].start_y)&&(x<=button[i].end_x)&&(y<=button[i].end_y)){
      if(button[i].touch_flag == 0){    /*ԭ����״̬Ϊû�а��£������״̬*/
        button[i].touch_flag = 1;       /* �ı䰴�±�־ */
        button[i].draw_btn(&button[i]); /*�ػ水ť*/  
      }
    }
    else{  /* �����Ƴ��˰����ķ�Χ��֮ǰ�а��°�ť */
      if(button[i].touch_flag == 1){    /* �����Ƴ��˰����ķ�Χ��֮ǰ�а��°�ť */
        button[i].touch_flag = 0;       /* ������±�־���ж�Ϊ�����*/
        button[i].draw_btn(&button[i]); /*�ػ水ť*/
      }
    }
  }
}  
  
  
/**
* @brief  Touch_Button_Up �������ͷ�ʱ���õĺ������ɴ���������
* @param  x ��������ͷ�ʱ��x����
* @param  y ��������ͷ�ʱ��y����
* @retval ��
*/ 
void Touch_Button_Up(uint16_t x,uint16_t y){ 
  uint8_t  i ;
  for(i=0 ;i<BUTTON_NUM ;i++ ){
    if((x>=button[i].start_x)&&(y>=button[i].start_y)&&(x<=button[i].end_x)&&(y<=button[i].end_y)){
      button[i].touch_flag = 0;       /* ������±�־���ж�Ϊ�����*/
      button[i].draw_btn(&button[i]); /*�ػ水ť*/
      
      button[i].btn_command(&button[i]);  /*ִ�а����Ĺ�������*/ 
      break;//���հ���˳�����ȼ�ִ��
    }
  } 
}


//==================================��ť�Ķ�������=====================================
//=====================================================================================
/**
* @brief  Draw_Color_Button ��ɫ��ť����溯��
* @param  btn Touch_Button ���͵İ�������
* @retval ��
*/
static void Draw_Color_Button(void *btn){
  Touch_Button *ptr = (Touch_Button *)btn;
  //�ͷŰ���
  if( ptr->touch_flag == 0){
    //����Ϊ���ܼ�����ɫ
    LCD_SetColors(ptr->para , CL_WHITE);
    ILI9341_DrawRectangle(ptr->start_x , ptr->start_y,\
                          ptr->end_x - ptr->start_x,\
												  ptr->end_y - ptr->start_y,1,1);
     
    //��ɫ�����߿�
    LCD_SetColors(CL_BOX_BORDER1 , CL_WHITE);
    ILI9341_DrawRectangle(ptr->start_x , ptr->start_y,\
                          ptr->end_x - ptr->start_x,\
												  ptr->end_y - ptr->start_y,0,2);
  }else{//��������
    //��ɫ����
    LCD_SetColors(CL_WHITE , CL_WHITE);
    ILI9341_DrawRectangle(ptr->start_x , ptr->start_y,\
                          ptr->end_x - ptr->start_x,\
												  ptr->end_y - ptr->start_y,1,1);
     //��ɫ�����߿�
    LCD_SetColors(CL_BOX_BORDER2 , CL_WHITE);
    ILI9341_DrawRectangle(ptr->start_x , ptr->start_y,\
                          ptr->end_x - ptr->start_x,\
												  ptr->end_y - ptr->start_y,0,2);
  }
  
}  

/**
* @brief  Draw_Shape_Button ��ˢ��ť����溯��
* @param  btn Touch_Button ���͵İ�������
* @retval ��
*/
static void Draw_Shape_Button(void *btn){
  Touch_Button *ptr = (Touch_Button *)btn;
  uint16_t i;
  //������ɫ û����ȥ��ʱ��Ϊ��ɫ ����ȥ�Ǻ�Ϊ��ɫ
  if(ptr->touch_flag == 0){
    LCD_SetColors(CL_BUTTON_GREY , CL_WHITE);
    ILI9341_DrawRectangle(	ptr->start_x,
                            ptr->start_y,
                            ptr->end_x - ptr->start_x,
                            ptr->end_y - ptr->start_y,1,1);
    //��ʾ���ֵı�����ɫ
    LCD_SetColors(CL_BLUE4 , CL_BUTTON_GREY);
    ILI9341_DrawRectangle(ptr->start_x,
                          ptr->start_y,
                          ptr->end_x - ptr->start_x,
                          ptr->end_y - ptr->start_y,0,1);
  }
  else{
    LCD_SetColors(CL_WHITE,CL_WHITE);
    ILI9341_DrawRectangle(	ptr->start_x,
                            ptr->start_y,
                            ptr->end_x - ptr->start_x,
                            ptr->end_y - ptr->start_y,1,1);
    
     /*��ʾ����ʱ�ı�����ɫ*/     
    LCD_SetColors(CL_BLUE4,CL_WHITE);
    ILI9341_DrawRectangle(	ptr->start_x,
                            ptr->start_y,
                            ptr->end_x - ptr->start_x,
                            ptr->end_y - ptr->start_y,0,1);
  }
  LCD_SetColors(CL_BLACK , CL_WHITE);
  // ���ݻ�ˢ��״��水ťͼ��
  switch(ptr->para){
    case LINE_SING_PIXCEL:
      LCD_SetColors(CL_BLACK , CL_WHITE);
      ILI9341_DrawLine(ptr->start_x+20,
                      ptr->start_y+(ptr->end_y-ptr->start_y)/2,
                      ptr->end_x-20,
                      ptr->start_y+(ptr->end_y-ptr->start_y)/2,1);
      break;
    case LINE_2_PIXCEL:
        LCD_DrawUniLineCircle(ptr->start_x+20,
                          ptr->start_y+(ptr->end_y-ptr->start_y)/2,ptr->end_x-20,
                          ptr->start_y+(ptr->end_y-ptr->start_y)/2,
                          1);
        break;
    case LINE_4_PIXCEL: 
        LCD_DrawUniLineCircle(ptr->start_x+20,
                          ptr->start_y+(ptr->end_y-ptr->start_y)/2,ptr->end_x-20,
                          ptr->start_y+(ptr->end_y-ptr->start_y)/2,
                          2);    
        break;
     case LINE_6_PIXCEL:
        LCD_DrawUniLineCircle(ptr->start_x+20,
                          ptr->start_y+(ptr->end_y-ptr->start_y)/2,ptr->end_x-20,
                          ptr->start_y+(ptr->end_y-ptr->start_y)/2,
                          3);
        break;      
     case LINE_8_PIXCEL:
       LCD_DrawUniLineCircle(ptr->start_x+20,
                          ptr->start_y+(ptr->end_y-ptr->start_y)/2,ptr->end_x-20,
                          ptr->start_y+(ptr->end_y-ptr->start_y)/2,
                          4);
        break;
      case LINE_16_PIXCEL:
            LCD_DrawUniLineCircle(ptr->start_x+20,
                          ptr->start_y+(ptr->end_y-ptr->start_y)/2,ptr->end_x-20,
                          ptr->start_y+(ptr->end_y-ptr->start_y)/2,
                          8 );
        break;
      case LINE_20_PIXCEL:
        LCD_DrawUniLineCircle(ptr->start_x+20,
                          ptr->start_y+(ptr->end_y-ptr->start_y)/2,ptr->end_x-20,
                          ptr->start_y+(ptr->end_y-ptr->start_y)/2,
                          10);
        break;
      case LINE_WITH_CIRCLE: 
        LCD_SetColors(CL_BLACK,CL_WHITE);      
        ILI9341_DrawLine(ptr->start_x+5,
                          ptr->start_y+(ptr->end_y-ptr->start_y)/2,
                          ptr->end_x-5,
                          ptr->start_y+(ptr->end_y-ptr->start_y)/2,1);
        for(i=0;i<((ptr->end_x - ptr->start_x-10)/10);i++){  
            ILI9341_DrawCircle(	ptr->start_x+5+i*10,
														ptr->start_y+(ptr->end_y-ptr->start_y)/2,
														3,1,1);
        }
        break;
       
  }
}    

/**
* @brief  Draw_RUBBER_Button ������ť����溯��
* @param  btn Touch_Button ���͵İ�������
* @retval ��
*/
static void Draw_RUBBER_Button(void *btn){
  Touch_Button *ptr = (Touch_Button *) btn;
  //�ͷŰ���
  if(ptr->touch_flag == 0){
    LCD_SetColors(CL_BUTTON_GREY,CL_WHITE);
    ILI9341_DrawRectangle(	ptr->start_x,
															ptr->start_y,
															ptr->end_x - ptr->start_x,
															ptr->end_y - ptr->start_y,1,1);
    LCD_SetColors(CL_RED , CL_BUTTON_GREY);
    /*ѡ�����壬ʹ����Ӣ����ʾʱ��������Ӣ��ѡ���8*16�����壬
		*���������С��16*16�ģ���Ҫ��������������������ģ*/
		/*�������ֻ��Ӣ������������*/
    LCD_SetFont(&Font8x16);
    ILI9341_DispString_EN_CH( ptr->start_x + (ptr->end_x - ptr->start_x - 16*2)/2,
                               ptr->start_y +(ptr->end_y - ptr->start_y - 16)/2,"��Ƥ");
  }
  else{ //��������
    LCD_SetColors(CL_WHITE , CL_WHITE);
    ILI9341_DrawRectangle(ptr->start_x,
                          ptr->start_y,
                          ptr->end_x - ptr->start_x,
                          ptr->end_y - ptr->start_y,1,1);
    LCD_SetColors(CL_RED,CL_WHITE);
		/*ѡ�����壬ʹ����Ӣ����ʾʱ��������Ӣ��ѡ���8*16�����壬
		*���������С��16*16�ģ���Ҫ��������������������ģ*/
		/*�������ֻ��Ӣ������������*/
		LCD_SetFont(&Font8x16);
    ILI9341_DispString_EN_CH( ptr->start_x + (ptr->end_x - ptr->start_x - 16*2 )/2,                   
																ptr->start_y+ ((ptr->end_y - ptr->start_y-16)/2),"��Ƥ");
  }
  //��ť�߿�
  LCD_SetColors(CL_BLUE4 , CL_WHITE);
  ILI9341_DrawRectangle(ptr->start_x,
														ptr->start_y,
														ptr->end_x - ptr->start_x,
														ptr->end_y - ptr->start_y,0,1);
}    
  
/**
* @brief  Draw_Clear_Button ������ť����溯��
* @param  btn Touch_Button ���͵İ�������
* @retval ��
*/
static void Draw_Clear_Button(void *btn){
  Touch_Button *ptr = (Touch_Button *) btn;
  //�ͷŰ���
  if(ptr->touch_flag == 0){
    LCD_SetColors(CL_BUTTON_GREY,CL_WHITE);
    ILI9341_DrawRectangle(	ptr->start_x,
															ptr->start_y,
															ptr->end_x - ptr->start_x,
															ptr->end_y - ptr->start_y,1,1);
    LCD_SetColors(CL_RED , CL_BUTTON_GREY);
    /*ѡ�����壬ʹ����Ӣ����ʾʱ��������Ӣ��ѡ���8*16�����壬
		*���������С��16*16�ģ���Ҫ��������������������ģ*/
		/*�������ֻ��Ӣ������������*/
    LCD_SetFont(&Font8x16);
    ILI9341_DispString_EN_CH( ptr->start_x + (ptr->end_x - ptr->start_x - 16*2)/2,
                               ptr->start_y +(ptr->end_y - ptr->start_y - 16)/2,"����");
  }
  else{ //��������
    LCD_SetColors(CL_WHITE , CL_WHITE);
    ILI9341_DrawRectangle(ptr->start_x,
                          ptr->start_y,
                          ptr->end_x - ptr->start_x,
                          ptr->end_y - ptr->start_y,1,1);
    LCD_SetColors(CL_RED,CL_WHITE);
		/*ѡ�����壬ʹ����Ӣ����ʾʱ��������Ӣ��ѡ���8*16�����壬
		*���������С��16*16�ģ���Ҫ��������������������ģ*/
		/*�������ֻ��Ӣ������������*/
		LCD_SetFont(&Font8x16);
    ILI9341_DispString_EN_CH( ptr->start_x + (ptr->end_x - ptr->start_x - 16*2 )/2,                   
																ptr->start_y+ ((ptr->end_y - ptr->start_y-16)/2),"����");
  }
  //��ť�߿�
  LCD_SetColors(CL_BLUE4 , CL_WHITE);
  ILI9341_DrawRectangle(ptr->start_x,
														ptr->start_y,
														ptr->end_x - ptr->start_x,
														ptr->end_y - ptr->start_y,0,1);
}  


//==================================��ť��ָ���=====================================
//=====================================================================================

/**
* @brief  Command_Select_Color �л���ˢ��ɫ����ɫ�����Ĺ���ִ�к���
* @param  btn Touch_Button ���͵İ�������
* @retval ��
*/
static void Command_Select_Color(void *btn){
  Touch_Button *ptr = (Touch_Button *)btn;
  brush.color = ptr->para;
  LCD_SetColors (brush.color , CL_WHITE );
}  

/**
* @brief  Command_Select_Brush �л���ˢ��ɫ����ˢ�����Ĺ���ִ�к���
* @param  btn Touch_Button ���͵İ�������
* @retval ��
*/
static void Command_Select_Brush(void *btn){
  Touch_Button *ptr = (Touch_Button *)btn;
  brush.shape = (SHAPE)ptr->para;  
  LCD_SetColors(brush.color , CL_WHITE); 
}  


/**
* @brief  Command_RUBBER_Palette �л���ˢ��ɫ�����������Ĺ���ִ�к���
* @param  btn Touch_Button ���͵İ�������
* @retval ��
*/
static void Command_RUBBER_Palette(void *btn){
  brush.color = CL_WHITE;
  brush.shape = LINE_6_PIXCEL;
  LCD_SetColors(brush.color , CL_WHITE);
}

/**
* @brief  Command_Select_Brush �л���ˢ��ɫ�����������Ĺ���ִ�к���
* @param  btn Touch_Button ���͵İ�������
* @retval ��
*/
static void Command_Clear_Palette(void *btn){
  LCD_SetColors(CL_WHITE , CL_WHITE);
  ILI9341_DrawRectangle(PALETTE_START_X,
                        PALETTE_START_Y,                    
                        PALETTE_END_X-(PALETTE_START_X+1), 
                        PALETTE_END_Y-PALETTE_START_Y ,1,1);
}

//==================================��ť�Ķ�������=====================================
//=====================================================================================


/**
* @brief  Draw_Trail �ڻ���������津���켣
* @param  pre_x ��һ���x����
* @param  pre_y ��һ���y����
* @param  x     ����һ���x����
* @param  y     ����һ���y����
* @param  brush ��ˢ����
* @retval ��
*/
void Draw_Trail(
  int16_t pre_x,int16_t pre_y,
  int16_t x,int16_t y,
  Brush_Style* brush
){  
  //���û�������Ϊ�����
  /*����λ���ڻ�������*/
  if((x > PALETTE_START_X)&&(pre_x > PALETTE_START_X)){
    switch( brush->shape ){ //  ���ݻ�ˢ�Ĳ�ͬ�滭��ͬ�Ĺ켣
      //���1���ؿ�ȵĹ켣��
      case LINE_SING_PIXCEL:
        if((pre_x < 0)&&(pre_y<0)){ //�µıʼ�
          ILI9341_SetPointPixel(x,y,1);
        }else{
          ILI9341_DrawLine(pre_x,pre_y,x,y,1);
        }
        break;
        
      case LINE_2_PIXCEL:
        if(((x-1) < PALETTE_START_X )|| ((y-1) < PALETTE_START_X))
          break;
        LCD_DrawUniLineCircle(pre_x,pre_y,x,y,1);
        break;
        
      case LINE_4_PIXCEL:				
				if(x-2<PALETTE_START_X||pre_x-2<PALETTE_START_X)	//������߽�
          break;        
        LCD_DrawUniLineCircle(pre_x,pre_y,x,y,2);
        break;
      
      case LINE_6_PIXCEL:        
				if(x-3<PALETTE_START_X||pre_x-3<PALETTE_START_X)	//������߽�
          break;      
        LCD_DrawUniLineCircle(pre_x,pre_y,x,y,3);
        break;
      
      case LINE_8_PIXCEL:				
				if(x-4<PALETTE_START_X||pre_x-4<PALETTE_START_X)	//������߽�
          break;        
        LCD_DrawUniLineCircle(pre_x,pre_y,x,y,4);
        break;
      
     case LINE_16_PIXCEL:			 
		 		if(x-8<PALETTE_START_X||pre_x-8<PALETTE_START_X)	//������߽�
					break;       
        LCD_DrawUniLineCircle(pre_x,pre_y,x,y,8);
        break;
        
      case LINE_20_PIXCEL:				
				if(x-10<PALETTE_START_X ||pre_x-10<PALETTE_START_X)	//������߽�
				break;
        LCD_DrawUniLineCircle(pre_x,pre_y,x,y,10);
        break;  
        
      /*�������ӵĵ�������*/
      case LINE_WITH_CIRCLE:  
		 		if(x-3<PALETTE_START_X||pre_x-3<PALETTE_START_X)	//������߽�
					break;			         
            if(pre_x< 0 || pre_y< 0){//�µıʼ�                 
              ILI9341_SetPointPixel(x,y,1); 
            }
            else{ //������һ�εıʼ�                  
              ILI9341_DrawLine(pre_x,pre_y,x,y,1);
              ILI9341_DrawCircle(x,y,3,1,1);
            } 
        break;    

      /*��Ƥ����*/            
      case RUBBER:
				
				if(x-20<PALETTE_START_X ||						//������߽�
					  x+20>LCD_X_LENGTH || x-20<0 || //Һ�����ұ߽�
						y+20>LCD_Y_LENGTH || y-20<0)	 //Һ�����±߽�				
					break;	
			
  //    if(x>PALETTE_START_X+20)       
				{ LCD_SetColors(CL_WHITE,CL_WHITE); 
					ILI9341_DrawRectangle( x-40/2,y-40/2,40,40,1,1);     
        }
      break;  

        
    }
  }
  
}
  










/* ------------------------------------------end of file---------------------------------------- */


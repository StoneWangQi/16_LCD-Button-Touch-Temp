/**
  ******************************************************************************
  * @file    palette.c
  * @author  fire
  * @version V1.0
  * @date     
  * @brief   触摸画板应用函数
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

#include "XPT2046_LCD_Device_book.h"

//按钮的结构体数组
Touch_Button button[BUTTON_NUM];
//画笔参数
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
* @brief  Palette_Init 画板初始化
* @param  无
* @retval 无
*/
void Palette_Init(
  uint8_t  _LCD_GramScan_Mode ,//屏幕方向
  uint16_t _Page_Color ,  //画板颜色
  uint16_t _Brush_Color   //画笔颜色
){
  uint8_t i ;
  ILI9341_GramScan(_LCD_GramScan_Mode);
  
  //初始化画板颜色
  LCD_SetBackColor(_Page_Color);
  ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);
  
  
  LCD_SetColors(GB888(0xEAC599) , CL_WHITE);
  ILI9341_DrawRectangle(0,
                        0,                    
                        LCD_X_LENGTH-(PALETTE_END_X-PALETTE_START_X), 
                        LCD_Y_LENGTH,1,1);
   
  //初始化按钮
  Touch_Button_Init();
  
  //描绘按钮
  for(i=0 ;i<BUTTON_NUM ;i++ ){
    button[i].draw_btn(&button[i]);
  }
  //初始化画笔
  brush.color = _Brush_Color;
  brush.shape = LINE_SING_PIXCEL ;//单像素线
  //初始化文本颜色
  
  LCD_SetTextColor(brush.color);
}
  
  
/**
* @brief  Touch_Button_Init 初始化按钮参数
* @param  无
* #define COLOR_BLOCK_WIDTH   40  //默认按钮的大小
* #define COLOR_BLOCK_HEIGHT  28  //默认按钮的高度 
* 
* @retval 无
*/
static void Touch_Button_Init(void){
  int i;
  //第一列
  for(i = 0 ;i<=8 ;i++ ){
    button[i].start_x = BUTTON_START_X; 
    button[i].end_x = button[i].start_x+COLOR_BLOCK_WIDTH ;
    button[i].start_y = COLOR_BLOCK_HEIGHT*(i+1);
    button[i].end_y = button[i].start_y + COLOR_BLOCK_HEIGHT;    
    button[i].touch_flag = 0;  
    button[i].draw_btn = Draw_Color_Button ;
    button[i].btn_command = Command_Select_Color ;
  }
  button[0].para = CL_BLACK; //构建按钮的属性
  button[1].para = CL_YELLOW;
  button[2].para = CL_BLUE ; 
  button[3].para = CL_CYAN;
  button[4].para = CL_RED;   
  button[5].para = CL_MAGENTA; 
  button[6].para = CL_GREEN;   
  button[7].para = CL_GREY1;
  button[8].para = CL_WHITE; 
 
  
  /*第二列，主要为画刷按钮*/
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


//==================================按钮触摸识别函数=====================================
//=====================================================================================
/**
* @brief  Touch_Button_Down 按键被按下时调用的函数，由触摸屏调用
* @param  x 触摸位置的x坐标
* @param  y 触摸位置的y坐标
* @retval 无
*/
void Touch_Button_Down(uint16_t x,uint16_t y){  
  uint8_t  i ;
  for(i=0 ;i<BUTTON_NUM ;i++ ){
    //触摸到了按钮
    if((x>=button[i].start_x)&&(y>=button[i].start_y)&&(x<=button[i].end_x)&&(y<=button[i].end_y)){
      if(button[i].touch_flag == 0){    /*原本的状态为没有按下，则更新状态*/
        button[i].touch_flag = 1;       /* 改变按下标志 */
        button[i].draw_btn(&button[i]); /*重绘按钮*/  
      }
    }
    else{  /* 触摸移出了按键的范围且之前有按下按钮 */
      if(button[i].touch_flag == 1){    /* 触摸移出了按键的范围且之前有按下按钮 */
        button[i].touch_flag = 0;       /* 清除按下标志，判断为误操作*/
        button[i].draw_btn(&button[i]); /*重绘按钮*/
      }
    }
  }
}  
  
  
/**
* @brief  Touch_Button_Up 按键被释放时调用的函数，由触摸屏调用
* @param  x 触摸最后释放时的x坐标
* @param  y 触摸最后释放时的y坐标
* @retval 无
*/ 
void Touch_Button_Up(uint16_t x,uint16_t y){ 
  uint8_t  i ;
  for(i=0 ;i<BUTTON_NUM ;i++ ){
    if((x>=button[i].start_x)&&(y>=button[i].start_y)&&(x<=button[i].end_x)&&(y<=button[i].end_y)){
      button[i].touch_flag = 0;       /* 清除按下标志，判断为误操作*/
      button[i].draw_btn(&button[i]); /*重绘按钮*/
      
      button[i].btn_command(&button[i]);  /*执行按键的功能命令*/ 
      break;//安照按键顺序优先级执行
    }
  } 
}


//==================================按钮的动作函数=====================================
//=====================================================================================
/**
* @brief  Draw_Color_Button 颜色按钮的描绘函数
* @param  btn Touch_Button 类型的按键参数
* @retval 无
*/
static void Draw_Color_Button(void *btn){
  Touch_Button *ptr = (Touch_Button *)btn;
  //释放按键
  if( ptr->touch_flag == 0){
    //背景为功能键的颜色
    LCD_SetColors(ptr->para , CL_WHITE);
    ILI9341_DrawRectangle(ptr->start_x , ptr->start_y,\
                          ptr->end_x - ptr->start_x,\
												  ptr->end_y - ptr->start_y,1,1);
     
    //白色背景边框
    LCD_SetColors(CL_BOX_BORDER1 , CL_WHITE);
    ILI9341_DrawRectangle(ptr->start_x , ptr->start_y,\
                          ptr->end_x - ptr->start_x,\
												  ptr->end_y - ptr->start_y,0,2);
  }else{//按键按下
    //白色背景
    LCD_SetColors(CL_WHITE , CL_WHITE);
    ILI9341_DrawRectangle(ptr->start_x , ptr->start_y,\
                          ptr->end_x - ptr->start_x,\
												  ptr->end_y - ptr->start_y,1,1);
     //白色背景边框
    LCD_SetColors(CL_BOX_BORDER2 , CL_WHITE);
    ILI9341_DrawRectangle(ptr->start_x , ptr->start_y,\
                          ptr->end_x - ptr->start_x,\
												  ptr->end_y - ptr->start_y,0,2);
  }
  
}  

/**
* @brief  Draw_Shape_Button 笔刷按钮的描绘函数
* @param  btn Touch_Button 类型的按键参数
* @retval 无
*/
static void Draw_Shape_Button(void *btn){
  Touch_Button *ptr = (Touch_Button *)btn;
  uint16_t i;
  //背景颜色 没按下去的时候为灰色 按下去是后为白色
  if(ptr->touch_flag == 0){
    LCD_SetColors(CL_BUTTON_GREY , CL_WHITE);
    ILI9341_DrawRectangle(	ptr->start_x,
                            ptr->start_y,
                            ptr->end_x - ptr->start_x,
                            ptr->end_y - ptr->start_y,1,1);
    //显示文字的背景颜色
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
    
     /*显示文字时的背景颜色*/     
    LCD_SetColors(CL_BLUE4,CL_WHITE);
    ILI9341_DrawRectangle(	ptr->start_x,
                            ptr->start_y,
                            ptr->end_x - ptr->start_x,
                            ptr->end_y - ptr->start_y,0,1);
  }
  LCD_SetColors(CL_BLACK , CL_WHITE);
  // 根据画刷形状描绘按钮图案
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
* @brief  Draw_RUBBER_Button 清屏按钮的描绘函数
* @param  btn Touch_Button 类型的按键参数
* @retval 无
*/
static void Draw_RUBBER_Button(void *btn){
  Touch_Button *ptr = (Touch_Button *) btn;
  //释放按键
  if(ptr->touch_flag == 0){
    LCD_SetColors(CL_BUTTON_GREY,CL_WHITE);
    ILI9341_DrawRectangle(	ptr->start_x,
															ptr->start_y,
															ptr->end_x - ptr->start_x,
															ptr->end_y - ptr->start_y,1,1);
    LCD_SetColors(CL_RED , CL_BUTTON_GREY);
    /*选择字体，使用中英文显示时，尽量把英文选择成8*16的字体，
		*中文字体大小是16*16的，需要其它字体请自行制作字模*/
		/*这个函数只对英文字体起作用*/
    LCD_SetFont(&Font8x16);
    ILI9341_DispString_EN_CH( ptr->start_x + (ptr->end_x - ptr->start_x - 16*2)/2,
                               ptr->start_y +(ptr->end_y - ptr->start_y - 16)/2,"橡皮");
  }
  else{ //按键按下
    LCD_SetColors(CL_WHITE , CL_WHITE);
    ILI9341_DrawRectangle(ptr->start_x,
                          ptr->start_y,
                          ptr->end_x - ptr->start_x,
                          ptr->end_y - ptr->start_y,1,1);
    LCD_SetColors(CL_RED,CL_WHITE);
		/*选择字体，使用中英文显示时，尽量把英文选择成8*16的字体，
		*中文字体大小是16*16的，需要其它字体请自行制作字模*/
		/*这个函数只对英文字体起作用*/
		LCD_SetFont(&Font8x16);
    ILI9341_DispString_EN_CH( ptr->start_x + (ptr->end_x - ptr->start_x - 16*2 )/2,                   
																ptr->start_y+ ((ptr->end_y - ptr->start_y-16)/2),"橡皮");
  }
  //按钮边框
  LCD_SetColors(CL_BLUE4 , CL_WHITE);
  ILI9341_DrawRectangle(ptr->start_x,
														ptr->start_y,
														ptr->end_x - ptr->start_x,
														ptr->end_y - ptr->start_y,0,1);
}    
  
/**
* @brief  Draw_Clear_Button 清屏按钮的描绘函数
* @param  btn Touch_Button 类型的按键参数
* @retval 无
*/
static void Draw_Clear_Button(void *btn){
  Touch_Button *ptr = (Touch_Button *) btn;
  //释放按键
  if(ptr->touch_flag == 0){
    LCD_SetColors(CL_BUTTON_GREY,CL_WHITE);
    ILI9341_DrawRectangle(	ptr->start_x,
															ptr->start_y,
															ptr->end_x - ptr->start_x,
															ptr->end_y - ptr->start_y,1,1);
    LCD_SetColors(CL_RED , CL_BUTTON_GREY);
    /*选择字体，使用中英文显示时，尽量把英文选择成8*16的字体，
		*中文字体大小是16*16的，需要其它字体请自行制作字模*/
		/*这个函数只对英文字体起作用*/
    LCD_SetFont(&Font8x16);
    ILI9341_DispString_EN_CH( ptr->start_x + (ptr->end_x - ptr->start_x - 16*2)/2,
                               ptr->start_y +(ptr->end_y - ptr->start_y - 16)/2,"清屏");
  }
  else{ //按键按下
    LCD_SetColors(CL_WHITE , CL_WHITE);
    ILI9341_DrawRectangle(ptr->start_x,
                          ptr->start_y,
                          ptr->end_x - ptr->start_x,
                          ptr->end_y - ptr->start_y,1,1);
    LCD_SetColors(CL_RED,CL_WHITE);
		/*选择字体，使用中英文显示时，尽量把英文选择成8*16的字体，
		*中文字体大小是16*16的，需要其它字体请自行制作字模*/
		/*这个函数只对英文字体起作用*/
		LCD_SetFont(&Font8x16);
    ILI9341_DispString_EN_CH( ptr->start_x + (ptr->end_x - ptr->start_x - 16*2 )/2,                   
																ptr->start_y+ ((ptr->end_y - ptr->start_y-16)/2),"清屏");
  }
  //按钮边框
  LCD_SetColors(CL_BLUE4 , CL_WHITE);
  ILI9341_DrawRectangle(ptr->start_x,
														ptr->start_y,
														ptr->end_x - ptr->start_x,
														ptr->end_y - ptr->start_y,0,1);
}  


//==================================按钮的指令函数=====================================
//=====================================================================================

/**
* @brief  Command_Select_Color 切换画刷颜色，颜色按键的功能执行函数
* @param  btn Touch_Button 类型的按键参数
* @retval 无
*/
static void Command_Select_Color(void *btn){
  Touch_Button *ptr = (Touch_Button *)btn;
  brush.color = ptr->para;
  LCD_SetColors (brush.color , CL_WHITE );
}  

/**
* @brief  Command_Select_Brush 切换画刷颜色，画刷按键的功能执行函数
* @param  btn Touch_Button 类型的按键参数
* @retval 无
*/
static void Command_Select_Brush(void *btn){
  Touch_Button *ptr = (Touch_Button *)btn;
  brush.shape = (SHAPE)ptr->para;  
  LCD_SetColors(brush.color , CL_WHITE); 
}  


/**
* @brief  Command_RUBBER_Palette 切换画刷颜色，清屏按键的功能执行函数
* @param  btn Touch_Button 类型的按键参数
* @retval 无
*/
static void Command_RUBBER_Palette(void *btn){
  brush.color = CL_WHITE;
  brush.shape = LINE_6_PIXCEL;
  LCD_SetColors(brush.color , CL_WHITE);
}

/**
* @brief  Command_Select_Brush 切换画刷颜色，清屏按键的功能执行函数
* @param  btn Touch_Button 类型的按键参数
* @retval 无
*/
static void Command_Clear_Palette(void *btn){
  LCD_SetColors(CL_WHITE , CL_WHITE);
  ILI9341_DrawRectangle(PALETTE_START_X,
                        PALETTE_START_Y,                    
                        PALETTE_END_X-(PALETTE_START_X+1), 
                        PALETTE_END_Y-PALETTE_START_Y ,1,1);
}

//==================================按钮的动作函数=====================================
//=====================================================================================


/**
* @brief  Draw_Trail 在画板区域描绘触摸轨迹
* @param  pre_x 上一点的x坐标
* @param  pre_y 上一点的y坐标
* @param  x     最新一点的x坐标
* @param  y     最新一点的y坐标
* @param  brush 画刷参数
* @retval 无
*/
void Draw_Trail(
  int16_t pre_x,int16_t pre_y,
  int16_t x,int16_t y,
  Brush_Style* brush
){  
  //设置画板区域为活动窗口
  /*触摸位置在画板区域*/
  if((x > PALETTE_START_X)&&(pre_x > PALETTE_START_X)){
    switch( brush->shape ){ //  根据画刷的不同绘画不同的轨迹
      //描绘1像素宽度的轨迹线
      case LINE_SING_PIXCEL:
        if((pre_x < 0)&&(pre_y<0)){ //新的笔记
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
				if(x-2<PALETTE_START_X||pre_x-2<PALETTE_START_X)	//画板左边界
          break;        
        LCD_DrawUniLineCircle(pre_x,pre_y,x,y,2);
        break;
      
      case LINE_6_PIXCEL:        
				if(x-3<PALETTE_START_X||pre_x-3<PALETTE_START_X)	//画板左边界
          break;      
        LCD_DrawUniLineCircle(pre_x,pre_y,x,y,3);
        break;
      
      case LINE_8_PIXCEL:				
				if(x-4<PALETTE_START_X||pre_x-4<PALETTE_START_X)	//画板左边界
          break;        
        LCD_DrawUniLineCircle(pre_x,pre_y,x,y,4);
        break;
      
     case LINE_16_PIXCEL:			 
		 		if(x-8<PALETTE_START_X||pre_x-8<PALETTE_START_X)	//画板左边界
					break;       
        LCD_DrawUniLineCircle(pre_x,pre_y,x,y,8);
        break;
        
      case LINE_20_PIXCEL:				
				if(x-10<PALETTE_START_X ||pre_x-10<PALETTE_START_X)	//画板左边界
				break;
        LCD_DrawUniLineCircle(pre_x,pre_y,x,y,10);
        break;  
        
      /*描绘带珠子的单像素线*/
      case LINE_WITH_CIRCLE:  
		 		if(x-3<PALETTE_START_X||pre_x-3<PALETTE_START_X)	//画板左边界
					break;			         
            if(pre_x< 0 || pre_y< 0){//新的笔迹                 
              ILI9341_SetPointPixel(x,y,1); 
            }
            else{ //继续上一次的笔迹                  
              ILI9341_DrawLine(pre_x,pre_y,x,y,1);
              ILI9341_DrawCircle(x,y,3,1,1);
            } 
        break;    

      /*橡皮功能*/            
      case RUBBER:
				
				if(x-20<PALETTE_START_X ||						//画板左边界
					  x+20>LCD_X_LENGTH || x-20<0 || //液晶左右边界
						y+20>LCD_Y_LENGTH || y-20<0)	 //液晶上下边界				
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


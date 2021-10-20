#include "LCD_Draw_book.h" 
#include "Systick_book.h"

sFONT *LCD_Currentfonts = &Font8x16;  //英文字体
uint16_t CurrentForecolor   = BLACK;//前景色
uint16_t CurrentBackColor   = BACKGROUND;//背景色

//================================ 接口函数=======================================

/**
 * @brief  在ILI9341显示器上开辟一个窗口
 * @param  usX ：在特定扫描方向下窗口的起点X坐标
 * @param  usY ：在特定扫描方向下窗口的起点Y坐标
 * @param  usWidth ：窗口的宽度
 * @param  usHeight ：窗口的高度
 * @retval 无
 */
void ILI9341_OpenWindow ( uint16_t usX, uint16_t usY, uint16_t usWidth, uint16_t usHeight ){
	ILI9341_Write_Cmd ( CMD_SetCoordinateX ); 				 /* 设置X坐标 */
	ILI9341_Write_Data ( (usX & 0xFF00) >> 8  );	 /* 先高8位，然后低8位 */
	ILI9341_Write_Data ( usX & 0xff  );	 /* 设置起始点和结束点*/
	ILI9341_Write_Data ( ( usX + usWidth - 1 ) >> 8  );
	ILI9341_Write_Data ( ( usX + usWidth - 1 ) & 0xff  );

	ILI9341_Write_Cmd ( CMD_SetCoordinateY ); 			     /* 设置Y坐标*/
	ILI9341_Write_Data ( (usY & 0xFF00 )>> 8  );
	ILI9341_Write_Data ( usY & 0xff  );
	ILI9341_Write_Data ( ( usY + usHeight - 1 ) >> 8 );
	ILI9341_Write_Data ( ( usY + usHeight - 1) & 0xff );
	
}

/**
 * @brief  设定ILI9341的光标坐标
 * @param  usX ：在特定扫描方向下光标的X坐标
 * @param  usY ：在特定扫描方向下光标的Y坐标
 * @retval 无
 */
static void ILI9341_SetCursor ( 
  uint16_t usX, 
  uint16_t usY,
  uint16_t usW
){
	ILI9341_OpenWindow ( usX, usY, usW, usW );
}



/**
 * @brief  读取ILI9341 GRAN 的一个像素数据
 * @param  无
 * @retval 像素数据
 */
static uint16_t ILI9341_Read_PixelData ( void )	
{	
	uint16_t usR=0, usG=0, usB=0 ;

	
	ILI9341_Write_Cmd ( 0x2E );   /* 读数据 */
	
	usR = ILI9341_Read_Data (); 	/*FIRST READ OUT DUMMY DATA*/
	
	usR = ILI9341_Read_Data ();  	/*READ OUT RED DATA  */
	usB = ILI9341_Read_Data ();  	/*READ OUT BLUE DATA*/
	usG = ILI9341_Read_Data ();  	/*READ OUT GREEN DATA*/	
	
  return ( ( ( usR >> 11 ) << 11 ) | ( ( usG >> 10 ) << 5 ) | ( usB >> 11 ) );
	
}


/**
 * @brief  获取 ILI9341 显示器上某一个坐标点的像素数据
 * @param  usX ：在特定扫描方向下该点的X坐标
 * @param  usY ：在特定扫描方向下该点的Y坐标
 * @retval 像素数据
 */
uint16_t ILI9341_GetPointPixel ( uint16_t usX, uint16_t usY )
{ 
	uint16_t usPixelData;

	
	ILI9341_SetCursor ( usX, usY ,1);
	
	usPixelData = ILI9341_Read_PixelData ();
	
	return usPixelData;
	
}



/**
 * @brief  在ILI9341显示器上以某一颜色填充像素点
 * @param  ulAmout_Point ：要填充颜色的像素点的总数目
 * @param  usColor ：颜色
 * @retval 无
 */
static __inline void ILI9341_FillColor ( uint32_t ulAmout_Point, uint16_t usColor ){
	uint32_t i = 0;
	/* memory write */
	ILI9341_Write_Cmd ( CMD_SetPixel );	
	for ( i = 0; i < ulAmout_Point; i ++ )
		ILI9341_Write_Data ( usColor );
}

/**
 * @brief  对ILI9341显示器的某一窗口以某种颜色进行清屏
 * @param  usX ：在特定扫描方向下窗口的起点X坐标
 * @param  usY ：在特定扫描方向下窗口的起点Y坐标
 * @param  usWidth ：窗口的宽度
 * @param  usHeight ：窗口的高度
 * @note 可使用LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors函数设置颜色
 * @retval 无
 */
void ILI9341_Clear ( 
  uint16_t usX, 
  uint16_t usY, 
  uint16_t usWidth,
  uint16_t usHeight 
){
	ILI9341_OpenWindow ( usX, usY, usWidth, usHeight );
	ILI9341_FillColor ( usWidth * usHeight, CurrentBackColor );		
}

/**
  * @brief  清除某行文字
  * @param  Line: 指定要删除的行
  *   本参数可使用宏LINE(0)、LINE(1)等方式指定要删除的行，
  *   宏LINE(x)会根据当前选择的字体来计算Y坐标值，并删除当前字体高度的第x行。
  * @retval None
  */
void LCD_ClearLine(uint16_t Line){
  ILI9341_Clear(0,Line,LCD_X_LENGTH,((sFONT *)LCD_GetFont())->Height);	/* 清屏，显示全黑 */

}
//================================ 颜色状态函数=======================================
/**
  * @brief  设置LCD的前景(字体)及背景颜色,RGB565
  * @param  TextColor: 指定前景(字体)颜色
  * @param  BackColor: 指定背景颜色
  * @retval None
  */
void LCD_SetColors(uint16_t TextColor, uint16_t BackColor){
  CurrentForecolor = TextColor; 
  CurrentBackColor = BackColor;
}
/**
  * @brief  设置LCD的前景(字体)颜色,RGB565
  * @param  Color: 指定前景(字体)颜色 
  * @retval None
  */
void LCD_SetForeColor(uint16_t Color){
  CurrentForecolor = Color;
}

/**
  * @brief  设置LCD的背景颜色,RGB565
  * @param  Color: 指定背景颜色 
  * @retval None
  */
void LCD_SetBackColor(uint16_t Color){
  CurrentBackColor = Color;
}

 
//================================ 形状绘制应用函数=======================================


/**
 * @brief  对ILI9341显示器的某一点以某种颜色进行填充
 * @param  usX ：在特定扫描方向下该点的X坐标
 * @param  usY ：在特定扫描方向下该点的Y坐标
 * @param  usW ：在特定扫描宽度
 * @note 可使用LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors函数设置颜色
 * @retval 无
 */
void ILI9341_SetPointPixel (
  uint16_t usX, 
  uint16_t usY,
  uint16_t usW
){	
	if ( ( usX < LCD_X_LENGTH ) && ( usY < LCD_Y_LENGTH ) ){
		ILI9341_SetCursor ( usX, usY ,usW);
		ILI9341_FillColor ( usW*usW, CurrentForecolor );
	}
}

/**
 * @brief  在 ILI9341 显示器上使用 Bresenham 算法画线段 
 * @param  usX1 ：在特定扫描方向下线段的一个端点X坐标
 * @param  usY1 ：在特定扫描方向下线段的一个端点Y坐标
 * @param  usX2 ：在特定扫描方向下线段的另一个端点X坐标
 * @param  usY2 ：在特定扫描方向下线段的另一个端点Y坐标
 * @note 可使用LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors函数设置颜色
 * @retval 无
 */
void ILI9341_DrawLine ( 
  uint16_t usX1, 
  uint16_t usY1,
  uint16_t usX2,
  uint16_t usY2, 
  uint16_t usW
){
	uint16_t us; 
	uint16_t usX_Current, usY_Current;
	
	int32_t lError_X = 0, lError_Y = 0, lDelta_X, lDelta_Y, lDistance; 
	int32_t lIncrease_X, lIncrease_Y; 	
	
	lDelta_X = usX2 - usX1; //计算坐标增量 
	lDelta_Y = usY2 - usY1; 
	
	usX_Current = usX1; 
	usY_Current = usY1; 
	
	if (lDelta_X > 0) lIncrease_X = 1; //设置单步方向 
	else if (lDelta_X == 0) lIncrease_X = 0;//垂直线 
	else { 
    lIncrease_X = -1;
    lDelta_X = - lDelta_X;
  } 

	if (lDelta_Y > 0) lIncrease_Y = 1; 
	else if (lDelta_Y == 0)	lIncrease_Y = 0;//水平线 
	else {
    lIncrease_Y = -1;
    lDelta_Y = - lDelta_Y;
  } 
	
	if (lDelta_X > lDelta_Y )
		lDistance = lDelta_X; //选取基本增量坐标轴 
	else 
		lDistance = lDelta_Y; 

	//-------------------进行化线处理--------------------
	for ( us = 0; us <= lDistance + 1; us ++ ){//画线输出 
    ILI9341_SetPointPixel ( usX_Current, usY_Current , usW );//画点 
    
		lError_X += lDelta_X ; 
		lError_Y += lDelta_Y ; 
		
		if ( lError_X > lDistance ) 
		{ 
			lError_X -= lDistance; 
			usX_Current += lIncrease_X; 
		}  
		
		if ( lError_Y > lDistance ) 
		{ 
			lError_Y -= lDistance; 
			usY_Current += lIncrease_Y; 
		} 
		
	}  
}   



/**
 * @brief  在 ILI9341 显示器上画一个矩形
 * @param  usX_Start ：在特定扫描方向下矩形的起始点X坐标
 * @param  usY_Start ：在特定扫描方向下矩形的起始点Y坐标
 * @param  usWidth：矩形的宽度（单位：像素）
 * @param  usHeight：矩形的高度（单位：像素）
 * @param  ucFilled ：选择是否填充该矩形
  *   该参数为以下值之一：
  *     @arg 0 :空心矩形
  *     @arg 1 :实心矩形 
 * @note 可使用LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors函数设置颜色
 * @retval 无
 */
void ILI9341_DrawRectangle ( 
  uint16_t usX_Start,     //X坐标
  uint16_t usY_Start,     //Y坐标 
  uint16_t usWidth,       //X宽度
  uint16_t usHeight,      //Y宽度
  uint8_t ucFilled ,      //填充类型
  uint16_t usW            //填充线宽度
){
	if ( ucFilled ){
		ILI9341_OpenWindow ( usX_Start, usY_Start, usWidth, usHeight );
		ILI9341_FillColor ( usWidth * usHeight ,CurrentForecolor);	
	}
	else	{
		ILI9341_DrawLine ( usX_Start, usY_Start, usX_Start + usWidth - 1, usY_Start ,usW);
		ILI9341_DrawLine ( usX_Start, usY_Start + usHeight - 1, usX_Start + usWidth - 1, usY_Start + usHeight - 1 ,usW);
		ILI9341_DrawLine ( usX_Start, usY_Start, usX_Start, usY_Start + usHeight - 1 ,usW);
		ILI9341_DrawLine ( usX_Start + usWidth - 1, usY_Start, usX_Start + usWidth - 1, usY_Start + usHeight - 1 ,usW);		
	}

}


/**
 * @brief  在 ILI9341 显示器上使用 Bresenham 算法画圆
 * @param  usX_Center ：在特定扫描方向下圆心的X坐标
 * @param  usY_Center ：在特定扫描方向下圆心的Y坐标
 * @param  usRadius：圆的半径（单位：像素）
 * @param  ucFilled ：选择是否填充该圆
  *   该参数为以下值之一：
  *     @arg 0 :空心圆
  *     @arg 1 :实心圆
 * @note 可使用LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors函数设置颜色
 * @retval 无
 */
void ILI9341_DrawCircle (
  uint16_t usX_Center, 
  uint16_t usY_Center, 
  uint16_t usRadius, 
  uint8_t ucFilled ,
  uint16_t usW
){
	int16_t sCurrentX, sCurrentY;
	int16_t sError;
	
	sCurrentX = 0; sCurrentY = usRadius;	  
	
	sError = 3 - ( usRadius << 1 );     //判断下个点位置的标志

	while ( sCurrentX <= sCurrentY ){
		int16_t sCountY;
		if ( ucFilled ) 			
			for ( sCountY = sCurrentX; sCountY <= sCurrentY; sCountY ++ ) 
			{                      
				ILI9341_SetPointPixel ( usX_Center + sCurrentX, usY_Center + sCountY ,usW );           //1，研究对象 
				ILI9341_SetPointPixel ( usX_Center - sCurrentX, usY_Center + sCountY ,usW);           //2       
				ILI9341_SetPointPixel ( usX_Center - sCountY,   usY_Center + sCurrentX ,usW);           //3
				ILI9341_SetPointPixel ( usX_Center - sCountY,   usY_Center - sCurrentX ,usW);           //4
				ILI9341_SetPointPixel ( usX_Center - sCurrentX, usY_Center - sCountY ,usW);           //5    
        ILI9341_SetPointPixel ( usX_Center + sCurrentX, usY_Center - sCountY ,usW);           //6
				ILI9341_SetPointPixel ( usX_Center + sCountY,   usY_Center - sCurrentX ,usW);           //7 	
        ILI9341_SetPointPixel ( usX_Center + sCountY,   usY_Center + sCurrentX ,usW);           //0				
			}
		
		else
		{          
			ILI9341_SetPointPixel ( usX_Center + sCurrentX, usY_Center + sCurrentY ,usW );             //1，研究对象
			ILI9341_SetPointPixel ( usX_Center - sCurrentX, usY_Center + sCurrentY ,usW );             //2      
			ILI9341_SetPointPixel ( usX_Center - sCurrentY, usY_Center + sCurrentX ,usW );             //3
			ILI9341_SetPointPixel ( usX_Center - sCurrentY, usY_Center - sCurrentX ,usW );             //4
			ILI9341_SetPointPixel ( usX_Center - sCurrentX, usY_Center - sCurrentY ,usW );             //5       
			ILI9341_SetPointPixel ( usX_Center + sCurrentX, usY_Center - sCurrentY ,usW );             //6
			ILI9341_SetPointPixel ( usX_Center + sCurrentY, usY_Center - sCurrentX ,usW );             //7 
			ILI9341_SetPointPixel ( usX_Center + sCurrentY, usY_Center + sCurrentX ,usW );             //0
    }			

		sCurrentX ++;
		if ( sError < 0 )	sError += 4 * sCurrentX + 6;	  
		
		else{
			sError += 10 + 4 * ( sCurrentX - sCurrentY );   
			sCurrentY --;
		} 	
	}
}


/**
  * @brief  在 ILI9341 上显示校正触摸时需要的十字
  * @param  usX ：在特定扫描方向下十字交叉点的X坐标
  * @param  usY ：在特定扫描方向下十字交叉点的Y坐标
  * @retval 无
  */
void ILI9341_DrawCross ( uint16_t usX, uint16_t usY ){
	ILI9341_DrawLine(usX-10,usY,usX+10,usY,2);
	ILI9341_DrawLine(usX, usY - 10, usX, usY+10,2);	
}




/**
  * @brief  在两点之间描绘轨迹
  * @param  x1: specifies the point 1 x position.
  * @param  y1: specifies the point 1 y position.
  * @param  x2: specifies the point 2 x position.
  * @param  y2: specifies the point 2 y position.
  * @retval None
  */
#define ABS(X)  ((X) > 0 ? (X) : -(X))
void LCD_DrawUniLineCircle(
  uint16_t x1, 
  uint16_t y1, 
  uint16_t x2, 
  uint16_t y2,
  uint8_t thick 
){
  int16_t deltax = 0 , deltay = 0 ;
  int16_t x = 0 , y = 0 , xinc1 = 0 , xinc2 = 0 , yinc1 = 0, yinc2 = 0 ;
  int16_t den = 0 , num = 0 ,numadd = 0 , numpixels = 0 , curpixel = 0 ;
  
  deltax = ABS(x2 - x1);
  deltay = ABS(y2 - y1);
  x = x1 ; 
  y = y1 ; 
  /* The x-values are increasing or decreasing*/
  if( x2 >= x1){
    xinc1 = 1 ;
    xinc2 = 1 ;
  }else{
    xinc1 = -1 ;
    xinc2 = -1 ;
  }
  /* The y-values are increasing or decreasing*/
  if( y2 >= y1){
    yinc1 = 1 ;
    yinc2 = 1 ;
  }else{
    yinc1 = -1 ;
    yinc2 = -1 ;
  }
  
  if(deltax > deltay){
    xinc1 = 0 ; 
    yinc2 = 0 ;
    den = deltax ; 
    num = deltax / 2;
    numadd = deltay ; 
    numpixels = deltax ; 
  }else{
    xinc2 = 0 ; 
    yinc1 = 0 ;
    den = deltay ; 
    num = deltay / 2;
    numadd = deltax ; 
    numpixels = deltay ; 
  }
  
  for(curpixel = 0 ; curpixel <= numpixels ; curpixel ++ ){
    //判断边界
    if( ((x+thick)>LCD_X_LENGTH)|| ((x-thick)<0) || //液晶左右边界
        ((y+thick)>LCD_Y_LENGTH)|| ((y-thick)<0) ){ //液晶上下边界
        continue;
    }
    ILI9341_DrawCircle(x,y,thick,1,1);
    num+= numadd ; 
    if(num >= den){
      num -= den;
      x += xinc1;
      y += yinc1;
    }
    x += xinc2;
    y += yinc2;
  }
  
}
//================================ 文字颜色状态绘制应用函数=======================================
void LCD_SetTextColor(uint16_t Color){
  CurrentForecolor = Color;
}
/**
  * @brief  设置英文字体类型
  * @param  fonts: 指定要选择的字体
	*		参数为以下值之一
  * 	@arg：Font24x32;
  * 	@arg：Font16x24;
  * 	@arg：Font8x16;
  * @retval None
  */
void LCD_SetFont(sFONT *fonts)
{
  LCD_Currentfonts = fonts;
}

/**
  * @brief  获取当前字体类型
  * @param  None.
  * @retval 返回当前字体类型
  */
sFONT *LCD_GetFont(void)
{
  return LCD_Currentfonts;
}
//================================ 文字绘制应用函数=======================================

//================================ 英文文字绘制应用函数==================================== 


/**
 * @brief  在 ILI9341 显示器上显示一个英文字符
 * @param  usX ：在特定扫描方向下字符的起始X坐标
 * @param  usY ：在特定扫描方向下该点的起始Y坐标
 * @param  cChar ：要显示的英文字符
 * @note 可使用LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors函数设置颜色
 * @retval 无
 */
void ILI9341_DispChar_EN ( uint16_t usX, uint16_t usY, const char cChar )
{
	uint8_t  byteCount, bitCount,fontLength;	
	uint16_t ucRelativePositon;
	uint8_t *Pfont;
  uint16_t back_color = ILI9341_GetPointPixel(usX,usY);
	//对ascii码表偏移（字模表不包含ASCII表的前32个非图形符号）
	ucRelativePositon = cChar - ' ';
	
	//每个字模的字节数
	fontLength = (LCD_Currentfonts->Width*LCD_Currentfonts->Height)/8;
		
	//字模首地址
	/*ascii码表偏移值乘以每个字模的字节数，求出字模的偏移位置*/
	Pfont = (uint8_t *)&LCD_Currentfonts->table[ucRelativePositon * fontLength];
 
	//设置显示窗口
	ILI9341_OpenWindow ( usX, usY, LCD_Currentfonts->Width, LCD_Currentfonts->Height);
	
	ILI9341_Write_Cmd ( CMD_SetPixel );			

	//按字节读取字模数据
	//由于前面直接设置了显示窗口，显示数据会自动换行
	for ( byteCount = 0; byteCount < fontLength; byteCount++ )
	{
			//一位一位处理要显示的颜色
			for ( bitCount = 0; bitCount < 8; bitCount++ )
			{
					if ( Pfont[byteCount] & (0x80>>bitCount) )
						ILI9341_Write_Data ( CurrentForecolor );			
				  else
						 ILI9341_Write_Data (back_color);
			}	
	}	
}


/**
 * @brief  在 ILI9341 显示器上显示英文字符串
 * @param  line ：在特定扫描方向下字符串的起始Y坐标
  *   本参数可使用宏LINE(0)、LINE(1)等方式指定文字坐标，
  *   宏LINE(x)会根据当前选择的字体来计算Y坐标值。
	*		显示中文且使用LINE宏时，需要把英文字体设置成Font8x16
 * @param  pStr ：要显示的英文字符串的首地址
 * @note 可使用LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors函数设置颜色
 * @retval 无
 */
void ILI9341_DispStringLine_EN (  uint16_t line,  char * pStr ){
	uint16_t usX = 0;
	
	while ( * pStr != '\0' )
	{
		if ( ( usX - ILI9341_DispWindow_X_Star + LCD_Currentfonts->Width ) > LCD_X_LENGTH )
		{
			usX = ILI9341_DispWindow_X_Star;
			line += LCD_Currentfonts->Height;
		}
		
		if ( ( line - ILI9341_DispWindow_Y_Star + LCD_Currentfonts->Height ) > LCD_Y_LENGTH )
		{
			usX = ILI9341_DispWindow_X_Star;
			line = ILI9341_DispWindow_Y_Star;
		}
		
		ILI9341_DispChar_EN ( usX, line, * pStr);
		
		pStr ++;
		
		usX += LCD_Currentfonts->Width;
		
	}
	
}


/**
 * @brief  在 ILI9341 显示器上显示英文字符串
 * @param  usX ：在特定扫描方向下字符的起始X坐标
 * @param  usY ：在特定扫描方向下字符的起始Y坐标
 * @param  pStr ：要显示的英文字符串的首地址
 * @note 可使用LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors函数设置颜色
 * @retval 无
 */
void ILI9341_DispString_EN ( 	uint16_t usX ,uint16_t usY,  char * pStr ){
	while ( * pStr != '\0' )
	{
		if ( ( usX - ILI9341_DispWindow_X_Star + LCD_Currentfonts->Width ) > LCD_X_LENGTH )
		{
			usX = ILI9341_DispWindow_X_Star;
			usY += LCD_Currentfonts->Height;
		}
		
		if ( ( usY - ILI9341_DispWindow_Y_Star + LCD_Currentfonts->Height ) > LCD_Y_LENGTH )
		{
			usX = ILI9341_DispWindow_X_Star;
			usY = ILI9341_DispWindow_Y_Star;
		}
		
		ILI9341_DispChar_EN ( usX, usY, * pStr);
		
		pStr ++;
		
		usX += LCD_Currentfonts->Width;
		
	}
	
}


/**
 * @brief  在 ILI9341 显示器上显示英文字符串(沿Y轴方向)
 * @param  usX ：在特定扫描方向下字符的起始X坐标
 * @param  usY ：在特定扫描方向下字符的起始Y坐标
 * @param  pStr ：要显示的英文字符串的首地址
 * @note 可使用LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors函数设置颜色
 * @retval 无
 */
void ILI9341_DispString_EN_YDir (	 uint16_t usX,uint16_t usY ,  char * pStr )
{	
	while ( * pStr != '\0' )
	{
		if ( ( usY - ILI9341_DispWindow_Y_Star + LCD_Currentfonts->Height ) >LCD_Y_LENGTH  )
		{
			usY = ILI9341_DispWindow_Y_Star;
			usX += LCD_Currentfonts->Width;
		}
		
		if ( ( usX - ILI9341_DispWindow_X_Star + LCD_Currentfonts->Width ) >  LCD_X_LENGTH)
		{
			usX = ILI9341_DispWindow_X_Star;
			usY = ILI9341_DispWindow_Y_Star;
		}
		
		ILI9341_DispChar_EN ( usX, usY, * pStr);
		
		pStr ++;
		
		usY += LCD_Currentfonts->Height;		
	}	
}
 


//================================ 中文文字绘制应用函数==================================== 

/**
 * @brief  在 ILI9341 显示器上显示一个中文字符
 * @param  usX ：在特定扫描方向下字符的起始X坐标
 * @param  usY ：在特定扫描方向下字符的起始Y坐标
 * @param  usChar ：要显示的中文字符（国标码）
 * @note 可使用LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors函数设置颜色
 * @retval 无
 */ 
void ILI9341_DispChar_CH ( uint16_t usX, uint16_t usY, uint16_t usChar )
{
	uint8_t rowCount, bitCount;
	uint8_t ucBuffer [ WIDTH_CH_CHAR*HEIGHT_CH_CHAR/8 ];	//16*16*8
  uint16_t usTemp; 	
  uint16_t back_color = ILI9341_GetPointPixel(usX,usY);
  
	//设置显示窗口 
	ILI9341_OpenWindow ( usX, usY, WIDTH_CH_CHAR, HEIGHT_CH_CHAR ); // 16* 16
	
	ILI9341_Write_Cmd ( CMD_SetPixel ); 
	
	//取字模数据  
  GetGBKCode ( ucBuffer, usChar );	//第一个参数为 返回的字符图像参数 第二个参数为 中文字符号 
 	
	for ( rowCount = 0; rowCount < HEIGHT_CH_CHAR; rowCount++ )
	{
    /* 取出两个字节的数据，在lcd上即是一个汉字的一行 */
		usTemp = ucBuffer [ rowCount * 2 ];
		usTemp = ( usTemp << 8 );
		usTemp |= ucBuffer [ rowCount * 2 + 1 ];
		
		for ( bitCount = 0; bitCount < WIDTH_CH_CHAR; bitCount ++ )
		{			
			if ( usTemp & ( 0x8000 >> bitCount ) )  //高位在前 
			  ILI9341_Write_Data ( CurrentForecolor );				
 			else
 				ILI9341_Write_Data ( back_color );			
		}		
	}
	
}


/**
 * @brief  在 ILI9341 显示器上显示中文字符串
 * @param  line ：在特定扫描方向下字符串的起始Y坐标
  *   本参数可使用宏LINE(0)、LINE(1)等方式指定文字坐标，
  *   宏LINE(x)会根据当前选择的字体来计算Y坐标值。
	*		显示中文且使用LINE宏时，需要把英文字体设置成Font8x16
 * @param  pStr ：要显示的英文字符串的首地址
 * @note 可使用LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors函数设置颜色
 * @retval 无
 */
void ILI9341_DispString_CH ( 	uint16_t usX , uint16_t usY, char * pStr )
{	
	uint16_t usCh;

	
	while( * pStr != '\0' )
	{		
		if ( ( usX - ILI9341_DispWindow_X_Star + WIDTH_CH_CHAR ) > LCD_X_LENGTH )
		{
			usX = ILI9341_DispWindow_X_Star;
			usY += HEIGHT_CH_CHAR;
		}
		
		if ( ( usY - ILI9341_DispWindow_Y_Star + HEIGHT_CH_CHAR ) > LCD_Y_LENGTH )
		{
			usX = ILI9341_DispWindow_X_Star;
			usY = ILI9341_DispWindow_Y_Star;
		}	
		
		usCh = * ( uint16_t * ) pStr;	
	  usCh = ( usCh << 8 ) + ( usCh >> 8 );

		ILI9341_DispChar_CH ( usX, usY, usCh );
		
		usX += WIDTH_CH_CHAR;
		
		pStr += 2;           //一个汉字两个字节 

	}	   
	
}

 
/**
 * @brief  在 ILI9341 显示器上显示中英文字符串
 * @param  line ：在特定扫描方向下字符串的起始Y坐标
  *   本参数可使用宏LINE(0)、LINE(1)等方式指定文字坐标，
  *   宏LINE(x)会根据当前选择的字体来计算Y坐标值。
	*		显示中文且使用LINE宏时，需要把英文字体设置成Font8x16
 * @param  pStr ：要显示的字符串的首地址
 * @note 可使用LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors函数设置颜色
 * @retval 无
 */
void ILI9341_DispStringLine_EN_CH ( uint16_t line, char * pStr )
{
	uint16_t usCh;
	uint16_t usX = 0;
	
	while( * pStr != '\0' )
	{
		if ( * pStr <= 126 )	           	//英文字符
		{
			if ( ( usX - ILI9341_DispWindow_X_Star + LCD_Currentfonts->Width ) > LCD_X_LENGTH )
			{
				usX = ILI9341_DispWindow_X_Star;
				line += LCD_Currentfonts->Height;
			}
			
			if ( ( line - ILI9341_DispWindow_Y_Star + LCD_Currentfonts->Height ) > LCD_Y_LENGTH )
			{
				usX = ILI9341_DispWindow_X_Star;
				line = ILI9341_DispWindow_Y_Star;
			}			
		
		  ILI9341_DispChar_EN ( usX, line, * pStr );
			
			usX +=  LCD_Currentfonts->Width;
		
		  pStr ++;

		}
		
		else	                            //汉字字符
		{
			if ( ( usX - ILI9341_DispWindow_X_Star + WIDTH_CH_CHAR ) > LCD_X_LENGTH )
			{
				usX = ILI9341_DispWindow_X_Star;
				line += HEIGHT_CH_CHAR;
			}
			
			if ( ( line - ILI9341_DispWindow_Y_Star + HEIGHT_CH_CHAR ) > LCD_Y_LENGTH )
			{
				usX = ILI9341_DispWindow_X_Star;
				line = ILI9341_DispWindow_Y_Star;
			}	
			
			usCh = * ( uint16_t * ) pStr;	
			
			usCh = ( usCh << 8 ) + ( usCh >> 8 );		

			ILI9341_DispChar_CH ( usX, line, usCh );
			
			usX += WIDTH_CH_CHAR;
			
			pStr += 2;           //一个汉字两个字节 
		
    }
		
  }	
} 

/**
 * @brief  在 ILI9341 显示器上显示中英文字符串
 * @param  usX ：在特定扫描方向下字符的起始X坐标
 * @param  usY ：在特定扫描方向下字符的起始Y坐标
 * @param  pStr ：要显示的字符串的首地址
 * @note 可使用LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors函数设置颜色
 * @retval 无
 */
void ILI9341_DispString_EN_CH ( uint16_t usX , uint16_t usY, char * pStr )
{
	uint16_t usCh;
	
	while( * pStr != '\0' )
	{
		if ( * pStr <= 126 )	           	//英文字符
		{
			if ( ( usX - ILI9341_DispWindow_X_Star + LCD_Currentfonts->Width ) > LCD_X_LENGTH )
			{
				usX = ILI9341_DispWindow_X_Star;
				usY += LCD_Currentfonts->Height;
			}
			
			if ( ( usY - ILI9341_DispWindow_Y_Star + LCD_Currentfonts->Height ) > LCD_Y_LENGTH )
			{
				usX = ILI9341_DispWindow_X_Star;
				usY = ILI9341_DispWindow_Y_Star;
			}			
		
		  ILI9341_DispChar_EN ( usX, usY, * pStr );
			
			usX +=  LCD_Currentfonts->Width;
		
		  pStr ++;

		}
		
		else	                            //汉字字符
		{
			if ( ( usX - ILI9341_DispWindow_X_Star + WIDTH_CH_CHAR ) > LCD_X_LENGTH )
			{
				usX = ILI9341_DispWindow_X_Star;
				usY += HEIGHT_CH_CHAR;
			}
			
			if ( ( usY - ILI9341_DispWindow_Y_Star + HEIGHT_CH_CHAR ) > LCD_Y_LENGTH )
			{
				usX = ILI9341_DispWindow_X_Star;
				usY = ILI9341_DispWindow_Y_Star;
			}	
			
			usCh = * ( uint16_t * ) pStr;	
			
			usCh = ( usCh << 8 ) + ( usCh >> 8 );		

			ILI9341_DispChar_CH ( usX, usY, usCh );
			
			usX += WIDTH_CH_CHAR;
			
			pStr += 2;           //一个汉字两个字节 
		
    }
		
  }	
} 


/**
 * @brief  在 ILI9341 显示器上显示中英文字符串(沿Y轴方向)
 * @param  usX ：在特定扫描方向下字符的起始X坐标
 * @param  usY ：在特定扫描方向下字符的起始Y坐标
 * @param  pStr ：要显示的中英文字符串的首地址
 * @note 可使用LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors函数设置颜色
 * @retval 无
 */
void ILI9341_DispString_EN_CH_YDir (  uint16_t usX,uint16_t usY , char * pStr )
{
	uint16_t usCh;
	
	while( * pStr != '\0' )
	{			
			//统一使用汉字的宽高来计算换行
			if ( ( usY - ILI9341_DispWindow_Y_Star + HEIGHT_CH_CHAR ) >LCD_Y_LENGTH  )
			{
				usY = ILI9341_DispWindow_Y_Star;
				usX += WIDTH_CH_CHAR;
			}			
			if ( ( usX - ILI9341_DispWindow_X_Star + WIDTH_CH_CHAR ) >  LCD_X_LENGTH)
			{
				usX = ILI9341_DispWindow_X_Star;
				usY = ILI9341_DispWindow_Y_Star;
			}
			
		//显示	
		if ( * pStr <= 126 )	           	//英文字符
		{			
			ILI9341_DispChar_EN ( usX, usY, * pStr);
			
			pStr ++;
			
			usY += HEIGHT_CH_CHAR;		
		}
		else	                            //汉字字符
		{			
			usCh = * ( uint16_t * ) pStr;	
			
			usCh = ( usCh << 8 ) + ( usCh >> 8 );		

			ILI9341_DispChar_CH ( usX,usY , usCh );
			
			usY += HEIGHT_CH_CHAR;
			
			pStr += 2;           //一个汉字两个字节 
		
    }
		
  }	
} 

/***********************缩放字体****************************/
#define ZOOMMAXBUFF 16384
uint8_t zoomBuff[ZOOMMAXBUFF] = {0};	//用于缩放的缓存，最大支持到128*128
uint8_t zoomTempBuff[1024] = {0};

/**
 * @brief  缩放字模，缩放后的字模由1个像素点由8个数据位来表示
										0x01表示笔迹，0x00表示空白区
 * @param  in_width ：原始字符宽度
 * @param  in_heig ：原始字符高度
 * @param  out_width ：缩放后的字符宽度
 * @param  out_heig：缩放后的字符高度
 * @param  in_ptr ：字库输入指针	注意：1pixel 1bit
 * @param  out_ptr ：缩放后的字符输出指针 注意: 1pixel 8bit
 *		out_ptr实际上没有正常输出，改成了直接输出到全局指针zoomBuff中
 * @param  en_cn ：0为英文，1为中文
 * @retval 无
 */
void ILI9341_zoomChar(
    uint16_t in_width,	//原始字符宽度
    uint16_t in_heig,		//原始字符高度
    uint16_t out_width,	//缩放后的字符宽度
    uint16_t out_heig,	//缩放后的字符高度
    uint8_t *in_ptr,	//字库输入指针	注意：1pixel 1bit
    uint8_t *out_ptr, //缩放后的字符输出指针 注意: 1pixel 8bit
    uint8_t en_cn)		//0为英文，1为中文	
{
	uint8_t *pts,*ots;
	//根据源字模及目标字模大小，设定运算比例因子，左移16是为了把浮点运算转成定点运算
	unsigned int xrIntFloat_16=(in_width<<16)/out_width+1; 
  unsigned int yrIntFloat_16=(in_heig<<16)/out_heig+1;
	
	unsigned int srcy_16=0;
	unsigned int y,x;
	uint8_t *pSrcLine;
	
	uint16_t byteCount,bitCount;
	
	//检查参数是否合法
	if(in_width >= 32) return;												//字库不允许超过32像素
	if(in_width * in_heig == 0) return;	
	if(in_width * in_heig >= 1024 ) return; 					//限制输入最大 32*32
	
	if(out_width * out_heig == 0) return;	
	if(out_width * out_heig >= ZOOMMAXBUFF ) return; //限制最大缩放 128*128
	pts = (uint8_t*)&zoomTempBuff;
	
	//为方便运算，字库的数据由1 pixel/1bit 映射到1pixel/8bit
	//0x01表示笔迹，0x00表示空白区
	if(en_cn == 0x00)//英文
	{
		//英文和中文字库上下边界不对，可在此处调整。需要注意tempBuff防止溢出
			for(byteCount=0;byteCount<in_heig*in_width/8;byteCount++)	
			{
				for(bitCount=0;bitCount<8;bitCount++)
					{						
						//把源字模数据由位映射到字节
						//in_ptr里bitX为1，则pts里整个字节值为1
						//in_ptr里bitX为0，则pts里整个字节值为0
						*pts++ = (in_ptr[byteCount] & (0x80>>bitCount))?1:0; 
					}
			}				
	}
	else //中文
	{			
			for(byteCount=0;byteCount<in_heig*in_width/8;byteCount++)	
			{
				for(bitCount=0;bitCount<8;bitCount++)
					{						
						//把源字模数据由位映射到字节
						//in_ptr里bitX为1，则pts里整个字节值为1
						//in_ptr里bitX为0，则pts里整个字节值为0
						*pts++ = (in_ptr[byteCount] & (0x80>>bitCount))?1:0; 
					}
			}		
	}

	//zoom过程
	pts = (uint8_t*)&zoomTempBuff;	//映射后的源数据指针
	ots = (uint8_t*)&zoomBuff;	//输出数据的指针
	for (y=0;y<out_heig;y++)	/*行遍历*/
    {
				unsigned int srcx_16=0;
        pSrcLine=pts+in_width*(srcy_16>>16);				
        for (x=0;x<out_width;x++) /*行内像素遍历*/
        {
            ots[x]=pSrcLine[srcx_16>>16]; //把源字模数据复制到目标指针中
            srcx_16+=xrIntFloat_16;			//按比例偏移源像素点
        }
        srcy_16+=yrIntFloat_16;				  //按比例偏移源像素点
        ots+=out_width;						
    }
	/*！！！缩放后的字模数据直接存储到全局指针zoomBuff里了*/
	out_ptr = (uint8_t*)&zoomBuff;	//out_ptr没有正确传出，后面调用直接改成了全局变量指针！
	
	/*实际中如果使用out_ptr不需要下面这一句！！！
		只是因为out_ptr没有使用，会导致warning。强迫症*/
	out_ptr++; 
}			


/**
 * @brief  利用缩放后的字模显示字符
 * @param  Xpos ：字符显示位置x
 * @param  Ypos ：字符显示位置y
 * @param  Font_width ：字符宽度
 * @param  Font_Heig：字符高度
 * @param  c ：要显示的字模数据
 * @param  DrawModel ：是否反色显示 
 * @retval 无
 */
void ILI9341_DrawChar_Ex(
    uint16_t usX, //字符显示位置x
    uint16_t usY, //字符显示位置y
    uint16_t Font_width, //字符宽度
    uint16_t Font_Height,  //字符高度 
    uint8_t *c,						//字模数据
    uint16_t DrawModel)		//是否反色显示
{
  uint32_t index = 0, counter = 0;
  uint16_t back_color = ILI9341_GetPointPixel(usX,usY);
	//设置显示窗口
	ILI9341_OpenWindow ( usX, usY, Font_width, Font_Height);
	
	ILI9341_Write_Cmd ( CMD_SetPixel );		
	
	//按字节读取字模数据
	//由于前面直接设置了显示窗口，显示数据会自动换行
	for ( index = 0; index < Font_Height; index++ )
	{
			//一位一位处理要显示的颜色
			for ( counter = 0; counter < Font_width; counter++ )
			{
					//缩放后的字模数据，以一个字节表示一个像素位
					//整个字节值为1表示该像素为笔迹
					//整个字节值为0表示该像素为背景
					if ( *c++ == DrawModel ){
						ILI9341_Write_Data ( back_color );	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!		
					}else
						ILI9341_Write_Data ( CurrentForecolor );
			}	
	}	
}


/**
 * @brief  利用缩放后的字模显示字符串
 * @param  Xpos ：字符显示位置x
 * @param  Ypos ：字符显示位置y
 * @param  Font_width ：字符宽度，英文字符在此基础上/2。注意为偶数
 * @param  Font_Heig：字符高度，注意为偶数
 * @param  c ：要显示的字符串
 * @param  DrawModel ：是否反色显示 
 * @retval 无
 */
void ILI9341_DisplayStringEx(
    uint16_t x, 		//字符显示位置x
    uint16_t y, 				//字符显示位置y
    uint16_t Font_width,	//要显示的字体宽度，英文字符在此基础上/2。注意为偶数
    uint16_t Font_Height,	//要显示的字体高度，注意为偶数
    uint8_t *ptr,					//显示的字符内容
    uint16_t DrawModel)  //是否反色显示
{
	uint16_t Charwidth = Font_width; //默认为Font_width，英文宽度为中文宽度的一半
	uint8_t *psr;
	uint8_t Ascii;	//英文
	uint16_t usCh;  //中文
	uint8_t ucBuffer [ WIDTH_CH_CHAR*HEIGHT_CH_CHAR/8 ];	
	
	while ( *ptr != '\0')
	{
			/****处理换行*****/
			if ( ( x - ILI9341_DispWindow_X_Star + Charwidth ) > LCD_X_LENGTH )
			{
				x = ILI9341_DispWindow_X_Star;
				y += Font_Height;
			}
			
			if ( ( y - ILI9341_DispWindow_Y_Star + Font_Height ) > LCD_Y_LENGTH )
			{
				x = ILI9341_DispWindow_X_Star;
				y = ILI9341_DispWindow_Y_Star;
			}	
			
		if(*ptr > 0x80) //如果是中文
		{			
			Charwidth = Font_width;
			usCh = * ( uint16_t * ) ptr;				
			usCh = ( usCh << 8 ) + ( usCh >> 8 );
			GetGBKCode ( ucBuffer, usCh );	//取字模数据
			//缩放字模数据，源字模为16*16
			ILI9341_zoomChar(WIDTH_CH_CHAR,HEIGHT_CH_CHAR,Charwidth,Font_Height,(uint8_t *)&ucBuffer,psr,1); 
			//显示单个字符
			ILI9341_DrawChar_Ex(x,y,Charwidth,Font_Height,(uint8_t*)&zoomBuff,DrawModel);
			x+=Charwidth;
			ptr+=2;
		}
		else
		{
				Charwidth = Font_width / 2;
				Ascii = *ptr - 32;
				//使用16*24字体缩放字模数据
				ILI9341_zoomChar(16,24,Charwidth,Font_Height,(uint8_t *)&Font16x24.table[Ascii * Font16x24.Height*Font16x24.Width/8],psr,0);
			  //显示单个字符
				ILI9341_DrawChar_Ex(x,y,Charwidth,Font_Height,(uint8_t*)&zoomBuff,DrawModel);
				x+=Charwidth;
				ptr++;
		}
	}
}


/**
 * @brief  利用缩放后的字模显示字符串(沿Y轴方向)
 * @param  Xpos ：字符显示位置x
 * @param  Ypos ：字符显示位置y
 * @param  Font_width ：字符宽度，英文字符在此基础上/2。注意为偶数
 * @param  Font_Heig：字符高度，注意为偶数
 * @param  c ：要显示的字符串
 * @param  DrawModel ：是否反色显示 
 * @retval 无
 */
void ILI9341_DisplayStringEx_YDir(
    uint16_t x, 		//字符显示位置x
    uint16_t y, 				//字符显示位置y
    uint16_t Font_width,	//要显示的字体宽度，英文字符在此基础上/2。注意为偶数
    uint16_t Font_Height,	//要显示的字体高度，注意为偶数
    uint8_t *ptr,					//显示的字符内容
    uint16_t DrawModel)  //是否反色显示
{
	uint16_t Charwidth = Font_width; //默认为Font_width，英文宽度为中文宽度的一半
	uint8_t *psr;
	uint8_t Ascii;	//英文
	uint16_t usCh;  //中文
	uint8_t ucBuffer [ WIDTH_CH_CHAR*HEIGHT_CH_CHAR/8 ];	
	
	while ( *ptr != '\0')
	{			
			//统一使用汉字的宽高来计算换行
			if ( ( y - ILI9341_DispWindow_X_Star + Font_width ) > LCD_X_LENGTH )
			{
				y = ILI9341_DispWindow_X_Star;
				x += Font_width;
			}
			
			if ( ( x - ILI9341_DispWindow_Y_Star + Font_Height ) > LCD_Y_LENGTH )
			{
				y = ILI9341_DispWindow_X_Star;
				x = ILI9341_DispWindow_Y_Star;
			}	
			
		if(*ptr > 0x80) //如果是中文
		{			
			Charwidth = Font_width;
			usCh = * ( uint16_t * ) ptr;				
			usCh = ( usCh << 8 ) + ( usCh >> 8 );
			GetGBKCode ( ucBuffer, usCh );	//取字模数据
			//缩放字模数据，源字模为16*16
			ILI9341_zoomChar(WIDTH_CH_CHAR,HEIGHT_CH_CHAR,Charwidth,Font_Height,(uint8_t *)&ucBuffer,psr,1); 
			//显示单个字符
			ILI9341_DrawChar_Ex(x,y,Charwidth,Font_Height,(uint8_t*)&zoomBuff,DrawModel);
			y+=Font_Height;
			ptr+=2;
		}
		else
		{
				Charwidth = Font_width / 2;
				Ascii = *ptr - 32;
				//使用16*24字体缩放字模数据
				ILI9341_zoomChar(16,24,Charwidth,Font_Height,(uint8_t *)&Font16x24.table[Ascii * Font16x24.Height*Font16x24.Width/8],psr,0);
			  //显示单个字符
				ILI9341_DrawChar_Ex(x,y,Charwidth,Font_Height,(uint8_t*)&zoomBuff,DrawModel);
				y+=Font_Height;
				ptr++;
		}
	}
}

void Lcd_display_String(uint8_t *ptr){
  LCD_SetColors(GB888(0x000000), GB888(0xFFFFFF));
  ILI9341_Clear(0,(LCD_Y_LENGTH-Font24x32.Height),LCD_X_LENGTH,Font24x32.Height);	/* 清屏，显示 粉色的界面 */
  
  LCD_SetFont(&Font24x32);  //制作对话框
  LCD_SetForeColor( GB888(0xBB3756)); //红色文字
  ILI9341_DisplayStringEx(0,LCD_Y_LENGTH-Font24x32.Height,Font24x32.Width  , Font24x32.Height , ptr,0);
  fn_Systick_Delay(1000,_Systick_ms);
}







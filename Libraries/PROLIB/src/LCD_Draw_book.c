#include "LCD_Draw_book.h" 
#include "Systick_book.h"

sFONT *LCD_Currentfonts = &Font8x16;  //Ӣ������
uint16_t CurrentForecolor   = BLACK;//ǰ��ɫ
uint16_t CurrentBackColor   = BACKGROUND;//����ɫ

//================================ �ӿں���=======================================

/**
 * @brief  ��ILI9341��ʾ���Ͽ���һ������
 * @param  usX �����ض�ɨ�跽���´��ڵ����X����
 * @param  usY �����ض�ɨ�跽���´��ڵ����Y����
 * @param  usWidth �����ڵĿ��
 * @param  usHeight �����ڵĸ߶�
 * @retval ��
 */
void ILI9341_OpenWindow ( uint16_t usX, uint16_t usY, uint16_t usWidth, uint16_t usHeight ){
	ILI9341_Write_Cmd ( CMD_SetCoordinateX ); 				 /* ����X���� */
	ILI9341_Write_Data ( (usX & 0xFF00) >> 8  );	 /* �ȸ�8λ��Ȼ���8λ */
	ILI9341_Write_Data ( usX & 0xff  );	 /* ������ʼ��ͽ�����*/
	ILI9341_Write_Data ( ( usX + usWidth - 1 ) >> 8  );
	ILI9341_Write_Data ( ( usX + usWidth - 1 ) & 0xff  );

	ILI9341_Write_Cmd ( CMD_SetCoordinateY ); 			     /* ����Y����*/
	ILI9341_Write_Data ( (usY & 0xFF00 )>> 8  );
	ILI9341_Write_Data ( usY & 0xff  );
	ILI9341_Write_Data ( ( usY + usHeight - 1 ) >> 8 );
	ILI9341_Write_Data ( ( usY + usHeight - 1) & 0xff );
	
}

/**
 * @brief  �趨ILI9341�Ĺ������
 * @param  usX �����ض�ɨ�跽���¹���X����
 * @param  usY �����ض�ɨ�跽���¹���Y����
 * @retval ��
 */
static void ILI9341_SetCursor ( 
  uint16_t usX, 
  uint16_t usY,
  uint16_t usW
){
	ILI9341_OpenWindow ( usX, usY, usW, usW );
}



/**
 * @brief  ��ȡILI9341 GRAN ��һ����������
 * @param  ��
 * @retval ��������
 */
static uint16_t ILI9341_Read_PixelData ( void )	
{	
	uint16_t usR=0, usG=0, usB=0 ;

	
	ILI9341_Write_Cmd ( 0x2E );   /* ������ */
	
	usR = ILI9341_Read_Data (); 	/*FIRST READ OUT DUMMY DATA*/
	
	usR = ILI9341_Read_Data ();  	/*READ OUT RED DATA  */
	usB = ILI9341_Read_Data ();  	/*READ OUT BLUE DATA*/
	usG = ILI9341_Read_Data ();  	/*READ OUT GREEN DATA*/	
	
  return ( ( ( usR >> 11 ) << 11 ) | ( ( usG >> 10 ) << 5 ) | ( usB >> 11 ) );
	
}


/**
 * @brief  ��ȡ ILI9341 ��ʾ����ĳһ����������������
 * @param  usX �����ض�ɨ�跽���¸õ��X����
 * @param  usY �����ض�ɨ�跽���¸õ��Y����
 * @retval ��������
 */
uint16_t ILI9341_GetPointPixel ( uint16_t usX, uint16_t usY )
{ 
	uint16_t usPixelData;

	
	ILI9341_SetCursor ( usX, usY ,1);
	
	usPixelData = ILI9341_Read_PixelData ();
	
	return usPixelData;
	
}



/**
 * @brief  ��ILI9341��ʾ������ĳһ��ɫ������ص�
 * @param  ulAmout_Point ��Ҫ�����ɫ�����ص������Ŀ
 * @param  usColor ����ɫ
 * @retval ��
 */
static __inline void ILI9341_FillColor ( uint32_t ulAmout_Point, uint16_t usColor ){
	uint32_t i = 0;
	/* memory write */
	ILI9341_Write_Cmd ( CMD_SetPixel );	
	for ( i = 0; i < ulAmout_Point; i ++ )
		ILI9341_Write_Data ( usColor );
}

/**
 * @brief  ��ILI9341��ʾ����ĳһ������ĳ����ɫ��������
 * @param  usX �����ض�ɨ�跽���´��ڵ����X����
 * @param  usY �����ض�ɨ�跽���´��ڵ����Y����
 * @param  usWidth �����ڵĿ��
 * @param  usHeight �����ڵĸ߶�
 * @note ��ʹ��LCD_SetBackColor��LCD_SetTextColor��LCD_SetColors����������ɫ
 * @retval ��
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
  * @brief  ���ĳ������
  * @param  Line: ָ��Ҫɾ������
  *   ��������ʹ�ú�LINE(0)��LINE(1)�ȷ�ʽָ��Ҫɾ�����У�
  *   ��LINE(x)����ݵ�ǰѡ�������������Y����ֵ����ɾ����ǰ����߶ȵĵ�x�С�
  * @retval None
  */
void LCD_ClearLine(uint16_t Line){
  ILI9341_Clear(0,Line,LCD_X_LENGTH,((sFONT *)LCD_GetFont())->Height);	/* ��������ʾȫ�� */

}
//================================ ��ɫ״̬����=======================================
/**
  * @brief  ����LCD��ǰ��(����)��������ɫ,RGB565
  * @param  TextColor: ָ��ǰ��(����)��ɫ
  * @param  BackColor: ָ��������ɫ
  * @retval None
  */
void LCD_SetColors(uint16_t TextColor, uint16_t BackColor){
  CurrentForecolor = TextColor; 
  CurrentBackColor = BackColor;
}
/**
  * @brief  ����LCD��ǰ��(����)��ɫ,RGB565
  * @param  Color: ָ��ǰ��(����)��ɫ 
  * @retval None
  */
void LCD_SetForeColor(uint16_t Color){
  CurrentForecolor = Color;
}

/**
  * @brief  ����LCD�ı�����ɫ,RGB565
  * @param  Color: ָ��������ɫ 
  * @retval None
  */
void LCD_SetBackColor(uint16_t Color){
  CurrentBackColor = Color;
}

 
//================================ ��״����Ӧ�ú���=======================================


/**
 * @brief  ��ILI9341��ʾ����ĳһ����ĳ����ɫ�������
 * @param  usX �����ض�ɨ�跽���¸õ��X����
 * @param  usY �����ض�ɨ�跽���¸õ��Y����
 * @param  usW �����ض�ɨ����
 * @note ��ʹ��LCD_SetBackColor��LCD_SetTextColor��LCD_SetColors����������ɫ
 * @retval ��
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
 * @brief  �� ILI9341 ��ʾ����ʹ�� Bresenham �㷨���߶� 
 * @param  usX1 �����ض�ɨ�跽�����߶ε�һ���˵�X����
 * @param  usY1 �����ض�ɨ�跽�����߶ε�һ���˵�Y����
 * @param  usX2 �����ض�ɨ�跽�����߶ε���һ���˵�X����
 * @param  usY2 �����ض�ɨ�跽�����߶ε���һ���˵�Y����
 * @note ��ʹ��LCD_SetBackColor��LCD_SetTextColor��LCD_SetColors����������ɫ
 * @retval ��
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
	
	lDelta_X = usX2 - usX1; //������������ 
	lDelta_Y = usY2 - usY1; 
	
	usX_Current = usX1; 
	usY_Current = usY1; 
	
	if (lDelta_X > 0) lIncrease_X = 1; //���õ������� 
	else if (lDelta_X == 0) lIncrease_X = 0;//��ֱ�� 
	else { 
    lIncrease_X = -1;
    lDelta_X = - lDelta_X;
  } 

	if (lDelta_Y > 0) lIncrease_Y = 1; 
	else if (lDelta_Y == 0)	lIncrease_Y = 0;//ˮƽ�� 
	else {
    lIncrease_Y = -1;
    lDelta_Y = - lDelta_Y;
  } 
	
	if (lDelta_X > lDelta_Y )
		lDistance = lDelta_X; //ѡȡ�������������� 
	else 
		lDistance = lDelta_Y; 

	//-------------------���л��ߴ���--------------------
	for ( us = 0; us <= lDistance + 1; us ++ ){//������� 
    ILI9341_SetPointPixel ( usX_Current, usY_Current , usW );//���� 
    
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
 * @brief  �� ILI9341 ��ʾ���ϻ�һ������
 * @param  usX_Start �����ض�ɨ�跽���¾��ε���ʼ��X����
 * @param  usY_Start �����ض�ɨ�跽���¾��ε���ʼ��Y����
 * @param  usWidth�����εĿ�ȣ���λ�����أ�
 * @param  usHeight�����εĸ߶ȣ���λ�����أ�
 * @param  ucFilled ��ѡ���Ƿ����þ���
  *   �ò���Ϊ����ֵ֮һ��
  *     @arg 0 :���ľ���
  *     @arg 1 :ʵ�ľ��� 
 * @note ��ʹ��LCD_SetBackColor��LCD_SetTextColor��LCD_SetColors����������ɫ
 * @retval ��
 */
void ILI9341_DrawRectangle ( 
  uint16_t usX_Start,     //X����
  uint16_t usY_Start,     //Y���� 
  uint16_t usWidth,       //X���
  uint16_t usHeight,      //Y���
  uint8_t ucFilled ,      //�������
  uint16_t usW            //����߿��
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
 * @brief  �� ILI9341 ��ʾ����ʹ�� Bresenham �㷨��Բ
 * @param  usX_Center �����ض�ɨ�跽����Բ�ĵ�X����
 * @param  usY_Center �����ض�ɨ�跽����Բ�ĵ�Y����
 * @param  usRadius��Բ�İ뾶����λ�����أ�
 * @param  ucFilled ��ѡ���Ƿ�����Բ
  *   �ò���Ϊ����ֵ֮һ��
  *     @arg 0 :����Բ
  *     @arg 1 :ʵ��Բ
 * @note ��ʹ��LCD_SetBackColor��LCD_SetTextColor��LCD_SetColors����������ɫ
 * @retval ��
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
	
	sError = 3 - ( usRadius << 1 );     //�ж��¸���λ�õı�־

	while ( sCurrentX <= sCurrentY ){
		int16_t sCountY;
		if ( ucFilled ) 			
			for ( sCountY = sCurrentX; sCountY <= sCurrentY; sCountY ++ ) 
			{                      
				ILI9341_SetPointPixel ( usX_Center + sCurrentX, usY_Center + sCountY ,usW );           //1���о����� 
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
			ILI9341_SetPointPixel ( usX_Center + sCurrentX, usY_Center + sCurrentY ,usW );             //1���о�����
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
  * @brief  �� ILI9341 ����ʾУ������ʱ��Ҫ��ʮ��
  * @param  usX �����ض�ɨ�跽����ʮ�ֽ�����X����
  * @param  usY �����ض�ɨ�跽����ʮ�ֽ�����Y����
  * @retval ��
  */
void ILI9341_DrawCross ( uint16_t usX, uint16_t usY ){
	ILI9341_DrawLine(usX-10,usY,usX+10,usY,2);
	ILI9341_DrawLine(usX, usY - 10, usX, usY+10,2);	
}




/**
  * @brief  ������֮�����켣
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
    //�жϱ߽�
    if( ((x+thick)>LCD_X_LENGTH)|| ((x-thick)<0) || //Һ�����ұ߽�
        ((y+thick)>LCD_Y_LENGTH)|| ((y-thick)<0) ){ //Һ�����±߽�
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
//================================ ������ɫ״̬����Ӧ�ú���=======================================
void LCD_SetTextColor(uint16_t Color){
  CurrentForecolor = Color;
}
/**
  * @brief  ����Ӣ����������
  * @param  fonts: ָ��Ҫѡ�������
	*		����Ϊ����ֵ֮һ
  * 	@arg��Font24x32;
  * 	@arg��Font16x24;
  * 	@arg��Font8x16;
  * @retval None
  */
void LCD_SetFont(sFONT *fonts)
{
  LCD_Currentfonts = fonts;
}

/**
  * @brief  ��ȡ��ǰ��������
  * @param  None.
  * @retval ���ص�ǰ��������
  */
sFONT *LCD_GetFont(void)
{
  return LCD_Currentfonts;
}
//================================ ���ֻ���Ӧ�ú���=======================================

//================================ Ӣ�����ֻ���Ӧ�ú���==================================== 


/**
 * @brief  �� ILI9341 ��ʾ������ʾһ��Ӣ���ַ�
 * @param  usX �����ض�ɨ�跽�����ַ�����ʼX����
 * @param  usY �����ض�ɨ�跽���¸õ����ʼY����
 * @param  cChar ��Ҫ��ʾ��Ӣ���ַ�
 * @note ��ʹ��LCD_SetBackColor��LCD_SetTextColor��LCD_SetColors����������ɫ
 * @retval ��
 */
void ILI9341_DispChar_EN ( uint16_t usX, uint16_t usY, const char cChar )
{
	uint8_t  byteCount, bitCount,fontLength;	
	uint16_t ucRelativePositon;
	uint8_t *Pfont;
  uint16_t back_color = ILI9341_GetPointPixel(usX,usY);
	//��ascii���ƫ�ƣ���ģ������ASCII���ǰ32����ͼ�η��ţ�
	ucRelativePositon = cChar - ' ';
	
	//ÿ����ģ���ֽ���
	fontLength = (LCD_Currentfonts->Width*LCD_Currentfonts->Height)/8;
		
	//��ģ�׵�ַ
	/*ascii���ƫ��ֵ����ÿ����ģ���ֽ����������ģ��ƫ��λ��*/
	Pfont = (uint8_t *)&LCD_Currentfonts->table[ucRelativePositon * fontLength];
 
	//������ʾ����
	ILI9341_OpenWindow ( usX, usY, LCD_Currentfonts->Width, LCD_Currentfonts->Height);
	
	ILI9341_Write_Cmd ( CMD_SetPixel );			

	//���ֽڶ�ȡ��ģ����
	//����ǰ��ֱ����������ʾ���ڣ���ʾ���ݻ��Զ�����
	for ( byteCount = 0; byteCount < fontLength; byteCount++ )
	{
			//һλһλ����Ҫ��ʾ����ɫ
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
 * @brief  �� ILI9341 ��ʾ������ʾӢ���ַ���
 * @param  line �����ض�ɨ�跽�����ַ�������ʼY����
  *   ��������ʹ�ú�LINE(0)��LINE(1)�ȷ�ʽָ���������꣬
  *   ��LINE(x)����ݵ�ǰѡ�������������Y����ֵ��
	*		��ʾ������ʹ��LINE��ʱ����Ҫ��Ӣ���������ó�Font8x16
 * @param  pStr ��Ҫ��ʾ��Ӣ���ַ������׵�ַ
 * @note ��ʹ��LCD_SetBackColor��LCD_SetTextColor��LCD_SetColors����������ɫ
 * @retval ��
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
 * @brief  �� ILI9341 ��ʾ������ʾӢ���ַ���
 * @param  usX �����ض�ɨ�跽�����ַ�����ʼX����
 * @param  usY �����ض�ɨ�跽�����ַ�����ʼY����
 * @param  pStr ��Ҫ��ʾ��Ӣ���ַ������׵�ַ
 * @note ��ʹ��LCD_SetBackColor��LCD_SetTextColor��LCD_SetColors����������ɫ
 * @retval ��
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
 * @brief  �� ILI9341 ��ʾ������ʾӢ���ַ���(��Y�᷽��)
 * @param  usX �����ض�ɨ�跽�����ַ�����ʼX����
 * @param  usY �����ض�ɨ�跽�����ַ�����ʼY����
 * @param  pStr ��Ҫ��ʾ��Ӣ���ַ������׵�ַ
 * @note ��ʹ��LCD_SetBackColor��LCD_SetTextColor��LCD_SetColors����������ɫ
 * @retval ��
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
 


//================================ �������ֻ���Ӧ�ú���==================================== 

/**
 * @brief  �� ILI9341 ��ʾ������ʾһ�������ַ�
 * @param  usX �����ض�ɨ�跽�����ַ�����ʼX����
 * @param  usY �����ض�ɨ�跽�����ַ�����ʼY����
 * @param  usChar ��Ҫ��ʾ�������ַ��������룩
 * @note ��ʹ��LCD_SetBackColor��LCD_SetTextColor��LCD_SetColors����������ɫ
 * @retval ��
 */ 
void ILI9341_DispChar_CH ( uint16_t usX, uint16_t usY, uint16_t usChar )
{
	uint8_t rowCount, bitCount;
	uint8_t ucBuffer [ WIDTH_CH_CHAR*HEIGHT_CH_CHAR/8 ];	//16*16*8
  uint16_t usTemp; 	
  uint16_t back_color = ILI9341_GetPointPixel(usX,usY);
  
	//������ʾ���� 
	ILI9341_OpenWindow ( usX, usY, WIDTH_CH_CHAR, HEIGHT_CH_CHAR ); // 16* 16
	
	ILI9341_Write_Cmd ( CMD_SetPixel ); 
	
	//ȡ��ģ����  
  GetGBKCode ( ucBuffer, usChar );	//��һ������Ϊ ���ص��ַ�ͼ����� �ڶ�������Ϊ �����ַ��� 
 	
	for ( rowCount = 0; rowCount < HEIGHT_CH_CHAR; rowCount++ )
	{
    /* ȡ�������ֽڵ����ݣ���lcd�ϼ���һ�����ֵ�һ�� */
		usTemp = ucBuffer [ rowCount * 2 ];
		usTemp = ( usTemp << 8 );
		usTemp |= ucBuffer [ rowCount * 2 + 1 ];
		
		for ( bitCount = 0; bitCount < WIDTH_CH_CHAR; bitCount ++ )
		{			
			if ( usTemp & ( 0x8000 >> bitCount ) )  //��λ��ǰ 
			  ILI9341_Write_Data ( CurrentForecolor );				
 			else
 				ILI9341_Write_Data ( back_color );			
		}		
	}
	
}


/**
 * @brief  �� ILI9341 ��ʾ������ʾ�����ַ���
 * @param  line �����ض�ɨ�跽�����ַ�������ʼY����
  *   ��������ʹ�ú�LINE(0)��LINE(1)�ȷ�ʽָ���������꣬
  *   ��LINE(x)����ݵ�ǰѡ�������������Y����ֵ��
	*		��ʾ������ʹ��LINE��ʱ����Ҫ��Ӣ���������ó�Font8x16
 * @param  pStr ��Ҫ��ʾ��Ӣ���ַ������׵�ַ
 * @note ��ʹ��LCD_SetBackColor��LCD_SetTextColor��LCD_SetColors����������ɫ
 * @retval ��
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
		
		pStr += 2;           //һ�����������ֽ� 

	}	   
	
}

 
/**
 * @brief  �� ILI9341 ��ʾ������ʾ��Ӣ���ַ���
 * @param  line �����ض�ɨ�跽�����ַ�������ʼY����
  *   ��������ʹ�ú�LINE(0)��LINE(1)�ȷ�ʽָ���������꣬
  *   ��LINE(x)����ݵ�ǰѡ�������������Y����ֵ��
	*		��ʾ������ʹ��LINE��ʱ����Ҫ��Ӣ���������ó�Font8x16
 * @param  pStr ��Ҫ��ʾ���ַ������׵�ַ
 * @note ��ʹ��LCD_SetBackColor��LCD_SetTextColor��LCD_SetColors����������ɫ
 * @retval ��
 */
void ILI9341_DispStringLine_EN_CH ( uint16_t line, char * pStr )
{
	uint16_t usCh;
	uint16_t usX = 0;
	
	while( * pStr != '\0' )
	{
		if ( * pStr <= 126 )	           	//Ӣ���ַ�
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
		
		else	                            //�����ַ�
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
			
			pStr += 2;           //һ�����������ֽ� 
		
    }
		
  }	
} 

/**
 * @brief  �� ILI9341 ��ʾ������ʾ��Ӣ���ַ���
 * @param  usX �����ض�ɨ�跽�����ַ�����ʼX����
 * @param  usY �����ض�ɨ�跽�����ַ�����ʼY����
 * @param  pStr ��Ҫ��ʾ���ַ������׵�ַ
 * @note ��ʹ��LCD_SetBackColor��LCD_SetTextColor��LCD_SetColors����������ɫ
 * @retval ��
 */
void ILI9341_DispString_EN_CH ( uint16_t usX , uint16_t usY, char * pStr )
{
	uint16_t usCh;
	
	while( * pStr != '\0' )
	{
		if ( * pStr <= 126 )	           	//Ӣ���ַ�
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
		
		else	                            //�����ַ�
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
			
			pStr += 2;           //һ�����������ֽ� 
		
    }
		
  }	
} 


/**
 * @brief  �� ILI9341 ��ʾ������ʾ��Ӣ���ַ���(��Y�᷽��)
 * @param  usX �����ض�ɨ�跽�����ַ�����ʼX����
 * @param  usY �����ض�ɨ�跽�����ַ�����ʼY����
 * @param  pStr ��Ҫ��ʾ����Ӣ���ַ������׵�ַ
 * @note ��ʹ��LCD_SetBackColor��LCD_SetTextColor��LCD_SetColors����������ɫ
 * @retval ��
 */
void ILI9341_DispString_EN_CH_YDir (  uint16_t usX,uint16_t usY , char * pStr )
{
	uint16_t usCh;
	
	while( * pStr != '\0' )
	{			
			//ͳһʹ�ú��ֵĿ�������㻻��
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
			
		//��ʾ	
		if ( * pStr <= 126 )	           	//Ӣ���ַ�
		{			
			ILI9341_DispChar_EN ( usX, usY, * pStr);
			
			pStr ++;
			
			usY += HEIGHT_CH_CHAR;		
		}
		else	                            //�����ַ�
		{			
			usCh = * ( uint16_t * ) pStr;	
			
			usCh = ( usCh << 8 ) + ( usCh >> 8 );		

			ILI9341_DispChar_CH ( usX,usY , usCh );
			
			usY += HEIGHT_CH_CHAR;
			
			pStr += 2;           //һ�����������ֽ� 
		
    }
		
  }	
} 

/***********************��������****************************/
#define ZOOMMAXBUFF 16384
uint8_t zoomBuff[ZOOMMAXBUFF] = {0};	//�������ŵĻ��棬���֧�ֵ�128*128
uint8_t zoomTempBuff[1024] = {0};

/**
 * @brief  ������ģ�����ź����ģ��1�����ص���8������λ����ʾ
										0x01��ʾ�ʼ���0x00��ʾ�հ���
 * @param  in_width ��ԭʼ�ַ����
 * @param  in_heig ��ԭʼ�ַ��߶�
 * @param  out_width �����ź���ַ����
 * @param  out_heig�����ź���ַ��߶�
 * @param  in_ptr ���ֿ�����ָ��	ע�⣺1pixel 1bit
 * @param  out_ptr �����ź���ַ����ָ�� ע��: 1pixel 8bit
 *		out_ptrʵ����û������������ĳ���ֱ�������ȫ��ָ��zoomBuff��
 * @param  en_cn ��0ΪӢ�ģ�1Ϊ����
 * @retval ��
 */
void ILI9341_zoomChar(
    uint16_t in_width,	//ԭʼ�ַ����
    uint16_t in_heig,		//ԭʼ�ַ��߶�
    uint16_t out_width,	//���ź���ַ����
    uint16_t out_heig,	//���ź���ַ��߶�
    uint8_t *in_ptr,	//�ֿ�����ָ��	ע�⣺1pixel 1bit
    uint8_t *out_ptr, //���ź���ַ����ָ�� ע��: 1pixel 8bit
    uint8_t en_cn)		//0ΪӢ�ģ�1Ϊ����	
{
	uint8_t *pts,*ots;
	//����Դ��ģ��Ŀ����ģ��С���趨����������ӣ�����16��Ϊ�˰Ѹ�������ת�ɶ�������
	unsigned int xrIntFloat_16=(in_width<<16)/out_width+1; 
  unsigned int yrIntFloat_16=(in_heig<<16)/out_heig+1;
	
	unsigned int srcy_16=0;
	unsigned int y,x;
	uint8_t *pSrcLine;
	
	uint16_t byteCount,bitCount;
	
	//�������Ƿ�Ϸ�
	if(in_width >= 32) return;												//�ֿⲻ������32����
	if(in_width * in_heig == 0) return;	
	if(in_width * in_heig >= 1024 ) return; 					//����������� 32*32
	
	if(out_width * out_heig == 0) return;	
	if(out_width * out_heig >= ZOOMMAXBUFF ) return; //����������� 128*128
	pts = (uint8_t*)&zoomTempBuff;
	
	//Ϊ�������㣬�ֿ��������1 pixel/1bit ӳ�䵽1pixel/8bit
	//0x01��ʾ�ʼ���0x00��ʾ�հ���
	if(en_cn == 0x00)//Ӣ��
	{
		//Ӣ�ĺ������ֿ����±߽粻�ԣ����ڴ˴���������Ҫע��tempBuff��ֹ���
			for(byteCount=0;byteCount<in_heig*in_width/8;byteCount++)	
			{
				for(bitCount=0;bitCount<8;bitCount++)
					{						
						//��Դ��ģ������λӳ�䵽�ֽ�
						//in_ptr��bitXΪ1����pts�������ֽ�ֵΪ1
						//in_ptr��bitXΪ0����pts�������ֽ�ֵΪ0
						*pts++ = (in_ptr[byteCount] & (0x80>>bitCount))?1:0; 
					}
			}				
	}
	else //����
	{			
			for(byteCount=0;byteCount<in_heig*in_width/8;byteCount++)	
			{
				for(bitCount=0;bitCount<8;bitCount++)
					{						
						//��Դ��ģ������λӳ�䵽�ֽ�
						//in_ptr��bitXΪ1����pts�������ֽ�ֵΪ1
						//in_ptr��bitXΪ0����pts�������ֽ�ֵΪ0
						*pts++ = (in_ptr[byteCount] & (0x80>>bitCount))?1:0; 
					}
			}		
	}

	//zoom����
	pts = (uint8_t*)&zoomTempBuff;	//ӳ����Դ����ָ��
	ots = (uint8_t*)&zoomBuff;	//������ݵ�ָ��
	for (y=0;y<out_heig;y++)	/*�б���*/
    {
				unsigned int srcx_16=0;
        pSrcLine=pts+in_width*(srcy_16>>16);				
        for (x=0;x<out_width;x++) /*�������ر���*/
        {
            ots[x]=pSrcLine[srcx_16>>16]; //��Դ��ģ���ݸ��Ƶ�Ŀ��ָ����
            srcx_16+=xrIntFloat_16;			//������ƫ��Դ���ص�
        }
        srcy_16+=yrIntFloat_16;				  //������ƫ��Դ���ص�
        ots+=out_width;						
    }
	/*���������ź����ģ����ֱ�Ӵ洢��ȫ��ָ��zoomBuff����*/
	out_ptr = (uint8_t*)&zoomBuff;	//out_ptrû����ȷ�������������ֱ�Ӹĳ���ȫ�ֱ���ָ�룡
	
	/*ʵ�������ʹ��out_ptr����Ҫ������һ�䣡����
		ֻ����Ϊout_ptrû��ʹ�ã��ᵼ��warning��ǿ��֢*/
	out_ptr++; 
}			


/**
 * @brief  �������ź����ģ��ʾ�ַ�
 * @param  Xpos ���ַ���ʾλ��x
 * @param  Ypos ���ַ���ʾλ��y
 * @param  Font_width ���ַ����
 * @param  Font_Heig���ַ��߶�
 * @param  c ��Ҫ��ʾ����ģ����
 * @param  DrawModel ���Ƿ�ɫ��ʾ 
 * @retval ��
 */
void ILI9341_DrawChar_Ex(
    uint16_t usX, //�ַ���ʾλ��x
    uint16_t usY, //�ַ���ʾλ��y
    uint16_t Font_width, //�ַ����
    uint16_t Font_Height,  //�ַ��߶� 
    uint8_t *c,						//��ģ����
    uint16_t DrawModel)		//�Ƿ�ɫ��ʾ
{
  uint32_t index = 0, counter = 0;
  uint16_t back_color = ILI9341_GetPointPixel(usX,usY);
	//������ʾ����
	ILI9341_OpenWindow ( usX, usY, Font_width, Font_Height);
	
	ILI9341_Write_Cmd ( CMD_SetPixel );		
	
	//���ֽڶ�ȡ��ģ����
	//����ǰ��ֱ����������ʾ���ڣ���ʾ���ݻ��Զ�����
	for ( index = 0; index < Font_Height; index++ )
	{
			//һλһλ����Ҫ��ʾ����ɫ
			for ( counter = 0; counter < Font_width; counter++ )
			{
					//���ź����ģ���ݣ���һ���ֽڱ�ʾһ������λ
					//�����ֽ�ֵΪ1��ʾ������Ϊ�ʼ�
					//�����ֽ�ֵΪ0��ʾ������Ϊ����
					if ( *c++ == DrawModel ){
						ILI9341_Write_Data ( back_color );	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!		
					}else
						ILI9341_Write_Data ( CurrentForecolor );
			}	
	}	
}


/**
 * @brief  �������ź����ģ��ʾ�ַ���
 * @param  Xpos ���ַ���ʾλ��x
 * @param  Ypos ���ַ���ʾλ��y
 * @param  Font_width ���ַ���ȣ�Ӣ���ַ��ڴ˻�����/2��ע��Ϊż��
 * @param  Font_Heig���ַ��߶ȣ�ע��Ϊż��
 * @param  c ��Ҫ��ʾ���ַ���
 * @param  DrawModel ���Ƿ�ɫ��ʾ 
 * @retval ��
 */
void ILI9341_DisplayStringEx(
    uint16_t x, 		//�ַ���ʾλ��x
    uint16_t y, 				//�ַ���ʾλ��y
    uint16_t Font_width,	//Ҫ��ʾ�������ȣ�Ӣ���ַ��ڴ˻�����/2��ע��Ϊż��
    uint16_t Font_Height,	//Ҫ��ʾ������߶ȣ�ע��Ϊż��
    uint8_t *ptr,					//��ʾ���ַ�����
    uint16_t DrawModel)  //�Ƿ�ɫ��ʾ
{
	uint16_t Charwidth = Font_width; //Ĭ��ΪFont_width��Ӣ�Ŀ��Ϊ���Ŀ�ȵ�һ��
	uint8_t *psr;
	uint8_t Ascii;	//Ӣ��
	uint16_t usCh;  //����
	uint8_t ucBuffer [ WIDTH_CH_CHAR*HEIGHT_CH_CHAR/8 ];	
	
	while ( *ptr != '\0')
	{
			/****������*****/
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
			
		if(*ptr > 0x80) //���������
		{			
			Charwidth = Font_width;
			usCh = * ( uint16_t * ) ptr;				
			usCh = ( usCh << 8 ) + ( usCh >> 8 );
			GetGBKCode ( ucBuffer, usCh );	//ȡ��ģ����
			//������ģ���ݣ�Դ��ģΪ16*16
			ILI9341_zoomChar(WIDTH_CH_CHAR,HEIGHT_CH_CHAR,Charwidth,Font_Height,(uint8_t *)&ucBuffer,psr,1); 
			//��ʾ�����ַ�
			ILI9341_DrawChar_Ex(x,y,Charwidth,Font_Height,(uint8_t*)&zoomBuff,DrawModel);
			x+=Charwidth;
			ptr+=2;
		}
		else
		{
				Charwidth = Font_width / 2;
				Ascii = *ptr - 32;
				//ʹ��16*24����������ģ����
				ILI9341_zoomChar(16,24,Charwidth,Font_Height,(uint8_t *)&Font16x24.table[Ascii * Font16x24.Height*Font16x24.Width/8],psr,0);
			  //��ʾ�����ַ�
				ILI9341_DrawChar_Ex(x,y,Charwidth,Font_Height,(uint8_t*)&zoomBuff,DrawModel);
				x+=Charwidth;
				ptr++;
		}
	}
}


/**
 * @brief  �������ź����ģ��ʾ�ַ���(��Y�᷽��)
 * @param  Xpos ���ַ���ʾλ��x
 * @param  Ypos ���ַ���ʾλ��y
 * @param  Font_width ���ַ���ȣ�Ӣ���ַ��ڴ˻�����/2��ע��Ϊż��
 * @param  Font_Heig���ַ��߶ȣ�ע��Ϊż��
 * @param  c ��Ҫ��ʾ���ַ���
 * @param  DrawModel ���Ƿ�ɫ��ʾ 
 * @retval ��
 */
void ILI9341_DisplayStringEx_YDir(
    uint16_t x, 		//�ַ���ʾλ��x
    uint16_t y, 				//�ַ���ʾλ��y
    uint16_t Font_width,	//Ҫ��ʾ�������ȣ�Ӣ���ַ��ڴ˻�����/2��ע��Ϊż��
    uint16_t Font_Height,	//Ҫ��ʾ������߶ȣ�ע��Ϊż��
    uint8_t *ptr,					//��ʾ���ַ�����
    uint16_t DrawModel)  //�Ƿ�ɫ��ʾ
{
	uint16_t Charwidth = Font_width; //Ĭ��ΪFont_width��Ӣ�Ŀ��Ϊ���Ŀ�ȵ�һ��
	uint8_t *psr;
	uint8_t Ascii;	//Ӣ��
	uint16_t usCh;  //����
	uint8_t ucBuffer [ WIDTH_CH_CHAR*HEIGHT_CH_CHAR/8 ];	
	
	while ( *ptr != '\0')
	{			
			//ͳһʹ�ú��ֵĿ�������㻻��
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
			
		if(*ptr > 0x80) //���������
		{			
			Charwidth = Font_width;
			usCh = * ( uint16_t * ) ptr;				
			usCh = ( usCh << 8 ) + ( usCh >> 8 );
			GetGBKCode ( ucBuffer, usCh );	//ȡ��ģ����
			//������ģ���ݣ�Դ��ģΪ16*16
			ILI9341_zoomChar(WIDTH_CH_CHAR,HEIGHT_CH_CHAR,Charwidth,Font_Height,(uint8_t *)&ucBuffer,psr,1); 
			//��ʾ�����ַ�
			ILI9341_DrawChar_Ex(x,y,Charwidth,Font_Height,(uint8_t*)&zoomBuff,DrawModel);
			y+=Font_Height;
			ptr+=2;
		}
		else
		{
				Charwidth = Font_width / 2;
				Ascii = *ptr - 32;
				//ʹ��16*24����������ģ����
				ILI9341_zoomChar(16,24,Charwidth,Font_Height,(uint8_t *)&Font16x24.table[Ascii * Font16x24.Height*Font16x24.Width/8],psr,0);
			  //��ʾ�����ַ�
				ILI9341_DrawChar_Ex(x,y,Charwidth,Font_Height,(uint8_t*)&zoomBuff,DrawModel);
				y+=Font_Height;
				ptr++;
		}
	}
}

void Lcd_display_String(uint8_t *ptr){
  LCD_SetColors(GB888(0x000000), GB888(0xFFFFFF));
  ILI9341_Clear(0,(LCD_Y_LENGTH-Font24x32.Height),LCD_X_LENGTH,Font24x32.Height);	/* ��������ʾ ��ɫ�Ľ��� */
  
  LCD_SetFont(&Font24x32);  //�����Ի���
  LCD_SetForeColor( GB888(0xBB3756)); //��ɫ����
  ILI9341_DisplayStringEx(0,LCD_Y_LENGTH-Font24x32.Height,Font24x32.Width  , Font24x32.Height , ptr,0);
  fn_Systick_Delay(1000,_Systick_ms);
}







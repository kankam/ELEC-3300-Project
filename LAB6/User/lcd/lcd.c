#include "lcd.h"
#include "ascii.h"	
#include "main.h"

char END_X1,END_X2,END_X3,END_X4;
char END_Y1,END_Y2,END_Y3,END_Y4;
char END_Z1,END_Z2,END_Z3,END_Z4;
char STR_X1,STR_X2,STR_X3,STR_X4;
char STR_Y1,STR_Y2,STR_Y3,STR_Y4;
char STR_Z1,STR_Z2,STR_Z3,STR_Z4;
char fames1,fames2,fames3,fames4;
char interval1,interval2,interval3,interval4;
char shutterT1, shutterT2, shutterT3, shutterT4;
char Frames_taken1,Frames_taken2,Frames_taken3,Frames_taken4;
char Hour1,Hour2,Minute1,Minute2,Second1,Second2;
long Hour, Minute,Second;

char nowX1,nowX2,nowX3,nowX4;
char nowY1,nowY2,nowY3,nowY4;
char nowZ1,nowZ2,nowZ3,nowZ4;
char speed1;

static void                   LCD_GPIO_Config         ( void );
static void                   LCD_FSMC_Config         ( void );
static void                   LCD_REG_Config          ( void );
static __inline void          LCD_FillColor           ( uint32_t ulAmout_Point, uint16_t usColor );
static uint16_t               LCD_Read_PixelData      ( void );

void Delay ( __IO uint32_t nCount ){  for ( ; nCount != 0; nCount -- );}

void LCD_INIT ( void )
{
	LCD_GPIO_Config ();
	LCD_FSMC_Config ();
	
	LCD_BackLed_Control ( ENABLE );      
	LCD_Rst ();
	LCD_REG_Config ();
	LCD_Clear ( 0, 0, 240, 320, BACKGROUND);
}



void LCD_Rst ( void )
{			
	GPIO_ResetBits ( LCD_RST_PORT, LCD_RST_PIN );	 

	Delay ( 0xAFFf<<2 ); 					   

	GPIO_SetBits ( LCD_RST_PORT, LCD_RST_PIN );		 	 

	Delay ( 0xAFFf<<2 ); 	
	
}


void LCD_BackLed_Control ( FunctionalState enumState )
{
	if ( enumState )
		GPIO_ResetBits ( LCD_BK_PORT, LCD_BK_PIN );
	
	else
		GPIO_SetBits ( LCD_BK_PORT, LCD_BK_PIN );
		
}


__inline void LCD_Write_Cmd ( uint16_t usCmd )
{
	* ( __IO uint16_t * ) ( FSMC_Addr_LCD_CMD ) = usCmd;
	
}




__inline void LCD_Write_Data ( uint16_t usData )
{
	* ( __IO uint16_t * ) ( FSMC_Addr_LCD_DATA ) = usData;
	
}




__inline uint16_t LCD_Read_Data ( void )
{
	return ( * ( __IO uint16_t * ) ( FSMC_Addr_LCD_DATA ) );
	
}


static void LCD_GPIO_Config ( void )
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	
	RCC_APB2PeriphClockCmd ( RCC_APB2Periph_AFIO, ENABLE );
	
	
	RCC_APB2PeriphClockCmd ( RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE, ENABLE );
		
	
	/* FSMC-D0~D15: PD 14 15 0 1,PE 7 8 9 10 11 12 13 14 15,PD 8 9 10 */	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF_PP;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0  | GPIO_Pin_1  | GPIO_Pin_8 | GPIO_Pin_9 | 
																GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init ( GPIOD, & GPIO_InitStructure );
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | 
																GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | 
																GPIO_Pin_15;
	GPIO_Init ( GPIOE,  & GPIO_InitStructure ); 
	
	
	/* 
	 * PD4-FSMC_NOE   :LCD-RD
	 * PD5-FSMC_NWE   :LCD-WR
	 * PD7-FSMC_NE1   :LCD-CS
	 * PD11-FSMC_A16  :LCD-DC
	 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF_PP;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; 
	GPIO_Init (GPIOD, & GPIO_InitStructure );
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 
	GPIO_Init (GPIOD, & GPIO_InitStructure );
	
	LCD_CS_APBxClock_FUN ( LCD_CS_CLK, ENABLE );
	GPIO_InitStructure.GPIO_Pin = LCD_CS_PIN; 
	GPIO_Init ( LCD_CS_PORT, & GPIO_InitStructure );  
	
	LCD_DC_APBxClock_FUN ( LCD_DC_CLK, ENABLE );
	GPIO_InitStructure.GPIO_Pin = LCD_DC_PIN; 
	GPIO_Init ( LCD_DC_PORT, & GPIO_InitStructure );
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	LCD_RST_APBxClock_FUN ( LCD_RST_CLK, ENABLE );
	GPIO_InitStructure.GPIO_Pin = LCD_RST_PIN; 
	GPIO_Init ( LCD_RST_PORT, & GPIO_InitStructure );
	

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	
	LCD_BK_APBxClock_FUN ( LCD_BK_CLK, ENABLE );
	GPIO_InitStructure.GPIO_Pin = LCD_BK_PIN; 
	GPIO_Init ( LCD_BK_PORT, & GPIO_InitStructure );
	

}




static void LCD_FSMC_Config ( void )
{
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef  p; 
	
	
	/* FSMC*/
	RCC_AHBPeriphClockCmd ( RCC_AHBPeriph_FSMC, ENABLE );


	p.FSMC_AddressSetupTime      = 0x02;	
	p.FSMC_AddressHoldTime       = 0x00;	
	p.FSMC_DataSetupTime         = 0x05;
	p.FSMC_BusTurnAroundDuration = 0x00;
	p.FSMC_CLKDivision           = 0x00;
	p.FSMC_DataLatency           = 0x00;
	p.FSMC_AccessMode            = FSMC_AccessMode_B;	 
	FSMC_NORSRAMInitStructure.FSMC_Bank                  = FSMC_Bank1_NORSRAMx;
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux        = FSMC_DataAddressMux_Disable;
	//FSMC_NORSRAMInitStructure.FSMC_MemoryType          = FSMC_MemoryType_SRAM;
	FSMC_NORSRAMInitStructure.FSMC_MemoryType            = FSMC_MemoryType_NOR;
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth       = FSMC_MemoryDataWidth_16b;
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode       = FSMC_BurstAccessMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity    = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_WrapMode              = FSMC_WrapMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive      = FSMC_WaitSignalActive_BeforeWaitState;
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation        = FSMC_WriteOperation_Enable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal            = FSMC_WaitSignal_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode          = FSMC_ExtendedMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst            = FSMC_WriteBurst_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = & p;
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct     = & p;  
	
	FSMC_NORSRAMInit ( & FSMC_NORSRAMInitStructure ); 
	
	
	/*  FSMC_Bank1_NORSRAM4 */
	FSMC_NORSRAMCmd ( FSMC_Bank1_NORSRAMx, ENABLE );  
		
		
}



static void LCD_REG_Config ( void )
{
	/*  Power control B (CFh)  */
	DEBUG_DELAY  ();
	LCD_Write_Cmd ( 0xCF  );
	LCD_Write_Data ( 0x00  );
	LCD_Write_Data ( 0x81  );
	LCD_Write_Data ( 0x30  );
	
	/*  Power on sequence control (EDh) */
	DEBUG_DELAY ();
	LCD_Write_Cmd ( 0xED );
	LCD_Write_Data ( 0x64 );
	LCD_Write_Data ( 0x03 );
	LCD_Write_Data ( 0x12 );
	LCD_Write_Data ( 0x81 );
	
	/*  Driver timing control A (E8h) */
	DEBUG_DELAY ();
	LCD_Write_Cmd ( 0xE8 );
	LCD_Write_Data ( 0x85 );
	LCD_Write_Data ( 0x10 );
	LCD_Write_Data ( 0x78 );
	
	/*  Power control A (CBh) */
	DEBUG_DELAY ();
	LCD_Write_Cmd ( 0xCB );
	LCD_Write_Data ( 0x39 );
	LCD_Write_Data ( 0x2C );
	LCD_Write_Data ( 0x00 );
	LCD_Write_Data ( 0x34 );
	LCD_Write_Data ( 0x02 );
	
	/* Pump ratio control (F7h) */
	DEBUG_DELAY ();
	LCD_Write_Cmd ( 0xF7 );
	LCD_Write_Data ( 0x20 );
	
	/* Driver timing control B */
	DEBUG_DELAY ();
	LCD_Write_Cmd ( 0xEA );
	LCD_Write_Data ( 0x00 );
	LCD_Write_Data ( 0x00 );
	
	/* Frame Rate Control (In Normal Mode/Full Colors) (B1h) */
	DEBUG_DELAY ();
	LCD_Write_Cmd ( 0xB1 );
	LCD_Write_Data ( 0x00 );
	LCD_Write_Data ( 0x1B );
	
	/*  Display Function Control (B6h) */
	DEBUG_DELAY ();
	LCD_Write_Cmd ( 0xB6 );
	LCD_Write_Data ( 0x0A );
	LCD_Write_Data ( 0xA2 );
	
	/* Power Control 1 (C0h) */
	DEBUG_DELAY ();
	LCD_Write_Cmd ( 0xC0 );
	LCD_Write_Data ( 0x35 );
	
	/* Power Control 2 (C1h) */
	DEBUG_DELAY ();
	LCD_Write_Cmd ( 0xC1 );
	LCD_Write_Data ( 0x11 );
	
	/* VCOM Control 1 (C5h) */
	LCD_Write_Cmd ( 0xC5 );
	LCD_Write_Data ( 0x45 );
	LCD_Write_Data ( 0x45 );
	
	/*  VCOM Control 2 (C7h)  */
	LCD_Write_Cmd ( 0xC7 );
	LCD_Write_Data ( 0xA2 );
	
	/* Enable 3G (F2h) */
	LCD_Write_Cmd ( 0xF2 );
	LCD_Write_Data ( 0x00 );
	
	/* Gamma Set (26h) */
	LCD_Write_Cmd ( 0x26 );
	LCD_Write_Data ( 0x01 );
	DEBUG_DELAY ();
	
	/* Positive Gamma Correction */
	LCD_Write_Cmd ( 0xE0 ); //Set Gamma
	LCD_Write_Data ( 0x0F );
	LCD_Write_Data ( 0x26 );
	LCD_Write_Data ( 0x24 );
	LCD_Write_Data ( 0x0B );
	LCD_Write_Data ( 0x0E );
	LCD_Write_Data ( 0x09 );
	LCD_Write_Data ( 0x54 );
	LCD_Write_Data ( 0xA8 );
	LCD_Write_Data ( 0x46 );
	LCD_Write_Data ( 0x0C );
	LCD_Write_Data ( 0x17 );
	LCD_Write_Data ( 0x09 );
	LCD_Write_Data ( 0x0F );
	LCD_Write_Data ( 0x07 );
	LCD_Write_Data ( 0x00 );
	
	/* Negative Gamma Correction (E1h) */
	LCD_Write_Cmd ( 0XE1 ); //Set Gamma
	LCD_Write_Data ( 0x00 );
	LCD_Write_Data ( 0x19 );
	LCD_Write_Data ( 0x1B );
	LCD_Write_Data ( 0x04 );
	LCD_Write_Data ( 0x10 );
	LCD_Write_Data ( 0x07 );
	LCD_Write_Data ( 0x2A );
	LCD_Write_Data ( 0x47 );
	LCD_Write_Data ( 0x39 );
	LCD_Write_Data ( 0x03 );
	LCD_Write_Data ( 0x06 );
	LCD_Write_Data ( 0x06 );
	LCD_Write_Data ( 0x30 );
	LCD_Write_Data ( 0x38 );
	LCD_Write_Data ( 0x0F );
	
	/* memory access control set */
	DEBUG_DELAY ();
	LCD_Write_Cmd ( 0x36 ); 	
	LCD_Write_Data ( 0xC8 );    
	DEBUG_DELAY ();
	
	/* column address control set */
	LCD_Write_Cmd ( CMD_Set_COLUMN ); 
	LCD_Write_Data ( 0x00 );
	LCD_Write_Data ( 0x00 );
	LCD_Write_Data ( 0x00 );
	LCD_Write_Data ( 0xEF );
	
	/* page address control set */
	DEBUG_DELAY ();
	LCD_Write_Cmd ( CMD_Set_PAGE ); 
	LCD_Write_Data ( 0x00 );
	LCD_Write_Data ( 0x00 );
	LCD_Write_Data ( 0x01 );
	LCD_Write_Data ( 0x3F );
	
	/*  Pixel Format Set (3Ah)  */
	DEBUG_DELAY ();
	LCD_Write_Cmd ( 0x3a ); 
	LCD_Write_Data ( 0x55 );
	
	/* Sleep Out (11h)  */
	LCD_Write_Cmd ( 0x11 );	
	Delay ( 0xAFFf<<2 );
	DEBUG_DELAY ();
	
	/* Display ON (29h) */
	LCD_Write_Cmd ( 0x29 ); 
	
	
}



void LCD_OpenWindow ( uint16_t usCOLUMN, uint16_t usPAGE, uint16_t usWidth, uint16_t usHeight )
{	
	LCD_Write_Cmd ( CMD_Set_COLUMN ); 				
	LCD_Write_Data ( usCOLUMN >> 8  );	 
	LCD_Write_Data ( usCOLUMN & 0xff  );	 
	LCD_Write_Data ( ( usCOLUMN + usWidth - 1 ) >> 8  );
	LCD_Write_Data ( ( usCOLUMN + usWidth - 1 ) & 0xff  );

	LCD_Write_Cmd ( CMD_Set_PAGE ); 			     
	LCD_Write_Data ( usPAGE >> 8  );
	LCD_Write_Data ( usPAGE & 0xff  );
	LCD_Write_Data ( ( usPAGE + usHeight - 1 ) >> 8 );
	LCD_Write_Data ( ( usPAGE + usHeight - 1) & 0xff );
	
}


static __inline void LCD_FillColor ( uint32_t ulAmout_Point, uint16_t usColor )
{
	uint32_t i = 0;
	
	
	/* memory write */
	LCD_Write_Cmd ( CMD_SetPixel );	
		
	for ( i = 0; i < ulAmout_Point; i ++ )
		LCD_Write_Data ( usColor );
		
	
}




void LCD_Clear ( uint16_t usCOLUMN, uint16_t usPAGE, uint16_t usWidth, uint16_t usHeight, uint16_t usColor )
{
	LCD_OpenWindow ( usCOLUMN, usPAGE, usWidth, usHeight );

	LCD_FillColor ( usWidth * usHeight, usColor );		
	
}



static uint16_t LCD_Read_PixelData ( void )	
{	
	uint16_t usR=0, usG=0, usB=0 ;

	
	LCD_Write_Cmd ( 0x2E ); 
	
	usR = LCD_Read_Data (); 	/*FIRST READ OUT DUMMY DATA*/
	
	usR = LCD_Read_Data ();  	/*READ OUT RED DATA  */
	usB = LCD_Read_Data ();  	/*READ OUT BLUE DATA*/
	usG = LCD_Read_Data ();  	/*READ OUT GREEN DATA*/	
	
  return ( ( ( usR >> 11 ) << 11 ) | ( ( usG >> 10 ) << 5 ) | ( usB >> 11 ) );
	
}




uint16_t LCD_GetPointPixel ( uint16_t usCOLUMN, uint16_t usPAGE )
{ 
	uint16_t usPixelData;

	LCD_OpenWindow ( usCOLUMN, usPAGE, 1, 1 );
	
	usPixelData = LCD_Read_PixelData ();
	
	return usPixelData;
	
}



void LCD_DrawLine ( uint16_t usC1, uint16_t usP1, uint16_t usC2, uint16_t usP2, uint16_t usColor )
{
	uint16_t us; 
	uint16_t usC_Current, usP_Current;
	
	int32_t lError_C = 0, lError_P = 0, lDelta_C, lDelta_P, lDistance; 
	int32_t lIncrease_C, lIncrease_P; 	
	
	
	lDelta_C = usC2 - usC1;
	lDelta_P = usP2 - usP1; 
	
	usC_Current = usC1; 
	usP_Current = usP1; 
	
	
	if ( lDelta_C > 0 ) 
		lIncrease_C = 1;  
	
	else if ( lDelta_C == 0 ) 
		lIncrease_C = 0;
	
	else 
  	{ 
    		lIncrease_C = -1;
    		lDelta_C = - lDelta_C;
  	} 

	
	if ( lDelta_P > 0 )
		lIncrease_P = 1; 
	
	else if ( lDelta_P == 0 )
		lIncrease_P = 0;
	else 
	{
  		lIncrease_P = -1;
    		lDelta_P = - lDelta_P;
 	} 

	if (  lDelta_C > lDelta_P )
		lDistance = lDelta_C; 
	
	else 
		lDistance = lDelta_P; 
	
	
	for ( us = 0; us <= lDistance + 1; us ++ ) 
	{  
		LCD_DrawDot ( usC_Current, usP_Current, usColor );
		
		lError_C += lDelta_C ; 
		lError_P += lDelta_P ; 
		
		if ( lError_C > lDistance ) 
		{ 
			lError_C -= lDistance; 
			usC_Current += lIncrease_C; 
		}  
		
		if ( lError_P > lDistance ) 
		{ 
			lError_P -= lDistance; 
			usP_Current += lIncrease_P; 
		} 
		
	}  
	
	
}   


void LCD_DrawChar ( uint16_t usC, uint16_t usP, const char cChar )
{
	uint8_t ucTemp, ucRelativePositon, ucPage, ucColumn;

	
	ucRelativePositon = cChar - ' ';
	
	LCD_OpenWindow ( usC, usP, WIDTH_EN_CHAR, HEIGHT_EN_CHAR );
	
	LCD_Write_Cmd ( CMD_SetPixel );	
	
	for ( ucPage = 0; ucPage < HEIGHT_EN_CHAR; ucPage ++ )
	{
		ucTemp = ucAscii_1608 [ ucRelativePositon ] [ ucPage ];
		
		for ( ucColumn = 0; ucColumn < WIDTH_EN_CHAR; ucColumn ++ )
		{
			if ( ucTemp & 0x01 )
				LCD_Write_Data ( 0x001F );
			
			else
				LCD_Write_Data (  0xFFFF );								
			
			ucTemp >>= 1;		
			
		}
		
	}
	
}




void LCD_DrawString ( uint16_t usC, uint16_t usP, const char * pStr )
{
	while ( * pStr != '\0' )
	{
		if ( ( usC - LCD_DispWindow_Start_COLUMN + WIDTH_EN_CHAR ) > LCD_DispWindow_COLUMN )
		{
			usC = LCD_DispWindow_Start_COLUMN;
			usP += HEIGHT_EN_CHAR;
		}
		
		if ( ( usP - LCD_DispWindow_Start_PAGE + HEIGHT_EN_CHAR ) > LCD_DispWindow_PAGE )
		{
			usC = LCD_DispWindow_Start_COLUMN;
			usP = LCD_DispWindow_Start_PAGE;
		}
		
		LCD_DrawChar ( usC, usP, * pStr );
		
		pStr ++;
		
		usC += WIDTH_EN_CHAR;
		
	}
	
}
void DrawMenu(int menu)
{
	
		switch(menu){
		case 0 : 
			LCD_DrawString(10, 10, "ELEC 3300 Project");
			LCD_DrawString(10, 30, "3 Axis Camera Slider");
			LCD_DrawString(20, 50, "Time Lapse Mode");
			LCD_DrawString(20, 70, "Video Mode");
			LCD_DrawString(20, 90, "Manual Mode");
			LCD_DrawString(20, 110, "Settings");
		break;
		case 1 :
			LCD_DrawString(10, 10, "Time Lapse Mode");
			LCD_DrawString(20, 30, "Interactive Mode");
			LCD_DrawString(20, 50, "Input Parameters Manually");		
			LCD_DrawString(20, 70, "Return");
		break;
		case 111 : 
			LCD_DrawString(10, 10, "Please slew the camera to");
			LCD_DrawString(10, 30, "the END position");
			LCD_DrawString(20, 50, "Slide: <<<");
			END_X1 = (END_X/1000)%10+48;
			END_X2 = (END_X/100)%10+48;
			END_X3 = (END_X/10)%10+48;
			END_X4 = END_X%10+48;
			LCD_DrawChar(100, 50, END_X1);
			LCD_DrawChar(110, 50, END_X2);
			LCD_DrawChar(120, 50, END_X3);
			LCD_DrawChar(130, 50, END_X4);
			LCD_DrawString(140, 50, ">>>");
			
			LCD_DrawString(20, 70, "Pan:   <<<");
			END_Y1 = (END_Y/1000)%10+48;
			END_Y2 = (END_Y/100)%10+48;
			END_Y3 = (END_Y/10)%10+48;
			END_Y4 = END_Y%10+48;
			LCD_DrawChar(100, 70, END_Y1);
			LCD_DrawChar(110, 70, END_Y2);
			LCD_DrawChar(120, 70, END_Y3);
			LCD_DrawChar(130, 70, END_Y4);
			LCD_DrawString(140, 70, ">>>");
			
			LCD_DrawString(20, 90, "Tilt:  <<<");
			END_Z1 = (END_Z/1000)%10+48;
			END_Z2 = (END_Z/100)%10+48;
			END_Z3 = (END_Z/10)%10+48;
			END_Z4 = END_Z%10+48;
			LCD_DrawChar(100, 90, END_Z1);
			LCD_DrawChar(110, 90, END_Z2);
			LCD_DrawChar(120, 90, END_Z3);
			LCD_DrawChar(130, 90, END_Z4);
			LCD_DrawString(140, 90, ">>>");
			
			LCD_DrawString(20, 110, "Set");
			LCD_DrawString(20, 130, "Return");
		break;
		case 112 : 
			LCD_DrawString(10, 10, "Please slew the camera to");
			LCD_DrawString(10, 30,  "the START position");
			LCD_DrawString(20, 50, "Slide: <<<");
			STR_X1 = (STR_X/1000)%10+48;
			STR_X2 = (STR_X/100)%10+48;
			STR_X3 = (STR_X/10)%10+48;
			STR_X4 = STR_X%10+48;
			LCD_DrawChar(100, 50, STR_X1);
			LCD_DrawChar(110, 50, STR_X2);
			LCD_DrawChar(120, 50, STR_X3);
			LCD_DrawChar(130, 50, STR_X4);
			LCD_DrawString(140, 50, ">>>");
			
			LCD_DrawString(20, 70, "Pan:   <<<");
			STR_Y1 = (STR_Y/1000)%10+48;
			STR_Y2 = (STR_Y/100)%10+48;
			STR_Y3 = (STR_Y/10)%10+48;
			STR_Y4 = STR_Y%10+48;
			LCD_DrawChar(100, 70, STR_Y1);
			LCD_DrawChar(110, 70, STR_Y2);
			LCD_DrawChar(120, 70, STR_Y3);
			LCD_DrawChar(130, 70, STR_Y4);
			LCD_DrawString(140, 70, ">>>");
			
			LCD_DrawString(20, 90, "Tilt:  <<<");
			STR_Z1 = (STR_Z/1000)%10+48;
			STR_Z2 = (STR_Z/100)%10+48;
			STR_Z3 = (STR_Z/10)%10+48;
			STR_Z4 = STR_Z%10+48;
			LCD_DrawChar(100, 90, STR_Z1);
			LCD_DrawChar(110, 90, STR_Z2);
			LCD_DrawChar(120, 90, STR_Z3);
			LCD_DrawChar(130, 90, STR_Z4);
			LCD_DrawString(140, 90, ">>>");
			
			LCD_DrawString(20, 110, "Set");
			LCD_DrawString(20, 130, "Return");
			break;
			case 113 : 
			LCD_DrawString(10, 10, "Please enter following");
			LCD_DrawString(10, 30, "parameters");
			LCD_DrawString(20, 50, "Fames:    <<<");
			fames1 = (fames/1000)%10+48;
			fames2 = (fames/100)%10+48;
			fames3 = (fames/10)%10+48;
			fames4 = fames%10+48;
			LCD_DrawChar(125, 50, fames1);
			LCD_DrawChar(135, 50, fames2);
			LCD_DrawChar(145, 50, fames3);
			LCD_DrawChar(155, 50, fames4);
			LCD_DrawString(165, 50, ">>>");
			
			LCD_DrawString(20, 70, "Interval: <<<");
			interval1 = (interval/1000)%10+48;
			interval2 = (interval/100)%10+48;
			interval3 = (interval/10)%10+48;
			interval4 = interval%10+48;
			LCD_DrawChar(125, 70, interval1);
			LCD_DrawChar(135, 70, interval2);
			LCD_DrawChar(145, 70, interval3);
			LCD_DrawChar(155, 70, interval4);
			LCD_DrawString(165, 70, ">>>");
			
			LCD_DrawString(20, 90, "Shutter:  <<<");
			shutterT1 = (shutterT/1000)%10+48;
			shutterT2 = (shutterT/100)%10+48;
			shutterT3 = (shutterT/10)%10+48;
			shutterT4 = shutterT%10+48;
			LCD_DrawChar(125, 90, shutterT1);
			LCD_DrawChar(135, 90, shutterT2);
			LCD_DrawChar(145, 90, shutterT3);
			LCD_DrawChar(155, 90, shutterT4);
			LCD_DrawString(165, 90, ">>>");
			
			LCD_DrawString(20, 110, "Set");
			LCD_DrawString(20, 130, "Return");	
			break;
			
			case 114 : 
			LCD_DrawString(10, 10, "Taking Time Lapse... ...");
			LCD_DrawString(10, 30, "Fames Taken: ");
			Frames_taken1 = (Frames_taken/1000)%10+48;
			Frames_taken2 = (Frames_taken/100)%10+48;
			Frames_taken3 = (Frames_taken/10)%10+48;
			Frames_taken4 = Frames_taken%10+48;
			LCD_DrawChar(110, 30, Frames_taken1);
			LCD_DrawChar(120, 30, Frames_taken2);
			LCD_DrawChar(130, 30, Frames_taken3);
			LCD_DrawChar(140, 30, Frames_taken4);
			
			LCD_DrawString(10, 50, "Time Left:");
			LCD_DrawString(130, 50, ":");
			LCD_DrawString(160, 50, ":");
			Hour = timeLeft/3600;
			Minute = (timeLeft - Hour * 60) / 60;
			Second = timeLeft - Hour * 3600 - Minute * 60;
			Hour1 = (Hour/10)%10+48;
			Hour2 = Hour%10+48;
			Minute1 = (Minute/10)%10+48;
			Minute2 = Minute%10+48;
			Second1 = (Second/10)%10+48;
			Second2 = Second%10+48;
			LCD_DrawChar(110, 50, Hour1);
			LCD_DrawChar(120, 50, Hour2);
			LCD_DrawChar(140, 50, Minute1);
			LCD_DrawChar(150, 50, Minute2);
			LCD_DrawChar(170, 50, Second1);
			LCD_DrawChar(180, 50, Second2);
				
			LCD_DrawString(10, 70, "X:");
			LCD_DrawString(80, 70, "Y:");	
			LCD_DrawString(150, 70, "Z:");
				
			nowX1 = (nowX/1000)%10+48;
			nowX2 = (nowX/100)%10+48;
			nowX3 = (nowX/10)%10+48;
			nowX4 = nowX%10+48;
			LCD_DrawChar(30, 70, nowX1);
			LCD_DrawChar(40, 70, nowX2);
			LCD_DrawChar(50, 70, nowX3);
			LCD_DrawChar(60, 70, nowX4);
			
			
			
			nowY1 = (nowY/1000)%10+48;
			nowY2 = (nowY/100)%10+48;
			nowY3 = (nowY/10)%10+48;
			nowY4 = nowY%10+48;
			LCD_DrawChar(100, 70, nowY1);
			LCD_DrawChar(110, 70, nowY2);
			LCD_DrawChar(120, 70, nowY3);
			LCD_DrawChar(130, 70, nowY4);
			
			
			nowZ1 = (nowZ/1000)%10+48;
			nowZ2 = (nowZ/100)%10+48;
			nowZ3 = (nowZ/10)%10+48;
			nowZ4 = nowZ%10+48;
			LCD_DrawChar(170, 70, nowZ1);
			LCD_DrawChar(180, 70, nowZ2);
			LCD_DrawChar(190, 70, nowZ3);
			LCD_DrawChar(200, 70, nowZ4);
			
			LCD_DrawString(20, 90, "Pause");
			LCD_DrawString(20, 110, "Stop");	
			break;
			case 1142 : 
			LCD_DrawString(10, 10, "Taking Time Lapse... ...");
			LCD_DrawString(10, 30, "Fames Taken: ");
			Frames_taken1 = (Frames_taken/1000)%10+48;
			Frames_taken2 = (Frames_taken/100)%10+48;
			Frames_taken3 = (Frames_taken/10)%10+48;
			Frames_taken4 = Frames_taken%10+48;
			LCD_DrawChar(110, 30, Frames_taken1);
			LCD_DrawChar(120, 30, Frames_taken2);
			LCD_DrawChar(130, 30, Frames_taken3);
			LCD_DrawChar(140, 30, Frames_taken4);
			
			LCD_DrawString(10, 50, "Time Left:");
			LCD_DrawString(130, 50, ":");
			LCD_DrawString(160, 50, ":");
			Hour = timeLeft/3600;
			Minute = (timeLeft - Hour * 60) / 60;
			Second = timeLeft - Hour * 3600 - Minute * 60;
			Hour1 = (Hour/10)%10+48;
			Hour2 = Hour%10+48;
			Minute1 = (Minute/10)%10+48;
			Minute2 = Minute%10+48;
			Second1 = (Second/10)%10+48;
			Second2 = Second%10+48;
			LCD_DrawChar(110, 50, Hour1);
			LCD_DrawChar(120, 50, Hour2);
			LCD_DrawChar(140, 50, Minute1);
			LCD_DrawChar(150, 50, Minute2);
			LCD_DrawChar(170, 50, Second1);
			LCD_DrawChar(180, 50, Second2);
			
			LCD_DrawString(10, 70, "X:");
			LCD_DrawString(80, 70, "Y:");	
			LCD_DrawString(150, 70, "Z:");
				
			nowX1 = (nowX/1000)%10+48;
			nowX2 = (nowX/100)%10+48;
			nowX3 = (nowX/10)%10+48;
			nowX4 = nowX%10+48;
			LCD_DrawChar(30, 70, nowX1);
			LCD_DrawChar(40, 70, nowX2);
			LCD_DrawChar(50, 70, nowX3);
			LCD_DrawChar(60, 70, nowX4);
			
			
			
			nowY1 = (nowY/1000)%10+48;
			nowY2 = (nowY/100)%10+48;
			nowY3 = (nowY/10)%10+48;
			nowY4 = nowY%10+48;
			LCD_DrawChar(100, 70, nowY1);
			LCD_DrawChar(110, 70, nowY2);
			LCD_DrawChar(120, 70, nowY3);
			LCD_DrawChar(130, 70, nowY4);
			
			
			nowZ1 = (nowZ/1000)%10+48;
			nowZ2 = (nowZ/100)%10+48;
			nowZ3 = (nowZ/10)%10+48;
			nowZ4 = nowZ%10+48;
			LCD_DrawChar(170, 70, nowZ1);
			LCD_DrawChar(180, 70, nowZ2);
			LCD_DrawChar(190, 70, nowZ3);
			LCD_DrawChar(200, 70, nowZ4);
			
			
			LCD_DrawString(20, 90, "Resume");
			LCD_DrawString(20, 110, "Stop");	
			break;
			case 123 : 
			LCD_DrawString(10, 10, "Please enter the following");
			LCD_DrawString(10, 30, "parameters");
			LCD_DrawString(20, 50, "Total Fames:<<<");
			fames1 = (fames/1000)%10+48;
			fames2 = (fames/100)%10+48;
			fames3 = (fames/10)%10+48;
			fames4 = fames%10+48;
			LCD_DrawChar(145, 50, fames1);
			LCD_DrawChar(155, 50, fames2);
			LCD_DrawChar(165, 50, fames3);
			LCD_DrawChar(175, 50, fames4);
			LCD_DrawString(185, 50, ">>>");
			
			LCD_DrawString(20, 70, "Interval:   <<<");
			interval1 = (interval/1000)%10+48;
			interval2 = (interval/100)%10+48;
			interval3 = (interval/10)%10+48;
			interval4 = interval%10+48;
			LCD_DrawChar(145, 70, interval1);
			LCD_DrawChar(155, 70, interval2);
			LCD_DrawChar(165, 70, interval3);
			LCD_DrawChar(175, 70, interval4);
			LCD_DrawString(185, 70, ">>>");
			
			LCD_DrawString(20, 90, "Shutter:    <<<");
			shutterT1 = (shutterT/1000)%10+48;
			shutterT2 = (shutterT/100)%10+48;
			shutterT3 = (shutterT/10)%10+48;
			shutterT4 = shutterT%10+48;
			LCD_DrawChar(145, 90, shutterT1);
			LCD_DrawChar(155, 90, shutterT2);
			LCD_DrawChar(165, 90, shutterT3);
			LCD_DrawChar(175, 90, shutterT4);
			LCD_DrawString(185, 90, ">>>");
			
			LCD_DrawString(20, 110, "Continue");
			LCD_DrawString(20, 130, "Return");
				
			break;
			case 121 : 
			LCD_DrawString(10, 10, "Please enter the following");
			LCD_DrawString(10, 30, "parameters ");
			LCD_DrawString(20, 50, "Start Position_X:");
			LCD_DrawString(20, 70, "<<<");
			STR_X1 = (STR_X/1000)%10+48;
			STR_X2 = (STR_X/100)%10+48;
			STR_X3 = (STR_X/10)%10+48;
			STR_X4 = STR_X%10+48;
			LCD_DrawChar(45, 70, STR_X1);
			LCD_DrawChar(55, 70, STR_X2);
			LCD_DrawChar(65, 70, STR_X3);
			LCD_DrawChar(75, 70, STR_X4);
			LCD_DrawString(85, 70, ">>>");
			
			LCD_DrawString(20, 90, "Start Position_Y:");
			LCD_DrawString(20, 110, "<<<");
			STR_Y1 = (STR_Y/1000)%10+48;
			STR_Y2 = (STR_Y/100)%10+48;
			STR_Y3 = (STR_Y/10)%10+48;
			STR_Y4 = STR_Y%10+48;
			LCD_DrawChar(45, 110, STR_Y1);
			LCD_DrawChar(55, 110, STR_Y2);
			LCD_DrawChar(65, 110, STR_Y3);
			LCD_DrawChar(75, 110, STR_Y4);
			LCD_DrawString(85, 110, ">>>");
			
			LCD_DrawString(20, 130, "Start Position_Z:");
			LCD_DrawString(20, 150, "<<<");
			STR_Z1 = (STR_Z/1000)%10+48;
			STR_Z2 = (STR_Z/100)%10+48;
			STR_Z3 = (STR_Z/10)%10+48;
			STR_Z4 = STR_Z%10+48;
			LCD_DrawChar(45, 150, STR_Z1);
			LCD_DrawChar(55, 150, STR_Z2);
			LCD_DrawChar(65, 150, STR_Z3);
			LCD_DrawChar(75, 150, STR_Z4);		
			LCD_DrawString(85, 150, ">>>");
			
			LCD_DrawString(20, 170, "Continue");	
			LCD_DrawString(20, 190, "Return");		
			break;
			case 122 : 
			LCD_DrawString(10, 10, "Please enter the following");
			LCD_DrawString(10, 30, "parameters ");
			LCD_DrawString(20, 50, "End Position_X:");
			LCD_DrawString(20, 70, "<<<");
			END_X1 = (END_X/1000)%10+48;
			END_X2 = (END_X/100)%10+48;
			END_X3 = (END_X/10)%10+48;
			END_X4 = END_X%10+48;
			LCD_DrawChar(45, 70, END_X1);
			LCD_DrawChar(55, 70, END_X2);
			LCD_DrawChar(65, 70, END_X3);
			LCD_DrawChar(75, 70, END_X4);
			LCD_DrawString(85, 70, ">>>");
			
			LCD_DrawString(20, 90, "End Position_Y:");
			LCD_DrawString(20, 110, "<<<");
			END_Y1 = (END_Y/1000)%10+48;
			END_Y2 = (END_Y/100)%10+48;
			END_Y3 = (END_Y/10)%10+48;
			END_Y4 = END_Y%10+48;
			LCD_DrawChar(45, 110, END_Y1);
			LCD_DrawChar(55, 110, END_Y2);
			LCD_DrawChar(65, 110, END_Y3);
			LCD_DrawChar(75, 110, END_Y4);
			LCD_DrawString(85, 110, ">>>");
			
			LCD_DrawString(20, 130, "End Position_Z:");
			LCD_DrawString(20, 150, "<<<");
			END_Z1 = (END_Z/1000)%10+48;
			END_Z2 = (END_Z/100)%10+48;
			END_Z3 = (END_Z/10)%10+48;
			END_Z4 = END_Z%10+48;
			LCD_DrawChar(45, 150, END_Z1);
			LCD_DrawChar(55, 150, END_Z2);
			LCD_DrawChar(65, 150, END_Z3);
			LCD_DrawChar(75, 150, END_Z4);		
			LCD_DrawString(85, 150, ">>>");
			
			LCD_DrawString(20, 170, "Continue");	
			LCD_DrawString(20, 190, "Return");		
			break;
			case 2:
				LCD_DrawString(10, 10, "Video Mode");
			LCD_DrawString(20, 30, "Interactive Mode");
			LCD_DrawString(20, 50, "Input Parameters Manually");
			LCD_DrawString(20, 70, "Return");
			break;
			case 223 :
			LCD_DrawString(10, 10, "Please enter the following");
			LCD_DrawString(10, 30, "parameters");
			LCD_DrawString(20, 50, "Speed(1-5):");

			speed1 = speed%10+48;
			LCD_DrawChar(125, 50, speed1);
			
			
			
			LCD_DrawString(20, 70, "Continue");
			LCD_DrawString(20, 90, "Return");		
				
			break;
			case 224 :
			LCD_DrawString(10, 10, "Taking Video... ...");
			LCD_DrawString(10, 30, "Time Left:");
			LCD_DrawString(130, 30, ":");
			LCD_DrawString(160, 30, ":");
			Hour = timeLeft/3600;
			Minute = (timeLeft - Hour * 60) / 60;
			Second = timeLeft - Hour * 3600 - Minute * 60;
			Hour1 = (Hour/10)%10+48;
			Hour2 = Hour%10+48;
			Minute1 = (Minute/10)%10+48;
			Minute2 = Minute%10+48;
			Second1 = (Second/10)%10+48;
			Second2 = Second%10+48;
			LCD_DrawChar(110, 30, Hour1);
			LCD_DrawChar(120, 30, Hour2);
			LCD_DrawChar(140, 30, Minute1);
			LCD_DrawChar(150, 30, Minute2);
			LCD_DrawChar(170, 30, Second1);
			LCD_DrawChar(180, 30, Second2);
			
			LCD_DrawString(10, 50, "X:");
			LCD_DrawString(80, 50, "Y:");	
			LCD_DrawString(150, 50, "Z:");
				
			nowX1 = (nowX/1000)%10+48;
			nowX2 = (nowX/100)%10+48;
			nowX3 = (nowX/10)%10+48;
			nowX4 = nowX%10+48;
			LCD_DrawChar(30, 50, nowX1);
			LCD_DrawChar(40, 50, nowX2);
			LCD_DrawChar(50, 50, nowX3);
			LCD_DrawChar(60, 50, nowX4);
			
			
			
			nowY1 = (nowY/1000)%10+48;
			nowY2 = (nowY/100)%10+48;
			nowY3 = (nowY/10)%10+48;
			nowY4 = nowY%10+48;
			LCD_DrawChar(100, 50, nowY1);
			LCD_DrawChar(110, 50, nowY2);
			LCD_DrawChar(120, 50, nowY3);
			LCD_DrawChar(130, 50, nowY4);
			
			
			nowZ1 = (nowZ/1000)%10+48;
			nowZ2 = (nowZ/100)%10+48;
			nowZ3 = (nowZ/10)%10+48;
			nowZ4 = nowZ%10+48;
			LCD_DrawChar(170, 50, nowZ1);
			LCD_DrawChar(180, 50, nowZ2);
			LCD_DrawChar(190, 50, nowZ3);
			LCD_DrawChar(200, 50, nowZ4);
			
			LCD_DrawString(20, 70, "Pause");
			LCD_DrawString(20, 90, "Stop");	
			break;
			case 2242 : 
			LCD_DrawString(10, 10, "Taking Video... ...");
			LCD_DrawString(10, 30, "Time Left:");
			LCD_DrawString(130, 30, ":");
			LCD_DrawString(160, 30, ":");
			Hour = timeLeft/3600;
			Minute = (timeLeft - Hour * 60) / 60;
			Second = timeLeft - Hour * 3600 - Minute * 60;
			Hour1 = (Hour/10)%10+48;
			Hour2 = Hour%10+48;
			Minute1 = (Minute/10)%10+48;
			Minute2 = Minute%10+48;
			Second1 = (Second/10)%10+48;
			Second2 = Second%10+48;
			LCD_DrawChar(110, 30, Hour1);
			LCD_DrawChar(120, 30, Hour2);
			LCD_DrawChar(140, 30, Minute1);
			LCD_DrawChar(150, 30, Minute2);
			LCD_DrawChar(170, 30, Second1);
			LCD_DrawChar(180, 30, Second2);
			
			LCD_DrawString(10, 50, "X:");
			LCD_DrawString(80, 50, "Y:");	
			LCD_DrawString(150, 50, "Z:");
				
			nowX1 = (nowX/1000)%10+48;
			nowX2 = (nowX/100)%10+48;
			nowX3 = (nowX/10)%10+48;
			nowX4 = nowX%10+48;
			LCD_DrawChar(30, 50, nowX1);
			LCD_DrawChar(40, 50, nowX2);
			LCD_DrawChar(50, 50, nowX3);
			LCD_DrawChar(60, 50, nowX4);
			
			
			
			nowY1 = (nowY/1000)%10+48;
			nowY2 = (nowY/100)%10+48;
			nowY3 = (nowY/10)%10+48;
			nowY4 = nowY%10+48;
			LCD_DrawChar(100, 50, nowY1);
			LCD_DrawChar(110, 50, nowY2);
			LCD_DrawChar(120, 50, nowY3);
			LCD_DrawChar(130, 50, nowY4);
			
			
			nowZ1 = (nowZ/1000)%10+48;
			nowZ2 = (nowZ/100)%10+48;
			nowZ3 = (nowZ/10)%10+48;
			nowZ4 = nowZ%10+48;
			LCD_DrawChar(170, 50, nowZ1);
			LCD_DrawChar(180, 50, nowZ2);
			LCD_DrawChar(190, 50, nowZ3);
			LCD_DrawChar(200, 50, nowZ4);
		
			
			LCD_DrawString(20, 70, "Resume");
			LCD_DrawString(20, 90, "Stop");	
			break;
		case 3 : 
			LCD_DrawString(10, 10, "Manual Mode");
			LCD_DrawString(20, 30, "Slide: <<<");
			nowX1 = (nowX/1000)%10+48;
			nowX2 = (nowX/100)%10+48;
			nowX3 = (nowX/10)%10+48;
			nowX4 = nowX%10+48;
			LCD_DrawChar(100, 30, nowX1);
			LCD_DrawChar(110, 30, nowX2);
			LCD_DrawChar(120, 30, nowX3);
			LCD_DrawChar(130, 30, nowX4);
			LCD_DrawString(140, 30, ">>>");
			
			LCD_DrawString(20, 50, "Pan:   <<<");
			nowY1 = (nowY/1000)%10+48;
			nowY2 = (nowY/100)%10+48;
			nowY3 = (nowY/10)%10+48;
			nowY4 = nowY%10+48;
			LCD_DrawChar(100, 50, nowY1);
			LCD_DrawChar(110, 50, nowY2);
			LCD_DrawChar(120, 50, nowY3);
			LCD_DrawChar(130, 50, nowY4);
			LCD_DrawString(140, 50, ">>>");
			
			LCD_DrawString(20, 70, "Tilt:  <<<");
			nowZ1 = (nowZ/1000)%10+48;
			nowZ2 = (nowZ/100)%10+48;
			nowZ3 = (nowZ/10)%10+48;
			nowZ4 = nowZ%10+48;
			LCD_DrawChar(100, 70, nowZ1);
			LCD_DrawChar(110, 70, nowZ2);
			LCD_DrawChar(120, 70, nowZ3);
			LCD_DrawChar(130, 70, nowZ4);
			LCD_DrawString(140, 70, ">>>");
			
			LCD_DrawString(20, 90, "Return");
		break;
		case 4:
		LCD_DrawString(10, 10, "Settings");
		LCD_DrawString(20, 30, "Re-initialize");
		LCD_DrawString(20, 50, "Return");
		break;
		
	}
}
	
void LCD_DrawArrow(int line)	
{
	switch(line){
		case 0 :
			LCD_DrawArrow_raw(5,15);
		break;
		case 1 :
			LCD_DrawArrow_raw(5,35);
		break;
		case 2 :
			LCD_DrawArrow_raw(5,55);
		break;
		case 3 :
			LCD_DrawArrow_raw(5,75);
		break;
		case 4 :
			LCD_DrawArrow_raw(5,95);
		break;
		case 5 :
			LCD_DrawArrow_raw(5,115);
		break;
		case 6 :
			LCD_DrawArrow_raw(5,135);
		break;
		case 7 :
			LCD_DrawArrow_raw(5,155);
		break;
		case 8 :
			LCD_DrawArrow_raw(5,175);
		break;
		case 9 :
			LCD_DrawArrow_raw(5,195);
		break;
		case 10 :
			LCD_DrawArrow_raw(5,215);
		break;
		case 11 :
			LCD_DrawArrow_raw(5,235);
		break;
		case 12 :
			LCD_DrawArrow_raw(5,255);
		break;
	}
}

void LCD_DrawDot(uint16_t usCOLUMN, uint16_t usPAGE, uint16_t usColor)	
{		
	
	LCD_OpenWindow ( usCOLUMN, usPAGE, 1, 1 );
	LCD_FillColor(1, usColor);		
}

void LCD_DrawArrow_raw(int x, int y){
	LCD_OpenWindow ( x, y, 10, 5 );
	LCD_FillColor(100, 0x0000);
	LCD_OpenWindow ( x+10, y+1, 1, 3);
	LCD_FillColor(18, 0x0000);
	LCD_OpenWindow ( x+11, y+2, 1, 1);
	LCD_FillColor(1, 0x0000);
	LCD_OpenWindow ( x+8, y-1, 2, 1);
	LCD_FillColor(18, 0x0000);
	LCD_OpenWindow ( x+8, y-2, 1, 1);
	LCD_FillColor(1, 0x0000);
	LCD_OpenWindow ( x+8, y+5, 2, 1);
	LCD_FillColor(18, 0x0000);
	LCD_OpenWindow ( x+8, y+6, 1, 1);
	LCD_FillColor(1, 0x0000);
}

void LCD_Clear_All(void){
	LCD_Clear ( 0, 0, 240, 320, BACKGROUND);
}

void LCD_Clear_Arrow(int i){
	LCD_Clear ( 5, 10 + i * 20, 15, 320 - (10 + i * 20), BACKGROUND);
}


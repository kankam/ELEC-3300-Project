#include "stm32f10x.h"
#include "lcd.h"
#include "i2c_GY80.h"
#include "math.h"
#include "stdio.h"

/*------------------------------------------------------------
LAB 6: I2C APPLICATION ON GY80 

Task 1 – Display Compass Result on LCD
Task 2 – Build your 7-segment display circuit and display the last digit on the 7-segment display

Before writing the function, please make sure in stm32f10x_conf.h, the following #include lines are uncommented
			"stm32f10x_fsmc.h" 
			"stm32f10x_gpio.h"
			"stm32f10x_i2c.h"
			"stm32f10x_rcc.h" 
			"misc.h" 
-------------------------------------------------------------*/

#define HMC5883L_Addr	0x3C
#define L3G4200_Addr	0xD2
#define BMP085_Addr	  0xEE
#define ADXL345_Addr	0xA6
#define M_PI 3.14159265358979323846


/* Private function prototypes -----------------------------------------------*/
void HMC_Init(void);
void Delayus(int duration);
void LCD_Print(int Angle);
int HMC_Read(void);
void DisplaySingleDigit(int digit);
void LEDCONFIG(void);
void digitalWrite(int i, int H_L);
void turnOff(void);

char first, first2, first3;
int ANGLE_LAST_DIGIT;
int ANGLE;
int main(void)
{

  LCD_INIT(); 						// LCD_INIT
  I2C_GY80_Init();        // I2C Init
  HMC_Init();             // HMC Init
	LEDCONFIG();


 
  while (1) {
	/* Please add code below to complete the LAB6 */
  /* You might want to create your own functions */	
		//TASK 1
		ANGLE = HMC_Read();
		//LCD_Print(ANGLE);
		LCD_DrawString(10, 10, "The Angle measured: ");
		first = (ANGLE/100)%10+48;
		first2 = (ANGLE/10)%10+48;
		first3 = ANGLE%10+48;
		LCD_DrawChar(50, 20, first);
		LCD_DrawChar(60, 20, first2);
		LCD_DrawChar(70, 20, first3);
		
		//Delayus(67000);
		
		//TASK 2
		ANGLE_LAST_DIGIT = (ANGLE % 100) %10;
		turnOff();
		DisplaySingleDigit(ANGLE_LAST_DIGIT);
		Delayus(67000);
			
  }
}



void HMC_Init(void)
{
  I2C_ByteWrite(HMC5883L_Addr, 0x00, 0x70);
  I2C_ByteWrite(HMC5883L_Addr, 0x01, 0xA0);
  Delayus(10000);
}

void Delayus(int duration)
{
		while(duration--) 
		{
			int i=0x02;				
			while(i--)
			__asm("nop");
		}
}
void LCD_Print(int Angle){
	char Angle_Char[3] = { 0 };
		sprintf(Angle_Char, "0%d", Angle);
		LCD_DrawString(10, 10, "The Angle measured: ");
		LCD_DrawString(10, 50, Angle_Char);
}
int HMC_Read(void){
	int Angle;
	int16_t X_MSB, X_LSB, Y_MSB, Y_LSB;
	int16_t X,Y;
	//int x,y;
	double heading;
		Angle = 0;
	heading = 0;
	I2C_ByteWrite(HMC5883L_Addr, 0x02, 0x01);
	Delayus(60000);
	X_MSB = I2C_ByteRead(HMC5883L_Addr, 0x03);
	X_LSB = I2C_ByteRead(HMC5883L_Addr, 0x04);
	X = X_MSB<<8 | X_LSB;
	Y_MSB = I2C_ByteRead(HMC5883L_Addr, 0x07);
	Y_LSB = I2C_ByteRead(HMC5883L_Addr, 0x08);
	Y = Y_MSB<<8 | Y_LSB;
	//x = (int)(X<<2)/4;
	//y = (int)(Y<<2)/4;
	heading = atan2(Y	, X);
	//heading = atan2(y	, x);
	if(heading < 0) {
		heading += 2 * M_PI;
	}
	Angle = (int)(heading * 180/M_PI);
	//Angle = (Angle - 180)*2; 
	return Angle;
}

void DisplaySingleDigit(int digit)
{
	 //Display Single Digit on 7 segment display
	 //Conditions for displaying segment a
	 if(digit!=1 && digit != 4)
	 digitalWrite(0,1);
	 
	 //Conditions for displaying segment b
	 if(digit != 5 && digit != 6)
	 digitalWrite(1,1);
	 
	 //Conditions for displaying segment c
	 if(digit !=2)
	 digitalWrite(2,1);
	 
	 //Conditions for displaying segment d
	 if(digit != 1 && digit !=4 && digit !=7)
	 digitalWrite(3,1);
	 
	 //Conditions for displaying segment e 
	 if(digit == 2 || digit ==6 || digit == 8 || digit==0)
	 digitalWrite(4,1);
	 
	 //Conditions for displaying segment f
	 if(digit != 1 && digit !=2 && digit!=3 && digit !=7)
	 digitalWrite(5,1);
	  //Conditions for displaying segment g
	 if (digit!=0 && digit!=1 && digit !=7)
	 digitalWrite(6,1);
 }


 void turnOff(void)
{
	  //Clear 7 segment display
	  digitalWrite(0,0);
	  digitalWrite(1,0);
	  digitalWrite(2,0);
	  digitalWrite(3,0);
	  digitalWrite(4,0);
	  digitalWrite(5,0);
	  digitalWrite(6,0);
}

void LEDCONFIG(void)
{		
	/* Task 1: Configure the 7seg as output(A2,A3,A4,A5,A6,A7) */
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 |GPIO_Pin_7 | GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIOA->BSRR = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;
}

void digitalWrite(int i, int H_L){
	switch(i){
		case 0 : 
			if (H_L == 1){ GPIOA->BRR=GPIO_Pin_2;}
			else{GPIOA->BSRR=GPIO_Pin_2;}
		break;
		case 1 : 
			if (H_L == 1){ GPIOA->BRR=GPIO_Pin_3;}
			else{GPIOA->BSRR=GPIO_Pin_3;}
		break;			
		case 2 : 
			if (H_L == 1){ GPIOA->BRR=GPIO_Pin_4;}
			else{GPIOA->BSRR=GPIO_Pin_4;}
		break;
		case 3 : 
			if (H_L == 1){ GPIOA->BRR=GPIO_Pin_5;}
			else{GPIOA->BSRR=GPIO_Pin_5;}
		break;
		case 4 : 
			if (H_L == 1){ GPIOA->BRR=GPIO_Pin_6;}
			else{GPIOA->BSRR=GPIO_Pin_6;}
		break;
		case 5 : 
			if (H_L == 1){ GPIOA->BRR=GPIO_Pin_7;}
			else{GPIOA->BSRR=GPIO_Pin_7;}
		break;
		case 6 : 
			if (H_L == 1){ GPIOA->BRR=GPIO_Pin_8;}
			else{GPIOA->BSRR=GPIO_Pin_8;}
		break;
		}			
}

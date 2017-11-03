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
void Delayus(int duration);
void LCD_Print(int Angle);
void GPIOConf(void);
void digitalWrite(int i, int H_L);
void turnOff(void);

char first, first2, first3;
int ANGLE_LAST_DIGIT;
int ANGLE;
int main(void)
{
	LCD_INIT(); 						// LCD_INIT 


 
  while (1) {
			
  }
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



void GPIOConf(void)
{		
	/* Task 1: Configure the folowing pin as output(A2,A3,A4,A5,A6,A7) */
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

#include "stm32f10x.h"
#include "lcd.h"
#include "math.h"
#include "stdio.h"
#include "stm32f10x_tim.h" 

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
Before writing the function, please make sure "stm32f10x_gpio.h", "stm32f10x_rcc.h", "stm32f10x_tim.h", #include "misc.h" 
located in stm32F10x_conf.h are uncommented, you will be using it.
-------------------------------------------------------------*/
GPIO_InitTypeDef GPIO_InitStructure;
TIM_TimeBaseInitTypeDef	TIM_TimeBaseStructure;
TIM_OCInitTypeDef TIM_OCInitStructure;
void Delayus(int duration);
void LCD_Print(int Angle);
void GPIOConf(void);
void digitalWrite(int i, int H_L);
int digitalRead(int i);
void turnOff(void);
void Step(int motor_mo);
void buzzer(int times);


int ANGLE_LAST_DIGIT;
int ANGLE;
int motor0Max, motor1Max, motor2Max;
int currentMenu;
void motor(int dir, int speed, int step, int motor_no);
int cursor = 0;

///*
int main(void)
{
	LCD_INIT(); 						// LCD_INIT 
	GPIOConf();
	buzzer(2);
	//initialize
	//LCD Print initializeing
	LCD_DrawString(10, 10, "Initializeing ... ...");
	//Slew the slider to min

	while(digitalRead(0) == 0){
		motor(0,0,1,0);
	}		
	//Slew the slider to max
	motor0Max = 0;
	while(digitalRead(1) == 0){
		motor(1,0,1,0);
		motor0Max ++;
	}
	//Change menu to 0
	currentMenu = 0;
  while (1) {
		if(currentMenu == 0){
			DrawMenu(currentMenu);
			LCD_DrawArrow(cursor);
			if(digitalRead(2) == 1){
				if(cursor < 3){
					cursor ++;}
				else{cursor = 0;}
			}
			if(digitalRead(3) == 1){
				if(cursor > 0){
					cursor --;}
				else{cursor = 3;}
			}
			if(cursor == 0 && digitalRead(1) == 1){
				currentMenu = 1;
			}
			if(cursor == 1 && digitalRead(1) == 1){
				currentMenu = 2;
			}
			if(cursor == 2 && digitalRead(1) == 1){
				currentMenu = 3;
			}
			if(cursor == 3 && digitalRead(1) == 1){
				currentMenu = 4;
			}
		}
			//motor(0,0,100,0);
  }
}
//*/

/*
int main(void){
	LCD_INIT();
	GPIOConf();
	buzzer(2);
	while (1) {
	motor(0,1,1,0);
  }
}
/*/


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
	/* Configure the folowing pin as output(A2,A3,A4,A5,A6,A7) */
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 |GPIO_Pin_7 | GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIOA->BSRR = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;
	//Configure the folowing pin as intput(B 0 1 2 3 4 5 6 7 8 )
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1| GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 |GPIO_Pin_7 | GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void digitalWrite(int i, int H_L){
	switch(i){
		case 0 : 
			if (H_L == 0){ GPIOA->BRR=GPIO_Pin_2;}
			else{GPIOA->BSRR=GPIO_Pin_2;}
		break;
		case 1 : 
			if (H_L == 0){ GPIOA->BRR=GPIO_Pin_3;}
			else{GPIOA->BSRR=GPIO_Pin_3;}
		break;			
		case 2 : 
			if (H_L == 0){ GPIOA->BRR=GPIO_Pin_4;}
			else{GPIOA->BSRR=GPIO_Pin_4;}
		break;
		case 3 : 
			if (H_L == 0){ GPIOA->BRR=GPIO_Pin_5;}
			else{GPIOA->BSRR=GPIO_Pin_5;}
		break;
		case 4 : 
			if (H_L == 0){ GPIOA->BRR=GPIO_Pin_6;}
			else{GPIOA->BSRR=GPIO_Pin_6;}
		break;
		case 5 : 
			if (H_L == 0){ GPIOA->BRR=GPIO_Pin_7;}
			else{GPIOA->BSRR=GPIO_Pin_7;}
		break;
		case 6 : 
			if (H_L == 0){ GPIOA->BRR=GPIO_Pin_8;}
			else{GPIOA->BSRR=GPIO_Pin_8;}
		break;
		}			
}

int digitalRead(int i){
	int r;
	switch(i){
		case 0 :
			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)){r = 1;}
			else{r = 0;}
		break;
		case 1 :
			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)){r = 1;}
			else{r = 0;}
		break;			
	}
	return r;
}
void MCO_CONFIG (void)
{		
	/*
	 *  Task 1 – Output SYSCLK via MCO.
	 */
  
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	RCC->CFGR |= (RCC_MCO_SYSCLK << 24);
}

void TIM3_CH1_PWM (void){
	//Generate a 250kHZ PWM
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = 288;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 144;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);
	
	TIM_Cmd(TIM3, ENABLE);
}
void MCO_CONFIG_A6 (void)
{		
	/*
	 *  Task 2
	 */
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
}
void MCO_CONFIG_B6 (void)
{		
	/*
	 *  Task 3
	 */
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
}
void TIM4_CH1_PWM (void){
		
	
		TIM_SelectMasterSlaveMode(TIM3, TIM_MasterSlaveMode_Enable);
	
		TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update);
	
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
		TIM_TimeBaseStructure.TIM_Period = 1285;
		TIM_TimeBaseStructure.TIM_Prescaler = 0;
		TIM_TimeBaseStructure.TIM_ClockDivision = 0;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
		
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_Pulse = 642;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
		TIM_OC1Init(TIM4, &TIM_OCInitStructure);
	
		TIM_SelectSlaveMode(TIM4, TIM_SlaveMode_Gated);
	
		TIM_SelectInputTrigger(TIM4, TIM_TS_ITR2);
		
		TIM_Cmd(TIM4, ENABLE);

	
}
void motor(int dir, int speed, int step, int motor_no){
	int delay_t, i;
	switch(speed){
		case 0 :
			delay_t = 0;
		break;
		case 1:
			delay_t = 1000;
		break;		
	}
	if(dir == 1){
		digitalWrite(2,1);//A4
	}
	else{digitalWrite(2,0);}
	for(i = 0; i < step; i++){
		Step(motor_no);
		Delayus(delay_t);
	}
	
}
void Step(int motor_no){
	switch(motor_no){
		case 0 :
			digitalWrite(1,1); //A3
			Delayus(20);
			digitalWrite(1,0);
			Delayus(20);
		break;
		
	}
}
void buzzer(int times){
	int i =0;
	while(i < times){
		Delayus(100000);
		digitalWrite(0,1);//A2
		Delayus(100000);
		digitalWrite(0,0);
		i++;
	}
}	

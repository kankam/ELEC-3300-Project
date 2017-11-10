#include "stm32f10x.h"
#include "lcd.h"
#include "math.h"
#include "stdio.h"
#include "stdlib.h"
#include "stm32f10x_tim.h" 
#include "millis.h"
#include "millis.c"
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
void GPIOConf(void);
void digitalWrite(int i, int H_L);
int digitalRead(int i);
void Step(int motor_mo);
void buzzer(int times);
void motor(int dir, int speed, int step, int motor_no);
void shutterNow(void);
void DelayAndAbuzz(void);

int ANGLE_LAST_DIGIT;
int ANGLE;
int motor0Max, motor1Max, motor2Max;
int currentMenu;
int cursor = 0;
int TimeLapseFlag =0;
int END_X, END_Y, END_Z;
int STR_X, STR_Y, STR_Z;
int fames, interval, shutterT;
int dir_X, dir_Y, dir_Z;
int totalTime;
int timeLeft;
int totalX, totalY, totalZ;
int STP_X, STP_Y, STP_Z;
long lastOpenShutter = 0, lastCloseShutter = 0;

volatile long counter;
long lastClear = 0;
long nowTime;
long lastSecond;
int changeMenuFlag = 0;
int Frames_taken;
int UpdateRate;
int shutterState = 0;


int main(void)
{
	LCD_INIT(); 						// LCD_INIT 
	GPIOConf();
	MILLIS_Init();

	
	buzzer(2);
	//initialize
	//LCD Print initializeing
	LCD_DrawString(10, 10, "Initializeing ... ...");
	/*
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
	*/
	
	//Change menu to 0
	LCD_Clear_All();
	lastClear = millis();
	
	currentMenu = 0;
	
	cursor = 0;
  while (1) {
		if(currentMenu == 0){
			changeMenuFlag =0;
			DrawMenu(currentMenu);
			LCD_DrawArrow(cursor);
			if(digitalRead(1) == 1){
				LCD_Clear_Arrow(0);
				if(cursor < 3){
					cursor ++;
				}
				else{cursor = 0;}
				DelayAndAbuzz();
			}
			if(digitalRead(0) == 1){
				LCD_Clear_Arrow(0);
				if(cursor > 0){
					cursor --;}
				else{cursor = 3;}
				DelayAndAbuzz();
			}
			if(cursor == 0 && digitalRead(4) == 1 && changeMenuFlag == 0){
				currentMenu = 1;
				cursor = 1;
				TimeLapseFlag =1;
				changeMenuFlag =1;
				DelayAndAbuzz();
				LCD_Clear_All();
			} 	
			if(cursor == 1 && digitalRead(4) == 1 && changeMenuFlag == 0){
				currentMenu = 2;
				cursor = 1;
				changeMenuFlag =1;
				DelayAndAbuzz();
				LCD_Clear_All();
			}
			if(cursor == 2 && digitalRead(4) == 1 && changeMenuFlag == 0){
				currentMenu = 3;
				cursor = 1;
				changeMenuFlag =1;
				DelayAndAbuzz();
				LCD_Clear_All();
			}
			if(cursor == 3 && digitalRead(4) == 1 && changeMenuFlag == 0){
				currentMenu = 4;
				cursor = 1;
				changeMenuFlag =1;
				DelayAndAbuzz();
				LCD_Clear_All();
			}
		}
		if(currentMenu == 1){
			changeMenuFlag =0;
			DrawMenu(currentMenu);
			LCD_DrawArrow(cursor);
			if(digitalRead(1) == 1){
				LCD_Clear_Arrow(1);
				if(cursor < 3){
					cursor ++;}
				else{cursor = 1;}
				DelayAndAbuzz();
			}
			if(digitalRead(0) == 1){
				LCD_Clear_Arrow(1);
				if(cursor > 1){
					cursor --;}
				else{cursor = 3;}
				DelayAndAbuzz();
			}	
			if(cursor == 1 && digitalRead(4) == 1 && changeMenuFlag == 0){
				currentMenu = 111;
				cursor = 2;
				changeMenuFlag =1;
				DelayAndAbuzz();
				LCD_Clear_All();
			}
			if(cursor == 2 && digitalRead(4) == 1 && changeMenuFlag == 0){
				currentMenu = 121;
				cursor = 1;
				changeMenuFlag =1;
				DelayAndAbuzz();
				LCD_Clear_All();
			}
			if(cursor == 3 && digitalRead(4) == 1 && changeMenuFlag == 0){
				currentMenu = 0;
				cursor = 0;
				TimeLapseFlag = 0;
				changeMenuFlag =1;
				DelayAndAbuzz();
				LCD_Clear_All();
			}
		}
		if(currentMenu == 111){
			changeMenuFlag =0;
			DrawMenu(currentMenu);
			LCD_DrawArrow(cursor);
			if(digitalRead(1) == 1){
				LCD_Clear_Arrow(2);
				if(cursor < 6){
					cursor ++;}
				else{cursor = 2;}
				DelayAndAbuzz();
			}
			if(digitalRead(0) == 1){
				LCD_Clear_Arrow(2);
				if(cursor > 2){
					cursor --;}
				else{cursor = 6;}
				DelayAndAbuzz();
			}	
			if(digitalRead(0) == 0 && digitalRead(1) == 0 && digitalRead(2) == 0 && digitalRead(3) == 0 && digitalRead(4) == 0)
				{
					UpdateRate = 500000;
				}
				
			if(cursor == 2 && digitalRead(2) == 1 && END_X > 0){
				END_X--;
				motor(0,0,160,0);
				Delayus(UpdateRate);
				if(UpdateRate > 10){
					UpdateRate = UpdateRate * 0.8;}
			}
			if(cursor == 2 && digitalRead(3) == 1 && END_X < 9999){
				END_X++;
				motor(1,0,160,0);
				Delayus(UpdateRate);
				if(UpdateRate > 10){
					UpdateRate = UpdateRate * 0.8;}
			}
			if(cursor == 3 && digitalRead(2) == 1 && END_Y > 0){
				END_Y--;
				Delayus(UpdateRate);
				if(UpdateRate > 10){
					UpdateRate = UpdateRate * 0.8;}
			}
			if(cursor == 3 && digitalRead(3) == 1 && END_Y < 9999){
				END_Y++;
				Delayus(UpdateRate);
				if(UpdateRate > 10){
					UpdateRate = UpdateRate * 0.8;}
			}
			if(cursor == 4 && digitalRead(2) == 1 && END_Z > 0){
				END_Z--;
				Delayus(UpdateRate);
				if(UpdateRate > 10){
					UpdateRate = UpdateRate * 0.8;}
			}
			if(cursor == 4 && digitalRead(3) == 1 && END_Z < 9999){
				END_Z++;
				Delayus(UpdateRate);
				if(UpdateRate > 10){
					UpdateRate = UpdateRate * 0.8;}
			}
			if(cursor == 5 && digitalRead(4) == 1 && changeMenuFlag == 0){
				currentMenu = 112;
				cursor = 2;
				changeMenuFlag =1;
				DelayAndAbuzz();
				LCD_Clear_All();
			}
			if(cursor == 6 && digitalRead(4) == 1 && changeMenuFlag == 0){
				currentMenu = 1;
				cursor = 1;
				changeMenuFlag =1;
				DelayAndAbuzz();
				LCD_Clear_All();
			}
		
		}
		if(currentMenu == 112){
			changeMenuFlag =0;
			DrawMenu(currentMenu);
			LCD_DrawArrow(cursor);
			if(digitalRead(1) == 1){
				LCD_Clear_Arrow(2);
				if(cursor < 6){
					cursor ++;}
				else{cursor = 2;}
				DelayAndAbuzz();
			}
			if(digitalRead(0) == 1){
				LCD_Clear_Arrow(2);
				if(cursor > 2){
					cursor --;}
				else{cursor = 6;}
				DelayAndAbuzz();
			}	
			if(digitalRead(0) == 0 && digitalRead(1) == 0 && digitalRead(2) == 0 && digitalRead(3) == 0 && digitalRead(4) == 0)
				{
					UpdateRate = 500000;
				}
				
			if(cursor == 2 && digitalRead(2) == 1){
				STR_X--;
				motor(0,0,160,0);
				Delayus(UpdateRate);
				if(UpdateRate > 10){
					UpdateRate = UpdateRate * 0.8;}
			}
			if(cursor == 2 && digitalRead(3) == 1){
				STR_X++;
				motor(1,0,160,0);
				Delayus(UpdateRate);
				if(UpdateRate > 10){
					UpdateRate = UpdateRate * 0.8;}
			}
			if(cursor == 3 && digitalRead(2) == 1){
				STR_Y--;
				Delayus(UpdateRate);
				if(UpdateRate > 10){
					UpdateRate = UpdateRate * 0.8;}
			}
			if(cursor == 3 && digitalRead(3) == 1){
				STR_Y++;
				Delayus(UpdateRate);
				if(UpdateRate > 10){
					UpdateRate = UpdateRate * 0.8;}
			}
			if(cursor == 4 && digitalRead(2) == 1){
				STR_Z--;
				Delayus(UpdateRate);
				if(UpdateRate > 10){
					UpdateRate = UpdateRate * 0.8;}
			}
			if(cursor == 4 && digitalRead(3) == 1){
				STR_Z++;
				Delayus(UpdateRate);
				if(UpdateRate > 10){
					UpdateRate = UpdateRate * 0.8;}
			}
			if(cursor == 5 && digitalRead(4) == 1 && changeMenuFlag == 0){
				currentMenu = 113;
				cursor = 2;
				changeMenuFlag =1;
				DelayAndAbuzz();
				LCD_Clear_All();
			}
			if(cursor == 6 && digitalRead(4) == 1 && changeMenuFlag == 0){
				currentMenu = 111;
				cursor = 2;
				changeMenuFlag =1;
				DelayAndAbuzz();
				LCD_Clear_All();
			}
		
		}		
		if(currentMenu == 113){
			changeMenuFlag =0;
			DrawMenu(currentMenu);
			LCD_DrawArrow(cursor);
			if(digitalRead(1) == 1){
				LCD_Clear_Arrow(2);
				if(cursor < 6){
					cursor ++;}
				else{cursor = 2;}
				DelayAndAbuzz();
			}
			if(digitalRead(0) == 1){
				LCD_Clear_Arrow(2);
				if(cursor > 2){
					cursor --;}
				else{cursor = 6;}
				DelayAndAbuzz();
			}	
			if(digitalRead(0) == 0 && digitalRead(1) == 0 && digitalRead(2) == 0 && digitalRead(3) == 0 && digitalRead(4) == 0)
				{
					UpdateRate = 500000;
				}
				
			if(cursor == 2 && digitalRead(2) == 1){
				fames--;
				Delayus(UpdateRate);
				if(UpdateRate > 10){
					UpdateRate = UpdateRate * 0.8;}
			}
			if(cursor == 2 && digitalRead(3) == 1){
				fames++;
				Delayus(UpdateRate);
				if(UpdateRate > 10){
					UpdateRate = UpdateRate * 0.8;}
			}
			if(cursor == 3 && digitalRead(2) == 1){
				interval--;
				Delayus(UpdateRate);
				if(UpdateRate > 10){
					UpdateRate = UpdateRate * 0.8;}
			}
			if(cursor == 3 && digitalRead(3) == 1){
				interval++;
				Delayus(UpdateRate);
				if(UpdateRate > 10){
					UpdateRate = UpdateRate * 0.8;}
			}
			if(cursor == 4 && digitalRead(2) == 1){
				shutterT--;
				Delayus(UpdateRate);
				if(UpdateRate > 10){
					UpdateRate = UpdateRate * 0.8;}
			}
			if(cursor == 4 && digitalRead(3) == 1){
				shutterT++;
				Delayus(UpdateRate);
				if(UpdateRate > 10){
					UpdateRate = UpdateRate * 0.8;}
			}
			if(cursor == 5 && digitalRead(4) == 1 && changeMenuFlag == 0){
				currentMenu = 114;
				cursor = 3;
				changeMenuFlag =1;
				DelayAndAbuzz();
				totalTime = fames * (interval + shutterT);
				timeLeft = totalTime;
				totalX = abs(END_X - STR_X);
				STP_X = totalX/fames*160;
				if(END_X > STR_X){
					dir_X = 1;
				}
				else{dir_X = 0;}
				totalY = abs(END_Y - STR_Y);
				STP_Y = totalY/fames*160;
				if(END_Y > STR_Y){
					dir_Y = 1;
				}
				else{dir_Y = 0;}
				totalZ = abs(END_Z - STR_Z);
				STP_Z = totalZ/fames*160;
				if(END_Z > STR_Z){
					dir_Z = 1;
				}
				else{dir_Z = 0;}
				lastSecond = millis();
				LCD_Clear_All();
			}
			if(cursor == 6 && digitalRead(4) == 1 && changeMenuFlag == 0){
				currentMenu = 112;
				cursor = 2;
				changeMenuFlag =1;
				DelayAndAbuzz();
				LCD_Clear_All();
			}
		
		}		
		if(currentMenu == 114){
			changeMenuFlag =0;
			
			nowTime = millis();
			if(nowTime - lastSecond > 1000){
				timeLeft --;
				lastSecond = millis();
			}
			DrawMenu(currentMenu);
			LCD_DrawArrow(cursor);
			if(digitalRead(1) == 1){
				LCD_Clear_Arrow(3);
				if(cursor < 4){
					cursor ++;}
				else{cursor = 3;}
				DelayAndAbuzz();
			}
			if(digitalRead(0) == 1){
				LCD_Clear_Arrow(3);
				if(cursor > 3){
					cursor --;}
				else{cursor = 4;}
				DelayAndAbuzz();
			}	
			if(cursor == 3 && digitalRead(4) == 1 && changeMenuFlag == 0){
				if(shutterState ==1){
					shutterNow();
					shutterState = 1;}
				currentMenu = 1142;
				cursor = 3;
				changeMenuFlag =1;
				DelayAndAbuzz();
				LCD_Clear_All();
			}
			if(cursor == 4 && digitalRead(4) == 1 && changeMenuFlag == 0){
				currentMenu = 0;
				cursor = 0;
				TimeLapseFlag = 0;
				timeLeft = 0;
				Frames_taken = 0;
				changeMenuFlag =1;
				DelayAndAbuzz();
				LCD_Clear_All();
			}
			nowTime = millis();
			if(((nowTime - lastCloseShutter))>(interval * 1000) && shutterState == 0){
				shutterNow();
				lastOpenShutter = millis();
				buzzer(1);
				shutterState = 1;
			}
			if(((nowTime - lastOpenShutter))>(shutterT * 1000) && shutterState == 1){
				shutterNow();
				lastCloseShutter = millis();
				shutterState = 0;
				buzzer(1);
				Frames_taken++;
				motor(dir_X,0,STP_X,0);
				motor(dir_Y,0,STP_Y,1);
				motor(dir_Z,0,STP_Z,2);
			}
			if(Frames_taken == fames){
				buzzer(5);
				currentMenu = 0;
				cursor = 0;
				TimeLapseFlag = 0;
				timeLeft = 0;
				Frames_taken = 0;
				changeMenuFlag =1;
				DelayAndAbuzz();
				LCD_Clear_All();}
		}
		if(currentMenu == 1142){
			changeMenuFlag =0;
			DrawMenu(currentMenu);
			LCD_DrawArrow(cursor);
			if(digitalRead(1) == 1){
				LCD_Clear_Arrow(3);
				if(cursor < 4){
					cursor ++;}
				else{cursor = 3;}
				DelayAndAbuzz();
			}
			if(digitalRead(0) == 1){
				LCD_Clear_Arrow(3);
				if(cursor > 3){
					cursor --;}
				else{cursor = 4;}
				DelayAndAbuzz();
			}	
			if(cursor == 3 && digitalRead(4) == 1 && changeMenuFlag == 0){
				currentMenu = 8;
				cursor = 3;
				changeMenuFlag =1;
				DelayAndAbuzz();
				LCD_Clear_All();
			}
			if(cursor == 4 && digitalRead(4) == 1 && changeMenuFlag == 0){
				currentMenu = 0;
				cursor = 0;
				TimeLapseFlag = 0;
				timeLeft = 0;
				Frames_taken = 0;
				changeMenuFlag =1;
				DelayAndAbuzz();
				LCD_Clear_All();
			}
			
		}
	}
}


/*
int main(void){
	LCD_INIT();
	GPIOConf();
	buzzer(2);
	while (1) {
	motor(1,0,1600,0);
	Delayus(5000000);
	motor(0,0,1600,0);
	Delayus(5000000);
	motor(1,1,1600,0);
	Delayus(5000000);
	motor(0,1,1600,0);
	Delayus(5000000);
	motor(1,2,1600,0);
	Delayus(5000000);
	motor(0,2,1600,0);
	Delayus(5000000);
	motor(1,3,1600,0);
	Delayus(5000000);
	motor(0,3,1600,0);
	Delayus(5000000);
	motor(1,4,1600,0);
	Delayus(5000000);
	motor(0,4,1600,0);
	Delayus(5000000);
  }
}
*/


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
	GPIOA->BRR = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	//Configure the folowing pin as intput(B 0 1 5 6 7 8 9)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1| GPIO_Pin_5 | GPIO_Pin_6 |GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
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
			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)){r = 0;}
			else{r = 1;}
		break;
		case 1 :
			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)){r = 0;}
			else{r = 1;}
		break;
		case 2 :
			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)){r = 0;}
			else{r = 1;}
		break;
		case 3 :
			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)){r = 0;}
			else{r = 1;}
		break;
		case 4 :
			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)){r = 0;}
			else{r = 1;}
		break;
		case 5 :
			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)){r = 0;}
			else{r = 1;}
		break;
		case 6 :
			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)){r = 0;}
			else{r = 1;}
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
			delay_t = 312;
		break;
		case 1 :
			delay_t = 625;
		break;
		case 2:
			delay_t = 1250;
		break;
		case 3:
			delay_t = 3125;
		break;
		case 4:
			delay_t = 6250;
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
void shutterNow(void){
	digitalWrite(6, 1);
	digitalWrite(6, 0);
}
void DelayAndAbuzz(void){
	buzzer(1);
	Delayus(500000);
}



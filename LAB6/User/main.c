#include "stm32f10x.h"
#include "lcd.h"
#include "math.h"
#include "stdio.h"
#include "stdlib.h"
#include "stm32f10x_tim.h" 
#include "millis.h"
#include "millis.c"
#include "delay.h"
#include "delay.c"
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
void toggleVideo(void);
void high(unsigned int time, int freq, int pinLED);
void DelayAndAbuzz(void);
int StpPSec(int spd);
void motor0init(void);



unsigned int motor0Max, motor1Max, motor2Max;
int currentMenu;
int cursor = 0;
int TimeLapseFlag =0, VideoFlag =0;
int END_X, END_Y, END_Z;
int STR_X, STR_Y, STR_Z;
int fames, interval, shutterT;
int dir_X, dir_Y, dir_Z;
int totalTime;
int timeLeft;
int totalX, totalY, totalZ;
int STP_X, STP_Y, STP_Z;
int nowX, nowY, nowZ;
long lastOpenShutter = 0, lastCloseShutter = 0;
int speed = 1;
int max;
long totalTime_us;
long PeriodX, PeriodY, PeriodZ;
long lastX, lastY, lastZ;
int STP_X_10, STP_Y_1, STP_Z_1;
int CountX, CountY, CountZ;

volatile long counter;
long lastClear = 0;
long nowTime;
long lastSecond;
int delta_time;
int changeMenuFlag = 0;
int Frames_taken;
int UpdateRate;
int shutterState = 0;


int main(void)
{
	LCD_INIT(); 						// LCD_INIT 
	GPIOConf();
	MILLIS_Init();
	delay_init();	
	buzzer(2);
	//initialize
	//LCD Print initializeing
	LCD_DrawString(10, 10, "Initializeing ... ...");
	motor0init();
 
	//Change menu to 0
	LCD_Clear_All();	
	currentMenu = 0;	
	cursor = 2;
  while (1) {
		if(currentMenu == 0){
			changeMenuFlag =0;
			DrawMenu(currentMenu);
			LCD_DrawArrow(cursor);
			if(digitalRead(1) == 1){
				LCD_Clear_Arrow(2);
				if(cursor < 5){
					cursor ++;
				}
				else{cursor = 2;}
				DelayAndAbuzz();
			}
			if(digitalRead(0) == 1){
				LCD_Clear_Arrow(2);
				if(cursor > 2){
					cursor --;}
				else{cursor = 5;}
				DelayAndAbuzz();
			}
			if(cursor == 2 && digitalRead(4) == 1 && changeMenuFlag == 0){
				currentMenu = 1;
				cursor = 1;
				TimeLapseFlag =1;
				changeMenuFlag =1;
				DelayAndAbuzz();
				LCD_Clear_All();
			} 	
			if(cursor == 3 && digitalRead(4) == 1 && changeMenuFlag == 0){
				currentMenu = 2;
				cursor = 1;
				VideoFlag = 1;
				changeMenuFlag =1;
				DelayAndAbuzz();
				LCD_Clear_All();
			}
			if(cursor == 4 && digitalRead(4) == 1 && changeMenuFlag == 0){
				currentMenu = 3;
				cursor = 1;
				changeMenuFlag =1;
				DelayAndAbuzz();
				LCD_Clear_All();
			}
			if(cursor == 5 && digitalRead(4) == 1 && changeMenuFlag == 0){
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
				END_X = nowX;
				END_Y = nowY;
				END_Z = nowZ;
				DelayAndAbuzz();
				LCD_Clear_All();
			}
			if(cursor == 2 && digitalRead(4) == 1 && changeMenuFlag == 0){
				currentMenu = 121;
				cursor = 2;
				changeMenuFlag =1;
				DelayAndAbuzz();
				LCD_Clear_All();
			}
			if(cursor == 3 && digitalRead(4) == 1 && changeMenuFlag == 0){
				currentMenu = 0;
				cursor = 2;
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
				nowX = END_X;
				motor(0,0,10,0);
				delay_us(UpdateRate);
				if(UpdateRate > 10){
					UpdateRate = UpdateRate * 0.8;}
			}
			if(cursor == 2 && digitalRead(3) == 1 && END_X < 9999 && END_X < motor0Max){
				END_X++;
				nowX = END_X;
				motor(1,0,10,0);
				delay_us(UpdateRate);
				if(UpdateRate > 10){
					UpdateRate = UpdateRate * 0.8;}
			}
			if(cursor == 3 && digitalRead(2) == 1 && END_Y > 0){
				END_Y--;
				nowY = END_Y;
				motor(0,0,1,1);
				delay_us(UpdateRate);
				if(UpdateRate > 10){
					UpdateRate = UpdateRate * 0.8;}
			}
			if(cursor == 3 && digitalRead(3) == 1 && END_Y < 9999){
				END_Y++;
				nowY = END_Y;
				motor(1,0,1,1);
				delay_us(UpdateRate);
				if(UpdateRate > 10){
					UpdateRate = UpdateRate * 0.8;}
			}
			if(cursor == 4 && digitalRead(2) == 1 && END_Z > 0){
				END_Z--;
				nowZ = END_Z;
				motor(0,0,1,2);
				delay_us(UpdateRate);
				if(UpdateRate > 10){
					UpdateRate = UpdateRate * 0.8;}
			}
			if(cursor == 4 && digitalRead(3) == 1 && END_Z < 9999){
				END_Z++;
				nowZ = END_Z;
				motor(1,0,1,2);
				delay_us(UpdateRate);
				if(UpdateRate > 10){
					UpdateRate = UpdateRate * 0.8;}
			}
			if(cursor == 5 && digitalRead(4) == 1 && changeMenuFlag == 0){
				currentMenu = 112;
				cursor = 2;
				STR_X = nowX;
				STR_Y = nowY;
				STR_Z = nowZ;
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
				
			if(cursor == 2 && digitalRead(2) == 1 && STR_X > 0){
				STR_X--;
				nowX = STR_X;
				motor(0,0,10,0);
				delay_us(UpdateRate);
				if(UpdateRate > 10){
					UpdateRate = UpdateRate * 0.8;}
			}
			if(cursor == 2 && digitalRead(3) == 1 && STR_X <9999 && STR_X < motor0Max){
				STR_X++;
				nowX = STR_X;
				motor(1,0,10,0);
				delay_us(UpdateRate);
				if(UpdateRate > 10){
					UpdateRate = UpdateRate * 0.8;}
			}
			if(cursor == 3 && digitalRead(2) == 1 && STR_Y > 0){
				STR_Y--;
				nowY = STR_Y;
				motor(0,0,1,1);
				delay_us(UpdateRate);
				if(UpdateRate > 10){
					UpdateRate = UpdateRate * 0.8;}
			}
			if(cursor == 3 && digitalRead(3) == 1 && STR_Y <9999){
				STR_Y++;
				nowY = STR_Y;
				motor(1,0,1,1);
				delay_us(UpdateRate);
				if(UpdateRate > 10){
					UpdateRate = UpdateRate * 0.8;}
			}
			if(cursor == 4 && digitalRead(2) == 1 && STR_Z > 0){
				STR_Z--;
				nowZ = STR_Z;
				motor(0,0,1,2);
				delay_us(UpdateRate);
				if(UpdateRate > 10){
					UpdateRate = UpdateRate * 0.8;}
			}
			if(cursor == 4 && digitalRead(3) == 1 && STR_Z <9999){
				STR_Z++;
				nowZ = STR_Z;
				motor(1,0,1,2);
				delay_us(UpdateRate);
				if(UpdateRate > 10){
					UpdateRate = UpdateRate * 0.8;}
			}
			if(cursor == 5 && digitalRead(4) == 1 && TimeLapseFlag == 1 && changeMenuFlag == 0){
				currentMenu = 113;
				cursor = 2;
				changeMenuFlag =1;
				DelayAndAbuzz();
				LCD_Clear_All();
			}
			if(cursor == 5 && digitalRead(4) == 1 && VideoFlag == 1 && changeMenuFlag == 0){
				currentMenu = 223;
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
				totalX = abs(END_X - STR_X);
				STP_X = totalX*10;
				if(END_X > STR_X){
					dir_X = 1;
				}
				else{dir_X = 0;}
				totalY = abs(END_Y - STR_Y);
				STP_Y = totalY*1;
				if(END_Y > STR_Y){
					dir_Y = 1;
				}
				else{dir_Y = 0;}
				totalZ = abs(END_Z - STR_Z);
				STP_Z = totalZ*1;
				if(END_Z > STR_Z){
					dir_Z = 1;
				}
				else{dir_Z = 0;}
				motor(dir_X,0,STP_X,0);
				nowX = END_X;
				motor(dir_Y,0,STP_Y,1);
				nowY = END_Y;
				motor(dir_Z,0,STP_Z,2);				
				nowZ = END_Z;
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
				
			if(cursor == 2 && digitalRead(2) == 1 && fames >0){
				fames--;
				delay_us(UpdateRate);
				if(UpdateRate > 10){
					UpdateRate = UpdateRate * 0.8;}
			}
			if(cursor == 2 && digitalRead(3) == 1 && fames <9999){
				fames++;
				delay_us(UpdateRate);
				if(UpdateRate > 10){
					UpdateRate = UpdateRate * 0.8;}
			}
			if(cursor == 3 && digitalRead(2) == 1 && interval >0){
				interval--;
				delay_us(UpdateRate);
				if(UpdateRate > 10){
					UpdateRate = UpdateRate * 0.8;}
			}
			if(cursor == 3 && digitalRead(3) == 1 && interval < 9999){
				interval++;
				delay_us(UpdateRate);
				if(UpdateRate > 10){
					UpdateRate = UpdateRate * 0.8;}
			}
			if(cursor == 4 && digitalRead(2) == 1 && shutterT >0){
				shutterT--;
				delay_us(UpdateRate);
				if(UpdateRate > 10){
					UpdateRate = UpdateRate * 0.8;}
			}
			if(cursor == 4 && digitalRead(3) == 1 && shutterT < 9999){
				shutterT++;
				delay_us(UpdateRate);
				if(UpdateRate > 10){
					UpdateRate = UpdateRate * 0.8;}
			}
			if(cursor == 5 && digitalRead(4) == 1 && changeMenuFlag == 0){
				currentMenu = 114;
				cursor = 4;
				changeMenuFlag =1;
				DelayAndAbuzz();
				totalTime = fames * (interval + shutterT);
				timeLeft = totalTime;
				totalX = abs(END_X - STR_X);
				STP_X_10 = totalX/fames;
				STP_X = STP_X_10 * 10;
				if(END_X > STR_X){
					dir_X = 1;
				}
				else{dir_X = 0;}
				totalY = abs(END_Y - STR_Y);
				STP_Y_1 = totalY/fames;
				STP_Y = STP_X_10 * 1;
				if(END_Y > STR_Y){
					dir_Y = 1;
				}
				else{dir_Y = 0;}
				totalZ = abs(END_Z - STR_Z);
				STP_Z_1 = totalZ/fames;
				STP_Z = STP_Z_1*1;
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

		if(currentMenu == 121){
			changeMenuFlag =0;
			DrawMenu(currentMenu);
			LCD_DrawArrow(cursor);
			if(digitalRead(1) == 1){
				LCD_Clear_Arrow(2);
				if(cursor < 8){
					cursor += 2;}
				else if(cursor == 8){
					cursor ++;}
				else{
					cursor = 2;}
				DelayAndAbuzz();
			}
			if(digitalRead(0) == 1){
				LCD_Clear_Arrow(2);
				if(cursor == 2){
					cursor = 9;}
				else if(cursor < 9){
					cursor -= 2;}
				else{
					cursor --;}
				DelayAndAbuzz();
			}	
			if(digitalRead(0) == 0 && digitalRead(1) == 0 && digitalRead(2) == 0 && digitalRead(3) == 0 && digitalRead(4) == 0)
				{
					UpdateRate = 500000;
				}
				
			if(cursor == 2 && digitalRead(2) == 1 && STR_X > 0){
				STR_X--;
				delay_us(UpdateRate);
				if(UpdateRate > 10){
					UpdateRate = UpdateRate * 0.8;}
			}
			if(cursor == 2 && digitalRead(3) == 1 && STR_X <9999 && STR_X < motor0Max){
				STR_X++;
				delay_us(UpdateRate);
				if(UpdateRate > 10){
					UpdateRate = UpdateRate * 0.8;}
			}
			if(cursor == 4 && digitalRead(2) == 1 && STR_Y > 0){
				STR_Y--;
				delay_us(UpdateRate);
				if(UpdateRate > 10){
					UpdateRate = UpdateRate * 0.8;}
			}
			if(cursor == 4 && digitalRead(3) == 1 && STR_Y <9999 && STR_X < motor0Max){
				STR_Y++;
				delay_us(UpdateRate);
				if(UpdateRate > 10){
					UpdateRate = UpdateRate * 0.8;}
			}
			if(cursor == 6 && digitalRead(2) == 1 && STR_Z > 0){
				STR_Z--;
				delay_us(UpdateRate);
				if(UpdateRate > 10){
					UpdateRate = UpdateRate * 0.8;}
			}
			if(cursor == 6 && digitalRead(3) == 1 && STR_Z <9999){
				STR_Z++;
				delay_us(UpdateRate);
				if(UpdateRate > 10){
					UpdateRate = UpdateRate * 0.8;}
			}
			if(cursor == 8 && digitalRead(4) == 1 && changeMenuFlag == 0){
				currentMenu = 122;
				cursor = 2;
				changeMenuFlag =1;
				DelayAndAbuzz();
				LCD_Clear_All();
			}
			if(cursor == 9 && digitalRead(4) == 1 && changeMenuFlag == 0){
				currentMenu = 1;
				cursor = 1;
				changeMenuFlag =1;
				DelayAndAbuzz();
				LCD_Clear_All();
			}
		}
		if(currentMenu == 122){
			changeMenuFlag =0;
			DrawMenu(currentMenu);
			LCD_DrawArrow(cursor);
			if(digitalRead(1) == 1){
				LCD_Clear_Arrow(2);
				if(cursor < 8){
					cursor += 2;}
				else if(cursor == 8){
					cursor ++;}
				else{
					cursor = 2;}
				DelayAndAbuzz();
			}
			if(digitalRead(0) == 1){
				LCD_Clear_Arrow(2);
				if(cursor == 2){
					cursor = 9;}
				else if(cursor < 9){
					cursor -= 2;}
				else{
					cursor --;}
				DelayAndAbuzz();
			}	
			if(digitalRead(0) == 0 && digitalRead(1) == 0 && digitalRead(2) == 0 && digitalRead(3) == 0 && digitalRead(4) == 0)
				{
					UpdateRate = 500000;
				}
				
			if(cursor == 2 && digitalRead(2) == 1 && END_X > 0){
				END_X--;
				delay_us(UpdateRate);
				if(UpdateRate > 10){
					UpdateRate = UpdateRate * 0.8;}
			}
			if(cursor == 2 && digitalRead(3) == 1 && END_X <9999 && END_X < motor0Max){
				END_X++;
				delay_us(UpdateRate);
				if(UpdateRate > 10){
					UpdateRate = UpdateRate * 0.8;}
			}
			if(cursor == 4 && digitalRead(2) == 1 && END_Y > 0){
				END_Y--;
				delay_us(UpdateRate);
				if(UpdateRate > 10){
					UpdateRate = UpdateRate * 0.8;}
			}
			if(cursor == 4 && digitalRead(3) == 1 && END_Y <9999){
				END_Y++;
				delay_us(UpdateRate);
				if(UpdateRate > 10){
					UpdateRate = UpdateRate * 0.8;}
			}
			if(cursor == 6 && digitalRead(2) == 1 && END_Z > 0){
				END_Z--;
				delay_us(UpdateRate);
				if(UpdateRate > 10){
					UpdateRate = UpdateRate * 0.8;}
			}
			if(cursor == 6 && digitalRead(3) == 1 && END_Z <9999){
				END_Z++;
				delay_us(UpdateRate);
				if(UpdateRate > 10){
					UpdateRate = UpdateRate * 0.8;}
			}
			if(cursor == 8 && digitalRead(4) == 1 && TimeLapseFlag == 1 && changeMenuFlag == 0){
				currentMenu = 123;
				cursor = 2;
				changeMenuFlag =1;
				DelayAndAbuzz();
				LCD_Clear_All();
				//slew to start position
				LCD_DrawString(10, 10, "Slewing to start position");
				totalX = abs(nowX - STR_X);
				STP_X  = totalX * 10;
				if(nowX > STR_X){
					dir_X = 1;
				}
				else{dir_X = 0;}
				totalY = abs(nowY - STR_Y);
				STP_Y  = totalY * 1;
				if(nowY > STR_Y){
					dir_Y = 1;
				}
				else{dir_Y = 0;}
				totalZ = abs(nowZ - STR_Z);
				STP_Z  = totalZ * 1;
				if(nowZ > STR_Z){
					dir_Z = 1;
				}
				else{dir_Z = 0;}
				motor(dir_X,0,STP_X,0);
				motor(dir_Y,0,STP_Y,1);
				motor(dir_Z,0,STP_Z,2);
				nowX = STR_X;
				nowY = STR_Y;
				nowZ = STR_Z;
				DelayAndAbuzz();
				LCD_Clear_All();
			}
			if(cursor == 8 && digitalRead(4) == 1 && VideoFlag == 1 && changeMenuFlag == 0){
				currentMenu = 223;
				cursor = 2;
				changeMenuFlag =1;
				DelayAndAbuzz();
				LCD_Clear_All();
				//slew to start position
				LCD_DrawString(10, 10, "Slewing to start position");
				totalX = abs(nowX - STR_X);
				STP_X  = totalX * 10;
				if(nowX > STR_X){
					dir_X = 1;
				}
				else{dir_X = 0;}
				totalY = abs(nowY - STR_Y);
				STP_Y  = totalY * 1;
				if(nowY > STR_Y){
					dir_Y = 1;
				}
				else{dir_Y = 0;}
				totalZ = abs(nowZ - STR_Z);
				STP_Z  = totalZ * 1;
				if(nowZ > STR_Z){
					dir_Z = 1;
				}
				else{dir_Z = 0;}
				motor(dir_X,0,STP_X,0);
				motor(dir_Y,0,STP_Y,1);
				motor(dir_Z,0,STP_Z,2);
				nowX = STR_X;
				nowY = STR_Y;
				nowZ = STR_Z;
				DelayAndAbuzz();
				LCD_Clear_All();
			}
			if(cursor == 9 && digitalRead(4) == 1 && changeMenuFlag == 0){
				currentMenu = 122;
				cursor = 2;
				changeMenuFlag =1;
				DelayAndAbuzz();
				LCD_Clear_All();
			}
		}
		if(currentMenu == 123){
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
				
			if(cursor == 2 && digitalRead(2) == 1 && fames >0){
				fames--;
				delay_us(UpdateRate);
				if(UpdateRate > 10){
					UpdateRate = UpdateRate * 0.8;}
			}
			if(cursor == 2 && digitalRead(3) == 1 && fames <9999){
				fames++;
				delay_us(UpdateRate);
				if(UpdateRate > 10){
					UpdateRate = UpdateRate * 0.8;}
			}
			if(cursor == 3 && digitalRead(2) == 1 && interval >0){
				interval--;
				delay_us(UpdateRate);
				if(UpdateRate > 10){
					UpdateRate = UpdateRate * 0.8;}
			}
			if(cursor == 3 && digitalRead(3) == 1 && interval < 9999){
				interval++;
				delay_us(UpdateRate);
				if(UpdateRate > 10){
					UpdateRate = UpdateRate * 0.8;}
			}
			if(cursor == 4 && digitalRead(2) == 1 && shutterT >0){
				shutterT--;
				delay_us(UpdateRate);
				if(UpdateRate > 10){
					UpdateRate = UpdateRate * 0.8;}
			}
			if(cursor == 4 && digitalRead(3) == 1 && shutterT < 9999){
				shutterT++;
				delay_us(UpdateRate);
				if(UpdateRate > 10){
					UpdateRate = UpdateRate * 0.8;}
			}
			if(cursor == 5 && digitalRead(4) == 1 && changeMenuFlag == 0){
				currentMenu = 114;
				cursor = 4;
				changeMenuFlag =1;
				DelayAndAbuzz();
				totalTime = fames * (interval + shutterT);
				timeLeft = totalTime;
				
				//calculate the movenent per interval
				totalX = abs(END_X - STR_X);
				STP_X_10 = totalX/fames;
				STP_X = STP_X_10 * 10;
				if(END_X > STR_X){
					dir_X = 1;
				}
				else{dir_X = 0;}
				totalY = abs(END_Y - STR_Y);
				STP_Y_1 = totalY/fames;
				STP_Y = STP_Y_1 * 1;
				if(END_Y > STR_Y){
					dir_Y = 1;
				}
				else{dir_Y = 0;}
				totalZ = abs(END_Z - STR_Z);
				STP_Z_1 = totalZ/fames;
				STP_Z = STP_Z_1 * 1;
				if(END_Z > STR_Z){
					dir_Z = 1;
				}
				else{dir_Z = 0;}
				lastSecond = millis();
				LCD_Clear_All();
			}
			if(cursor == 6 && digitalRead(4) == 1 && changeMenuFlag == 0){
				currentMenu = 122;
				cursor = 1;
				changeMenuFlag =1;
				DelayAndAbuzz();
				LCD_Clear_All();
			}
		}

		if(currentMenu == 114){
			changeMenuFlag =0;
			
			nowTime = millis();
			if(nowTime - lastSecond > 1000){
				//delta_time = (nowTime - lastSecond) / 1000;
				//timeLeft -= delta_time;
				timeLeft --;
				lastSecond = millis();
			}
			DrawMenu(currentMenu);
			LCD_DrawArrow(cursor);
			if(digitalRead(1) == 1){
				LCD_Clear_Arrow(3);
				if(cursor < 5){
					cursor ++;}
				else{cursor = 4;}
				DelayAndAbuzz();
			}
			if(digitalRead(0) == 1){
				LCD_Clear_Arrow(3);
				if(cursor > 4){
					cursor --;}
				else{cursor = 5;}
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
				cursor = 2;
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
				if(dir_X == 1){
					nowX = nowX + STP_X_10;}
				else {nowX = nowX - STP_X_10;}
				motor(dir_Y,0,STP_Y,1);
				if(dir_Y == 1){
					nowY = nowY + STP_Y_1;}
				else {nowY = nowY - STP_Y_1;}
				motor(dir_Z,0,STP_Z,2);
				if(dir_Z == 1){
					nowZ = nowZ + STP_Z_1;}
				else {nowZ = nowZ - STP_Z_1;}
			}
			if(Frames_taken == fames){
				buzzer(5);
				currentMenu = 0;
				cursor = 2;
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
				cursor = 2;
				TimeLapseFlag = 0;
				timeLeft = 0;
				Frames_taken = 0;
				changeMenuFlag =1;
				DelayAndAbuzz();
				LCD_Clear_All();
			}		
		}
		
	  if(currentMenu == 2){
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
				END_X = nowX;
				END_Y = nowY;
				END_Z = nowZ;
				
			}
			if(cursor == 2 && digitalRead(4) == 1 && changeMenuFlag == 0){
				currentMenu = 121;
				cursor = 2;
				changeMenuFlag =1;
				DelayAndAbuzz();
				LCD_Clear_All();
			}
			if(cursor == 3 && digitalRead(4) == 1 && changeMenuFlag == 0){
				currentMenu = 0;
				cursor = 2;
				VideoFlag = 0;
				changeMenuFlag =1;
				DelayAndAbuzz();
				LCD_Clear_All();
			}
		}
		if(currentMenu == 223){
			changeMenuFlag =0;
			DrawMenu(currentMenu);
			LCD_DrawArrow(cursor);
			if(digitalRead(1) == 1){
				LCD_Clear_Arrow(2);
				if(cursor < 4){
					cursor ++;}
				else{cursor = 2;}
				DelayAndAbuzz();
			}
			if(digitalRead(0) == 1){
				LCD_Clear_Arrow(2);
				if(cursor > 2){
					cursor --;}
				else{cursor = 4;}
				DelayAndAbuzz();
			}		
			if(cursor == 2 && digitalRead(2) == 1 && speed >1){
				speed--;
				DelayAndAbuzz();
			}
			if(cursor == 2 && digitalRead(3) == 1 && speed <5	){
				speed++;
				DelayAndAbuzz();
			}
			if(cursor == 3 && digitalRead(4) == 1 && changeMenuFlag == 0){
				currentMenu = 224;
				cursor = 3;
				changeMenuFlag =1;
				DelayAndAbuzz();
				totalX = abs(END_X - STR_X)* 10;
				if(END_X > STR_X){
					dir_X = 1;
				}
				else{dir_X = 0;}
				totalY = abs(END_Y - STR_Y)* 1;
				if(END_Y > STR_Y){
					dir_Y = 1;
				}
				else{dir_Y = 0;}
				totalZ = abs(END_Z - STR_Z)* 1;
				if(END_Z > STR_Z){
					dir_Z = 1;
				}
				else{dir_Z = 0;}
				if (totalX > totalY)
				{
					if(totalX > totalZ)
					{max = totalX;}
					else{max = totalZ;}
				}
				else{
					if(totalY > totalZ)
					{max = totalY;}
					else{max = totalZ;}
				}
				totalTime = max / StpPSec(speed);
				timeLeft = totalTime;
				//totalTime_us = max * 1000000 / StpPSec(speed);
				PeriodX = (max  * 2000 / StpPSec(speed)) / totalX ;
				PeriodY = (max  * 2000 / StpPSec(speed)) / totalY ;
				PeriodZ = (max  * 2000 / StpPSec(speed)) / totalZ ;
				lastSecond = millis();
				LCD_Clear_All();
				toggleVideo();
			}
			if(cursor == 4 && digitalRead(4) == 1 && changeMenuFlag == 0){
				currentMenu = 112;
				cursor = 2;
				changeMenuFlag =1;
				DelayAndAbuzz();
				LCD_Clear_All();
			}
		
		}
		if(currentMenu == 224){
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
				currentMenu = 2242;
				cursor = 3;
				changeMenuFlag =1;
				DelayAndAbuzz();
				LCD_Clear_All();
			}
			if(cursor == 4 && digitalRead(4) == 1 && changeMenuFlag == 0){
				currentMenu = 0;
				cursor = 2;
				VideoFlag = 0;
				timeLeft = 0;
				changeMenuFlag =1;
				DelayAndAbuzz();
				LCD_Clear_All();
			}
			nowTime = millis();
			if(((nowTime - lastX))>(PeriodX) && nowX != END_X){
				motor(dir_X,0,2,0);
				lastX = millis();
				CountX ++;
			}
			if(CountX == 80){
				if(dir_X == 1){
					nowX ++;}
				else{
					nowX --;}
				CountX = 0;
			}
			if(((nowTime - lastY))>(PeriodY)&& nowY != END_Y){
				motor(dir_Y,0,2,1);
				lastY = millis();
				CountY ++;
			}
			if(CountY == 80){
				if(dir_Y == 1){
					nowY ++;}
				else{
					nowY --;}
				CountY = 0;
			}
			if(((nowTime - lastZ))>(PeriodZ)&& nowZ != END_Z){
				motor(dir_Z,0,2,2);
				lastZ = millis();
				CountZ ++;
			}
			if(CountZ == 80){
				if(dir_Z == 1){
					nowZ ++;}
				else{
					nowZ --;}
				CountZ = 0;
			}
			if(nowX == END_X && nowY == END_Y && nowZ == END_Z ){
				toggleVideo();
				buzzer(5);
				currentMenu = 0;
				cursor = 2;
				VideoFlag = 0;
				timeLeft = 0;
				Frames_taken = 0;
				changeMenuFlag =1;
				DelayAndAbuzz();
				LCD_Clear_All();}
		}
		if(currentMenu == 2242){
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
				if(shutterState ==1){
					shutterNow();
					shutterState = 1;}
				currentMenu = 224;
				cursor = 3;
				changeMenuFlag =1;
				DelayAndAbuzz();
				LCD_Clear_All();
			}
			if(cursor == 4 && digitalRead(4) == 1 && changeMenuFlag == 0){
				currentMenu = 0;
				cursor = 2;
				VideoFlag = 0;
				timeLeft = 0;
				changeMenuFlag =1;
				DelayAndAbuzz();
				LCD_Clear_All();
			}
		}
		
		if(currentMenu == 3){
			changeMenuFlag =0;
			DrawMenu(currentMenu);
			LCD_DrawArrow(cursor);
			if(digitalRead(1) == 1){
				LCD_Clear_Arrow(1);
				if(cursor < 4){
					cursor ++;}
				else{cursor = 1;}
				DelayAndAbuzz();
			}
			if(digitalRead(0) == 1){
				LCD_Clear_Arrow(1);
				if(cursor > 1){
					cursor --;}
				else{cursor = 4;}
				DelayAndAbuzz();
			}
			if(digitalRead(0) == 0 && digitalRead(1) == 0 && digitalRead(2) == 0 && digitalRead(3) == 0 && digitalRead(4) == 0)
				{
					UpdateRate = 500000;
				}
			if(cursor == 1 && digitalRead(2) == 1 && nowX > 0){
				nowX--;
				motor(0,0,10,0);
				delay_us(UpdateRate);
				if(UpdateRate > 10){
					UpdateRate = UpdateRate * 0.8;}
			}
			if(cursor == 1 && digitalRead(3) == 1 && nowX < 9999 && nowX < motor0Max){
				nowX++;
				motor(1,0,10,0);
				delay_us(UpdateRate);
				if(UpdateRate > 10){
					UpdateRate = UpdateRate * 0.8;}
			}
			if(cursor == 2 && digitalRead(2) == 1 && nowY > 0){
				nowY--;
				motor(0,0,1,1);
				delay_us(UpdateRate);
				if(UpdateRate > 10){
					UpdateRate = UpdateRate * 0.8;}
			}
			if(cursor == 2 && digitalRead(3) == 1 && nowY < 9999){
				nowY++;
				motor(1,0,1,1);
				delay_us(UpdateRate);
				if(UpdateRate > 10){
					UpdateRate = UpdateRate * 0.8;}
			}
			if(cursor == 3 && digitalRead(2) == 1 && nowZ > 0){
				nowZ--;
				motor(0,0,1,2);
				delay_us(UpdateRate);
				if(UpdateRate > 10){
					UpdateRate = UpdateRate * 0.8;}
			}
			if(cursor == 3 && digitalRead(3) == 1 && nowZ < 9999){
				nowZ++;
				motor(1,0,1,2);
				delay_us(UpdateRate);
				if(UpdateRate > 10){
					UpdateRate = UpdateRate * 0.8;}
			}
			if(cursor == 4 && digitalRead(4) == 1 && changeMenuFlag == 0){
				currentMenu = 0;
				cursor = 2;
				changeMenuFlag =1;
				DelayAndAbuzz();
				LCD_Clear_All();}
		}
		if(currentMenu == 4){
			changeMenuFlag =0;
			DrawMenu(currentMenu);
			LCD_DrawArrow(cursor);
			if(digitalRead(1) == 1){
				LCD_Clear_Arrow(1);
				if(cursor < 2){
					cursor ++;}
				else{cursor = 1;}
				DelayAndAbuzz();
			}
			if(digitalRead(0) == 1){
				LCD_Clear_Arrow(1);
				if(cursor > 1){
					cursor --;}
				else{cursor = 2;}
				DelayAndAbuzz();
			}	
			if(cursor == 2&& digitalRead(4) == 1 && changeMenuFlag == 0){
				LCD_Clear_All();
				LCD_DrawString(10, 10, "Initializeing ... ...");
				motor0init();
				buzzer(2);
				LCD_Clear_All();
			}
			if(cursor == 2&& digitalRead(4) == 1 && changeMenuFlag == 0){
				currentMenu = 0;
				cursor = 2;
				changeMenuFlag =1;
				DelayAndAbuzz();
				LCD_Clear_All();
			}
		}
	}
}


/*
int main(void){
	LCD_INIT(); 						// LCD_INIT 
	GPIOConf();
	MILLIS_Init();
	delay_init();
	buzzer(2);
	while (1) {
		toggleVideo();
		delay_ms(10000);
	
  }
}
*/


void Delayus(int duration){
		while(duration--) 
		{
			int i=0x02;				
			while(i--)
			__asm("nop");
		}
}



void GPIOConf(void){		
	/* Configure the folowing pin as output(A 2 B 8 12 13 14 15 C 6 7) */
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIOB->BRR = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIOC->BRR = GPIO_Pin_6 | GPIO_Pin_7;
	//Configure the folowing pin as intput(B 0 A 4 5 6 7)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 |GPIO_Pin_7;
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
			if (H_L == 0){ GPIOC->BRR=GPIO_Pin_6;}
			else{GPIOC->BSRR=GPIO_Pin_6;}
		break;			
		case 2 : 
			if (H_L == 0){ GPIOB->BRR=GPIO_Pin_14;}
			else{GPIOB->BSRR=GPIO_Pin_14;}
		break;
		case 3 : 
			if (H_L == 0){ GPIOB->BRR=GPIO_Pin_15;}
			else{GPIOB->BSRR=GPIO_Pin_15;}
		break;
		case 4 : 
			if (H_L == 0){ GPIOB->BRR=GPIO_Pin_12;}
			else{GPIOB->BSRR=GPIO_Pin_12;}
		break;
		case 5 : 
			if (H_L == 0){ GPIOC->BRR=GPIO_Pin_7;}
			else{GPIOC->BSRR=GPIO_Pin_7;}
		break;
		case 6 : 
			if (H_L == 0){ GPIOB->BRR=GPIO_Pin_13;}
			else{GPIOB->BSRR=GPIO_Pin_13;}
		break;
		case 7 : 
			if (H_L == 0){ GPIOB->BRR=GPIO_Pin_8;}
			else{GPIOB->BSRR=GPIO_Pin_8;}
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
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)){r = 0;}
			else{r = 1;}
		break;
		case 2 :
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)){r = 0;}
			else{r = 1;}
		break;
		case 3 :
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)){r = 0;}
			else{r = 1;}
		break;
		case 4 :
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)){r = 0;}
			else{r = 1;}
		break;
		/*
		case 5 :
			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)){r = 0;}
			else{r = 1;}
		break;
		case 6 :
			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)){r = 0;}
			else{r = 1;}
		break;
		*/			
	}
	return r;
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
		digitalWrite(4,1);//A5
		digitalWrite(6,1);//A7
	}
	else{
		digitalWrite(2,0);
		digitalWrite(4,0);
		digitalWrite(6,0);}
	for(i = 0; i < step; i++){
		Step(motor_no);
		delay_us(delay_t);
	}
	
}
void Step(int motor_no){
	switch(motor_no){
		case 0 :
			digitalWrite(1,1); //A3
			delay_us(20);
			digitalWrite(1,0);
			delay_us(20);
		break;
		case 1 :
			digitalWrite(3,1); //A6
			delay_us(20);
			digitalWrite(3,0);
			delay_us(20);
		break;
		case 2 :
			digitalWrite(5,1); //A8
			delay_us(20);
			digitalWrite(5,0);
			delay_us(20);
		break;
		
	}
}
void buzzer(int times){
	int i =0;
	while(i < times){
		delay_us(100000);
		digitalWrite(0,1);//A2
		delay_us(100000);
		digitalWrite(0,0);
		i++;
	}
}	
void shutterNow(void){
	int _pin = 7;
	int _seq[] = {1,0,1,1,0,1,0,0,1,0,1,1,1,0,0,0,1,1,1,1};
	int i, j;
  for (j=0;j<3;j++) {
    high(2320,40,_pin);
    delay_us(650);
    for (i=0;i<20;i++){
      if (_seq[i]==0){
        high(575,40,_pin);
        delay_us(650);
      }
      else{
        high(1175,40,_pin);
        delay_us(650);
      }
    }
    delay_us(10000);
  }
}
void toggleVideo(void){
  int _pin = 7;
	int _seqToggle[] = {0,0,0,1,0,0,1,0,1,0,1,1,1,0,0,0,1,1,1,1};
	int i, j;
  for (j=0;j<3;j++) {
    high(2320,40,_pin);
    delay_us(650);
    for (i=0;i<20;i++){
      if (_seqToggle[i]==0){
        high(575,40,_pin);
        delay_us(650);
      }
      else{
        high(1175,40,_pin);
        delay_us(650);
      }
    }
    delay_us(10000);
  }
}
void high(unsigned int time, int freq, int pinLED){
  unsigned int pause, i;	
  //unsigned long start = micros();
  //while(micros()-start<=time){
	pause = (1000/freq/2)-4;
	i = time / (pause * 2 + 1);
	while(i > 0){
    digitalWrite(pinLED,1);
    delay_us(pause);
    digitalWrite(pinLED,0);
    delay_us(pause);
		i --;
  }
}
void DelayAndAbuzz(void){
	buzzer(1);
	delay_us(500000);
}

int StpPSec(int spd){
	int x;
	switch (spd) {
		case 1 :
			x = 400;
		break;
		case 2 :
			x = 200;
		break;
		case 3 :
			x = 100;
		break;
		case 4 :
			x = 50;
		break;
		case 5 :
			x = 25;
		break;
	}
	return x;
}

void motor0init(void){
	while(digitalRead(0) == 0){
		motor(0,0,1,0);
		delay_us(1000);
	}		
	motor(1,0,50,0);
	//Slew the slider to max
	motor0Max = 0;
	while(digitalRead(1) == 0){
		motor(1,0,1,0);
		delay_us(1000);
		motor0Max ++;
	}
	motor(0,0,50,0);
	motor0Max -= 50;
	nowX = motor0Max / 10;
	motor(0,0,motor0Max / 2 ,0);
	nowX = nowX /2;
}

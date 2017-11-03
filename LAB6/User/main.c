#include "stm32f10x.h"
#include "lcd.h"
#include "i2c_GY80.h"
#include "math.h"

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


/* Private function prototypes -----------------------------------------------*/
void HMC_Init(void);
void Delayus(int duration);



int main(void)
{

  LCD_INIT(); 						// LCD_INIT
  I2C_GY80_Init();        // I2C Init
  HMC_Init();             // HMC Init
 
  while (1) {
	/* Please add code below to complete the LAB6 */
  /* You might want to create your own functions */	
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

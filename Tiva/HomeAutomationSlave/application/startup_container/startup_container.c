/*--------------------Type Includes------------------*/
#include "stdbool.h"
#include "stdint.h"
/*-------------------HW define Includes--------------*/
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
  
#include "driverlib/interrupt.h"
#include "startup_container.h"
#include "timer_handler.h"
#include "adc_handler.h"
#include "i2c_handler.h"
#include "gpio_handler.h"  
#include "pwm_handler.h"  
#include "spi_handler.h"
#include "driverlib/gpio.h" 
#include "driverlib/timer.h"
#include "display.h"
  
void Init_Drivers(void)
{
	
	I2C_Init(I2C0_BASE,1);
  I2C_Init_LuminositySensor(0x39);
	I2C_Init(I2C1_BASE,0);
	
	Add_ADC_Channel(0);//channel 0
	Add_ADC_Channel(1);//channel 0
	ADC_Init();
	
	
	SetGPIOInterrupt(GPIO_PORTF_BASE,GPIO_PIN_4,GPIO_RISING_EDGE);
	//debouncing pf4
	Init_Timer(TIMER4_BASE, TIMER_A,1000);
	
	SetGPIOInput(GPIO_PORTF_BASE,GPIO_PIN_0,1);
	
		//Cyclic 50 ms
  Init_Timer(TIMER0_BASE, TIMER_A,50); 
	TimerEnable(TIMER0_BASE, TIMER_A);
	
	//Cyclic 100 ms
  Init_Timer(TIMER1_BASE, TIMER_A,100);  
	TimerEnable(TIMER1_BASE, TIMER_A); 	
	
	//Cyclic 500 ms
  Init_Timer(TIMER2_BASE, TIMER_A,500);  
	TimerEnable(TIMER2_BASE, TIMER_A); 
	
	//Cyclic 1000 ms
	Init_Timer(TIMER3_BASE, TIMER_A,1000);  
	TimerEnable(TIMER3_BASE, TIMER_A); 
	
	Init_PWM(GPIO_PORTF_BASE,GPIO_PIN_2, Clock_Ticks(20));
	Duty_Cycle(GPIO_PORTF_BASE,GPIO_PIN_2, 925);
	

	SSI0_InitSlave(); //nu merge bine MISO daca I2C si Timerele se initializeaza
	
	Display_Init();
	
	Display_String("Hello");
	
	IntMasterEnable();
}

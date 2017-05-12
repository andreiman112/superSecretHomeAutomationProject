/*--------------------Project Includes------------------*/
#include "heat_app.h"
#include "pwm_handler.h"
#include "gpio_handler.h"
#include "adc_handler.h"
#include "custom_types.h"
/*-------------------Driverlib Includes-----------------*/
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "inc/hw_memmap.h"
extern uint8_t SlaveResults[256];

void Vent1(uint8_t i){ Duty_Cycle(GPIO_PORTB_BASE,GPIO_PIN_6, i*10); }
void Vent2(uint8_t i){ Duty_Cycle(GPIO_PORTB_BASE,GPIO_PIN_7, i*10); }

void RedPWM(unsigned long duty_cycle){ Duty_Cycle(GPIO_PORTF_BASE,GPIO_PIN_1, duty_cycle*10);}
void GreenPWM(unsigned long duty_cycle){ Duty_Cycle(GPIO_PORTF_BASE,GPIO_PIN_2, duty_cycle*10);}
void BluePWM(unsigned long duty_cycle){ Duty_Cycle(GPIO_PORTF_BASE,GPIO_PIN_3, duty_cycle*10);}


void Init_RGB_Temp()
{
	uint8_t temp1, temp2, temp3;
	
	temp1=SlaveResults[T1Result];
	temp2=SlaveResults[T2Result];
	temp3=SlaveResults[T3Result];
	
	SetT1(temp1);
	SetT2(temp2);
	SetT3(temp3);
}

void SetT1(uint8_t value){
	if(value >= 20 && value <= 22)
		RedPWM(300);
	if(value <20)
		RedPWM(10);
	if(value > 22)
		RedPWM(1000);

	
}
void SetT2(uint8_t value){
	if(value >= 20 && value <= 22)
		GreenPWM(300);
	if(value <20)
		GreenPWM(10);
	if(value > 22)
		GreenPWM(1000);
}
void SetT3(uint8_t value){
	if(value >= 20 && value <= 22)
		BluePWM(300);
	if(value <20)
		BluePWM(10);
	if(value > 22)
		BluePWM(1000);
}

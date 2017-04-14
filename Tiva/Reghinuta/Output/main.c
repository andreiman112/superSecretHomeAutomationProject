/*--------------------Type Includes------------------*/
#include "stdbool.h"
#include "stdint.h"
/*------------------Project Includes-----------------*/

#include "gpio_handler.h"
#include "pwm_handler.h"
#include "ssi_handler.h"
#include "rgb_st.h"

/*-------------------Driver Includes-----------------*/
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "inc/hw_memmap.h"

#include "timer.h"

#include "display.h"

#define RED (0x02)	//PF1
#define BLUE (0x04)	//PF2
#define GREEN (0x08)  //PF3
#define Servo_DutyCycle_Step (1)

#ifdef __TI_COMPILER_VERSION__
	//Code Composer Studio Code
	void Delay(uint32_t ulCount){
	__asm (	"loop    subs    r0, #1\n"
			"    bne     loop\n");
}

#else
	//Keil uVision Code
	__asm void
	Delay(uint32_t ulCount)
	{
    subs    r0, #1
    bne     Delay
    bx      lr
	}
#endif

void TIMER1A_Handler(void)  //Timer 1 A ISR used to debounce SW2
{
	unsigned long switch_value = 0;
	unsigned long duty_cycle = 950;
	//Display_String("Timer 1 ISR launched -> ");
	if(TimerIntStatus(TIMER1_BASE,false))
	{
		TimerDisable(TIMER1_BASE, TIMER_A);
		TimerIntClear(TIMER1_BASE, TIMER_A);
		switch_value = GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0);
		if(!(switch_value & GPIO_PIN_0)) {
		 			 Display_String("->");
			Duty_Cycle(GPIO_PORTF_BASE,GPIO_PIN_2,duty_cycle);
		}
		//if(duty_cycle<=(1000-Servo_DutyCycle_Step)) duty_cycle += Servo_DutyCycle_Step;
		GPIOIntEnable(GPIO_PORTF_BASE, GPIO_INT_PIN_0);  //Enable GPIO pin interrupt		
	}
}

void TIMER2A_Handler(void)  //Timer 2 A ISR used to debounce SW1
{
	unsigned long switch_value = 0;
	static unsigned long duty_cycle = 900;
	if(TimerIntStatus(TIMER2_BASE,false))
	{
		TimerDisable(TIMER2_BASE, TIMER_A);
		TimerIntClear(TIMER2_BASE, TIMER_A);
		switch_value = GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4);
		if(!(switch_value & GPIO_PIN_4)) {
			 Display_String("<-");
			Duty_Cycle(GPIO_PORTF_BASE,GPIO_PIN_2,duty_cycle);
		}
		//if(duty_cycle>=Servo_DutyCycle_Step) duty_cycle -= Servo_DutyCycle_Step;
		GPIOIntEnable(GPIO_PORTF_BASE, GPIO_INT_PIN_0);  //Re-Enable GPIO pin interrupt		
	}
}
void GPIOF_Handler(void) 	//GPIO port F ISR
{
	unsigned long switch_status = 0;
	//static int pwm_dc=0;

	switch_status = GPIOIntStatus(GPIO_PORTF_BASE,false);
	GPIOIntClear(GPIO_PORTF_BASE,GPIO_INT_PIN_0 | GPIO_INT_PIN_4);
	if((switch_status&(GPIO_INT_PIN_0 | GPIO_INT_PIN_4)) == (GPIO_INT_PIN_0 | GPIO_INT_PIN_4))
	{
		//Both switches are pressed
	}
	else if(switch_status & GPIO_INT_PIN_4)
	{
		//PF4
		TimerEnable(TIMER2_BASE, TIMER_A);
		GPIOIntDisable(GPIO_PORTF_BASE,GPIO_PIN_0);  //Disable GPIO pin interrupt	
	}
	else if(switch_status & GPIO_INT_PIN_0)
	{
		//PF0
		TimerEnable(TIMER1_BASE, TIMER_A);
		GPIOIntDisable(GPIO_PORTF_BASE,GPIO_PIN_0);  //Disable GPIO pin interrupt
	}
}
int main(void)
{

	unsigned long ui32SysClock;
	tstRGB LedColor = {0x00,0x00,0x00};
	
	//unsigned long duty_cycle = 3;
	//unsigned long duty_cycle_rgb=0;
	
	SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN); //80 Mhz
	ui32SysClock = SysCtlClockGet();
   
	Display_Init();
	Display_NewLine();
	Display_String("heeloo "); 
	
	/*
	Init_PWM(GPIO_PORTF_BASE,GPIO_PIN_2, Clock_Ticks(20));
	Duty_Cycle(GPIO_PORTF_BASE,GPIO_PIN_2, 925);
	
	SetGPIOInput(GPIO_PORTC_BASE,GPIO_PIN_7);
	 SetGPIOInterrupt(GPIO_PORTF_BASE,GPIO_PIN_4,GPIO_FALLING_EDGE);
	TIMER_2_Init();
	TIMER_1_Init();
 
	SetGPIOInterrupt(GPIO_PORTF_BASE,GPIO_PIN_0,GPIO_FALLING_EDGE);
	
*/


	SSI0_Init();
	SET_GREEN
	
	while(ui32SysClock)  //Clock working
	{
		Set_Point_Color(LedColor, 0, LedColor, 4);
		//Set_Moving_Point(LedColor);
		Send_RGB_Data();
	  Delay(10000000); 

		//duty_cycle_rgb = (duty_cycle_rgb+1) % 99; //rgb brightness
		//Duty_Cycle(GPIO_PORTF_BASE,GPIO_PIN_2, duty_cycle_rgb );
		
		//if(!ReadPinState(PORT_C,GPIO_PIN_7)){ Display_NewLine();}
		
		//example rgb blink
			/*RGB_Led_Blink(GPIO_PORTF_BASE,GPIO_PIN_1);
			RGB_Led_Blink(GPIO_PORTF_BASE,GPIO_PIN_2);
			RGB_Led_Blink(GPIO_PORTF_BASE,GPIO_PIN_3);*/

	}
	return 0;
}
//EOF

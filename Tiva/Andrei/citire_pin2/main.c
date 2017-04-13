/*--------------------Type Includes------------------*/
#include "stdbool.h"
#include "stdint.h"

/*-------------------Driver Includes-----------------*/
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "driverlib/adc.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "pwm_handler.h"

/*-------------------HW define Includes--------------*/
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"

#include "driverlib/timer.h"

#include "display.h"
/*------------------Project Includes-----------------*/
#include "gpio_handler.h"
#include "adc_handler.h"
/*-------------------Macro Definitions----------------*/
#define WTIMER0_PRIO (0x01)
#define TIMER1_PRIO (0x00)
#define TIMER2_PRIO (0x00)

void TIMER2A_Handler(void)  //Timer 1 A ISR used to debounce SW2
{ 	
		static int intCounter=0;	
 if(TimerIntStatus(TIMER2_BASE,false))
	{
		TimerDisable(TIMER2_BASE, TIMER_A);
		TimerIntClear(TIMER2_BASE, TIMER_A);
     //switch_value = ReadPinState(GPIO_PORTF_BASE,GPIO_PIN_4);
	
		///if(switch_value)
		{
		Display_NewLine();
	  Display_String("Usa s-a deschis. Counter");
		Display_Decimal(++intCounter);
			
		}
	 
	  GPIOIntEnable(GPIO_PORTF_BASE, GPIO_INT_PIN_4);  //Re-Enable GPIO pin interrupt		
	
}

}
void TIMER1A_Handler(void)  //Timer 1 A ISR used to debounce SW2
{
	TimerDisable(TIMER1_BASE, TIMER_A);
		TimerIntClear(TIMER1_BASE, TIMER_A);
    Display_NewLine();
	  Display_String("MISCARE...");
	 
	  GPIOIntEnable(GPIO_PORTD_BASE, GPIO_INT_PIN_6);
}

void TIMER4A_Handler(void)  //Timer 1 A ISR used to debounce SW2
{
//	TimerDisable(TIMER1_BASE, TIMER_A);
		TimerIntClear(TIMER4_BASE, TIMER_A);
	Display_NewLine();
	   Display_String("timer 4");
	   TimerEnable(TIMER4_BASE, TIMER_A);
}

void WTIMER1A_Handler(void)		//Wide Timer 0 A ISR
{ 
		TimerIntClear(WTIMER1_BASE, TIMER_A);
	Display_NewLine();
	  Display_String("WTIMER1A_Handler.... super");
	
	   TimerEnable(WTIMER1_BASE, TIMER_A);
	 
}
void GPIOF_Handler(void) 	//GPIO port F ISR
{	 
	  GPIOIntClear(GPIO_PORTF_BASE,  GPIO_INT_PIN_4);
    TimerEnable(TIMER2_BASE, TIMER_A);
	  GPIOIntDisable(GPIO_PORTF_BASE,GPIO_PIN_4); 
} 

void GPIOD_Handler(void) 	//GPIO port D ISR
{	 
	  //GPIOIntClear(GPIO_PORTD_BASE,  GPIO_INT_PIN_6);
   // TimerEnable(TIMER1_BASE, TIMER_A);
	 // GPIOIntDisable(GPIO_PORTD_BASE,GPIO_PIN_6); 
} 
int main(void)
{
	
	unsigned long ui32SysClock; 
	SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN); //80 Mhz
	ui32SysClock = SysCtlClockGet();
	
	
	 
	Display_Init();
	Display_NewLine(); 
  Display_String("Hello!");	
  Init_Timer(WTIMER0_BASE, TIMER_A,500);
  Init_Timer(TIMER4_BASE, TIMER_A,1500);   	
	Init_Timer(TIMER1_BASE, TIMER_A,500);	  	
	Init_Timer(TIMER2_BASE, TIMER_A,500);
	
	
	// TimerEnable(TIMER4_BASE, TIMER_A);
	 //TimerEnable(TIMER1_BASE, TIMER_A); 
   TimerEnable(WTIMER0_BASE, TIMER_A);
	
	 SetGPIOInterrupt(GPIO_PORTF_BASE,GPIO_PIN_4,GPIO_RISING_EDGE);
	 SetGPIOInterrupt(GPIO_PORTD_BASE,GPIO_PIN_6,GPIO_RISING_EDGE);
	 SetGPIOInput(GPIO_PORTF_BASE,GPIO_PIN_0,1);	
	 SetGPIOInput(GPIO_PORTC_BASE,GPIO_PIN_7,1);
  
	
  	Add_ADC_Channel(ADC_CTL_CH1);
	  Add_ADC_Channel(ADC_CTL_CH4);
	  Add_ADC_Channel(ADC_CTL_CH7);
    ADC_Init();	
 
	 // Sensor2_Init();
	 IntMasterEnable();	//Global interrupt enable
	 Display_NewLine();
	 Display_String("before while");


  // Init_PWM(GPIO_PORTF_BASE,GPIO_PIN_2,Clock_Ticks(20));
	
	
			 Display_NewLine(); 
			 Display_Decimal(INT_GPIOF);
			 Display_NewLine(); 
			 Display_Decimal(INT_TIMER0B);
			 
	while(ui32SysClock)  //Clock working
	{
		 
		if(!ReadPinState(GPIO_PORTF_BASE,GPIO_PIN_0))
		 {
	 Display_NewLine();
			 Display_String("poetentionemtru:  ");	 
		  Display_Decimal(Get_ADC_Value(0));
		 
			 }
		if(!ReadPinState(GPIO_PORTC_BASE,GPIO_PIN_7))
		 {
		  Display_NewLine();
			 Display_String("senzor lumina ambientala: ");	 
		 Display_Decimal(Get_ADC_Value(1));
			  Display_NewLine();
	 	 Display_String("temperatura: ");	 
	  Display_Decimal(Get_ADC_Value(2));
			 }
		  
      
			 
			 
		 
		  
	} 
}

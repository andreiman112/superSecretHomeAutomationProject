/*------------------Project Includes-----------------*/
#include "timer.h" 
#include "display.h"

/*-------------------Driver Includes-----------------*/
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "driverlib/adc.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"

/*-------------------HW define Includes--------------*/
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"

/*-------------------Macro Definitions----------------*/
#define WTIMER0_PRIO (0x01)
#define TIMER1_PRIO (0x00)
#define TIMER2_PRIO (0x00)

extern unsigned long Sensor_Temperature;
extern unsigned long Sensor_AnalogVoltage;
signed char PWM_DutyCycle = 50;

unsigned long TIMER_reload_calculator(unsigned long delay_time_ms)
{
	unsigned long clock_cycle_required = 0;
	unsigned long clock_speed = 0;
	
	clock_speed = SysCtlClockGet();	//Get clock speed in Hz
	clock_cycle_required = ((clock_speed / 1000) * delay_time_ms) - 1;	//Returns the clock cycles required for the requested time in mili seconds
	return clock_cycle_required;
}

void TIMER_Wide_0_Init(void)	//cyclic wide timer configuration
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER0);		//Wide Timer 0 enable 
	IntDisable(INT_WTIMER0A);													//Wide Timer 0A disable of interrupts
	TimerIntDisable(WTIMER0_BASE,TIMER_TIMA_TIMEOUT);
	
	TimerDisable(WTIMER0_BASE, TIMER_A);
	TimerClockSourceSet(WTIMER0_BASE, TIMER_CLOCK_SYSTEM);
	TimerConfigure(WTIMER0_BASE, TIMER_CFG_A_PERIODIC);

	TimerLoadSet(WTIMER0_BASE, TIMER_A, TIMER_reload_calculator(500));	//Set cycle nr for 1000 ms	
	TimerEnable(WTIMER0_BASE, TIMER_A);
	
	TimerIntEnable(WTIMER0_BASE,TIMER_TIMA_TIMEOUT);
	IntPrioritySet(INT_WTIMER0A,(WTIMER0_PRIO)<<5); 			//Priority 1 = "001"0.0000
	IntEnable(INT_WTIMER0A);	//Wide Timer 0A enable of interrupts
}
char getINTTIMER_Address(unsigned long base, unsigned long timer)
{
	unsigned long timerNumber=0xf;
	
	char timerType=((0xff00)&timer)>>8;
	
	char intRet=0;
	
	int debug=0;
	timerNumber=timerNumber<<12;
	timerNumber=base&timerNumber;
	timerNumber=timerNumber>>12;
	if(timerNumber>=0 && timerNumber<=2) intRet=35+timerNumber*timerNumber+timerType;
	else if(timerNumber==3) intRet=51+timerType;
	else if(timerNumber==4) intRet=86+timerType;
	else if(timerNumber==5) intRet=108+timerType;
	return intRet;
}
void InitDebouncingTimer(unsigned long base, unsigned long timer)
{
	char intTimer=getINTTIMER_Address(base, timer);
	unsigned long offset=+(base&(0xf)<<12)>>12;
  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0+offset);  //Wide Timer 0 enable 
 
	
	IntDisable(intTimer);  //Wide Timer 0A disable of interrupts
	TimerIntDisable(base,TIMER_TIMA_TIMEOUT);
	
	TimerDisable(base, timer);
	TimerClockSourceSet(base, TIMER_CLOCK_SYSTEM);
	TimerConfigure(base, TIMER_CFG_A_PERIODIC);

	TimerLoadSet(base, timer, TIMER_reload_calculator(500));	//Set cycle nr for 300 ms	
	//TimerEnable(TIMER2_BASE, TIMER_A);  //Timer will be enabled by GPIO switch ISR
	
	TimerIntEnable(base,TIMER_TIMA_TIMEOUT);
	IntPrioritySet(intTimer,(TIMER2_PRIO)<<5);  //Priority 1 = "001"0.0000
	IntEnable(intTimer);	//Wide Timer 0A enable of interrupts
}

  

void TIMER_delay(unsigned long delay_time_ms)
{
	unsigned long clock_cycle_required = 0;
	clock_cycle_required = TIMER_reload_calculator(delay_time_ms);
	while(clock_cycle_required)
	{
		clock_cycle_required --;
	}
}
void TIMER_delay_No_Int(unsigned long delay_time_ms)
{
	unsigned long clock_cycle_required = 0;
	IntMasterDisable();	//Global interrupt disable
	clock_cycle_required = TIMER_reload_calculator(delay_time_ms);
	while(clock_cycle_required)
	{
		clock_cycle_required --;
	}
	IntMasterEnable();	//Global interrupt enable
}

void WTIMER0A_Handler(void)		//Wide Timer 0 A ISR
{unsigned long var=2;
	unsigned long timer_value=0;
	if(TimerIntStatus(WTIMER0_BASE,false))
	{
		TimerIntClear(WTIMER0_BASE, TIMER_A);
		timer_value = TimerValueGet(WTIMER0_BASE, TIMER_A);

	 ADCProcessorTrigger(ADC1_BASE, var);  //Trigger Temperature sensor ADC
	// ADCProcessorTrigger(ADC0_BASE, 3);  //Trigger Temperature sensor ADC
		

	}
}
/*void DebouncingTimerHandler(unsigned long  base, unsigned long  timer,unsigned long base, unsigned long port)  //Timer 1 A ISR used to debounce SW2
{
	unsigned long switch_value = 0;
	//Display_String("Timer 1 ISR launched -> ");
	if(TimerIntStatus(base,false))
	{
		TimerDisable(base, timer);
		TimerIntClear(base, timer);
		switch_value = GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0);
		if(!(switch_value & GPIO_PIN_0)) {
			if (PWM_DutyCycle >= 99)
			{
				Display_String("HOT enough!");
				Display_NewLine();
			}
			else
			{
				Display_String("Getting hotter...");	
				Display_String(" Simulated temperature: ");
				Display_Decimal(PWM_DutyCycle);
				Display_String(" % of 100%");
				Display_NewLine();
			}
			PWM_DutyCycle += 2;
			if (PWM_DutyCycle > 100) {
				PWM_DutyCycle = 100;
			}
		//	RED_PWM_DutyCycle(PWM_DutyCycle);
		//	BLUE_PWM_DutyCycle(100-PWM_DutyCycle);			
		}
	GPIOIntEnable(GPIO_PORTF_BASE, GPIO_INT_PIN_0);  //Enable GPIO pin interrupt		
	}
}
*/
 
//EOF

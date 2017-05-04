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

char prioTimerArray[12] = { /*T0=*/1, /*T1=*/ 6, /*T2=*/ 4, /*T3=*/1, /*T4=*/1, /*T5=*/ 4,/*WT0=*/1,/*WT1=*/ 3,/*WT2=*/ 4, /*WT3=*/1, /*WT4=*/ 3 , /*WT1=*/ };

signed char PWM_DutyCycle = 50;

unsigned long TIMER_reload_calculator(unsigned long delay_time_ms)
{
	unsigned long clock_cycle_required = 0;
	unsigned long clock_speed = 0;
	
	clock_speed = SysCtlClockGet();	//Get clock speed in Hz
	clock_cycle_required = ((clock_speed / 1000) * delay_time_ms) - 1;	//Returns the clock cycles required for the requested time in mili seconds
	return clock_cycle_required;
}

unsigned long getPeriphTimerAddress(unsigned long port)
{
  
	 char	index=(port&(0xf)<<12)>>12 ;
	unsigned long retAdr=SYSCTL_PERIPH_TIMER0;
if(index<=(0x5)) retAdr=SYSCTL_PERIPH_TIMER0+index; //T0,T1,T2,T3,T4,T5
else if(index==6 || index==7) retAdr=SYSCTL_PERIPH_WTIMER0+(index-6);//WIDET0, 	WIDET1
 else if(index>=(0xC)&&index<=(0xF))retAdr=SYSCTL_PERIPH_WTIMER2+(index-0xc);//WIDET2,WIDET3,WIDET4,WIDET5
	
	 return retAdr;
	
} 
char getINTTIMER_Address(unsigned long base, unsigned long timer)
{
	unsigned long timerNumber=0xf;
	
	char timerType=((0xff00)&timer)>>8;
	
	char intRet=0;
	
 
	timerNumber=timerNumber<<12;
	timerNumber=base&timerNumber;
	timerNumber=timerNumber>>12;
	if(timerNumber<=2) intRet=35+timerNumber*2+timerType;
	else if(timerNumber==3) intRet=51+timerType;
	else if(timerNumber==4) intRet=86+timerType;
	else if(timerNumber>=5) intRet=108+(timerNumber-5)*2+timerType;
	return intRet;
}
char getPrioIndex(unsigned long base)
{
	char	index=(base&(0xf)<<12)>>12 ;
	if(index<=7)return index;
	else return index-4;
		
}
void Init_Timer(unsigned long base, unsigned long timer, unsigned long period)
{
	char intTimer=getINTTIMER_Address(base, timer);
	unsigned long periphAdr=getPeriphTimerAddress(base);
  SysCtlPeripheralEnable(periphAdr);  //Wide Timer 0 enable 
 
	
	IntDisable(intTimer);  //Wide Timer 0A disable of interrupts
	TimerIntDisable(base,TIMER_TIMA_TIMEOUT);
	
	TimerDisable(base, timer);
	TimerClockSourceSet(base, TIMER_CLOCK_SYSTEM);
	TimerConfigure(base, TIMER_CFG_A_PERIODIC);

	TimerLoadSet(base, timer, TIMER_reload_calculator(period));	//Set cycle nr for 300 ms	
 
	
	TimerIntEnable(base,TIMER_TIMA_TIMEOUT);
	IntPrioritySet(intTimer,(prioTimerArray[getPrioIndex(base)])<<5);  //Priority 1 = "001"0.0000
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
//	unsigned long timer_value;
	if(TimerIntStatus(WTIMER0_BASE,false))
	{
		TimerIntClear(WTIMER0_BASE, TIMER_A);
		///timer_value = TimerValueGet(WTIMER0_BASE, TIMER_A);

	   ADCProcessorTrigger(ADC1_BASE, var);  //Trigger Temperature sensor ADC
 
		 TimerEnable(WTIMER0_BASE, TIMER_A);

	}
} 
//EOF

#include "stdbool.h"
#include "stdint.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
/*-------------------HW define Includes--------------*/
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"

#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"

#define WTIMER0_PRIO (0x01)
#define TIMER1_PRIO (0x00)
#define TIMER2_PRIO (0x00)
unsigned long TIMER_reload_calculator(unsigned long delay_time_ms)
{
	unsigned long clock_cycle_required = 0;
	unsigned long clock_speed = 0;
	
  clock_speed = SysCtlClockGet();	//Get clock speed in Hz
	clock_cycle_required = ((clock_speed / 1000) * delay_time_ms) - 1;	//Returns the clock cycles required for the requested time in mili seconds
	return clock_cycle_required;
}
void TIMER_1_Init(void)	//cyclic timer configuration for PF0 / SW2 debounce
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);  //Wide Timer 0 enable 
	IntDisable(INT_TIMER1A);  //Wide Timer 0A disable of interrupts
	TimerIntDisable(TIMER1_BASE,TIMER_TIMA_TIMEOUT);
	
	TimerDisable(TIMER1_BASE, TIMER_A);
	TimerClockSourceSet(TIMER1_BASE, TIMER_CLOCK_SYSTEM);
	TimerConfigure(TIMER1_BASE, TIMER_CFG_A_PERIODIC);

	TimerLoadSet(TIMER1_BASE, TIMER_A, TIMER_reload_calculator(50));	//Set cycle nr for 300 ms	
	//TimerEnable(TIMER1_BASE, TIMER_A);  //Timer will be enabled by GPIO switch ISR
	
	TimerIntEnable(TIMER1_BASE,TIMER_TIMA_TIMEOUT);
	IntPrioritySet(INT_TIMER1A,(TIMER1_PRIO)<<5);  //Priority 1 = "001"0.0000
	IntEnable(INT_TIMER1A);	//Wide Timer 0A enable of interrupts
}

void TIMER_2_Init(void)	//cyclic timer configuration for PF4 / SW1 debounce
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);  //Wide Timer 0 enable 
	IntDisable(INT_TIMER2A);  //Wide Timer 0A disable of interrupts
	TimerIntDisable(TIMER2_BASE,TIMER_TIMA_TIMEOUT);
	
	TimerDisable(TIMER2_BASE, TIMER_A);
	TimerClockSourceSet(TIMER2_BASE, TIMER_CLOCK_SYSTEM);
	TimerConfigure(TIMER2_BASE, TIMER_CFG_A_PERIODIC);

	TimerLoadSet(TIMER2_BASE, TIMER_A, TIMER_reload_calculator(50));	//Set cycle nr for 300 ms	
	//TimerEnable(TIMER2_BASE, TIMER_A);  //Timer will be enabled by GPIO switch ISR
	
	TimerIntEnable(TIMER2_BASE,TIMER_TIMA_TIMEOUT);
	IntPrioritySet(INT_TIMER2A,(TIMER2_PRIO)<<5);  //Priority 1 = "001"0.0000
	IntEnable(INT_TIMER2A);	//Wide Timer 0A enable of interrupts
}

void InitDebouncingTimer(unsigned long base, unsigned long timer)
{
	unsigned long offset=+(base&(0xf)<<12)>>12;
  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0+offset);  //Wide Timer 0 enable 
 
	IntDisable(INT_TIMER2A);  //Wide Timer 0A disable of interrupts
	TimerIntDisable(base,TIMER_TIMA_TIMEOUT);
	
	TimerDisable(base, timer);
	TimerClockSourceSet(base, TIMER_CLOCK_SYSTEM);
	TimerConfigure(base, TIMER_CFG_A_PERIODIC);

	TimerLoadSet(base, timer, TIMER_reload_calculator(500));	//Set cycle nr for 300 ms	
	//TimerEnable(TIMER2_BASE, TIMER_A);  //Timer will be enabled by GPIO switch ISR
	
	TimerIntEnable(base,TIMER_TIMA_TIMEOUT);
	IntPrioritySet(INT_TIMER2A,(TIMER2_PRIO)<<5);  //Priority 1 = "001"0.0000
	IntEnable(INT_TIMER2A);	//Wide Timer 0A enable of interrupts
}

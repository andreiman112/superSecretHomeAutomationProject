/*--------------------Type Includes------------------*/
#include "stdbool.h"
#include "stdint.h"

#include "driverlib/sysctl.h"

#include "adc_handler.h"
#include "i2c_handler.h"
#include "timer_handler.h"
#include "adc_handler.h"
#include "display.h"
#include "startup_container.h"

int main(void)
{
	unsigned long ui32SysClock; 
	SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN); //80 Mhz
	ui32SysClock = SysCtlClockGet();
	
	Init_Drivers();
	while(ui32SysClock)
	{
	
	}
	
}

/*--------------------Type Includes------------------*/
#include "stdbool.h"
#include "stdint.h"
#include "custom_types.h"

/*-------------------Driverlib Includes-----------------*/
#include "driverlib/sysctl.h"

/*--------------------Project Includes------------------*/
#include "startup_container.h"
#include "adc_handler.h"
#include "i2c_handler.h"
#include "spi_handler.h"
#include "timer_handler.h"
#include "display.h"
#include "light_app.h"

uint8_t SlaveResults[256];
CommandStruct SlaveCommands[255];

int main(void)
{
	uint16_t i = 0;
	unsigned long ui32SysClock; 
	SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN); //80 Mhz
	ui32SysClock = SysCtlClockGet();
	
	for (i = 0; i <=255; i++) {
		SlaveResults[i] = i;  //Fill in global variable for slave results with dummy values
	}
	
	Init_Drivers();
	while(ui32SysClock)
	{
	}
	
}

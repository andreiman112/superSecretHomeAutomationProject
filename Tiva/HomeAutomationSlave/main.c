/*--------------------Type Includes------------------*/
#include "stdbool.h"
#include "stdint.h"
#include "custom_types.h"

#include "driverlib/sysctl.h"

#include "adc_handler.h"
#include "i2c_handler.h"
#include "spi_handler.h"

#include "timer_handler.h"
#include "adc_handler.h"
#include "display.h"
#include "startup_container.h"
#include "light_app.h"

uint8_t SlaveResults[256];
void (*SlaveCommands[255])(uint8_t) ;

void Init_Commands()
{
	SlaveCommands[20]=SetL1Red;
	SlaveCommands[21]=SetL1Green;
	SlaveCommands[22]=SetL1Blue;
}

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

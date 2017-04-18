#include "stdbool.h"
#include "stdint.h"
#include "ssi_handler_tw.h"
#include "shift_rg.h"


// pin for latch, enable
void Init_Shift_Rg(unsigned long port_latch, unsigned long pin_latch, unsigned long port_enable, unsigned long pin_enable)
{
	SetGPIOOutput(port_latch,pin_latch); //pin for latch shift rg (rclk)
	
	SetGPIOOutput(port_enable,pin_enable); //pint for enable shift rg (oe)
	ClearGPIOPin(port_enable,pin_enable);
}

void Led_Shift()
{
		uint8_t number = 0x01;
		while(number <= 0x80)
		{
			SSI0_DataOut(number);
			delay(1000);
			number = number << 1;
		}
}

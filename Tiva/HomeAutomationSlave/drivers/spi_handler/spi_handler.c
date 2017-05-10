//spi_handler.c
/*--------------------Type Includes------------------*/
#include "stdbool.h"
#include "stdint.h"
#include "custom_types.h"

/*--------------------Project Includes------------------*/
#include "spi_handler.h"
#include "gpio_handler.h"
#include "timer_handler.h"
#include "display.h"

/*-------------------Driverlib Includes-----------------*/
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/ssi.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"

/*-------------------HW define Includes--------------*/
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"

#define DIVISOR_rgb 12
#define FREQ_SHIFT_RG 25000000 
#define SSI0_SR_R (*((volatile unsigned long *)0x4000800C))
	
extern uint8_t SlaveResults[256];
extern CommandStruct SlaveCommands[256];

void SetGpioPinSPI(int set)
{
	if(set)
	{SetGPIOPin(GPIO_PORTF_BASE,GPIO_PIN_1); }
	else
	{ClearGPIOPin(GPIO_PORTF_BASE,GPIO_PIN_1); }
}

void SSI0_InitSlave(void){ 
	uint8_t delay = 0;
	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);		//SSI 0 enable
	for(delay=0; delay<100; delay=delay+1);// delay

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);	//Port A enable
	for(delay=0; delay<100; delay=delay+1);// delay
	
	IntMasterDisable();
	IntDisable(INT_SSI0);
	SSIDisable(SSI0_BASE);												 //Disable SSI0
	SSIIntDisable(SSI0_BASE, SSI_RXTO);
	
	GPIOPinConfigure(GPIO_PA2_SSI0CLK);		//PA2 - Clock
	GPIOPinConfigure(GPIO_PA5_SSI0TX);		//PA5 - TX
	GPIOPinConfigure(GPIO_PA4_SSI0RX);		//PA4 - RX
	GPIOPinConfigure(GPIO_PA3_SSI0FSS);		//PA3 - FSS
	
	GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_5 | GPIO_PIN_4);	// Configure PA2, PA5, PA4 as SSI
	SSIClockSourceSet(SSI0_BASE, SSI_CLOCK_SYSTEM);	// Set the SSI clock source

	//Peripherial base, Input clock, Frame format(freescale format), Mode, Bit Data Rate,	Data Width	
	SSIConfigSetExpClk(SSI0_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_1, SSI_MODE_SLAVE, 6666666/*8000000 SysCtlClockGet()/12*/, 8);
	SSIIntEnable(SSI0_BASE, SSI_RXTO);
	SSIEnable(SSI0_BASE);				//Enable SSI
	IntPrioritySet(INT_SSI0,(2)<<5);  //Priority 2 = "010"0.0000
	IntEnable(INT_SSI0);
	IntMasterEnable();
	
  for(delay=0; delay<10; delay=delay+1);// delay minimum 100 ns
}

void SSI0_DataOut(uint8_t data){ 
	SSIDataPut(SSI0_BASE,(uint32_t)data); //Puts a data element into the SSI transmit FIFO.
	Display_NewLine();	
	Display_Decimal(data); 
	Display_NewLine();	
}
void SSI0_Handler(void){ //Time-out interrupt
	uint32_t byte0, byte1, byteCrc, junk=0;
	uint8_t b0, b1, bCrc=0;
	uint8_t return_result = 0;
	
	if (SSIIntStatus(SSI0_BASE,true)==SSI_RXTO) {
	
		//Read SPI
		SSIDataGet(SSI0_BASE, &byte0);  //read 1st byte
		b0 = (uint8_t)byte0;
		SSIDataGet(SSI0_BASE, &byte1);  //read 2nd byte
		b1 = (uint8_t)byte1;
		SSIDataGet(SSI0_BASE, &byteCrc);	//read CRC
		bCrc = (uint8_t)byteCrc;
		if(b0 == 0)  //data request scenario
		{			
			return_result = SlaveResults[b1];
			SSI0_DataOut(b1);
			SSI0_DataOut(return_result);
			SSI0_DataOut(b1^return_result);
		}	
		else  //command scenario
		{
			SSI0_DataOut(b0);
			SSI0_DataOut(b1);
			SSI0_DataOut(b0^b1);
			SlaveCommands[b0].set = 1;
			SlaveCommands[b0].value = b1;
		}	
		while(SSI0_SR_R&4){
			SSIDataGet(SSI0_BASE, &junk);  //Discard everything else on SPI bus
		}	
		SSIIntClear(SSI0_BASE,SSI_RXTO);
	}
}

void SSI0_Init_ShiftRg(void){ //for shift register
	uint8_t delay = 0;
	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);		//SSI 0 enable 
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);	//Port A enable
	
	SSIDisable(SSI0_BASE);												 //Disable SSI0

	GPIOPinConfigure(GPIO_PA2_SSI0CLK);		//PA2 - Clock
	GPIOPinConfigure(GPIO_PA5_SSI0TX);		//PA5 - TX
	GPIOPinConfigure(GPIO_PA4_SSI0RX);		//PA4 - RX
	GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_5 | GPIO_PIN_4);	// Configure PA2, PA5, PA4 as SSI
	
	SSIClockSourceSet(SSI0_BASE, SSI_CLOCK_SYSTEM);	// Set the SSI clock source

	
	//Peripherial base, Input clock, Frame format(freescale format), Mode, Bit Data Rate,	Data Width	
	SSIConfigSetExpClk(SSI0_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, FREQ_SHIFT_RG, 8);
	SSIEnable(SSI0_BASE);				//Enable SSI
	
	
	//for latch rclk
	SetGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_2); //pin for latch shift rg (rclk)
	SetGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_3); //pint for enable shift rg (oe)
	ClearGPIOPin(GPIO_PORTF_BASE,GPIO_PIN_2);
	

  for(delay=0; delay<10; delay=delay+1);// delay minimum 100 ns
}

void SSI1_Init_RGB(void){ //for rgb strip
	uint8_t delay = 0;
	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI1);  //SSI 1 enable 
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);	//Port D enable
	
	SSIDisable(SSI1_BASE);  //Disable SSI1

	GPIOPinConfigure(GPIO_PD0_SSI1CLK);		//PD0 - Clock
	GPIOPinConfigure(GPIO_PD3_SSI1TX);		//PD3 - TX
	GPIOPinTypeSSI(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_3);	// Configure PD0 and PD3 as SSI
	
	SSIClockSourceSet(SSI1_BASE, SSI_CLOCK_SYSTEM);	// Set the SSI clock source

	
	//Peripherial base, Input clock, Frame format(freescale format), Mode, Bit Data Rate,	Data Width	
	SSIConfigSetExpClk(SSI1_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, SysCtlClockGet()/DIVISOR_rgb, 8); //bit data rate for RGB 80MHz/12 = 6666666
	SSIEnable(SSI1_BASE);				//Enable SSI

  for(delay=0; delay<10; delay=delay+1);// delay minimum 100 ns
}

void SSI1_DataOut(uint8_t data){ 
	SSIDataPut(SSI1_BASE,(uint32_t)data); //Puts a data element into the SSI transmit FIFO.
	//SSIDataPutNonBlocking(SSI1_BASE,data); //Puts a data element into the SSI transmit FIFO.
}
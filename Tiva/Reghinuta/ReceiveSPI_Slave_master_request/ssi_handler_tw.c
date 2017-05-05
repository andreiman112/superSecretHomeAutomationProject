#include "stdbool.h"
#include "stdint.h"
#include "ssi_handler_tw.h"
#include "gpio_handler.h"

/*-------------------Driver Includes-----------------*/
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/ssi.h"
#include "driverlib/sysctl.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/interrupt.h"
#include "display.h"
#include "timer.h"

#define DIVISOR_rgb 12
#define FREQ_SHIFT_RG 25000000 



void SSI0_DataOut(uint8_t data){ 
	SSIDataPut(SSI0_BASE,data); //Puts a data element into the SSI transmit FIFO.
	Display_NewLine();	
	//Display_String("temp: ");
	Display_Decimal(data); 
}
/*
void SSI1_DataOut(uint8_t data){ 
	SSIDataPut(SSI1_BASE,data); //Puts a data element into the SSI transmit FIFO.
}*/


void SSI0_InitSlave(void){ //
	uint8_t delay = 0;
	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);		//SSI 0 enable 
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);	//Port A enable
	
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
	SSIConfigSetExpClk(SSI0_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0, SSI_MODE_SLAVE, 4000000 /*SysCtlClockGet()/15*/, 8);
	SSIIntEnable(SSI0_BASE, SSI_RXTO);
	SSIEnable(SSI0_BASE);				//Enable SSI
	IntPrioritySet(INT_SSI0,(2)<<5);  //Priority 2 = "010"0.0000
	IntEnable(INT_SSI0);  //GPIO Port F enable of interrupts
	IntMasterEnable();
	
  for(delay=0; delay<10; delay=delay+1);// delay minimum 100 ns
}
void SSI0_InitMaster(void){//for shift register
	uint8_t delay = 0;
	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);		//SSI 0 enable 
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);	//Port A enable
	
	SSIDisable(SSI0_BASE);												 //Disable SSI0

	GPIOPinConfigure(GPIO_PA2_SSI0CLK);		//PA2 - Clock
	GPIOPinConfigure(GPIO_PA5_SSI0TX);		//PA5 - TX
	GPIOPinConfigure(GPIO_PA4_SSI0RX);		//PA4 - RX
	GPIOPinConfigure(GPIO_PA3_SSI0FSS);		//PA3 - FSS
	
	GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_5 | GPIO_PIN_4);	// Configure PA2, PA5, PA4 as SSI
	
	SSIClockSourceSet(SSI0_BASE, SSI_CLOCK_SYSTEM);	// Set the SSI clock source

	
	//Peripherial base, Input clock, Frame format(freescale format), Mode, Bit Data Rate,	Data Width	
	SSIConfigSetExpClk(SSI0_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, 4000000 /*SysCtlClockGet()/8*/, 8);
	SSIEnable(SSI0_BASE);				//Enable SSI

  for(delay=0; delay<10; delay=delay+1);// delay minimum 100 ns
}

void Send_ok(void)
{
	SetGPIOPin(GPIO_PORTF_BASE,GPIO_INT_PIN_1);
		 //	TIMER_delay(500);
     	SSI0_DataOut('o');
			SSI0_DataOut('k'); 
	ClearGPIOPin(GPIO_PORTF_BASE,GPIO_INT_PIN_1);
}
void SSI0_Handler(void){
	uint32_t temp1 = 30;
	uint32_t temp2 = 20;
	uint32_t val[3];
	static uint8_t state = 1;
	if (SSIIntStatus(SSI0_BASE,true)==SSI_RXTO) {
		//Time-out interrupt
		//Read SPI
		SSIDataGet(SSI0_BASE, &val[0]);
		SSIDataGet(SSI0_BASE, &val[1]);
		
		//SSIDataGet(SSI0_BASE, &val[1]);
		//SSIDataGet(SSI0_BASE,  &val[2]);
		/*if ( (val[0]=='s')&&(val[1]=='p')&&(val[2]=='i')){
		
			SetGPIOPin(GPIO_PORTF_BASE,GPIO_INT_PIN_1);
		 //	TIMER_delay(500);
     	SSI0_DataOut('o');
			SSI0_DataOut('k'); 
			SSI0_DataOut('i');
			SSI0_DataOut('j'); 
			ClearGPIOPin(GPIO_PORTF_BASE,GPIO_INT_PIN_1);
			
			state ^= 1;
		}*/
		
		if(val[0] == 0x00 && val[1] == 0x01)
		{
			//SetGPIOPin(GPIO_PORTF_BASE,GPIO_INT_PIN_1);
			SSI0_DataOut(val[1]);
			SSI0_DataOut(temp1);
			//ClearGPIOPin(GPIO_PORTF_BASE,GPIO_INT_PIN_1);
			state ^= 1;
		}
		if(val[0] == 0x00 && val[1] == 0x02)
		{
		//	SetGPIOPin(GPIO_PORTF_BASE,GPIO_INT_PIN_1);
			SSI0_DataOut(val[1]);
			SSI0_DataOut(temp2);
		//	ClearGPIOPin(GPIO_PORTF_BASE,GPIO_INT_PIN_1);
			state ^= 1;
		}
		SSIIntClear(SSI0_BASE,SSI_RXTO);
	}
}

/*
void SSIIntClear (uint32_t ui32Base, uint32
void SSIIntDisable (uint32_t ui32Base, uint SSI_TXFF, SSI_RXFF, SSI_RXTO, or SSI_RXOR
void SSIIntEnable (uint32_t ui32Base, uint3
void SSIIntRegister (uint32_t ui32Base, vo
uint32_t SSIIntStatus (uint32_t ui32Base, b
void SSIIntUnregister (uint32_t ui32Base)

*/


void SSI1_Init(void){ //for rgb strip
	uint8_t delay = 0;
	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI1);		//SSI 1 enable 
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);	//Port D enable
	
	SSIDisable(SSI1_BASE);												 //Disable SSI1

	GPIOPinConfigure(GPIO_PD0_SSI1CLK);		//PD0 - Clock
	GPIOPinConfigure(GPIO_PD3_SSI1TX);		//PD3 - TX
	GPIOPinTypeSSI(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_3);	// Configure PD0 and PD3 as SSI
	
	SSIClockSourceSet(SSI1_BASE, SSI_CLOCK_SYSTEM);	// Set the SSI clock source

	
	//Peripherial base, Input clock, Frame format(freescale format), Mode, Bit Data Rate,	Data Width	
	SSIConfigSetExpClk(SSI1_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, SysCtlClockGet()/DIVISOR_rgb, 8); //bit data rate for RGB 80MHz/12 = 6666666
	SSIEnable(SSI1_BASE);				//Enable SSI

  for(delay=0; delay<10; delay=delay+1);// delay minimum 100 ns
}

#include "spi_handler.h"
#include "stdbool.h"
#include "stdint.h" 
#include "gpio_handler.h"
#include "timer_handler.h"
/*-------------------Driver Includes-----------------*/
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/ssi.h"
#include "driverlib/sysctl.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/interrupt.h"
#include "display.h"
uint8_t SensorValues[255];
uint8_t Commands[255];
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

void SSI0_DataOut(uint8_t data){ 
	SSIDataPut(SSI0_BASE,data); //Puts a data element into the SSI transmit FIFO.
	Display_NewLine();	
	//Display_String("temp: ");
	Display_Decimal(data); 
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
		//	ClearGPIOPin(GPIO_PORTF_BASE,GPIO_INT_PIN_1);
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

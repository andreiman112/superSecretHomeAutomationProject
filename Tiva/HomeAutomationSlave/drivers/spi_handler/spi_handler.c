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

#define DIVISOR_rgb 12
#define FREQ_SHIFT_RG 25000000 
#define SSI0_SR_R (*((volatile unsigned long *)0x4000800C))
uint32_t SensorValues[255];
void (*Commands[255])(int) ;
void SetGpioPinSPI(int set)
{
	if(set)
	{SetGPIOPin(GPIO_PORTF_BASE,GPIO_PIN_1); }
	else
	{ClearGPIOPin(GPIO_PORTF_BASE,GPIO_PIN_1); }
}
void Init_Commands()
{
	int i=1;
		 Commands[i]=SetGpioPinSPI;
		 i++;
}
void SSI0_InitSlave(void){ 
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
	SSIConfigSetExpClk(SSI0_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_1, SSI_MODE_SLAVE, 4000000 /*SysCtlClockGet()/15*/, 8);
	SSIIntEnable(SSI0_BASE, SSI_RXTO);
	SSIEnable(SSI0_BASE);				//Enable SSI
	IntPrioritySet(INT_SSI0,(2)<<5);  //Priority 2 = "010"0.0000
	IntEnable(INT_SSI0);
	IntMasterEnable();
	
  for(delay=0; delay<10; delay=delay+1);// delay minimum 100 ns
}

void SSI0_DataOut(uint8_t data){ 
	SSIDataPut(SSI0_BASE,data); //Puts a data element into the SSI transmit FIFO.
	Display_NewLine();	
	Display_Decimal(data); 
	Display_NewLine();	
}
void SSI0_Handler(void){ //Time-out interrupt
	uint32_t val1, val2,val3, junk=0;
	
	if (SSIIntStatus(SSI0_BASE,true)==SSI_RXTO) {
	
		//Read SPI
		SSIDataGet(SSI0_BASE, &val1);
	
		SSIDataGet(SSI0_BASE, &val2);
	
		SSIDataGet(SSI0_BASE, &val3);	
		if(val1==0) //request
		{			
		/*	SSI0_DataOut(val2);
			SSI0_DataOut(SensorValues[val2]);
			SSI0_DataOut(val2^SensorValues[val2]);*/
				SSI0_DataOut(1);
			SSI0_DataOut(2);
			SSI0_DataOut(3);
		}	
		else //command
		{
		/*Commands[val1](val2);
			SSI0_DataOut(val1);
			SSI0_DataOut(val2);
			SSI0_DataOut(val1^val2); */
			
				SSI0_DataOut(4);
			SSI0_DataOut(5);
			SSI0_DataOut(6);
		}	
		while(SSI0_SR_R&4){
			SSIDataGet(SSI0_BASE, &junk);
		 
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

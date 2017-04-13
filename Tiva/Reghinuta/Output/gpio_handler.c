#include "gpio_handler.h"

#include "driverlib/timer.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#define GPIOF_PRIO (0x01)  //GPIO Port F priority

void delay(int x)
{
	while(x)
		x--;
}
void delayMS(int x)
{
	SysCtlDelay((SysCtlClockGet()/(3*1000))*x);
}


//example rgb blink

void RGB_Led_Blink(unsigned long port, unsigned long pin)
{
	SetGPIOInput(port, pin); //write pin
	delay(8000000);
	ClearGPIOPin(port, pin); //clear pin
	delay(8000000);

}
unsigned long getPeriphAddress(unsigned long port)
{
	char litera, offset=port;
	if(port>10) {
		litera=(port&(0xf)<<12)>>12;
	  
		if((port&(0xf)<<16)==0) offset=litera-4;
		else offset=litera;
	}
	
	 return SYSCTL_PERIPH_GPIOA+offset;
}
unsigned long getPortAddress(unsigned long port)
{
	
	unsigned long start=GPIO_PORTA_BASE; 
	unsigned long portAddr;
	if(port>(0x10))	
		return port;
	
   if(port>=PORT_E)
	{
		start=GPIO_PORTE_BASE; 
		port-=4;
	}  
 			 
	portAddr=port<<12;
	portAddr=start+portAddr;
	return portAddr;
}
void SetGPIOInput(unsigned long port, unsigned long pin){
	
	unsigned long portAddr=getPortAddress(port); 
  
	SysCtlPeripheralEnable(getPeriphAddress(port));  //Enable clock on port  
		
	 
	(*((volatile unsigned long *)(portAddr+0x520))) = 0x4C4F434B;  //Unlock Portx    -> GPIOLOCK register
  (*((volatile unsigned long *)(portAddr+0x524)))|= pin;  //Allow changes to pin -> 
	//The contents of the GPIOCR register can only be modified if the status in the GPIOLOCK register
  //is unlocked
	 
	GPIOPinTypeGPIOInput(portAddr, pin);  //Set PF0 and PF4 as GPIO Input
	GPIODirModeSet(portAddr, pin, GPIO_DIR_MODE_IN);  //Set direction input for PF0 and PF4
	GPIOPadConfigSet(portAddr, pin, GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU); //Configure PUR for PF0 and PF4
	
 
}
int ReadPinState(unsigned long port, unsigned long pin ){

	port=getPortAddress(port);
	
	if(GPIOPinRead(port,pin)!=pin) return 0;
	else return 1;

}
void SetGPIOInterrupt(unsigned long port , unsigned long pin,unsigned long edge )
{
	 unsigned long portAddr=getPortAddress(port);
	 
	 IntDisable(INT_GPIOF);  //GPIO Port   disable of interrupts
	
   SetGPIOInput(portAddr,pin);
	
 	GPIOIntDisable(portAddr,pin);  //Disable GPIO pin interrupt
 	GPIOIntTypeSet(portAddr,pin, /*GPIO_FALLING_EDGE*/  edge);  //Set Low level interrupt type
  GPIOIntEnable(portAddr, pin);  //Enable GPIO pin interrupt	
 	IntPrioritySet(INT_GPIOF,(GPIOF_PRIO)<<5);  //Priority 2 = "010"0.0000
	IntEnable(INT_GPIOF);  //GPIO Port F enable of interrupts
}  

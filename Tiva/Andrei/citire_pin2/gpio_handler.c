
/*-------------------HW define Includes--------------*/
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"

#include "gpio_handler.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"

#define WTIMER0_PRIO (0x01)
#define TIMER1_PRIO (0x00)
#define TIMER2_PRIO (0x00)
#define GPIO_PORTF_LOCK_R       (*((volatile unsigned long *)0x40025520))
#define GPIO_PORTF_CR_R         (*((volatile unsigned long *)0x40025524))
#define GPIOF_PRIO (0x01)  //GPIO Port F priority


void delay(int x)
{
	while(x)
		x--;
}
//example rgb blink
void RGB_Led_Blink(unsigned long port, unsigned long pin)
{
	SetPin(port, pin); //write pin
	delay(8000000);
	ClearPin(port, pin); //clear pin
	delay(8000000);

}
unsigned long getPeriphAddress(unsigned long port)
{
	char litera,prefix, offset=0;
	 
		litera=(port&(0xf)<<12)>>12;
	  
		if((port&(0xf)<<16)==0) offset=litera-4;
		else offset=litera;
	 
	
	 return SYSCTL_PERIPH_GPIOA+offset;
	
}
unsigned long getPortAddress(unsigned long port)
{
		return port;
}
char getINTAdrress(unsigned long port)
{
char litera,prefix, offset=0;
	 
		char id=(port&(0xff)<<12)>>12;
	  
	
	if(id>=4 && id<=7)
	 return id+12;//A,B,C,D
	else if(id==24) return 20; //E
	else if(id>=25 && id<=27) return id+21;//F,G,H

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
void SetGPIOInterrupt(unsigned long port , unsigned long pin)
{
	 unsigned long portAddr=getPortAddress(port);
	 char int_addr=getINTAdrress(port);
	 IntDisable(int_addr);  //GPIO Port   disable of interrupts
	
   SetGPIOInput(portAddr,pin);
	
 	GPIOIntDisable(portAddr,pin);  //Disable GPIO pin interrupt
 	GPIOIntTypeSet(portAddr,pin, /*GPIO_FALLING_EDGE*/  GPIO_RISING_EDGE);  //Set Low level interrupt type
  GPIOIntEnable(portAddr, pin);  //Enable GPIO pin interrupt	
 	IntPrioritySet(int_addr,(GPIOF_PRIO)<<5);  //Priority 2 = "010"0.0000
	IntEnable(int_addr);  //GPIO Port F enable of interrupts
	
}
  

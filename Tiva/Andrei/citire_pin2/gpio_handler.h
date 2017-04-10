#ifndef GPIO_HANDLER_H
#define GPIO_HANDLER_H
/*-------------------Macro Definitions----------------*/
/*--------------------Type Includes------------------*/
#include "stdbool.h"
#include "stdint.h"
#define PORT_A 0x0
#define PORT_B 0x1
#define PORT_C 0x2
#define PORT_D 0x3
#define PORT_E 0x4 
#define PORT_F 0x5
#define SetPin(port, pin)     (GPIOPinWrite(port,pin,pin))

#define ClearPin(port, pin)     (GPIOPinWrite(port,pin,0)) 

#define SetOut(port, pin)   {SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);\
														GPIOPinTypeGPIOOutput(port, pin);\
														GPIODirModeSet(port,pin, GPIO_DIR_MODE_OUT);\
														GPIOPadConfigSet(port, pin, GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD);\
														GPIOPinWrite(port,pin,0);};

//SetOut - init: set GPIO Output, set direction output, configure, set initially 0
//SetPin - write
//ClearPin - write 0 = clear
														
/*-----------External function definisions-----------*/
void RGB_Led_Blink(unsigned long, unsigned long);
void delay(int);
void SetGPIOInput(unsigned long , unsigned long );
void SetGPIOInterrupt(unsigned long , unsigned long );
 
int ReadPinState(unsigned long , unsigned long );
void SavePinState(char, unsigned long , unsigned long )	;													

														
#endif


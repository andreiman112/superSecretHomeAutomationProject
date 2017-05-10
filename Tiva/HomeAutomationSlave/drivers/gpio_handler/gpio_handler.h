#ifndef GPIO_HANDLER_H
#define GPIO_HANDLER_H

/*--------------------Type Includes------------------*/
#include "stdbool.h"
#include "stdint.h"

/*-------------------Macro Definitions----------------*/
#define PORT_A 0x0
#define PORT_B 0x1
#define PORT_C 0x2
#define PORT_D 0x3
#define PORT_E 0x4 
#define PORT_F 0x5
														
/*-----------External function definisions-----------*/
void RGB_Led_Blink(unsigned long, unsigned long);
void delay(int);
void SetGPIOInput(unsigned long , unsigned long , bool );
void SetGPIOInterrupt(unsigned long , unsigned long ,unsigned long); 
int ReadPinState(unsigned long , unsigned long );											
void SetGPIOOutput(unsigned long, unsigned long);
void SetGPIOPin(unsigned long, unsigned long);
void ClearGPIOPin(unsigned long ,unsigned long );

void RedOn(uint8_t i);
void RedOff(uint8_t i);
void BlueOn(uint8_t i);
void BlueOff(uint8_t i);
void GreenOn(uint8_t i);
void GreenOff(uint8_t i);


#endif


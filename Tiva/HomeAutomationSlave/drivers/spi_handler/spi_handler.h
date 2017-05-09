#ifndef SPI_HANDLER_H
#define SPI_HANDLER_H
#include "stdbool.h"
#include "stdint.h"
enum SensorsID{ TemperatureI2C1=10, 
								TemperatureI2C2,
								TemperatureADC,
								LuminosityI2C1=20,
								LuminosityI2C2,
								LuminosityADC
								/*etc*/};

extern uint32_t SensorValues[255];
extern void (*Commands[255])(int);
void SSI0_DataOut(uint8_t);								
void SSI0_InitSlave(void);
void SSI0_Handler(void);
void SSI0_Init_ShiftRg(void);
void Init_Commands(void);	

void SSI1_Init_RGB(void);							
void SSI1_DataOut(uint8_t);									
#endif

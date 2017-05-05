#ifndef SPI_HANDLER_H
#define SPI_HANDLER_H
#include "stdbool.h"
#include "stdint.h"
enum SensorsID{ TemperatureI2C=0, 
								TemperatureADC,
								Temperature3,
								LuminosityI2C1,
								LuminosityI2C2
								/*etc*/};

extern uint32_t SensorValues[255];
extern uint32_t Commands[255];

void SSI0_DataOut(uint8_t);								
void SSI0_InitSlave(void);
void SSI0_Handler(void);
void SSI0_Init_ShiftRg(void);
void SSI1_Init_RGB(void);
								
#endif

#include "stdbool.h"
#include "stdint.h"
enum SensorsID{ TemperatureI2C=0, 
								TemperatureADC,
								Temperature3,
								LuminosityI2C1,
								LuminosityI2C2
								/*etc*/};
void SSI0_InitSlave(void);
extern uint8_t SensorValues[255];
extern uint8_t Commands[255];

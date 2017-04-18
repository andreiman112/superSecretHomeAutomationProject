#include "stdbool.h"
#include "stdint.h"
void INIT_I2C(void);
unsigned long I2C_Read(unsigned char Slave_Address, unsigned char Register_Address);
void I2C_Init(void);

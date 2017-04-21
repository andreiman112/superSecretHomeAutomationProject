#include "stdbool.h"
#include "stdint.h" 
void I2C_Init(void);
unsigned long I2C_ReadTemp(unsigned char Slave_Address, unsigned char Register_Address);
unsigned long I2C_RealLum(unsigned char Slave_Address);
 void I2C_ReadTEMPCONF2(unsigned char Slave_Address);
void I2C_ConfTemp(unsigned char Slave_Address, unsigned char Register_Address);

#ifndef DISPLAY
#define DISPLAY

/*--------------------Type Includes------------------*/
#include "stdbool.h"
#include "stdint.h"

/*-------------------Macro Definitions----------------*/
// U0Rx (VCP receive) connected to PA0
// U0Tx (VCP transmit) connected to PA1
// standard ASCII symbols
#define CR   0x0D
#define LF   0x0A
#define BS   0x08
#define ESC  0x1B
#define SP   0x20
#define DEL  0x7F
#define BAUD_RATE (115200)

/*-----------External function definisions-----------*/
void Display_Init(void);
void Display_Char(unsigned char data);
void Display_String(char *pt);
void Display_Decimal(unsigned long n);
void Display_Hex(unsigned long number);
void Display_NewLine(void);

#endif
//EOF

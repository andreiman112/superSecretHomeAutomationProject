#ifndef SSI_HANDLER_TW_H
#define SSI_HANDLER_TW_H

#include <stdint.h>
//#include "inc/tm4c123gh6pm.h"


#define NUM_SSI_DATA 3

// Global variables used in interrupt handler and the main loop.
extern uint32_t g_ulSSI1RXTO;
extern uint32_t g_ulDataRx1[NUM_SSI_DATA];


void SSI0_DataOut(uint8_t data);
//void SSI1_DataOut(uint8_t data);

void SSI0_InitSlave(void);
void SSI1_InitSlave(void);

void SSI1_Init(void);

#endif

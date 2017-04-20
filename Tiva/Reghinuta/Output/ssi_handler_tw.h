#ifndef SSI_HANDLER_TW_H
#define SSI_HANDLER_TW_H

#include <stdint.h>
#include "inc/tm4c123gh6pm.h"

void SSI0_DataOut(uint8_t data);
void SSI1_DataOut(uint8_t data);
void SSI0_DataIn(uint32_t);
void SSI0_Init(void);
void SSI1_Init(void);


#endif

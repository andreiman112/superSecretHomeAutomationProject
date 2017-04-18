#ifndef SHIFT_RG_H
#define SHIFT_RG_H

#include "stdbool.h"
#include "stdint.h"
#include "ssi_handler_tw.h"
#include "gpio_handler.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/gpio.h"

void Led_Shift(void);
void Init_Shift_Rg(unsigned long port_latch, unsigned long pin_latch, unsigned long port_enable, unsigned long pin_enable);
#endif

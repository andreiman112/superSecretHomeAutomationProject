#ifndef PWM_HANDLER_H
#define PWM_HANDLER_H
/*--------------------Type Includes------------------*/
#include "stdbool.h"
#include "stdint.h"
/*-------------------Driver Includes-----------------*/
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "inc/hw_memmap.h"

#define PWM_DIV_for32 32

void GPIO_Configure_PWM(unsigned long,unsigned long);
void Init_PWM(unsigned long, unsigned long, unsigned long);
void Duty_Cycle(unsigned long, unsigned long, unsigned long);
void State_PWM(unsigned long, unsigned long, bool);
unsigned long PWM_Base(unsigned long,unsigned long);
unsigned long BitId_PWM(unsigned long,unsigned long);
unsigned long Encoded_Offset_PWM(unsigned long,unsigned long);
unsigned long Gen_PWM(unsigned long,unsigned long);
unsigned long Sys_Ctl_Enable(unsigned long);
unsigned long Clock_Ticks(unsigned long);
unsigned long Periph_PWM(unsigned long, unsigned long);

#endif

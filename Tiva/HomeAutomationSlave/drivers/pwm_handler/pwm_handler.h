#ifndef PWM_HANDLER_H
#define PWM_HANDLER_H

#include "stdbool.h"
#include "stdint.h"


void Init_PWM(unsigned long, unsigned long, unsigned long);
void Duty_Cycle(unsigned long, unsigned long, unsigned long);
void State_PWM(unsigned long, unsigned long, bool);
unsigned long Sys_Ctl_Enable(unsigned long);
unsigned long Gen_PWM(unsigned long,unsigned long);
unsigned long BitId_PWM(unsigned long,unsigned long);
unsigned long Encoded_Offset_PWM(unsigned long,unsigned long);
unsigned long PWM_Base(unsigned long,unsigned long);
void GPIO_Configure_PWM(unsigned long,unsigned long);
unsigned long Clock_Ticks(unsigned long );
unsigned long Periph_PWM(unsigned long, unsigned long);

#endif

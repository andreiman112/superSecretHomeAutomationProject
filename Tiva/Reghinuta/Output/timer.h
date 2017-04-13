#ifndef TIMER
#define TIMER

/*--------------------Type Includes------------------*/
#include "stdbool.h"
#include "stdint.h"

#include "driverlib/timer.h"
/*-----------External function definisions-----------*/
void TIMER_delay(unsigned long delay_time_ms);
void TIMER_delay_No_Int(unsigned long delay_time_ms);
void TIMER_Wide_0_Init(void);
void InitDebouncingTimer(unsigned long, unsigned long); 

void TIMER_2_Init(void);
void TIMER_1_Init(void);
unsigned long TIMER_reload_calculator(unsigned long delay_time_ms);
#endif
//EOF

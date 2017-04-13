#include "stdbool.h"
#include "stdint.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "driverlib/adc.h"
#include "timer.h"
unsigned long Get_ADC_Value(char index);
void ADC_Init(void) ;
void Add_ADC_Channel(unsigned long channel);
//EOF

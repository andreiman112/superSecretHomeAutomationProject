
#include "adc_handler.h"
#include "stdbool.h"
#include "stdint.h"
/*-------------------HW define Includes--------------*/
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"

#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "driverlib/adc.h"

#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"

#define ADC1SS0_PRIO (0x02) //Priority 2

uint32_t adcChannels[13];
uint32_t adcValues[12];

void Add_ADC_Channel(unsigned long channel)
{
    adcChannels[0]++;
    adcChannels[adcChannels[0]] = channel;
}
void ADC_Init(void)
{
    unsigned char sampleSeq = 0;
    char i;
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC1);

    IntDisable(INT_ADC1SS0);

    ADCIntClear(ADC1_BASE, sampleSeq);
    ADCIntDisable(ADC1_BASE, sampleSeq);
    ADCSequenceDisable(ADC1_BASE, sampleSeq);
    ADCSequenceConfigure(ADC1_BASE, sampleSeq, /*ADC_TRIGGER_ALWAYS*/ ADC_TRIGGER_PROCESSOR, 0);
	
    for (i = 1; i < adcChannels[0]; i++)
        ADCSequenceStepConfigure(ADC1_BASE, sampleSeq, i - 1, adcChannels[i]);

    ADCSequenceStepConfigure(ADC1_BASE, sampleSeq, adcChannels[0] - 1, adcChannels[adcChannels[0]] | ADC_CTL_IE | ADC_CTL_END);
    ADCSequenceEnable(ADC1_BASE, sampleSeq);
    ADCIntEnable(ADC1_BASE, sampleSeq);

    IntPrioritySet(INT_ADC1SS0, (ADC1SS0_PRIO) << 5);
    IntEnable(INT_ADC1SS0);
}

unsigned long Get_ADC_Value(char index)
{
    return adcValues[index];
}
void ADC1SS0_Handler(void)
{ 
    unsigned char sampleSeq = 0;

    if (ADCIntStatus(ADC1_BASE, sampleSeq, false)) {
        ADCIntClear(ADC1_BASE, sampleSeq); //Clear interrupt flag
        ADCSequenceDataGet(ADC1_BASE, sampleSeq, adcValues);
    }
}

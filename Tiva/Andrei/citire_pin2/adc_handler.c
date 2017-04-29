#include "adc_handler.h"

/*-------------------HW define Includes--------------*/
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"

#include "timer.h"
#include "gpio_handler.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
/*-------------------Macro Definitions----------------*/
#define ADC1SS0_PRIO (0x02)  //Priority 2
#define ADC_VREFP (3.3)  //2.5 Volt
#define ADC_VREFN (0)  //8 Volt
#define ADC_Ref_Voltage (ADC_VREFP - ADC_VREFN)	//33 represents 3v3, 50 represents 5v

#define ADC0SS3_PRIO (0x02)  //Priority 2
unsigned long Sensor_Temperature = 0;
unsigned long Sensor_AnalogVoltage = 0;
uint32_t adcChannels[13];
uint32_t adcValues[12];
 void Add_ADC_Channel(unsigned long channel)
{
	adcChannels[0]++;
	adcChannels[adcChannels[0]]=channel;
}
void ADC_Init(void) //Initialize temperature sensor input
{
	unsigned char sampleSeq=0;
	char i; 
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC1);  //The ADC1 peripheral must be enabled for use.

	IntDisable(INT_ADC1SS0);
	
	ADCIntClear(ADC1_BASE, sampleSeq); 
	ADCIntDisable(ADC1_BASE, sampleSeq);
	ADCSequenceDisable(ADC1_BASE,sampleSeq);  
	
  ADCSequenceConfigure(ADC1_BASE,sampleSeq, /*ADC_TRIGGER_ALWAYS*/ADC_TRIGGER_PROCESSOR, 0); 
	for(i=1;i<adcChannels[0];i++)
	ADCSequenceStepConfigure(ADC1_BASE,sampleSeq,i-1, adcChannels[i]);  
 
	ADCSequenceStepConfigure(ADC1_BASE,sampleSeq,adcChannels[0]-1, adcChannels[adcChannels[0]]| ADC_CTL_IE | ADC_CTL_END);  
	ADCSequenceEnable(ADC1_BASE,sampleSeq);   
	ADCIntEnable(ADC1_BASE, sampleSeq); 
		
	IntPrioritySet(INT_ADC1SS0,(ADC1SS0_PRIO)<<5);
	IntEnable(INT_ADC1SS0);	
}

void Sensor2_Init(void) //Initialize temperature sensor input
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);  //The ADC0 peripheral must be enabled for use.

	IntDisable(INT_ADC0SS3);
	
	ADCIntClear(ADC0_BASE, 3);  //Clear interrupt flag	
	ADCIntDisable(ADC0_BASE, 3);
	ADCSequenceDisable(ADC0_BASE,3);  //Disable sequencer 3
	
	ADCSequenceConfigure(ADC0_BASE,3, /*ADC_TRIGGER_ALWAYS*/ADC_TRIGGER_PROCESSOR, 0); //Sequence 3 will do a single sample when the processor (SW) sends a signal to start the conversion
	ADCSequenceStepConfigure(ADC0_BASE,3,0, ADC_CTL_CH1 | ADC_CTL_IE | ADC_CTL_END);  //Configure step 0 on sequencer 3 to channel 1, interrupt enable and end conversion when after 1 conversion
	
	ADCSequenceEnable(ADC0_BASE,3);  //Enable sequencer 3
	ADCIntEnable(ADC0_BASE, 3); 
		
	IntPrioritySet(INT_ADC0SS3,(ADC0SS3_PRIO)<<5);
	IntEnable(INT_ADC0SS3);	
}
unsigned long Get_ADC_Value(char index)
{
return adcValues[index];
}
void ADC1SS0_Handler(void) {  //ADC1 Seq3 ISR
unsigned char sampleSeq=0;
 
	if(ADCIntStatus(ADC1_BASE, sampleSeq, false))
	{
		ADCIntClear(ADC1_BASE, sampleSeq);  //Clear interrupt flag
		ADCSequenceDataGet(ADC1_BASE,sampleSeq,  adcValues);
	}
	}

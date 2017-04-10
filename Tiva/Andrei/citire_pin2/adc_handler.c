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
#define ADC1SS2_PRIO (0x02)  //Priority 2
#define ADC_VREFP (3.3)  //2.5 Volt
#define ADC_VREFN (0)  //8 Volt
#define ADC_Ref_Voltage (ADC_VREFP - ADC_VREFN)	//33 represents 3v3, 50 represents 5v

#define ADC0SS3_PRIO (0x02)  //Priority 2
unsigned long Sensor_Temperature = 0;
unsigned long Sensor_AnalogVoltage = 0;
unsigned long adcChannels[12];
unsigned long adcValues[2];
 void Add_ADC_Channel(unsigned long channel)
{
	adcChannels[0]++;
	adcChannels[adcChannels[0]]=channel;
}
void ADC_Init(void) //Initialize temperature sensor input
{
	unsigned char var=adcChannels[0]-1;
	char i;
	unsigned char step=0;
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC1);  //The ADC1 peripheral must be enabled for use.

	IntDisable(INT_ADC1SS2);
	
	ADCIntClear(ADC1_BASE, var);  //Clear interrupt flag	
	ADCIntDisable(ADC1_BASE, var);
	ADCSequenceDisable(ADC1_BASE,var);  //Disable sequencer 3
	
  ADCSequenceConfigure(ADC1_BASE,var, /*ADC_TRIGGER_ALWAYS*/ADC_TRIGGER_PROCESSOR, 0); //Sequence 3 will do a single sample when the processor (SW) sends a signal to start the conversion
	for(i=1;i<adcChannels[0];i++)
	ADCSequenceStepConfigure(ADC1_BASE,var,i-1, adcChannels[i]);  //Configure step 0 on sequencer 3 to channel 1, interrupt enable and end conversion when after 1 conversion
 
	ADCSequenceStepConfigure(ADC1_BASE,var,adcChannels[0]-1, adcChannels[adcChannels[0]]| ADC_CTL_IE | ADC_CTL_END);  //Configure step 0 on sequencer 3 to channel 1, interrupt enable and end conversion when after 1 conversion
	 
	// adcValues=(unsigned long*)malloc(sizeof(unsigned long)*adcChannels[0]);
	//for(i=0;i<adcChannels[0];i++)
//	adcValues[i]=0;
	
	ADCSequenceEnable(ADC1_BASE,var);  //Enable sequencer 3
	ADCIntEnable(ADC1_BASE, var); 
		
	IntPrioritySet(INT_ADC1SS2,(ADC1SS2_PRIO)<<5);
	IntEnable(INT_ADC1SS2);	
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
void ADC1SS2_Handler(void) {  //ADC1 Seq3 ISR
unsigned char var=2;
	  
	if(ADCIntStatus(ADC1_BASE, var, false))
	{
		ADCIntClear(ADC1_BASE, var);  //Clear interrupt flag
		ADCSequenceDataGet(ADC1_BASE,var,  adcValues);
		
		
		 //  Display_NewLine();
			// Display_Decimal(Get_ADC_Value(0));
		 //Display_Decimal(Get_ADC_Value(0));
		
		   //Display_NewLine();
		 	// Display_Decimal(Get_ADC_Value(0));
		 //Display_Decimal(adcValues[1]);
		
		 //  Display_NewLine();
		 	// Display_Decimal(Get_ADC_Value(0));
		/// Display_Decimal(adcValues[2]);
	}		
		/*
		IntMasterDisable();	//Global interrupt disable
		//Sensor_Temperature = (147.5 - ((75 * (ADC_Ref_Voltage) * digital_value) / 4095));
		//Sensor_AnalogVoltage = ((digital_value * ADC_Ref_Voltage)/4096)*1000;
		IntMasterEnable();	//Global interrupt enable
		
		Display_NewLine();
		Display_String("PE2 ");			
		Display_Decimal(digital_value[0]);
 	
 	  Display_NewLine();
		Display_String("PD0 ");
    Display_Decimal(digital_value[2]);
		
		
		Display_NewLine();
		Display_String("PD3 ");
    Display_Decimal(digital_value[1]);
		
		Duty_Cycle(GPIO_PORTF_BASE,GPIO_PIN_2, digital_value[0]/4000.0*100.0);
	 
		
//		Duty_Cycle(GPIO_PORTF_BASE,GPIO_PIN_2, duty_cycle);
			
			/*
 Display_Decimal(digital_value[2]);
				Display_NewLine();
 Display_Decimal(digital_value[3]);*/
		/*
		The internal temperature sensor converts a temperature measurement into a voltage. This voltage
		value, VTSENS, is given by the following equation (where TEMP is the temperature in °C):
		VTSENS = 2.7 - ((TEMP + 55) / 75)
		
		The temperature sensor reading can be sampled in a sample sequence by setting the TSn bit in
		the ADCSSCTLn register. The temperature reading from the temperature sensor can also be given
		as a function of the ADC value. The following formula calculates temperature (TEMP in ?) based
		on the ADC reading (ADCCODE, given as an unsigned decimal number from 0 to 4095) and the
		maximum ADC voltage range (VREFP - VREFN):
		TEMP = 147.5 - ((75 * (VREFP - VREFN) × ADCCODE) / 4096)
		*/
	}

void ADC0SS3_Handler(void) {  //ADC0 Seq3 ISR

	uint32_t digital_value;
	if(ADCIntStatus(ADC0_BASE, 3, false))
	{  
		ADCIntClear(ADC0_BASE, 3);  //Clear interrupt flag
		ADCSequenceDataGet(ADC0_BASE, 3, &digital_value);		
		
	 	IntMasterDisable();	//Global interrupt disable
		Sensor_Temperature = (147.5 - ((75 * (ADC_Ref_Voltage) * digital_value) / 4095));
		Sensor_AnalogVoltage = ((digital_value * ADC_Ref_Voltage)/4096)*1000;
		IntMasterEnable();	//Global interrupt enable
		
			Display_NewLine();
		Display_String("PE2 ");			
		Display_Decimal(digital_value);
		/*
		The internal temperature sensor converts a temperature measurement into a voltage. This voltage
		value, VTSENS, is given by the following equation (where TEMP is the temperature in °C):
		VTSENS = 2.7 - ((TEMP + 55) / 75)
		
		The temperature sensor reading can be sampled in a sample sequence by setting the TSn bit in
		the ADCSSCTLn register. The temperature reading from the temperature sensor can also be given
		as a function of the ADC value. The following formula calculates temperature (TEMP in ?) based
		on the ADC reading (ADCCODE, given as an unsigned decimal number from 0 to 4095) and the
		maximum ADC voltage range (VREFP - VREFN):
		TEMP = 147.5 - ((75 * (VREFP - VREFN) × ADCCODE) / 4096)
		*/
	}
}

 


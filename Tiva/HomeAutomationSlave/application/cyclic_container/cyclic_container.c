#include "stdbool.h"
#include "stdint.h"

#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"

#include "cyclic_container.h"
#include "display.h"
#include "i2c_handler.h"
#include "gpio_handler.h"
#include "adc_handler.h"
#include "driverlib/adc.h" 
#include "driverlib/gpio.h" 
void Cyclic_50ms()
{
  //Display_NewLine();
	//Display_String("50 ms");
		if(!ReadPinState(GPIO_PORTF_BASE,GPIO_PIN_0))
	{
		Display_NewLine();
	  Display_String("pf0 pressed");
	}
}
void Cyclic_100ms()
{
	 ADCProcessorTrigger(ADC1_BASE, 0);  //Trigger Temperature sensor ADC
//	Display_NewLine();
//	Display_String("100 ms");
}
void Cyclic_500ms()
{
	//Display_NewLine();
	//Display_String("500 ms: ");
 
}
void Cyclic_1000ms()
{
	unsigned long h=0,l=0,tmp=0; 
  float	tmp_adc=0;
  Display_NewLine();
 	Display_String("1000 ms: lum senzor=");	
	h=I2C_Read(I2C0_BASE,0x39,0xAD);
	l=I2C_Read(I2C0_BASE,0x39,0xAC);
	Display_Decimal(h*256+l);
	tmp=I2C_Read(I2C1_BASE,0x48,0x00);
	Display_String("; tmp= ");	
  Display_Decimal(tmp);	
	Display_String("; adc[0]= ");	 
	Display_Decimal(Get_ADC_Value(0));
	Display_String("; adc[1]-temp= ");	 
	tmp_adc=Get_ADC_Value(1)*5.0/4095*1000;
	Display_Decimal(tmp_adc);
}

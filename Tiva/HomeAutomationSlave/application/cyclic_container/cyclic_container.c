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
#include "spi_handler.h"
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
	unsigned long h=0,luminositateI2C=0, l=0; 
  float	tmp_adc=0;
  Display_NewLine();
 
	h=I2C_Read(I2C0_BASE,0x49,0xAD);
	l=I2C_Read(I2C0_BASE,0x49,0xAC);
	Display_Decimal(h*256+l);
	 
	luminositateI2C=((h*256+l)*4*255.0)/65535 ;//!!!!!!!!*2
  
	tmp_adc=Get_ADC_Value(1)*5.0/4095*1000;
	
	 	
	SensorValues[LuminosityI2C1]=1;//luminositateI2C;
	SensorValues[LuminosityI2C2]=2;
	SensorValues[LuminosityADC]=3;//Get_ADC_Value(0)%255;//!!!!!!!!!!!!
	
	
	SensorValues[TemperatureI2C1]=21;//I2C_Read(I2C0_BASE,0x48,0x00);
  SensorValues[TemperatureI2C2]=22;
	SensorValues[TemperatureADC]=23;//tmp_adc;

	
	Display_NewLine();
	Display_String("1000 ms: lum i2c =");
	Display_Decimal(SensorValues[LuminosityI2C1]);
	Display_String("; lum adc =");
	Display_Decimal(SensorValues[LuminosityADC]);
	Display_String("tmp i2c =");
	Display_Decimal(SensorValues[TemperatureI2C1]);
	Display_String("; tmp adc =");
	Display_Decimal(SensorValues[TemperatureADC]);
}

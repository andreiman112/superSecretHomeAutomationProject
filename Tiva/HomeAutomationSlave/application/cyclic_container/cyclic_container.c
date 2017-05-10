#include "stdbool.h"
#include "stdint.h"

#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "custom_types.h"
 
#include "cyclic_container.h"
#include "display.h"
#include "i2c_handler.h"
#include "gpio_handler.h"
#include "adc_handler.h"
#include "driverlib/adc.h" 
#include "driverlib/gpio.h" 
#include "spi_handler.h"

extern uint8_t SlaveResults[255];
extern void (*SlaveCommands[255])(uint8_t);

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
//	Display_NewLine();
	//Display_String("100 ms");
	ADCProcessorTrigger(ADC1_BASE, 0);  //Trigger Temperature sensor ADC
}
void Cyclic_500ms()
{
	Display_NewLine();
	Display_String("500 ms: ");
}
void Cyclic_1000ms()
{
	unsigned long h=0,luminositateI2C1=0,luminositateI2C2=0, l=0; 
  float	tmp_adc=0;
  Display_NewLine();
	 
	h=I2C_Read(I2C0_BASE,0x49,0xAD);
	l=I2C_Read(I2C0_BASE,0x49,0xAC);	 
	luminositateI2C1=((h*256+l)*255.0)/16383 ;//!!!!!!!!*2
  
	h=I2C_Read(I2C0_BASE,0x39,0xAD);
	l=I2C_Read(I2C0_BASE,0x39,0xAC);	 
	luminositateI2C2=((h*256+l)*255.0)/16383 ;//!!!!!!!!*2 
	
	 	
	SlaveResults[L1Result]=luminositateI2C1;
	SlaveResults[L2Result]=luminositateI2C2;
	SlaveResults[L3Result]=Get_ADC_Value(1)*255/4080;//!!!!!!!!!!!!
	
	
	SlaveResults[T1Result]=I2C_Read(I2C0_BASE,0x48,0x00);
  SlaveResults[T2Result]=I2C_Read(I2C0_BASE,0x48,0x00);
	SlaveResults[T3Result]=Get_ADC_Value(0)*3.3/4095*1000/19/1.9;//tmp_adc;

	
	Display_NewLine();
	Display_String("1000 ms: lum i2c1 =");
	Display_Decimal(luminositateI2C1);
	Display_String("; lum i2c2=");
	Display_Decimal(luminositateI2C2);
	Display_String("; lum adc=");
	Display_Decimal(SlaveResults[L3Result]);
	
	Display_String("; tmp i2c1 =");
	Display_Decimal(SlaveResults[T1Result]);
	Display_String("; tmp i2c2 =");
	Display_Decimal(SlaveResults[T2Result]);
	Display_String("; tmp adc =");
	Display_Decimal(SlaveResults[T3Result]);
	 
}

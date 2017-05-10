//cyclic_container.c
/*--------------------Type Includes------------------*/
#include "stdbool.h"
#include "stdint.h"
#include "custom_types.h"

/*--------------------Project Includes------------------*/
#include "cyclic_container.h"
#include "display.h"
#include "i2c_handler.h"
#include "gpio_handler.h"
#include "adc_handler.h"
#include "light_app.h"
#include "spi_handler.h"

/*-------------------Driverlib Includes-----------------*/
#include "driverlib/adc.h" 
#include "driverlib/gpio.h" 

/*-------------------HW define Includes--------------*/
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"

extern CommandStruct SlaveCommands[256];
extern uint8_t SlaveResults[256];

void Cyclic_50ms()
{

}
void Cyclic_100ms()
{
//	Display_NewLine();
	//Display_String("100 ms");
	ADCProcessorTrigger(ADC1_BASE, 0);  //Trigger Temperature sensor ADC
}

void Cyclic_500ms()
{
	uint16_t cmd_index = 0;
	uint8_t cmd_value = 0;
	
	Display_NewLine();
	Display_String("500 ms: ");
	
	for(cmd_index=0;cmd_index<=255;cmd_index++) {
		if(SlaveCommands[cmd_index].set == 1) {
			SlaveCommands[cmd_index].set = 0;  //Reset function call
			cmd_value = SlaveCommands[cmd_index].value;  //get function call argument value
			SlaveCommands[cmd_index].function(cmd_value);  //call function
		}
	}
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
	LightHandler();
}

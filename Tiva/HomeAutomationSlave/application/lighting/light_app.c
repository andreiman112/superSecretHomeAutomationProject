//light_app.c


#include "stdbool.h"
#include "stdint.h"
#include "light_app.h"
#include "driverlib/sysctl.h"
#include "spi_handler.h"


tstRGB LightData[NR_OF_LEDS];

static void Send_RGB_Data(void) {
	uint8_t i = 0; //index of LEDs on PCB
	uint8_t j = 0; //index of bits in HighBit and LowBit frames
	for (i=0;i<NR_OF_LEDS;i++){
			for (j=0;j<8;j++){ //Process Green
				if (((LightData[i].Green) & (BitMask>>j)) != 0){
					SSI1_DataOut(HighBit);
				}
				else{ 
					SSI1_DataOut(LowBit);
				}
			}
			for (j=0;j<8;j++){ //Process Red
				if (((LightData[i].Red) & (BitMask>>j)) != 0){
					SSI1_DataOut(HighBit);
				}
				else{
					SSI1_DataOut(LowBit);
				}
			}
			for (j=0;j<8;j++){ //Process Blue
				if (((LightData[i].Blue) & (BitMask>>j)) != 0){
					SSI1_DataOut(HighBit);
				}
				else{
					SSI1_DataOut(LowBit);
				}
			}
		}
}


void SetL1Red(uint8_t red){
	LightData[0].Red = red;
}
void SetL1Green(uint8_t green){
	LightData[0].Green = green;
}
void SetL1Blue(uint8_t blue){
	LightData[0].Blue = blue;
	//Send_RGB_Data();
}


void L1_LightHandler(){
}

void L2_LightHandler(){
}

void L3_LightHandler(){
}
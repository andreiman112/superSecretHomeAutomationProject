//light_app.c

/*--------------------Project Includes------------------*/
#include "light_app.h"
#include "spi_handler.h"

/*-------------------Driverlib Includes-----------------*/
#include "driverlib/sysctl.h"

/*-------------------Macro Definitions----------------*/
#define NR_OF_LEDS (15)
#define HighBit (0xF8)
#define LowBit (0xE0)
#define FullPower (0x80)
#define BitMask (0x80)
#define L1_Start (0)
#define L1_End (4)
#define L2_Start (5)
#define L2_End (9)
#define L3_Start (10)
#define L3_End (14)
#define RedIndex (0)
#define GreenIndex (1)
#define BlueIndex (2)


tstRGB LightData[NR_OF_LEDS];
uint8_t LedSet[NR_OF_LEDS][3];

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
	uint8_t led_index = 0;
	for (led_index = L1_Start; led_index<=L1_End; led_index++){
		LightData[led_index].Red = red;
		LedSet[led_index][RedIndex] = 1;
	}
}
void SetL1Green(uint8_t green){
	uint8_t led_index = 0;
	for (led_index = L1_Start; led_index<=L1_End; led_index++){
		LightData[led_index].Green = green;
		LedSet[led_index][GreenIndex] = 1;
	}
}
void SetL1Blue(uint8_t blue){
	uint8_t led_index = 0;
	for (led_index = L1_Start; led_index<=L1_End; led_index++){
		LightData[led_index].Blue = blue;
		LedSet[led_index][BlueIndex] = 1;
	}
}
void SetL2Red(uint8_t red){
	uint8_t led_index = 0;
	for (led_index = L2_Start; led_index<=L2_End; led_index++){
		LightData[led_index].Red = red;
		LedSet[led_index][RedIndex] = 1;
	}
}
void SetL2Green(uint8_t green){
	uint8_t led_index = 0;
	for (led_index = L2_Start; led_index<=L2_End; led_index++){
		LightData[led_index].Green = green;
		LedSet[led_index][GreenIndex] = 1;
	}
}
void SetL2Blue(uint8_t blue){
	uint8_t led_index = 0;
	for (led_index = L2_Start; led_index<=L2_End; led_index++){
		LightData[led_index].Blue = blue;
		LedSet[led_index][BlueIndex] = 1;
	}
}

void SetL3Red(uint8_t red){
	uint8_t led_index = 0;
	for (led_index = L3_Start; led_index<=L3_End; led_index++){
		LightData[led_index].Red = red;
		LedSet[led_index][RedIndex] = 1;
	}
}
void SetL3Green(uint8_t green){
	uint8_t led_index = 0;
	for (led_index = L3_Start; led_index<=L3_End; led_index++){
		LightData[led_index].Green = green;
		LedSet[led_index][GreenIndex] = 1;
	}
}
void SetL3Blue(uint8_t blue){
	uint8_t led_index = 0;
	for (led_index = L3_Start; led_index<=L3_End; led_index++){
		LightData[led_index].Blue = blue;
		LedSet[led_index][BlueIndex] = 1;
	}
}

	
void LightHandler(){  //Cyclic function to check if something changed
	uint8_t led_index = 0;
	uint8_t color_index = 0;
	uint8_t send_data = 0;
	for (led_index = 0; led_index <= NR_OF_LEDS; led_index++){  //search for set flag through leds
		for (color_index = 0; color_index <= 2; color_index ++) {  //search for set flag through color channels
			if (LedSet[led_index][color_index] == 1) {
				LedSet[led_index][color_index] = 0;  //Reset flags
				send_data = 1;  //seet flag to send data through SPI
			}
		}
	}
	if(send_data == 1) {
		Send_RGB_Data();
	}
}
//EOF

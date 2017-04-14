
#include "stdbool.h"
#include "stdint.h"
#include "driverlib/sysctl.h"
#include "ssi_handler.h"
#include "rgb_st.h"

tstRGB data[NR_OF_LEDS];

static void Clear_Data(void){
	uint8_t i = 0;
	for (i=0;i<NR_OF_LEDS;i++){
		data[i].Red = 0;
		data[i].Green = 0;
		data[i].Blue = 0;
	}
}

static void Set_Off(void){
	Clear_Data();
	Send_RGB_Data();
}

static uint8_t Set_RGB(uint8_t led_nr, tstRGB color) {
	if(led_nr < NR_OF_LEDS) {
		data[led_nr].Green = color.Green;
		data[led_nr].Red = color.Red;
		data[led_nr].Blue = color.Blue;
		return 1; //Ok
	}
	else {
		return 0; //Error
	}
}
void Set_Moving_Point(tstRGB color){
	static uint8_t ix = 0;
	Clear_Data();
	Set_RGB(ix,color);
	ix = ((ix + 1) % NR_OF_LEDS);
}



void Set_Point_Color(tstRGB color, uint8_t ix, tstRGB color2, uint8_t ix2){	
	//al ix-lea led on
	Clear_Data();
	Set_RGB(ix,color);
	Set_RGB(ix2,color2);
}

static uint8_t Set_Bargraph(uint8_t percentage, tstRGB color) {
	uint8_t NrOfDataPoints = BargraphResolutionMultiplier * (NR_OF_LEDS * percentage) / 100;  //Nr of data points (LEDs and half powered LEDs)
	uint8_t NrOfLEDsOn = NrOfDataPoints / BargraphResolutionMultiplier;  //Number of LEDs at full power
	uint8_t i = 0;
	tstRGB LedColor = {0x00,0x00,0x00};
	if(percentage > 100) {
		return 0;  //Error
	}
	else {
		//Set_Off();
		Clear_Data();
		for (i=0;i<NrOfLEDsOn;i++) {
			Set_RGB(i, color); //Turn on LEDs at full Brightness
		}
		if (NrOfDataPoints % BargraphResolutionMultiplier != 0) {
			//Turn on last LED at partial brightness
			if(color.Green > 1) { LedColor.Green = color.Green>>1; }
			if(color.Red > 1) { LedColor.Red = color.Red>>1; }
			if(color.Blue > 1) { LedColor.Blue = color.Blue>>1; }
			Set_RGB(NrOfLEDsOn, LedColor); //Turn on LEDs at full Brightness
		}
		return 1;  //Ok
	}
	
}

void Send_RGB_Data(void) {
	uint8_t i = 0; //index of LEDs on PCB
	uint8_t j = 0; //index of bits in HighBit and LowBit frames
	for (i=0;i<NR_OF_LEDS;i++){
			for (j=0;j<8;j++){ //Process Green
				if (((data[i].Green) & (BitMask>>j)) != 0){
					SSI0_DataOut(HighBit);
				}
				else{ 
					SSI0_DataOut(LowBit);
				}
			}
			for (j=0;j<8;j++){ //Process Red
				if (((data[i].Red) & (BitMask>>j)) != 0){
					SSI0_DataOut(HighBit);
				}
				else{
					SSI0_DataOut(LowBit);
				}
			}
			for (j=0;j<8;j++){ //Process Blue
				if (((data[i].Blue) & (BitMask>>j)) != 0){
					SSI0_DataOut(HighBit);
				}
				else{
					SSI0_DataOut(LowBit);
				}
			}
		}
}

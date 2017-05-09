//light_app.h
#include "stdbool.h"
#include "stdint.h"

#define NR_OF_LEDS (5)
#define HighBit (0xF8)
#define LowBit (0xE0)
#define FullPower (0x80)
#define BitMask (0x80)
#define BargraphResolutionMultiplier (2)
#define SET_RED		LedColor.Green = 0x00; LedColor.Red = FullPower; LedColor.Blue = 0x00; //Red Color
#define SET_GREEN	LedColor.Green = FullPower; LedColor.Red = 0x00; LedColor.Blue = 0x00; //Green Color
#define SET_BLUE	LedColor.Green = 0x00; LedColor.Red = 0x00; LedColor.Blue = FullPower; //Blue Color
	
typedef struct stRGB {
	uint8_t Red;
	uint8_t Green;
	uint8_t Blue;
} tstRGB;

//void Send_RGB_Data(void);
void SetL1Red(uint8_t red);
void SetL1Green(uint8_t green);
void SetL1Blue(uint8_t blue);

//
#ifndef RGB_ST_HANDLER_H
#define RGB_ST_HANDLER_H

#include "stdbool.h"
#include "stdint.h"

#include "inc/tm4c123gh6pm.h"

#define NR_OF_LEDS (5)
#define HighBit (0xF8)
#define LowBit (0xE0)
#define FullPower (0x80)
#define BitMask (0x80)
#define BargraphResolutionMultiplier (2)
	
#define SET_RED		LedColor.Green = 0x00; LedColor.Red = FullPower; LedColor.Blue = 0x00; //Red Color
#define SET_GREEN	LedColor.Green = FullPower; LedColor.Red = 0x00; LedColor.Blue = 0x00; //Green Color
#define SET_BLUE	LedColor.Green = 0x00; LedColor.Red = 0x00; LedColor.Blue = FullPower; //Blue Color
#define SET_OFF		LedColor.Green = 0x00; LedColor.Red = 0x00; LedColor.Blue = 0x00; //Off
#define SET_WHITE	LedColor.Green = FullPower; LedColor.Red = FullPower; LedColor.Blue = FullPower; //White Color
#define SET_YELLOW LedColor.Green = FullPower; LedColor.Red = FullPower; LedColor.Blue = 0x00; //Yellow Color


typedef struct stRGB {
	uint8_t Red;
	uint8_t Green;
	uint8_t Blue;
} tstRGB;

static void Clear_Data(void);
static void Set_Off(void);
static uint8_t Set_RGB(uint8_t led_nr, tstRGB color);
static uint8_t Set_Bargraph(uint8_t percentage, tstRGB color);
void Send_RGB_Data(void);
void Set_Moving_Point(tstRGB color);
void Set_Point_Color(tstRGB color, uint8_t ix, tstRGB color2, uint8_t ix2);


#endif

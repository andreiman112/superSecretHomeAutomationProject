//light_app.h
/*--------------------Type Includes------------------*/
#include "stdbool.h"
#include "stdint.h"

typedef struct stRGB {
	uint8_t Red;
	uint8_t Green;
	uint8_t Blue;
} tstRGB;

void LightHandler();

void SetL1Red(uint8_t red);
void SetL1Green(uint8_t green);
void SetL1Blue(uint8_t blue);

void SetL2Red(uint8_t red);
void SetL2Green(uint8_t green);
void SetL2Blue(uint8_t blue);

void SetL3Red(uint8_t red);
void SetL3Green(uint8_t green);
void SetL3Blue(uint8_t blue);
//EOF
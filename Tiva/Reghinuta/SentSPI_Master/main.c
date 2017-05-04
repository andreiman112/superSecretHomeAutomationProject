/*--------------------Type Includes------------------*/
#include "stdbool.h"
#include "stdint.h"

/*-------------------Driver Includes-----------------*/
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "driverlib/adc.h"
#include "driverlib/ssi.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "utils/uartstdio.h"
#include "pwm_handler.h"
#include "i2c_handler.h"
/*-------------------HW define Includes--------------*/
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"

#include "driverlib/timer.h"

#include "display.h"
/*------------------Project Includes-----------------*/
#include "gpio_handler.h"
#include "adc_handler.h"

#include "ssi_handler_tw.h"
/*-------------------Macro Definitions----------------*/
#define WTIMER0_PRIO (0x01)
#define TIMER1_PRIO (0x00)
#define TIMER2_PRIO (0x00)
#ifdef __TI_COMPILER_VERSION__
	//Code Composer Studio Code
	void Delay(uint32_t ulCount){
	__asm (	"loop    subs    r0, #1\n"
			"    bne     loop\n");
}

#else
	//Keil uVision Code
	__asm void
	Delay(uint32_t ulCount)
	{
    subs    r0, #1
    bne     Delay
    bx      lr
	}
#endif
	
uint32_t g_ulSSI1RXTO;
uint32_t g_ulDataRx1[NUM_SSI_DATA];	
	

void TIMER2A_Handler(void)  //Timer 1 A ISR used to debounce SW2
{ 	
		static int intCounter=0;	
 if(TimerIntStatus(TIMER2_BASE,false))
	{
		TimerDisable(TIMER2_BASE, TIMER_A);
		TimerIntClear(TIMER2_BASE, TIMER_A);
     //switch_value = ReadPinState(GPIO_PORTF_BASE,GPIO_PIN_4);
	
		///if(switch_value)
		{
		Display_NewLine();
	  Display_String("Usa s-a deschis. Counter");
		Display_Decimal(++intCounter);
			
		}
	 
	  GPIOIntEnable(GPIO_PORTF_BASE, GPIO_INT_PIN_4);  //Re-Enable GPIO pin interrupt		
	
}

}
void TIMER1A_Handler(void)  //Timer 1 A ISR used to debounce SW2
{
	TimerDisable(TIMER1_BASE, TIMER_A);
		TimerIntClear(TIMER1_BASE, TIMER_A);
    Display_NewLine();
	  Display_String("MISCARE...");
	 
	  GPIOIntEnable(GPIO_PORTD_BASE, GPIO_INT_PIN_6);
}

void TIMER4A_Handler(void)  //Timer 1 A ISR used to debounce SW2
{
//	TimerDisable(TIMER1_BASE, TIMER_A);
		TimerIntClear(TIMER4_BASE, TIMER_A);
	Display_NewLine();
	   Display_String("timer 4");
	   TimerEnable(TIMER4_BASE, TIMER_A);
}

void WTIMER1A_Handler(void)		//Wide Timer 0 A ISR
{ 
		TimerIntClear(WTIMER1_BASE, TIMER_A);
	Display_NewLine();
	  Display_String("WTIMER1A_Handler.... super");
	
	   TimerEnable(WTIMER1_BASE, TIMER_A);
	 
}






int main(void)
{
	
	unsigned long ui32SysClock; 

	uint32_t ulDataRx0[NUM_SSI_DATA];
	uint32_t ulDataTx0[NUM_SSI_DATA];
	uint32_t ulDataRx1[NUM_SSI_DATA];
	uint32_t ulDataTx1[NUM_SSI_DATA];


  uint32_t ulindex;
	
	SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN); //80 Mhz
	ui32SysClock = SysCtlClockGet();

	Display_Init();
	Display_String("SPI:");
	

	SSI0_InitMaster(); //initializare master
	
   // Initialize the data to send from MASTER TO SLAVE
   ulDataTx0[0] = 115;
   ulDataTx0[1] = 112;
   ulDataTx0[2] = 105;
		
		SetGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);
		//SSI0_IntInit(); //initializare intrerupere pe  SSI_TXFF | SSI_RXFF
		 
	while(1)
	{
			 for(ulindex = 0; ulindex < NUM_SSI_DATA; ulindex++)
			{
					SSI0_DataOut(ulDataTx0[ulindex]);
			}
			
			Delay(20000000);
			SetGPIOPin(GPIO_PORTF_BASE, GPIO_PIN_2);
			Delay(500000);
			ClearGPIOPin(GPIO_PORTF_BASE, GPIO_PIN_2);
			
		
	
	}
	
}

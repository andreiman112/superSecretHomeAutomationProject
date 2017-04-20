/*#include <stdint.h>
#include "ssi_handler.h"

#define DIVISOR 12 //doar pt rgb

void SSI0_DataOut(uint8_t data){
  while((SSI0_SR_R&0x00000002)==0){}; // wait until transmit FIFO not full
  SSI0_DR_R = data;                // data out
}

//********SSI0_Init*****************
void SSI0_Init(void){
  volatile uint32_t delay;
  SYSCTL_RCGCSSI_R |= 0x01;  // activate SSI0
  SYSCTL_RCGCGPIO_R |= 0x01;  // activate port A
  delay = SYSCTL_RCGC2_R;  // allow time to finish activating

  GPIO_PORTA_AFSEL_R |= 0x24;  // enable alt funct on PA2,5                  
  GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R&0xFF0FF0FF)+0x00200200; // configure PA2,5 as SSI
  GPIO_PORTA_DEN_R |= 0x24;  // enable digital I/O on PA2, 5 
  GPIO_PORTA_AMSEL_R &= ~0x24;  // disable analog functionality on PA2, 5
	
  SSI0_CR1_R &= ~SSI_CR1_SSE;  // disable SSI
  SSI0_CR1_R &= ~SSI_CR1_MS;  // master mode , for slave mode set the bits
  SSI0_CC_R = (SSI0_CC_R & ~SSI_CC_CS_M)+SSI_CC_CS_SYSPLL;  // configure for system clock/PLL baud clock source
  //BitRate = SysClk /(CPSDVSR*(1+SCR))
  SSI0_CPSR_R = ((SSI0_CPSR_R & ~SSI_CPSR_CPSDVSR_M) + DIVISOR); // must be even number
  SSI0_CR0_R &= ~(SSI_CR0_SCR_M |       // SCR = 0
                  SSI_CR0_SPH |         // SPH = 0
                  SSI_CR0_SPO);         // SPO = 0
  SSI0_CR0_R = (SSI0_CR0_R&~SSI_CR0_FRF_M)+SSI_CR0_FRF_MOTO;  // FRF = Freescale SPI format                                  
  SSI0_CR0_R = (SSI0_CR0_R&~SSI_CR0_DSS_M)+SSI_CR0_DSS_8;  // DSS = 8-bit data
  SSI0_CR1_R |= SSI_CR1_SSE;  // enable SSI

  for(delay=0; delay<10; delay=delay+1);// delay minimum 100 ns
}*/





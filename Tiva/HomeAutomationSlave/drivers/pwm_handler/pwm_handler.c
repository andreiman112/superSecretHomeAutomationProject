#include "pwm_handler.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "inc/hw_memmap.h"

#define PWM_DIV_for32 32

void Init_PWM(unsigned long port, unsigned long pin, unsigned long period)
{
	SysCtlPWMClockSet(SYSCTL_PWMDIV_32);  //Enable clock for PWM - div32
	
	SysCtlPeripheralEnable(Periph_PWM(port,pin));  //PWM enable module
	SysCtlPeripheralEnable(Sys_Ctl_Enable(port)); //PWM enable port

	GPIO_Configure_PWM(port,pin); //Configure & Set PWM 

	PWMGenConfigure(PWM_Base(port,pin), Gen_PWM(port, pin), PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);	//Configure Generator Gen_PWM(port, pin)
	PWMGenPeriodSet(PWM_Base(port,pin), Gen_PWM(port, pin), period);  //Configure Generator Gen_PWM(port, pin) period (frequency)
}

void Duty_Cycle(unsigned long port, unsigned long pin, unsigned long duty_cycle)
{
	unsigned long PWM_Gen_Period = 0;
	unsigned long PWM_Duty_Cycle = 0;
	//PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, (duty_cycle*(PWMGenPeriodGet(PWM1_BASE,PWM_GEN_3))/100));	//Set duty cycle
	
	PWM_Gen_Period = PWMGenPeriodGet(PWM_Base(port,pin),Gen_PWM(port, pin));
	
	PWM_Duty_Cycle = duty_cycle*PWM_Gen_Period/1000;
	
	if(duty_cycle == 0)
		State_PWM(port,pin, false);
	else {
		State_PWM(port,pin, true);
		PWMPulseWidthSet(PWM_Base(port,pin), Encoded_Offset_PWM(port,pin),PWM_Duty_Cycle );	//Set duty cycle
	}
}
void State_PWM(unsigned long port, unsigned long pin, bool state)
{
	
	if (state == true)
	{
		PWMGenEnable(PWM_Base(port,pin), Gen_PWM(port, pin));
		PWMOutputState(PWM_Base(port,pin), BitId_PWM(port,pin), state);
		
	}
	else
	{
		PWMGenDisable(PWM_Base(port,pin), Gen_PWM(port, pin));
		PWMOutputState(PWM_Base(port,pin), BitId_PWM(port,pin), state);
	}
}
unsigned long Sys_Ctl_Enable(unsigned long port)
{
	unsigned long SysCtl;
		switch(port)
	{
		
		case GPIO_PORTA_BASE: SysCtl = SYSCTL_PERIPH_GPIOA; break;
		case GPIO_PORTF_BASE:	SysCtl = SYSCTL_PERIPH_GPIOF; break;			
		case GPIO_PORTB_BASE: SysCtl = SYSCTL_PERIPH_GPIOB; break;
		case GPIO_PORTC_BASE: SysCtl = SYSCTL_PERIPH_GPIOC; break;
		case GPIO_PORTD_BASE: SysCtl = SYSCTL_PERIPH_GPIOD; break;
		case GPIO_PORTE_BASE: SysCtl = SYSCTL_PERIPH_GPIOE; break;
		
		default: SysCtl = 0;
	}
	return SysCtl;
}

unsigned long Gen_PWM(unsigned long port,unsigned long pin)
{
	unsigned long Gen_PWM = 0;
	switch(port)
	{
		case GPIO_PORTA_BASE:
			switch(pin)
					{
						case GPIO_PIN_6:
						case GPIO_PIN_7:
							Gen_PWM = PWM_GEN_1; break;
						default: Gen_PWM = 0;
					}									
			break;
												
		case GPIO_PORTB_BASE:
			switch(pin)
					{
						case GPIO_PIN_4:
						case GPIO_PIN_5:
							Gen_PWM = PWM_GEN_1; break;
						case GPIO_PIN_6:
						case GPIO_PIN_7:
							Gen_PWM = PWM_GEN_0; break;
						default: Gen_PWM = 0;
					}					
			break;
					
		case GPIO_PORTC_BASE:
			switch(pin)
					{
						case GPIO_PIN_4:
						case GPIO_PIN_5:
							Gen_PWM = PWM_GEN_3; break;
						default: Gen_PWM = 0;
					}					
			break;
					
		case GPIO_PORTD_BASE:
			switch(pin)
					{
						case GPIO_PIN_0:
						case GPIO_PIN_1:
							Gen_PWM = PWM_GEN_3; break;
						default: Gen_PWM = 0;
					}									
			break;
	
		case GPIO_PORTE_BASE:
			switch(pin)
					{
						case GPIO_PIN_4:
						case GPIO_PIN_5:
							Gen_PWM = PWM_GEN_2; break;
						default: Gen_PWM = 0;
					}	
			break;
					
		case GPIO_PORTF_BASE:
		{
			switch(pin)
					{
						case GPIO_PIN_0:
						case GPIO_PIN_1:
							Gen_PWM = PWM_GEN_2; break;
						case GPIO_PIN_2:
						case GPIO_PIN_3:
							Gen_PWM = PWM_GEN_3; break;
						default: Gen_PWM = 0;
					}									
			break;
		}
		default: Gen_PWM = 0;
	}
	return Gen_PWM;
}

//Bit-wise ID for PWM nr:   PWM_OUT_nr_BIT
unsigned long BitId_PWM(unsigned long port,unsigned long pin)
{
	unsigned long ID_PWM_OUT = 0;
	switch(port)
	{
		case GPIO_PORTA_BASE:
			switch(pin)
					{
						case GPIO_PIN_6:
							ID_PWM_OUT = PWM_OUT_2_BIT; break;
						case GPIO_PIN_7:
							ID_PWM_OUT = PWM_OUT_3_BIT; break;
						default: ID_PWM_OUT = 0;
					}									
			break;
												
		case GPIO_PORTB_BASE:
			switch(pin)
					{
						case GPIO_PIN_4:
							ID_PWM_OUT = PWM_OUT_2_BIT; break;
						case GPIO_PIN_5:
							ID_PWM_OUT = PWM_OUT_3_BIT; break;
						case GPIO_PIN_6:
							ID_PWM_OUT = PWM_OUT_0_BIT; break;
						case GPIO_PIN_7:
							ID_PWM_OUT = PWM_OUT_1_BIT; break;
						default: ID_PWM_OUT = 0;
					}					
			break;
					
		case GPIO_PORTC_BASE:
			switch(pin)
					{
						case GPIO_PIN_4:
							ID_PWM_OUT = PWM_OUT_6_BIT; break;
						case GPIO_PIN_5:
							ID_PWM_OUT = PWM_OUT_7_BIT; break;
						default: ID_PWM_OUT = 0;
					}					
			break;
					
		case GPIO_PORTD_BASE:
			switch(pin)
					{
						case GPIO_PIN_0:
							ID_PWM_OUT = PWM_OUT_6_BIT; break;
						case GPIO_PIN_1:
							ID_PWM_OUT = PWM_OUT_7_BIT; break;
						default: ID_PWM_OUT = 0;
					}									
			break;
	
		case GPIO_PORTE_BASE:
			switch(pin)
					{
						case GPIO_PIN_4:
							ID_PWM_OUT = PWM_OUT_4_BIT; break;
						case GPIO_PIN_5:
							ID_PWM_OUT = PWM_OUT_5_BIT; break;
						default: ID_PWM_OUT = 0;
					}	
			break;
					
		case GPIO_PORTF_BASE:
		{
			switch(pin)
					{
						case GPIO_PIN_0:
							ID_PWM_OUT = PWM_OUT_4_BIT; break;
						case GPIO_PIN_1:
							ID_PWM_OUT = PWM_OUT_5_BIT; break;
						case GPIO_PIN_2:
							ID_PWM_OUT = PWM_OUT_6_BIT; break;
						case GPIO_PIN_3:
							ID_PWM_OUT = PWM_OUT_7_BIT; break;
						default: ID_PWM_OUT = 0;
					}									
			break;
		}
		default: ID_PWM_OUT = 0;
	}
	return ID_PWM_OUT;
}

//Encoded offset address of PWM nr: PWM_OUT_nr
unsigned long Encoded_Offset_PWM(unsigned long port,unsigned long pin)
{
	unsigned long GPIO_PIN_Encod = 0;
	switch(port)
	{
		case GPIO_PORTA_BASE:
			switch(pin)
					{
						case GPIO_PIN_6:
							GPIO_PIN_Encod = PWM_OUT_2; break;
						case GPIO_PIN_7:
							GPIO_PIN_Encod = PWM_OUT_3; break;
						default: GPIO_PIN_Encod = 0;
					}									
			break;
												
		case GPIO_PORTB_BASE:
			switch(pin)
					{
						case GPIO_PIN_4:
							GPIO_PIN_Encod = PWM_OUT_2; break;
						case GPIO_PIN_5:
							GPIO_PIN_Encod = PWM_OUT_3; break;
						case GPIO_PIN_6:
							GPIO_PIN_Encod = PWM_OUT_0; break;
						case GPIO_PIN_7:
							GPIO_PIN_Encod = PWM_OUT_1; break;
						default: GPIO_PIN_Encod = 0;
					}					
			break;
					
		case GPIO_PORTC_BASE:
			switch(pin)
					{
						case GPIO_PIN_4:
							GPIO_PIN_Encod = PWM_OUT_6; break;
						case GPIO_PIN_5:
							GPIO_PIN_Encod = PWM_OUT_7; break;
						default: GPIO_PIN_Encod = 0;
					}					
			break;
					
		case GPIO_PORTD_BASE:
			switch(pin)
					{
						case GPIO_PIN_0:
							GPIO_PIN_Encod = PWM_OUT_6; break;
						case GPIO_PIN_1:
							GPIO_PIN_Encod = PWM_OUT_7; break;
						default: GPIO_PIN_Encod = 0;
					}									
			break;
	
		case GPIO_PORTE_BASE:
			switch(pin)
					{
						case GPIO_PIN_4:
							GPIO_PIN_Encod = PWM_OUT_4; break;
						case GPIO_PIN_5:
							GPIO_PIN_Encod = PWM_OUT_5; break;
						default: GPIO_PIN_Encod = 0;
					}	
			break;
					
		case GPIO_PORTF_BASE:
		{
			switch(pin)
					{
						case GPIO_PIN_0:
							GPIO_PIN_Encod = PWM_OUT_4; break;
						case GPIO_PIN_1:
							GPIO_PIN_Encod = PWM_OUT_5; break;
						case GPIO_PIN_2:
							GPIO_PIN_Encod = PWM_OUT_6; break;
						case GPIO_PIN_3:
							GPIO_PIN_Encod = PWM_OUT_7; break;
						default: GPIO_PIN_Encod = 0;
					}									
			break;
		}
		default: GPIO_PIN_Encod = 0;
	}
	return GPIO_PIN_Encod;
}

unsigned long PWM_Base(unsigned long port,unsigned long pin) //pwm module
{
	unsigned long PWM_Base = 0;
	switch(port)
		{
		case GPIO_PORTA_BASE:
		case GPIO_PORTF_BASE:
			PWM_Base = PWM1_BASE; break;						
		case GPIO_PORTB_BASE:
		case GPIO_PORTC_BASE:
		case GPIO_PORTD_BASE:
		case GPIO_PORTE_BASE:
			PWM_Base = PWM0_BASE;	break;
		default: PWM_Base =0;
	}
		return PWM_Base;
}

void GPIO_Configure_PWM(unsigned long port,unsigned long pin)
{
	switch(port)
	{
		case GPIO_PORTA_BASE:
			switch(pin)
					{
						case GPIO_PIN_6:
						{GPIOPinConfigure(GPIO_PA6_M1PWM2); GPIOPinTypePWM(GPIO_PORTA_BASE,GPIO_PIN_6);}
							break;
						case GPIO_PIN_7:
						{GPIOPinConfigure(GPIO_PA7_M1PWM3); GPIOPinTypePWM(GPIO_PORTA_BASE,GPIO_PIN_7);}
							break;
						default: break;
					}									
			break;
												
		case GPIO_PORTB_BASE:
			switch(pin)
					{
						case GPIO_PIN_4:
						{GPIOPinConfigure(GPIO_PB4_M0PWM2);  GPIOPinTypePWM(GPIO_PORTB_BASE,GPIO_PIN_4);}
							break;
						case GPIO_PIN_5:
						{GPIOPinConfigure(GPIO_PB5_M0PWM3); GPIOPinTypePWM(GPIO_PORTB_BASE,GPIO_PIN_5);}
							break;
						case GPIO_PIN_6:
						{GPIOPinConfigure(GPIO_PB6_M0PWM0); GPIOPinTypePWM(GPIO_PORTB_BASE,GPIO_PIN_6);}
							break;
						case GPIO_PIN_7:
						{GPIOPinConfigure(GPIO_PB7_M0PWM1); GPIOPinTypePWM(GPIO_PORTB_BASE,GPIO_PIN_7);}
							break;
						default: break;
					}					
			break;
					
		case GPIO_PORTC_BASE:
			switch(pin)
					{
						case GPIO_PIN_4:
						{GPIOPinConfigure(GPIO_PC4_M0PWM6); GPIOPinTypePWM(GPIO_PORTC_BASE,GPIO_PIN_4);}
							break;
						case GPIO_PIN_5:
						{GPIOPinConfigure(GPIO_PC5_M0PWM7); GPIOPinTypePWM(GPIO_PORTC_BASE,GPIO_PIN_5);}
							break;
						default: break;
					}					
			break;
					
		case GPIO_PORTD_BASE:
			switch(pin)
					{
						case GPIO_PIN_0:
						{GPIOPinConfigure(GPIO_PD0_M0PWM6);  GPIOPinTypePWM(GPIO_PORTD_BASE,GPIO_PIN_0);}
							break;
						case GPIO_PIN_1:
						{GPIOPinConfigure(GPIO_PD1_M0PWM7); GPIOPinTypePWM(GPIO_PORTD_BASE,GPIO_PIN_1);}
							break;
						default: break;
					}									
			break;
	
		case GPIO_PORTE_BASE:
			switch(pin)
					{
						case GPIO_PIN_4:
						{GPIOPinConfigure(GPIO_PE4_M0PWM4); GPIOPinTypePWM(GPIO_PORTE_BASE,GPIO_PIN_4);}
							break;
						case GPIO_PIN_5:
						{GPIOPinConfigure(GPIO_PE5_M0PWM5); GPIOPinTypePWM(GPIO_PORTE_BASE,GPIO_PIN_5);}
							break;
						default: break;
					}	
			break;
					
		case GPIO_PORTF_BASE:
		{
			switch(pin)
					{
						case GPIO_PIN_0:
						{GPIOPinConfigure(GPIO_PF0_M1PWM4); GPIOPinTypePWM(GPIO_PORTF_BASE,GPIO_PIN_0);}
							break;
						case GPIO_PIN_1:
						{GPIOPinConfigure(GPIO_PF1_M1PWM5); GPIOPinTypePWM(GPIO_PORTF_BASE,GPIO_PIN_1);}
							break;
						case GPIO_PIN_2:
						{GPIOPinConfigure(GPIO_PF2_M1PWM6); GPIOPinTypePWM(GPIO_PORTF_BASE,GPIO_PIN_2);}
							break;
						case GPIO_PIN_3:
						{GPIOPinConfigure(GPIO_PF3_M1PWM7); GPIOPinTypePWM(GPIO_PORTF_BASE,GPIO_PIN_3);}
							break;
						default: break;
					}									
			break;
		}
		default:
						break;
	}
}

unsigned long Clock_Ticks(unsigned long period)
{
	// only for PWM_DIV_32 - SYSCTL_PWMDIV_32
	unsigned long ui32SysClock = SysCtlClockGet();
	return (((ui32SysClock / (1000 * PWM_DIV_for32)) * period) - 1);
}
unsigned long Periph_PWM(unsigned long port, unsigned long pin)
{
	unsigned long periph_pwm = 0;
	switch(port)
		{
		case GPIO_PORTA_BASE:
		case GPIO_PORTF_BASE:
			periph_pwm = SYSCTL_PERIPH_PWM1; break;						
		case GPIO_PORTB_BASE:
		case GPIO_PORTC_BASE:
		case GPIO_PORTD_BASE:
		case GPIO_PORTE_BASE:
			periph_pwm = SYSCTL_PERIPH_PWM0;	break;
		default: periph_pwm =0;
	}
		return periph_pwm;
}

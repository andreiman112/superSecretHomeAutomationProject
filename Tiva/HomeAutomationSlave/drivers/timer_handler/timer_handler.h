#ifndef TIMER
#define TIMER

void TIMER_delay(unsigned long delay_time_ms);
void TIMER_delay_No_Int(unsigned long delay_time_ms);
void TIMER_Wide_0_Init(void);
void Init_Timer(unsigned long base, unsigned long timer, unsigned long period);
char getPrioIndex(unsigned long base);
char getINTTIMER_Address(unsigned long base, unsigned long timer);
unsigned long getPeriphTimerAddress(unsigned long port);
void TIMER_2_Init(void);
unsigned long TIMER_reload_calculator(unsigned long delay_time_ms);
#endif
//EOF

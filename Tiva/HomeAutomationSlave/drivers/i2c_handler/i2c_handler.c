#include "stdbool.h"
#include "stdint.h"

#include "inc/hw_memmap.h"
#include "inc/hw_ints.h" 

#include "i2c_handler.h"

#include "i2c_handler.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/i2c.h"

#include "inc/hw_i2c.h"
#include "display.h"
#define SLAVE_ADDRESS_EXT 0x4E
#define NUM_OF_I2CBYTES 255

#define I2C0_MCS (*((volatile uint32_t*)0x40020004))
#define Acc_Slave_Adress (0x1D) //MMA 7455 accelerometer adress is 0x1D, see datasheet page 23: $0D: I2C Device Address
#define Master_Rx_Slave_Tx (1) //Configures master to receive data from the slave
#define Master_Tx_Slave_Rx (0) //Configures master to send data to the slave
#define I2C_Rate_100kbps (0) //Configure I2C transfer rate to normal mode 100 kbps

void I2C_Master_Wait(void)
{
     while (I2CMasterBusy(I2C0_BASE))
        ;
}
unsigned long I2C_Read(unsigned long I2C_Base, unsigned char Slave_Address, unsigned long Register)
{
    unsigned char error_nr = 0;
    unsigned long Register_Read_Value = 0;  
    //Step 1.1. Set Slave adress and Write mode (R/W bit = 0)
    I2CMasterSlaveAddrSet(I2C_Base, Slave_Address, 0); //Set slave address and send mode

    //Step 1.2. Send the 8bit register adress to read from
    I2CMasterDataPut(I2C_Base, Register); //Send the register adress to the Slave device

    while (I2CMasterBusBusy(I2C_Base)) {
    }

    I2CMasterControl(I2C_Base, 7);
    I2C_Master_Wait();

    //Step 1.1. Set Slave adress and Write mode (R/W bit = 0)
    I2CMasterSlaveAddrSet(I2C_Base, Slave_Address, 1); //Set slave address and send mode
    while (I2CMasterBusBusy(I2C_Base)) {
    }
    error_nr = I2CMasterErr(I2C_Base);
		if(error_nr){}
    I2CMasterControl(I2C_Base, I2C_MASTER_CMD_SINGLE_RECEIVE);
    I2C_Master_Wait();
    
    Register_Read_Value = I2CMasterDataGet(I2C_Base);

    return Register_Read_Value;
}

unsigned long Get_I2C_Periph(unsigned long I2C_BASE)
{
    char index = (I2C_BASE & (0xf << 12)) >> 12;
    return SYSCTL_PERIPH_I2C0 + index;
}

unsigned long Get_Port_Periph(unsigned long I2C_BASE)
{
    char index = (I2C_BASE & (0xf) << 12) >> 12;

    switch (index) {
    case 0:
        return SYSCTL_PERIPH_GPIOB;
    case 1:
        return SYSCTL_PERIPH_GPIOA;
    case 2:
        return SYSCTL_PERIPH_GPIOE;
    case 3:
        return SYSCTL_PERIPH_GPIOD;
    }
    return 0;
}
unsigned long Get_GPIO_SCL(unsigned long I2C_BASE)
{
    char index = (I2C_BASE & (0xf) << 12) >> 12;

    switch (index) {
    case 0:
        return GPIO_PB2_I2C0SCL;
    case 1:
        return GPIO_PA6_I2C1SCL;
    case 2:
        return GPIO_PE4_I2C2SCL;
    case 3:
        return GPIO_PD0_I2C3SCL;
    }
    return 0;
}
unsigned long Get_Port_Base(unsigned long I2C_BASE)
{
    char index = (I2C_BASE & (0xf) << 12) >> 12;

    switch (index) {
    case 0:
        return GPIO_PORTB_BASE;
    case 1:
        return GPIO_PORTA_BASE;
    case 2:
        return GPIO_PORTE_BASE;
    case 3:
        return GPIO_PORTD_BASE;
    }
    return 0;
}
char Get_SCL_Pin(unsigned long I2C_BASE)
{
    char index = (I2C_BASE & (0xf) << 12) >> 12;

    switch (index) {
    case 0:
        return GPIO_PIN_2;
    case 1:
        return GPIO_PIN_6;
    case 2:
        return GPIO_PIN_4;
    case 3:
        return GPIO_PIN_0;
    }
    return 1;
}
char Get_SDA_Pin(unsigned long I2C_BASE)
{
    char index = (I2C_BASE & (0xf) << 12) >> 12;

    switch (index) {
    case 0:
        return GPIO_PIN_3;
    case 1:
        return GPIO_PIN_7;
    case 2:
        return GPIO_PIN_5;
    case 3:
        return GPIO_PIN_1;
    }
    return 1;
}
void I2C_Init_LuminositySensor(unsigned char Slave_Address)
{
    unsigned char error_nr = 0; 
    unsigned long Register_Address = 0x80;
    unsigned long Register_Value = 0x3;

    I2CMasterSlaveAddrSet(I2C0_BASE, Slave_Address, 0); //Set slave address and send mode

    I2CMasterDataPut(I2C0_BASE, Register_Address); //Send the register adress to the Slave device

    while (I2CMasterBusBusy(I2C0_BASE)) {    }

    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START); I2C_Master_Wait();

    I2CMasterDataPut(I2C0_BASE, Register_Value); //Send the register adress to the Slave device
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH); I2C_Master_Wait();
 
    error_nr = I2CMasterErr(I2C0_BASE);
		if(error_nr){
    Display_NewLine();
    Display_String("lum sensor err: ");
    Display_Decimal(error_nr);
		}
    
}
void I2C_Init(unsigned long I2C_BASE, char FastMode)
{
    //Init PB2 as I2C_0 SCL
    //Init PB3 as I2C_0 SDA
    unsigned long gpio_scl;
    unsigned long gpio_sda;
    unsigned long port_base, periph, port_periph;
    char SCL_pin;
    char SDA_pin;
    gpio_scl = Get_GPIO_SCL(I2C_BASE);
    gpio_sda = gpio_scl + 0x400;
    port_base = Get_Port_Base(I2C_BASE);
    SCL_pin = Get_SCL_Pin(I2C_BASE);
    SDA_pin = Get_SDA_Pin(I2C_BASE);
    periph = Get_I2C_Periph(I2C_BASE);
    port_periph = Get_Port_Periph(I2C_BASE);
    SysCtlPeripheralEnable(periph); //The I2C0 peripheral must be enabled for use.
    SysCtlPeripheralEnable(port_periph); //The GPIOB peripheral must be enabled for use.

    GPIOPinConfigure(gpio_scl);
    GPIOPinConfigure(gpio_sda);
    GPIOPinTypeI2CSCL(port_base, SCL_pin);
    GPIOPinTypeI2C(port_base, SDA_pin);
    //GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_2, GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU); //Configure PUR for PB2
    GPIOPadConfigSet(port_base, SDA_pin, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_OD); //Configure OD for PB3
    //GPIODirModeSet(GPIO_PORTB_BASE, GPIO_PIN_2|GPIO_PIN_3, GPIO_DIR_MODE_HW);	//Set direction by HW for PB2 and PB3

    I2CMasterInitExpClk(I2C_BASE, SysCtlClockGet(), FastMode); //Set System clock and normal (100 kbps) transfer rate for I2C_0
}
void I2C0_Init(void)
{
    //Init PB2 as I2C_0 SCL
    //Init PB3 as I2C_0 SDA
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0); //The I2C0 peripheral must be enabled for use.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB); //The GPIOB peripheral must be enabled for use.

    GPIOPinConfigure(GPIO_PB2_I2C0SCL);
    GPIOPinConfigure(GPIO_PB3_I2C0SDA);
    GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
    GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);
    //GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_2, GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU); //Configure PUR for PB2
    GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_3, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_OD); //Configure OD for PB3
    //GPIODirModeSet(GPIO_PORTB_BASE, GPIO_PIN_2|GPIO_PIN_3, GPIO_DIR_MODE_HW);	//Set direction by HW for PB2 and PB3

    I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), 1); //Set System clock and normal (100 kbps) transfer rate for I2C_0
}

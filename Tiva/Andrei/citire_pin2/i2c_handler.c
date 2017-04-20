#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"

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

#define I2C0_MCS (*((volatile uint32_t *)0x40020004))
	#define Acc_Slave_Adress			(0x1D)//MMA 7455 accelerometer adress is 0x1D, see datasheet page 23: $0D: I2C Device Address
#define Master_Rx_Slave_Tx		(1)		//Configures master to receive data from the slave
#define Master_Tx_Slave_Rx		(0)		//Configures master to send data to the slave
#define I2C_Rate_100kbps			(0)		//Configure I2C transfer rate to normal mode 100 kbps 
/*
void INIT_I2C(void)
{
	//
// Setup System Clock for 120MHz
//
	unsigned long g_ui8MasterBytes=2;
unsigned long ui32SysClock = SysCtlClockFreqSet((SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_XTAL_25MHZ |
SYSCTL_CFG_VCO_480), 120000000);
//
// Stop the Clock, Reset and Enable I2C Module
// in Master Function
//
SysCtlPeripheralDisable(SYSCTL_PERIPH_I2C2);
SysCtlPeripheralReset(SYSCTL_PERIPH_I2C2);
SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C2);
//
// Wait for the Peripheral to be ready for programming
//
while(!SysCtlPeripheralReady(SYSCTL_PERIPH_I2C2));
//
// Initialize and Configure the Master Module
//
I2CMasterInitExpClk(I2C2_BASE, ui32SysClock, false);
//
// Enable Interrupts for Arbitration Lost, Stop, NAK, Clock Low
// Timeout and Data.
//
I2CMasterIntEnableEx(I2C2_BASE, (I2C_MASTER_INT_ARB_LOST |
I2C_MASTER_INT_STOP | I2C_MASTER_INT_NACK |
I2C_MASTER_INT_TIMEOUT | I2C_MASTER_INT_DATA));
//
// Enable the Interrupt in the NVIC from I2C Master
//
// Send the Slave Address with RnW as Transmit
// and First Data Byte. Based on Number of bytes the
// command would be either START or FINISH
//
I2CMasterSlaveAddrSet(I2C2_BASE, SLAVE_ADDRESS_EXT, false);
I2CMasterDataPut(I2C2_BASE,0xA);

I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_BURST_SEND_START);
/*if(g_ui8MasterBytes == g_ui8MasterBytesLength)
{
I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_SINGLE_SEND);
}
else
{
I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_BURST_SEND_START);
} 
 
  Display_String("5");	
}*/
void I2C_Master_Wait(void)
{
	static unsigned long cnt=0;
	 Display_NewLine();	
	
	Display_String("cnt ");
	Display_Decimal(++cnt);
	Display_String("mcs: ");
	Display_Decimal(I2C0_MCS);
	while(I2CMasterBusy(I2C0_BASE));
}
unsigned char I2C_Write(unsigned char Slave_Address, unsigned char Register_Address, unsigned char Register_Write_Value)
{	
	
	unsigned char error_nr = 0;
	//Step 0. Send a start condition - Test
	I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);
	
	//Step 1. Set Slave adress and Write mode (R/W bit = 0)
	//I2CMasterSlaveAddrSet(I2C0_BASE,Slave_Address,Master_Tx_Slave_Rx);	//Set slave address and send mode
	I2CMasterSlaveAddrSet(I2C0_BASE,Slave_Address,0);	//Set slave address and send mode
	
	//Step 2. Send the 8bit register adress to write to
	I2CMasterDataPut(I2C0_BASE, Register_Address); //Send the register adress to the Slave device
	//while(I2CMasterBusBusy(I2C0_BASE)){}
	
	I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);	//0x00000003
	//I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_CONT); //Try also this 0x00000001
	
//	while(I2CMasterBusBusy(I2C0_BASE)){}
	while(I2CMasterBusy(I2C0_BASE)){}
		
	error_nr = I2CMasterErr(I2C0_BASE);
		if(error_nr)	//If any error
	{
		if(error_nr&0x10)	//If arbitration lost error (bit 4 is set)
		{
			//I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_ERROR_STOP); //0x00000004
		}
		return 0;
	}
	else
	{
		//Step 3. Send data to write on register
		I2CMasterDataPut(I2C0_BASE, Register_Write_Value);	//Send the register value to the Slave device
		I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH); //0x00000005
		while(I2CMasterBusy(I2C0_BASE)){}
			
		error_nr = I2CMasterErr(I2C0_BASE);
			Display_NewLine();	
	Display_String("err nr: ");
	Display_Decimal(error_nr);
		if(error_nr !=0)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
		 Display_NewLine();	
	Display_String("1 mcs: ");
	Display_Decimal(I2C0_MCS);
}
unsigned long I2C_ReadTemp(unsigned char Slave_Address, unsigned char Register_Address)
{
	unsigned char error_nr = 0;
	unsigned long Register_Read_Value = 0;
	
	//Step 1.1. Set Slave adress and Write mode (R/W bit = 0)
	I2CMasterSlaveAddrSet(I2C0_BASE,Slave_Address,1);	//Set slave address and send mode

	//Step 1.2. Send the 8bit register adress to read from
	//I2CMasterDataPut(I2C0_BASE, Register_Address); //Send the register adress to the Slave device
 
    while(I2CMasterBusBusy(I2C0_BASE)){}
		  
	 I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);
	 I2C_Master_Wait();
	  error_nr = I2CMasterErr(I2C0_BASE);
		if(error_nr==0){
			Register_Read_Value = I2CMasterDataGet(I2C0_BASE);
	
    Display_NewLine();	
		Display_String("temp: ");
		Display_Decimal(Register_Read_Value); 
		}
		else
		{
		 Display_NewLine();	
		Display_String("err: ");
		Display_Decimal(error_nr); 
		}
}

unsigned long I2C_Read2(unsigned char Slave_Address, unsigned char Register_Address)
{
	unsigned char error_nr = 0;
	unsigned long Register_Read_Value = 0;
	
	//Step 1.1. Set Slave adress and Write mode (R/W bit = 0)
	I2CMasterSlaveAddrSet(I2C0_BASE,Slave_Address,0);	//Set slave address and send mode

	//Step 1.2. Send the 8bit register adress to read from
	I2CMasterDataPut(I2C0_BASE, Register_Address); //Send the register adress to the Slave device
 
    while(I2CMasterBusBusy(I2C0_BASE)){}
		  
	 I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);
	 I2C_Master_Wait();
	  error_nr = I2CMasterErr(I2C0_BASE);
		
    Display_NewLine();	
		Display_String("2: ");
		Display_Decimal(error_nr); 
		
}
unsigned long I2C_Read(unsigned char Slave_Address, unsigned char Register_Address)
{
	unsigned char error_nr = 0;
	unsigned long Register_Read_Value = 0;
	
	//Step 1.1. Set Slave adress and Write mode (R/W bit = 0)
	I2CMasterSlaveAddrSet(I2C0_BASE,Slave_Address,0);	//Set slave address and send mode

	//Step 1.2. Send the 8bit register adress to read from
	I2CMasterDataPut(I2C0_BASE, Register_Address); //Send the register adress to the Slave device
 
    while(I2CMasterBusBusy(I2C0_BASE)){}
		
	//I2C0_MCS = I2C_MASTER_CMD_SINGLE_SEND;
	//HWREG(I2C0_BASE + I2C_O_MCS) = I2C_MASTER_CMD_SINGLE_SEND;
	
	//I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);
	//I2C_Master_Wait();
	error_nr = I2CMasterErr(I2C0_BASE);
		
		Display_NewLine();
		Display_String("1: ");
		Display_Decimal(error_nr);
		
		 Display_NewLine();	
	Display_String("1 mcs: ");
	Display_Decimal(I2C0_MCS);
	 //Step 2. Send the 8bit register adress to read
	 I2CMasterDataPut(I2C0_BASE, Register_Address); //Send the register adress to the Slave device
 
	 I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);
	I2C_Master_Wait();
	error_nr = I2CMasterErr(I2C0_BASE);
		
    Display_NewLine();	
		Display_String("2: ");
		Display_Decimal(error_nr); 
		
	//Step 3. Set Read mode (R/W bit = 1)
	I2CMasterSlaveAddrSet(I2C0_BASE,Slave_Address,Master_Rx_Slave_Tx);
	
	 Display_NewLine();	
	Display_String("2 mcs: ");
	Display_Decimal(I2C0_MCS);
	I2C_Master_Wait();
	error_nr = I2CMasterErr(I2C0_BASE);
	
  Display_NewLine();	
	Display_String("3: ");
		Display_Decimal(error_nr);
		
	 Display_NewLine();		 
	Display_String("3 mcs: ");	
	Display_Decimal(I2C0_MCS);
		
	Register_Read_Value = I2CMasterDataGet(I2C0_BASE);
	I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);
	
	Display_NewLine();	
	Display_String("temp....: ");
		Display_Decimal(Register_Read_Value);
	if(I2CMasterErr(I2C0_BASE) == 0)
	{
		return Register_Read_Value;
	}
	else
	{
		return 0;
	}
	/*
		SINGLE BYTE READ
		The MMA7455L has an 10-bit ADC that can sample, convert and return sensor data on request. The transmission of an 8-bit
		command begins on the falling edge of SCL. After the eight clock cycles are used to send the command, note that the data returned
		is sent with the MSB first once the data is received. Figure 7 shows the timing diagram for the accelerometer 8-bit I2C
		read operation. The Master (or MCU) transmits a start condition (ST) to the MMA7455L, slave address ($1D), with the R/W bit
		set to “0” for a write, and the MMA7455L sends an acknowledgement. Then the Master (or MCU) transmits the 8-bit address of
		the register to read and the MMA7455L sends an acknowledgement. The Master (or MCU) transmits a repeated start condition
		(SR) and then addresses the MMA7455L ($1D) with the R/W bit set to “1” for a read from the previously selected register. The
		Slave then acknowledges and transmits the data from the requested register. The Master does not acknowledge (NAK) it received
		the transmitted data, but transmits a stop condition to end the data transfer.
	*/
}
void I2C_Init(void)
{
	//Init PB2 as I2C_0 SCL
	//Init PB3 as I2C_0 SDA	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);			//The I2C0 peripheral must be enabled for use.
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);		//The GPIOB peripheral must be enabled for use.
	
	GPIOPinConfigure(GPIO_PB2_I2C0SCL);
	GPIOPinConfigure(GPIO_PB3_I2C0SDA);
	GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
	GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);
	//GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_2, GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU); //Configure PUR for PB2
	GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_3, GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_OD); //Configure OD for PB3
	//GPIODirModeSet(GPIO_PORTB_BASE, GPIO_PIN_2|GPIO_PIN_3, GPIO_DIR_MODE_HW);	//Set direction by HW for PB2 and PB3

	I2CMasterInitExpClk(I2C0_BASE,SysCtlClockGet(),0);		//Set System clock and normal (100 kbps) transfer rate for I2C_0
	
}
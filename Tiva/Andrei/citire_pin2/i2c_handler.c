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
 
void I2C_Master_Wait(void)
{
	static unsigned long cnt=0;
//	 Display_NewLine();	
	
	//Display_String("cnt ");
///	Display_Decimal(++cnt);
//	Display_String("mcs: ");
//	Display_Decimal(I2C0_MCS);
	while(I2CMasterBusy(I2C0_BASE));
}
void I2C_ConfTemp(unsigned char Slave_Address, unsigned char Register_Address)
 {
	 	unsigned char error_nr = 0;
	unsigned long Register_Value=128;
 	//Step 1.1. Set Slave adress and Write mode (R/W bit = 0)
	I2CMasterSlaveAddrSet(I2C0_BASE,Slave_Address,0);	//Set slave address and send mode

	//Step 1.2. Send the 8bit register adress to read from
	I2CMasterDataPut(I2C0_BASE,Register_Address ); //Send the register adress to the Slave device
 
    while(I2CMasterBusBusy(I2C0_BASE)){}
		  
	    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);  I2C_Master_Wait();
	
	
			I2CMasterDataPut(I2C0_BASE,Register_Value); //Send the register adress to the Slave device
       I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);  I2C_Master_Wait();
	
		
		 // I2CMasterDataPut(I2C0_BASE,0xF0 ); //Send the register adress to the Slave device
      //I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_CONT/*I2C_MASTER_CMD_BURST_SEND_START*/);  I2C_Master_Wait();
	 
	 
	 	 
		 error_nr = I2CMasterErr(I2C0_BASE);

    Display_NewLine();	
		Display_String("2: ");
		Display_Decimal(error_nr); 
	
 }
 unsigned long I2C_ReadLumSimp(unsigned char Slave_Address, unsigned long Reg)
{
	//!!!!!!!!!! dara R/W==R nu face nimic la DataPut
	unsigned char error_nr = 0;
	unsigned long Register_Read_Value = 0;
	
	//Step 1.1. Set Slave adress and Write mode (R/W bit = 0)
	I2CMasterSlaveAddrSet(I2C0_BASE,Slave_Address,1);	//Set slave address and send mode

	//Step 1.2. Send the 8bit register adress to read from
	I2CMasterDataPut(I2C0_BASE, Reg); //Send the register adress to the Slave device
 
    while(I2CMasterBusBusy(I2C0_BASE)){}
		  
	 I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);
	 I2C_Master_Wait();
	  error_nr = I2CMasterErr(I2C0_BASE);
		if(error_nr==0){
			Register_Read_Value = I2CMasterDataGet(I2C0_BASE);
	
    Display_NewLine();	
		Display_String("lumi: ");
		Display_Decimal(Register_Read_Value); 
		}
		else
		{
		 Display_NewLine();	
		Display_String("err: ");
		Display_Decimal(error_nr); 
		}
			return Register_Read_Value;
}
unsigned long I2C_ReadTemp(unsigned char Slave_Address, unsigned char Register_Address)
{
	//!!!!!!!!!! dara R/W==R nu face nimic la DataPut
	unsigned char error_nr = 0;
	unsigned long Register_Read_Value = 0;
	
	//Step 1.1. Set Slave adress and Write mode (R/W bit = 0)
	I2CMasterSlaveAddrSet(I2C0_BASE,Slave_Address,1);	//Set slave address and send mode

	//Step 1.2. Send the 8bit register adress to read from
	I2CMasterDataPut(I2C0_BASE, Register_Address); //Send the register adress to the Slave device
 
    while(I2CMasterBusBusy(I2C0_BASE)){}
		  
	 I2CMasterControl(I2C0_BASE, 0x0000000f);
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
			return Register_Read_Value;
}

unsigned long I2C_RealLum(unsigned char Slave_Address)
{
	unsigned char error_nr = 0;
	unsigned long Register_Read_Value = 0;
	unsigned long Register_Address=0x80;
	unsigned long Register_Value=0x3;
	//Step 1.1. Set Slave adress and Write mode (R/W bit = 0)
	I2CMasterSlaveAddrSet(I2C0_BASE,Slave_Address,0);	//Set slave address and send mode

	//Step 1.2. Send the 8bit register adress to read from
	I2CMasterDataPut(I2C0_BASE,Register_Address ); //Send the register adress to the Slave device
 
   while(I2CMasterBusBusy(I2C0_BASE)){}
		 
			I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);  I2C_Master_Wait();
	
	
			I2CMasterDataPut(I2C0_BASE,Register_Value); //Send the register adress to the Slave device
       I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);  I2C_Master_Wait();
	
		
		 // I2CMasterDataPut(I2C0_BASE,0xF0 ); //Send the register adress to the Slave device
      //I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_CONT/*I2C_MASTER_CMD_BURST_SEND_START*/);  I2C_Master_Wait();
	 
	 
	 	 
		 error_nr = I2CMasterErr(I2C0_BASE);

    Display_NewLine();	
		Display_String("2: ");
		Display_Decimal(error_nr); 
			
	
		return Register_Read_Value;
}
unsigned long I2C_RealLum2(unsigned char Slave_Address, unsigned long Reg)
{
	unsigned char error_nr = 0;
	unsigned long Register_Read_Value = 0;
	unsigned long Register_Address=0xAC;
	unsigned long Register_Value=0x3;
	//Step 1.1. Set Slave adress and Write mode (R/W bit = 0)
	I2CMasterSlaveAddrSet(I2C0_BASE,Slave_Address,0);	//Set slave address and send mode

	//Step 1.2. Send the 8bit register adress to read from
	I2CMasterDataPut(I2C0_BASE,Reg ); //Send the register adress to the Slave device
 
   while(I2CMasterBusBusy(I2C0_BASE)){}
		 
			I2CMasterControl(I2C0_BASE, 7);  I2C_Master_Wait();
	
	 
	 	 //Step 1.1. Set Slave adress and Write mode (R/W bit = 0)
	  I2CMasterSlaveAddrSet(I2C0_BASE,Slave_Address,1);	//Set slave address and send mode
    while(I2CMasterBusBusy(I2C0_BASE)){}
		error_nr = I2CMasterErr(I2C0_BASE);
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);  I2C_Master_Wait();
			
		Register_Read_Value = I2CMasterDataGet(I2C0_BASE);
	
    Display_NewLine();	
		Display_String("lumi: ");
		Display_Decimal(Register_Read_Value); 
	
	
		return Register_Read_Value;
}
 void I2C_ReadTEMPCONF2(unsigned char Slave_Address)
{
	unsigned char error_nr = 0;
	unsigned long Register_Read_Value = -1;
	unsigned long Register_Address=0x1; 
			
	 
	  //while(I2CMasterBusBusy(I2C0_BASE)){}	 
//	 I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_CONT); I2C_Master_Wait(); 
	
   I2CMasterSlaveAddrSet(I2C0_BASE,Slave_Address,0);	
   I2CMasterDataPut(I2C0_BASE,  Register_Address); //Send the register adress to the Slave device  	
	 I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START); I2C_Master_Wait(); 
	
   I2CMasterSlaveAddrSet(I2C0_BASE,Slave_Address,1);	
   I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE); I2C_Master_Wait(); 
	 Register_Read_Value = I2CMasterDataGet(I2C0_BASE);
	
   Display_NewLine();	
	 Display_String("3: ");
	 Display_Decimal(Register_Read_Value);	  
	 
	  
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

	I2CMasterInitExpClk(I2C0_BASE,SysCtlClockGet(),1);		//Set System clock and normal (100 kbps) transfer rate for I2C_0
}

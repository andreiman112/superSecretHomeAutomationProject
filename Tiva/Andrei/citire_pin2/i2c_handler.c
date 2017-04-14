#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"

#include "i2c_handler.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/i2c.h"

void INIT_I2C(void)
{
///
// Initialize Master and Slave
//
I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), true);
//
// Specify slave address
//
I2CMasterSlaveAddrSet(I2C0_BASE,/*0x4D+0x01=0x4E*/0x4E, false);
//
// Place the character to be sent in the data register
//
I2CMasterDataPut(I2C0_BASE, 'Q');
//
// Initiate send of character from Master to Slave
//
I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);
//
// Delay until transmission completes
//
while(I2CMasterBusBusy(I2C0_BASE))
{
}

}

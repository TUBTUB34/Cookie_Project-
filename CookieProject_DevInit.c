// Filename:            CookieProject_DeviceInit.c
//
// Description:	        Initialization code
//
// Version:             1.0
//
// Target:              TMS320F28379D
//
// Author:              Alex Weir
//
// Date:                25 October 2024

#include "Headers/F2837xD_device.h"

void InitScibGpio(void);
void InitScib(void);
void InitScicGpio(void);
void InitScic(void);
void clocks_init(void);
void InitI2CGpio(void);
void InitI2C(void);
void DelayS(int s);
extern void DelayUs(Uint16);

void DeviceInit(void)
{

    EALLOW; // Allow access to protected registers
    // Configure D10 (blue LED)
    GpioCtrlRegs.GPAMUX2.bit.GPIO31 = 0; // Set pin as GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO31 = 1;  // Set GPIO as output
    GpioDataRegs.GPASET.bit.GPIO31 = 1;  // Initialize output value to "1" (LED off)

    GpioCtrlRegs.GPBMUX1.bit.GPIO34 = 0; //D9 (red LED)
    GpioCtrlRegs.GPBDIR.bit.GPIO34 = 1;
    GpioDataRegs.GPBCLEAR.bit.GPIO34 = 1;

    EDIS;

    // Initialize SPI
    clocks_init();

    // Initialize SCI-B
    // For raspberry PI
    InitScibGpio();
    InitScib();

    // Initialize SCI-C
    // For robotArm
    InitScicGpio();
    InitScic();

    // Initialize ADC
    //InitADCGpio();
    //InitADC();

    // Initialize I2C
    // InitI2CGpio();
    // InitI2C();
}

void clocks_init()
{
    EALLOW;
    CpuSysRegs.PCLKCR7.bit.SCI_B = 1;
    CpuSysRegs.PCLKCR7.bit.SCI_C = 1;
    CpuSysRegs.PCLKCR9.bit.I2C_A = 1;
    EDIS;
}

//void init_arm()
//{
//    EALLOW;
//    GpioCtrlRegs.GPBGMUX2.bit.GPIO56 = 0;  // General-purpose MUX selection
//    GpioCtrlRegs.GPBMUX2.bit.GPIO56 = 0;   // General-purpose function
//    GpioCtrlRegs.GPBDIR.bit.GPIO56 = 1;    // Set GPIO56 as output
//    GpioDataRegs.GPBSET.bit.GPIO56 = 1;  // Drive GPIO56 High
//
//    DelayS(1);
//    GpioDataRegs.GPBCLEAR.bit.GPIO56 = 1;
//    DelayUs(9);
//    GpioDataRegs.GPBSET.bit.GPIO56 = 1;
//    DelayUs(8000);
//    GpioDataRegs.GPBCLEAR.bit.GPIO56 = 1;
//    DelayUs(9);
//    GpioDataRegs.GPBSET.bit.GPIO56 = 1;
//    DelayS(1);
//    EDIS;
//
//
//}

void InitScibGpio(void)
{
    EALLOW;

    // Configure GPIO19 as SCIRXDB (SCI-B Receive)
    GpioCtrlRegs.GPAGMUX2.bit.GPIO19 = 0;
    GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 2;    // Set GPIO19 to SCIRXDB
    GpioCtrlRegs.GPADIR.bit.GPIO19 = 0;     // Set GPIO19 as input for RX
    GpioCtrlRegs.GPAQSEL2.bit.GPIO19 = 3;   // Async

    // Configure GPIO18 as SCITXDB (SCI-B Transmit)
    GpioCtrlRegs.GPAGMUX2.bit.GPIO18 = 0;
    GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 2;    // Set GPIO18 to SCITXDB
    GpioCtrlRegs.GPADIR.bit.GPIO18 = 1;     // Set GPIO18 as output for TX
    GpioCtrlRegs.GPAQSEL2.bit.GPIO18 = 3;   // Async

    EDIS;
}

void InitScicGpio(void)
{
    EALLOW;

    // Configure GPIO139 as SCIRXDc (SCI-c Receive)
    GpioCtrlRegs.GPEGMUX1.bit. GPIO139 = 1;
    GpioCtrlRegs.GPEMUX1.bit.GPIO139 = 6;    // Set GPIO139 to SCIRXDB
    GpioCtrlRegs.GPEDIR.bit.GPIO139 = 0;     // Set GPIO139 as input for RX
    GpioCtrlRegs.GPEQSEL1.bit.GPIO139 = 3;   // Async

    // Configure GPIO56 as SCITXDB (SCI-B Transmit)
    GpioCtrlRegs.GPBGMUX2.bit.GPIO56 = 1;
    GpioCtrlRegs.GPBMUX2.bit.GPIO56 = 6;    // Set GPIO56 to SCITXDB
    GpioCtrlRegs.GPBDIR.bit.GPIO56 = 1;     // Set GPIO56 as output for TX
    GpioCtrlRegs.GPBQSEL2.bit.GPIO56 = 3;   // Async

    EDIS;
}


void InitScib(void)
{
    //EALLOW;

    ScibRegs.SCICTL1.bit.SWRESET = 0;    // hold in reset

    ScibRegs.SCICCR.bit.ADDRIDLE_MODE = 0;
    ScibRegs.SCICCR.bit.STOPBITS = 0;    // 1 stop bit
    ScibRegs.SCICCR.bit.PARITYENA = 0;   // No parity
    ScibRegs.SCICCR.bit.LOOPBKENA = 0;   //No Loopback
    ScibRegs.SCICCR.bit.SCICHAR = 0x7;   // async mode, idle-line protocol

    ScibRegs.SCICTL2.bit.TXINTENA = 1;
    ScibRegs.SCICTL2.bit.RXBKINTENA = 1;
    //
    // SCIA at 9600 baud
    // @LSPCLK = 50 MHz (200 MHz SYSCLK) HBAUD = 0x02 and LBAUD = 0x8B.
    // @LSPCLK = 30 MHz (120 MHz SYSCLK) HBAUD = 0x01 and LBAUD = 0x86.
    //
    ScibRegs.SCIHBAUD.all = 0x0002;
    ScibRegs.SCILBAUD.all = 0x008B;

    ScibRegs.SCICTL1.bit.TXENA = 1;
    ScibRegs.SCICTL1.bit.RXENA = 1;
    ScibRegs.SCICTL1.bit.SWRESET = 1; // Out of reset


    //EDIS;
}

void InitScic(void)
{
    //EALLOW;

    ScicRegs.SCICTL1.bit.SWRESET = 0;    // hold in reset

    ScicRegs.SCICCR.bit.ADDRIDLE_MODE = 0;
    ScicRegs.SCICCR.bit.STOPBITS = 0;    // 1 stop bit
    ScicRegs.SCICCR.bit.PARITYENA = 0;   // No parity
    ScicRegs.SCICCR.bit.LOOPBKENA = 0;   //No Loopback
    ScicRegs.SCICCR.bit.SCICHAR = 0x7;   // async mode, idle-line protocol

    ScicRegs.SCICTL2.bit.TXINTENA = 1;
    ScicRegs.SCICTL2.bit.RXBKINTENA = 1;
    //
    // SCIA at 9600 baud
    // @LSPCLK = 50 MHz (200 MHz SYSCLK) HBAUD = 0x02 and LBAUD = 0x8B.
    // @LSPCLK = 30 MHz (120 MHz SYSCLK) HBAUD = 0x01 and LBAUD = 0x86.
    //
    ScicRegs.SCIHBAUD.all = 0x0000;
    ScicRegs.SCILBAUD.all = 0x0035;

    ScicRegs.SCICTL1.bit.TXENA = 1;
    ScicRegs.SCICTL1.bit.RXENA = 1;
    ScicRegs.SCICTL1.bit.SWRESET = 1; // Out of reset


    //EDIS;
}

void InitI2CGpio(void)
{
    EALLOW;
    GpioCtrlRegs.GPDGMUX1.bit.GPIO104 = 0; // GMUX for SDA
    GpioCtrlRegs.GPDMUX1.bit.GPIO104 = 1;  // MUX for SDA
    GpioCtrlRegs.GPDQSEL1.bit.GPIO104 = 3;  // Async mode for SDA
    GpioCtrlRegs.GPDPUD.bit.GPIO104 = 1;  // Disable pullup for SDA

    GpioCtrlRegs.GPDGMUX1.bit.GPIO105 = 0; // GMUX for SCL
    GpioCtrlRegs.GPDMUX1.bit.GPIO105 = 1;  // MUX for SCL
    GpioCtrlRegs.GPDQSEL1.bit.GPIO105 = 3;  // Async mode for SCL
    GpioCtrlRegs.GPDPUD.bit.GPIO105 = 1;  // Disable pullup for SCL

}



void InitI2C(void)
{

    I2cbRegs.I2CMDR.bit.IRS = 0;  // Disable the I2C module for configuration
    I2cbRegs.I2CPSC.all = 16;  // Set prescaler (adjust formula as needed)
    I2cbRegs.I2CCLKL = 10;                                            // Clock low-time divider for 50% duty cycle
    I2cbRegs.I2CCLKH = 5;                                            // Clock high-time divider for 50% duty cycle
    I2cbRegs.I2CMDR.bit.MST = 0;   // Set to master mode
    I2cbRegs.I2CMDR.bit.TRX = 0;   // Set to transmit mode
    I2cbRegs.I2CSAR.all = 0x50;  // Set slave address (0x50 in decimal is 80)
    I2cbRegs.I2CMDR.bit.DLB = 0;  // Disable digital loopback mode
    I2cbRegs.I2CMDR.bit.BC = 0;  // Set 8-bit data count
    I2cbRegs.I2CCNT = 2;         // Set data count to 2 bytes
    I2cbRegs.I2CMDR.bit.XA = 0;  // Set address mode to 7 bits
    I2cbRegs.I2CIER.bit.RRDY = 1;        // Enable Interupt
    I2cbRegs.I2CMDR.bit.IRS = 1;  // Enable the I2C module after configuration
}

void DelayS(int s)
{
    int i;
    for (i = 0; i < s; i++)
    {
        DelayUs(10000);
        DelayUs(10000);
        DelayUs(10000);
        DelayUs(10000);
        DelayUs(10000);
        DelayUs(10000);
        DelayUs(10000);
        DelayUs(10000);
        DelayUs(10000);
        DelayUs(10000);
    }
}

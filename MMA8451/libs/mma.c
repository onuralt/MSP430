/***************************************************************************//**
 * @file    mma.c
 * @author  <Onur Altinordu>
 * @email   <onur.altinordu@mars.uni-freiburg.de>
 * @matno   <5250773>
 * @exno    <Exercise 3>
 * @date    <11.06.2022>
 *
 * @brief   <mma8451q library>
 *
 * Here goes a detailed description if required.
 ******************************************************************************/

#include "./mma.h"

/******************************************************************************
 * VARIABLES
 *****************************************************************************/
unsigned char SEND_CMD[CMD_LENGTH];
unsigned char SEND_REG[REG_LENGTH];

unsigned char READ_BYTE[RD8_LENGTH];
unsigned char READ_TWO_BYTES[RD14_LENGTH];

unsigned char CMD_CTRL_REG1 = 0x00;
unsigned char CMD_CTRL_REG2 = 0x00;
unsigned char CMD_XYZ_DATA_CFG = 0x00;
unsigned char CMD_PULSE_CFG = 0x00;
unsigned char CMD_CTRL_REG3 = 0x00;
unsigned char CMD_CTRL_REG4 = 0x00;
unsigned char CMD_CTRL_REG5 = 0x00;
unsigned char CMD_PULSE_LTCY = 0x00;
unsigned char CMD_PULSE_THSZ = 0x00;
unsigned char CMD_PULSE_TMLT = 0x00;
unsigned char CMD_PULSE_WIND = 0x00;

unsigned char pass;

int val_x;
int val_y;
int val_z;
int i;

int X_ST_ON_2g;
int Y_ST_ON_2g;
int Z_ST_ON_2g;

int X_ST_ON_4g;
int Y_ST_ON_4g;
int Z_ST_ON_4g;

int X_ST_ON_8g;
int Y_ST_ON_8g;
int Z_ST_ON_8g;

int X_ST_OFF_2g;
int Y_ST_OFF_2g;
int Z_ST_OFF_2g;

int X_ST_OFF_4g;
int Y_ST_OFF_4g;
int Z_ST_OFF_4g;

int X_ST_OFF_8g;
int Y_ST_OFF_8g;
int Z_ST_OFF_8g;

int X_ST_2g;
int Y_ST_2g;
int Z_ST_2g;

int X_ST_4g;
int Y_ST_4g;
int Z_ST_4g;

int X_ST_8g;
int Y_ST_8g;
int Z_ST_8g;

double READ_AXIS[AX_LENGTH];

/******************************************************************************
 * LOCAL FUNCTION PROTOTYPES
 *****************************************************************************/

/******************************************************************************
 * LOCAL FUNCTION IMPLEMENTATION
 *****************************************************************************/
void configure_gpio(void)
{
    /*Configure the P1.0 pin (connected to MMA_INT1) for falling edge interrupts*/
    P1DIR &= ~ BIT0;    // Set as input
    P1REN |= BIT0;      // Enable pull - resistors
    P1OUT |= BIT0;      // Set to pull-up

    P1IES |= BIT0;      // High /Low - Edge
    P1IFG &= ~ BIT0;    // Clear interrupt flag

    P3OUT = 0x00;
    // P3 .6 output
    P3DIR |= (BIT6 | BIT0 | BIT1);
    // P3 .6 TA0.2 option
    P3SEL |= BIT6;
    // CCR2 set / reset
    TA0CCTL2 = OUTMOD_3;

    // initialize Timer0_A
    // set up terminal count
    TA1CCR0 = 31250;

    // configure and start timer
    TA1CTL = TASSEL_1 + ID_3 + MC_1;

    P3OUT = 0x00;
    // P3 .6 output
    P3DIR |= BIT6;
    // P3 .6 TA0.2 option
    P3SEL |= BIT6;
    // CCR2 set / reset
    TA0CCTL2 = OUTMOD_3;

    // initialize Timer0_A
    // set up terminal count
    TA1CCR0 = 31250;
    // configure and start timer
    TA1CTL = TASSEL_2 + ID_3 + MC_1;
    // configure and start timer
    TA0CTL = TASSEL_2 + MC_1;
    __enable_interrupt();
}

unsigned char mma8451q_write_cmd(unsigned char reg, unsigned char cmd)
{
    unsigned char error = 0;
    SEND_CMD[REG_IX] = reg; // register address
    SEND_CMD[CMD_IX] = cmd; // command
    error = i2c_write(CMD_LENGTH, SEND_CMD, STOP_EN);
    return error;
}

unsigned char mma8451q_read_byte(unsigned char reg)
{
    unsigned char error = 0;

    READ_BYTE[0] = 0;
    SEND_REG[0] = reg; // register address
    error = i2c_write(REG_LENGTH, SEND_REG, STOP_DIS);
    i2c_read(RD8_LENGTH, READ_BYTE);

    return error;
}

unsigned char mma8451q_read_two_bytes(unsigned char reg)
{
    unsigned char error = 0;
    READ_TWO_BYTES[0] = 0;
    READ_TWO_BYTES[1] = 0;
    SEND_REG[0] = reg; // register address
    error = i2c_write(REG_LENGTH, SEND_REG, STOP_DIS);
    i2c_read(RD14_LENGTH, READ_TWO_BYTES);

    return error;
}

double mma_getReal(unsigned char axis)
{
    double acc, DIV;

    mma8451q_read_byte(CTRL_REG1);
    if (!(READ_BYTE[0] & F_READ)) // check if 14 bit mode enabled
    {
        switch (axis)
        {
        case 0:
            acc = mma_get14X();
            break;
        case 1:
            acc = mma_get14Y();
            break;
        case 2:
            acc = mma_get14Z();
            break;
        }
        /*Determine divider*/
        mma8451q_read_byte(CMD_XYZ_DATA_CFG);
        if (!(READ_BYTE[0] & FS0) && !(READ_BYTE[0] & FS1))
        {
            DIV = DIV_14_2G;
        }
        else if ((READ_BYTE[0] & FS0) && !(READ_BYTE[0] & FS1))
        {
            DIV = DIV_14_4G;
        }
        else if (!(READ_BYTE[0] & FS0) && (READ_BYTE[0] & FS1))
        {
            DIV = DIV_14_8G;
        }
    }
    else // 8 bit mode
    {
        switch (axis)
        {
        case 0:
            acc = mma_get8X();
            break;
        case 1:
            acc = mma_get8Y();
            break;
        case 2:
            acc = mma_get8Z();
            break;
        }
        mma8451q_read_byte(CMD_XYZ_DATA_CFG);
        if (!(READ_BYTE[0] & FS0) && !(READ_BYTE[0] & FS1))
        {
            DIV = DIV_8_2G;
        }
        else if ((READ_BYTE[0] & FS0) && !(READ_BYTE[0] & FS1))
        {
            DIV = DIV_8_4G;
        }
        else if (!(READ_BYTE[0] & FS0) && (READ_BYTE[0] & FS1))
        {
            DIV = DIV_8_8G;
        }
    }
    acc /= DIV;
    return acc * GRAVITY;
}

int mma_get14(unsigned char axis_reg)
{
    int acceleration;

    CMD_CTRL_REG1 &= ~ACTIVE; // Standby mode
    mma8451q_write_cmd(CTRL_REG1, CMD_CTRL_REG1);
    __delay_cycles(10000);

    mma8451q_read_byte(CTRL_REG1);

    CMD_CTRL_REG1 |= ACTIVE; // Active mode
    mma8451q_write_cmd(CTRL_REG1, CMD_CTRL_REG1);
    __delay_cycles(10000);

    if ((READ_BYTE[0] & F_READ))
    {
        mma8451q_read_byte(axis_reg);

        CMD_CTRL_REG1 &= ~ACTIVE; // Standby mode
        mma8451q_write_cmd(CTRL_REG1, CMD_CTRL_REG1);
        __delay_cycles(10000);

        return (int) READ_BYTE[0];
    }

    mma8451q_read_two_bytes(axis_reg);

    acceleration = (READ_TWO_BYTES[0] << 6) | (READ_TWO_BYTES[1] >> 2);

    if ((acceleration / 8192) == 1)
    {
        acceleration = ~acceleration + 1;
    }

    CMD_CTRL_REG1 &= ~ACTIVE; // Standby mode
    mma8451q_write_cmd(CTRL_REG1, CMD_CTRL_REG1);
    __delay_cycles(10000);

    return acceleration;
}

signed char mma_get8(unsigned char axis_reg)
{
    signed char acceleration;

    CMD_CTRL_REG1 &= ~ACTIVE; // Standby mode
    mma8451q_write_cmd(CTRL_REG1, CMD_CTRL_REG1);
    __delay_cycles(10000);

    mma8451q_read_byte(CTRL_REG1);
    if (!(READ_BYTE[0] & F_READ))
    {
        if (axis_reg == OUT_X_MSB)
        {
            acceleration = mma_get14X();
        }
        else if (axis_reg == OUT_Y_MSB)
        {
            acceleration = mma_get14Y();
        }
        else if (axis_reg == OUT_Z_MSB)
        {
            acceleration = mma_get14Z();
        }
        return acceleration;
    }

    CMD_CTRL_REG1 |= ACTIVE; // Active mode
    mma8451q_write_cmd(CTRL_REG1, CMD_CTRL_REG1);
    __delay_cycles(10000);

    mma8451q_read_byte(axis_reg);

    CMD_CTRL_REG1 &= ~ACTIVE; // Standby mode
    mma8451q_write_cmd(CTRL_REG1, CMD_CTRL_REG1);
    __delay_cycles(10000);

    acceleration = READ_BYTE[0];

    if ((acceleration / 128) == 1)
    {
        acceleration = ~acceleration + 1;
    }

    return acceleration;
}

/******************************************************************************
 * FUNCTION IMPLEMENTATION
 *****************************************************************************/

unsigned char mma_init(void)
{
    unsigned char error = 0;

    /*Initialize I2C for MMA8451Q*/
    i2c_init(MMA8451Q);

    /*Configure Registers*/

    CMD_CTRL_REG1 &= ~ACTIVE; // Standby mode
    error = mma8451q_write_cmd(CTRL_REG1, CMD_CTRL_REG1);
    __delay_cycles(10000);

    CMD_CTRL_REG2 |= RST;
    error = mma8451q_write_cmd(CTRL_REG2, CMD_CTRL_REG2); // Reset all registers to POR values
    __delay_cycles(10000);
    do // Wait for the RST bit to clear
    {
        error = mma8451q_read_byte(CTRL_REG2);
        READ_BYTE[0] &= RST;
    }
    while (READ_BYTE[0]);

    error = mma_setResolution(0);                   // 8-bit resolution
    error = mma_setRange(1); // 4g range                        // 4G measurement range

    return error;                             // Return 1 if there exists a NACK
}

unsigned char mma_setRange(unsigned char range)
{
    unsigned char error = 0;

    CMD_CTRL_REG1 &= ~ACTIVE; // Standby mode   // Standby mode
    error = mma8451q_write_cmd(CTRL_REG1, CMD_CTRL_REG1);
    __delay_cycles(10000);

    if (range == 0)
    {
        /*2G measurement range*/
        CMD_XYZ_DATA_CFG &= ~(FS0 | FS1);
        error = mma8451q_write_cmd(XYZ_DATA_CFG, CMD_XYZ_DATA_CFG);
    }
    else if (range == 1)
    {
        /*4G measurement range*/
        CMD_XYZ_DATA_CFG &= ~FS1;
        CMD_XYZ_DATA_CFG |= FS0;
        error = mma8451q_write_cmd(XYZ_DATA_CFG, CMD_XYZ_DATA_CFG);
    }
    else if (range > 1)
    {
        /*8G measurement range*/
        CMD_XYZ_DATA_CFG &= ~FS0;
        CMD_XYZ_DATA_CFG |= FS1;
        error = mma8451q_write_cmd(XYZ_DATA_CFG, CMD_XYZ_DATA_CFG);
    }
    else
    {
        error = 1;
    }

    return error;
}

unsigned char mma_setResolution(unsigned char resolution)
{
    unsigned char error = 0;

    CMD_CTRL_REG1 &= ~ACTIVE; // Standby mode
    error = mma8451q_write_cmd(CTRL_REG1, CMD_CTRL_REG1);
    __delay_cycles(10000);

    if (resolution == 0)
    {
        /*8-bit resolution*/
        CMD_CTRL_REG1 |= F_READ;
        error = mma8451q_write_cmd(CTRL_REG1, CMD_CTRL_REG1);
    }
    else if (resolution >= 1)
    {
        /*14-bit resolution*/
        CMD_CTRL_REG1 &= ~F_READ;
        error = mma8451q_write_cmd(CTRL_REG1, CMD_CTRL_REG1);
    }
    else
    {
        error = 1;
    }

    return error;
}

unsigned char mma_selftest(void)
{
    unsigned char error = 0;
    CMD_CTRL_REG2 = 0x00;
    CMD_CTRL_REG1 = 0x00;

    CMD_CTRL_REG1 &= ~ACTIVE;
    error = mma8451q_write_cmd(CTRL_REG1, CMD_CTRL_REG1);
    __delay_cycles(10000);

    mma_setResolution(1);
    mma_setRange(0); // 2g range

    CMD_CTRL_REG2 &= ~ST;
    error = mma8451q_write_cmd(CTRL_REG2, CMD_CTRL_REG2);
    __delay_cycles(10000);

    CMD_CTRL_REG1 |= ACTIVE;
    error = mma8451q_write_cmd(CTRL_REG1, CMD_CTRL_REG1);
    __delay_cycles(1000000);

    X_ST_OFF_2g = mma_get14X();
    Y_ST_OFF_2g = mma_get14Y();
    Z_ST_OFF_2g = mma_get14Z();

    CMD_CTRL_REG1 &= ~ACTIVE;
    error = mma8451q_write_cmd(CTRL_REG1, CMD_CTRL_REG1);
    __delay_cycles(10000);

    CMD_CTRL_REG2 |= ST; // Self test bit set
    error = mma8451q_write_cmd(CTRL_REG2, CMD_CTRL_REG2);
    __delay_cycles(10000);

    CMD_CTRL_REG1 |= ACTIVE;
    error = mma8451q_write_cmd(CTRL_REG1, CMD_CTRL_REG1);
    __delay_cycles(1000000);

    X_ST_ON_2g = mma_get14X();
    Y_ST_ON_2g = mma_get14Y();
    Z_ST_ON_2g = mma_get14Z();

    CMD_CTRL_REG1 &= ~ACTIVE;
    error = mma8451q_write_cmd(CTRL_REG1, CMD_CTRL_REG1);
    __delay_cycles(10000);

    mma_setRange(1); // 4g range

    CMD_CTRL_REG2 &= ~ST;
    error = mma8451q_write_cmd(CTRL_REG2, CMD_CTRL_REG2);
    __delay_cycles(10000);

    CMD_CTRL_REG1 |= ACTIVE;
    error = mma8451q_write_cmd(CTRL_REG1, CMD_CTRL_REG1);
    __delay_cycles(1000000);

    X_ST_OFF_4g = mma_get14X();
    Y_ST_OFF_4g = mma_get14Y();
    Z_ST_OFF_4g = mma_get14Z();

    CMD_CTRL_REG1 &= ~ACTIVE;
    error = mma8451q_write_cmd(CTRL_REG1, CMD_CTRL_REG1);
    __delay_cycles(10000);

    CMD_CTRL_REG2 |= ST; // Self test bit set
    error = mma8451q_write_cmd(CTRL_REG2, CMD_CTRL_REG2);
    __delay_cycles(10000);

    CMD_CTRL_REG1 |= ACTIVE;
    error = mma8451q_write_cmd(CTRL_REG1, CMD_CTRL_REG1);
    __delay_cycles(1000000);

    X_ST_ON_4g = mma_get14X();
    Y_ST_ON_4g = mma_get14Y();
    Z_ST_ON_4g = mma_get14Z();

    CMD_CTRL_REG1 &= ~ACTIVE;
    error = mma8451q_write_cmd(CTRL_REG1, CMD_CTRL_REG1);
    __delay_cycles(10000);

    mma_setRange(2); // 8g range

    CMD_CTRL_REG2 &= ~ST;
    error = mma8451q_write_cmd(CTRL_REG2, CMD_CTRL_REG2);
    __delay_cycles(10000);

    CMD_CTRL_REG1 |= ACTIVE;
    error = mma8451q_write_cmd(CTRL_REG1, CMD_CTRL_REG1);
    __delay_cycles(1000000);

    X_ST_OFF_8g = mma_get14X();
    Y_ST_OFF_8g = mma_get14Y();
    Z_ST_OFF_8g = mma_get14Z();

    CMD_CTRL_REG1 &= ~ACTIVE;
    error = mma8451q_write_cmd(CTRL_REG1, CMD_CTRL_REG1);
    __delay_cycles(10000);

    CMD_CTRL_REG2 |= ST; // Self test bit set
    error = mma8451q_write_cmd(CTRL_REG2, CMD_CTRL_REG2);
    __delay_cycles(10000);

    CMD_CTRL_REG1 |= ACTIVE;
    error = mma8451q_write_cmd(CTRL_REG1, CMD_CTRL_REG1);
    __delay_cycles(1000000);

    X_ST_ON_8g = mma_get14X();
    Y_ST_ON_8g = mma_get14Y();
    Z_ST_ON_8g = mma_get14Z();

    CMD_CTRL_REG1 &= ~ACTIVE;
    error = mma8451q_write_cmd(CTRL_REG1, CMD_CTRL_REG1);
    __delay_cycles(10000);

    //mma_setResolution(0);
    //mma_setRange(1); // 4g range

    X_ST_2g = X_ST_ON_2g - X_ST_OFF_2g;
    Y_ST_2g = Y_ST_ON_2g - Y_ST_OFF_2g;
    Z_ST_2g = Z_ST_ON_2g - Z_ST_OFF_2g;

    X_ST_4g = X_ST_ON_4g - X_ST_OFF_4g;
    Y_ST_4g = Y_ST_ON_4g - Y_ST_OFF_4g;
    Z_ST_4g = Z_ST_ON_4g - Z_ST_OFF_4g;

    X_ST_8g = X_ST_ON_8g - X_ST_OFF_8g;
    Y_ST_8g = Y_ST_ON_8g - Y_ST_OFF_8g;
    Z_ST_8g = Z_ST_ON_8g - Z_ST_OFF_8g;

    if ((X_ST_2g < 365) && (X_ST_2g > 315) && (Y_ST_2g < 465) && (Y_ST_2g > 415)
            && (Z_ST_2g < 2385) && (Z_ST_2g > 2335) && (X_ST_4g < 195)
            && (X_ST_4g > 145) && (Y_ST_4g < 235) && (Y_ST_4g > 185)
            && (Z_ST_4g < 1195) && (Z_ST_4g > 1145) && (X_ST_8g < 110)
            && (X_ST_8g > 60) && (Y_ST_8g < 135) && (Y_ST_8g > 85)
            && (Z_ST_8g < 6125) && (Z_ST_8g > 575))
    {
        error = 0;
    }
    else
    {
        error = 1;
    }

    return error;
}

signed char mma_get8X(void)
{
    return mma_get8(OUT_X_MSB); // X value
}

signed char mma_get8Y(void)
{
    return mma_get8(OUT_Y_MSB); // Y value
}

signed char mma_get8Z(void)
{
    return mma_get8(OUT_Z_MSB); // Z value
}

int mma_get14X(void)
{
    return mma_get14(OUT_X_MSB); // X value
}

int mma_get14Y(void)
{
    return mma_get14(OUT_Y_MSB); // Y value
}

int mma_get14Z(void)
{
    return mma_get14(OUT_Z_MSB); // Z value
}

double mma_getRealX(void)
{
    return mma_getReal(0); // X value
}
double mma_getRealY(void)
{
    return mma_getReal(1); // Y value
}
double mma_getRealZ(void)
{
    return mma_getReal(2); // Z value
}

unsigned char mma_read(void)
{
    unsigned char error;

    CMD_CTRL_REG1 |= ACTIVE; // Active mode
    error = mma8451q_write_cmd(CTRL_REG1, CMD_CTRL_REG1);
    __delay_cycles(10000);

    READ_AXIS[0] = mma_getRealX(); // X value
    READ_AXIS[1] = mma_getRealY(); // Y value
    READ_AXIS[2] = mma_getRealZ(); // Z value

    CMD_CTRL_REG1 &= ~ACTIVE; // Standby mode
    error = mma8451q_write_cmd(CTRL_REG1, CMD_CTRL_REG1);
    __delay_cycles(10000);

    return error;
}

unsigned char mma_enableTapInterrupt(void)
{
    unsigned char error;
    CMD_CTRL_REG1 &= ~ACTIVE;
    error = mma8451q_write_cmd(CTRL_REG1, CMD_CTRL_REG1);
    __delay_cycles(10000);

    CMD_PULSE_CFG |= ZDPEFE;
    error = mma8451q_write_cmd(PULSE_CFG, CMD_PULSE_CFG);
    __delay_cycles(10000);

    CMD_PULSE_THSZ |= THSZ5;
    error = mma8451q_write_cmd(PULSE_THSZ, CMD_PULSE_THSZ);
    __delay_cycles(10000);

    CMD_PULSE_TMLT |= (TMLT7 | TMLT5);
    error = mma8451q_write_cmd(PULSE_TMLT, CMD_PULSE_TMLT);
    __delay_cycles(10000);

    CMD_PULSE_LTCY |= (LTCY7 | LTCY5);
    error = mma8451q_write_cmd(PULSE_LTCY, CMD_PULSE_LTCY);
    __delay_cycles(10000);

    CMD_PULSE_WIND |= (WIND6 | WIND3);
    error = mma8451q_write_cmd(PULSE_WIND, CMD_PULSE_WIND);
    __delay_cycles(10000);

    CMD_CTRL_REG4 |= INT_EN_PULSE;
    error = mma8451q_write_cmd(CTRL_REG4, CMD_CTRL_REG4);
    __delay_cycles(10000);

    CMD_CTRL_REG5 |= INT_CFG_PULSE;
    error = mma8451q_write_cmd(CTRL_REG5, CMD_CTRL_REG5);
    __delay_cycles(10000);

    CMD_CTRL_REG3 |= IPOL;
    error = mma8451q_write_cmd(CTRL_REG3, CMD_CTRL_REG3);
    __delay_cycles(10000);

    CMD_CTRL_REG1 |= ACTIVE;
    error = mma8451q_write_cmd(CTRL_REG1, CMD_CTRL_REG1);
    __delay_cycles(1000000);

    return error;
}

unsigned char mma_disableTapInterrupt(void)
{
    unsigned char error;
    CMD_CTRL_REG1 &= ~ACTIVE;
    error = mma8451q_write_cmd(CTRL_REG1, CMD_CTRL_REG1);
    __delay_cycles(10000);

    CMD_PULSE_CFG &= CLR;
    error = mma8451q_write_cmd(PULSE_CFG, CMD_PULSE_CFG);
    __delay_cycles(10000);

    CMD_PULSE_THSZ &= CLR;
    error = mma8451q_write_cmd(PULSE_THSZ, CMD_PULSE_THSZ);
    __delay_cycles(10000);

    CMD_PULSE_TMLT &= CLR;
    error = mma8451q_write_cmd(PULSE_TMLT, CMD_PULSE_TMLT);
    __delay_cycles(10000);

    CMD_PULSE_LTCY &= CLR;
    error = mma8451q_write_cmd(PULSE_LTCY, CMD_PULSE_LTCY);
    __delay_cycles(10000);

    CMD_PULSE_WIND &= CLR;
    error = mma8451q_write_cmd(PULSE_WIND, CMD_PULSE_WIND);
    __delay_cycles(10000);

    CMD_CTRL_REG4 &= CLR;
    error = mma8451q_write_cmd(CTRL_REG4, CMD_CTRL_REG4);
    __delay_cycles(10000);

    CMD_CTRL_REG5 &= CLR;
    error = mma8451q_write_cmd(CTRL_REG5, CMD_CTRL_REG5);
    __delay_cycles(10000);

    CMD_CTRL_REG3 &= CLR;
    error = mma8451q_write_cmd(CTRL_REG3, CMD_CTRL_REG3);
    __delay_cycles(10000);

    CMD_CTRL_REG1 |= ACTIVE;
    error = mma8451q_write_cmd(CTRL_REG1, CMD_CTRL_REG1);
    __delay_cycles(1000000);

    return error;
}


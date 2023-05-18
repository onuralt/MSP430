/***************************************************************************//**
 * @file    flash.c
 * @author  <Onur Altinordu>
 * @email   <onur.altinordu@mars.uni-freiburg.de>
 * @matno   <5250773>
 * @exno    <Exercise 4>
 * @date    <25.06.2022>
 *
 * @brief   <M25P16 library function implementations>
 *
 * Here goes a detailed description if required.
 ******************************************************************************/

#include "./flash.h"

/******************************************************************************
 * VARIABLES
 *****************************************************************************/
unsigned char WRSR[1] = { 0x01 };
unsigned char PP[1] = { 0x02 };
unsigned char READ[1] = { 0x03 };
unsigned char WRDI[1] = { 0x04 };
unsigned char RDSR[1] = { 0x05 };
unsigned char WREN[1] = { 0x06 };
unsigned char BE[1] = { 0xC7 };
unsigned char SE[1] = { 0xD8 };
/******************************************************************************
 * FUNCTION IMPLEMENTATION
 *****************************************************************************/
// Initialise the flash chip (in case you need it, else leave this function
// empty).
void flash_init(void)
{
    P3DIR |= BIT7;
    spi_init();

    /*Set Sector protection bits to 0*/
    flash_write_status(0, 0, 0);
}

/*Write BP0, BP1 & BP2 bits to the status register*/
void flash_write_status(unsigned char BP_2, unsigned char BP_1,
                        unsigned char BP_0)
{
    unsigned char status[1];
    if ((BP_2 == 0) & (BP_1 == 0) & (BP_0 == 0))
    {
        status[0] = 0x00;
    }
    else if ((BP_2 == 0) & (BP_1 == 0) & (BP_0 == 1))
    {
        status[0] = 0x04;
    }
    else if ((BP_2 == 0) & (BP_1 == 1) & (BP_0 == 0))
    {
        status[0] = 0x08;
    }
    else if ((BP_2 == 0) & (BP_1 == 1) & (BP_0 == 1))
    {
        status[0] = 0x0C;
    }
    else if ((BP_2 == 1) & (BP_1 == 0) & (BP_0 == 0))
    {
        status[0] = 0x10;
    }
    else if ((BP_2 == 1) & (BP_1 == 0) & (BP_0 == 1))
    {
        status[0] = 0x14;
    }
    else if ((BP_2 == 1) & (BP_1 == 1) & (BP_0 == 0))
    {
        status[0] = 0x18;
    }
    else if ((BP_2 == 1) & (BP_1 == 1) & (BP_0 == 1))
    {
        status[0] = 0x1C;
    }

    /*Send WREN*/
    P3OUT &= ~(BIT7);
    spi_write(1, WREN);
    P3OUT |= BIT7;

    /*Send WRSR*/
    P3OUT &= ~(BIT7);
    spi_write(1, WRSR);
    spi_write(1, status);
    P3OUT |= BIT7;

    /*Send WRDI*/
    P3OUT &= ~(BIT7);
    spi_write(1, WRDI);
    P3OUT |= BIT7;
}

/*Read status register*/
unsigned char flash_read_status()
{
    unsigned char STAT_READ[1];
    __delay_cycles(100000);
    spi_transcieve(1, 1, RDSR, STAT_READ);
    return STAT_READ[0];
}

void flash_BE(void)
{
    /*Send WREN*/
    P3OUT &= ~(BIT7);
    spi_write(1, WREN);
    P3OUT |= BIT7;

    /*Send BE*/
    P3OUT &= ~(BIT7);
    spi_write(1, BE);
    P3OUT |= BIT7;

    /*Send WRDI*/
    P3OUT &= ~(BIT7);
    spi_write(1, WRDI);
    P3OUT |= BIT7;

}

void flash_SE(long int address)
{
    unsigned char addr[3];
    unsigned char addr_ix;

    /*Divide the address into bytes*/
    for (addr_ix = 0; addr_ix < 3; addr_ix++)
    {
        addr[addr_ix] = (address << (8 * addr_ix)) / 65536;
    }

    /*Send WREN*/
    P3OUT &= ~(BIT7);
    spi_write(1, WREN);
    P3OUT |= BIT7;

    P3OUT &= ~(BIT7);
    spi_write(1, SE);
    spi_write(3, addr);
    P3OUT |= BIT7;

    /*Send WRDI*/
    P3OUT &= ~(BIT7);
    spi_write(1, WRDI);
    P3OUT |= BIT7;

    //__delay_cycles(1000000);
}

// Read <length> bytes into <rxData> starting from address <address> (1 pt.)
void flash_read(long int address, unsigned char length, unsigned char *rxData)
{
    unsigned char addr[3];
    unsigned char addr_ix;

    /*Divide the address into bytes*/
    for (addr_ix = 0; addr_ix < 3; addr_ix++)
    {
        addr[addr_ix] = (address << (8 * addr_ix)) / 65536;
    }

    /*Send READ and then 24 bit address*/
    P3OUT &= ~(BIT7);
    spi_write(1, READ);
    spi_write(3, addr);
    spi_read(length, rxData);
    P3OUT |= BIT7;
}

// Write <length> bytes from <txData>, starting at address <address> (1 pt.)
void flash_write(long int address, unsigned char length, unsigned char *txData)
{
    unsigned char addr[3];
    unsigned char addr_ix;

    /*Divide the address into bytes*/
    for (addr_ix = 0; addr_ix < 3; addr_ix++)
    {
        addr[addr_ix] = (address << (8 * addr_ix)) / 65536;
    }

    /*Send WREN*/
    P3OUT &= ~(BIT7);
    spi_write(1, WREN);
    P3OUT |= BIT7;

    /*Send PP, then the 24 bit address and then the data*/
    P3OUT &= ~(BIT7);
    spi_write(1, PP);
    spi_write(3, addr);
    spi_write(length, txData);
    P3OUT |= BIT7;

    /*Send WRDI*/
    P3OUT &= ~(BIT7);
    spi_write(1, WRDI);
    P3OUT |= BIT7;
}

// Returns 1 if the FLASH is busy or 0 if not.
// Note: this is optional. You will probably need this, but you don't have to
// implement this if you solve it differently.
unsigned char flash_busy(void)
{
    if (flash_read_status() & WIP)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/*Check WEL bit in the status register*/
unsigned char flash_WEL_DIS()
{
    if (flash_read_status() & WEL)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

/***************************************************************************//**
 * @file    i2c.c
 * @author  <Onur Altinordu>
 * @email   <onur.altinordu@mars.uni-freiburg.de>
 * @matno   <5250773>
 * @exno    <Exercise 4>
 * @date    <25.06.2022>
 *
 * @brief   <SPI library function implementations>
 *
 * Here goes a detailed description if required.
 ******************************************************************************/

#include "./spi.h"
#include "./USCIAB0.h"

/******************************************************************************
 * VARIABLES
 *****************************************************************************/
unsigned char SPI_RX_CTR = 0;
unsigned char SPI_RX_IDX = 0;
unsigned char SPI_TX_IDX = 0;
unsigned char SPI_TX_CTR = 0;
unsigned char SPI_COPY_IDX;
unsigned char SPI_RX_BUFF[MAX_BUFFER_SIZE] = { 0 };
unsigned char SPI_TX_BUFF[MAX_BUFFER_SIZE] = { 0 };
unsigned char SPI_DUMMY[MAX_BUFFER_SIZE] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                             0x00, 0x00 };
/******************************************************************************
 * FUNCTION IMPLEMENTATION
 *****************************************************************************/

// Returns 1 if the SPI is still busy or 0 if not.
// Note: this is optional. You will most likely need this, but you don't have
// to implement or use this.
unsigned char spi_busy(void)
{
    return (UCB0STAT & UCBUSY);
}

// Set the USCI-machine to SPI and switch the 74HCT4066 (1 pt.)
void spi_init(void)
{
    /*Disable TX and RX interrupts*/
    IE2 &= ~UCB0RXIE;
    IE2 &= ~UCB0TXIE;

    /*Empty TX and RX Buffers*/
    UCB0TXBUF = 0;
    UCB0RXBUF = 0;

    /*Hook SPI RX and TX functions to the ISR and unhook
     * I2C functions*/
    USCIAB0_InterruptEnable(SPI, RX, &SPI_RX);
    USCIAB0_InterruptEnable(SPI, TX, &SPI_TX);

    /*P 1.3 must be reset in order to route the SPI bus*/
    P3DIR |= BIT3;
    P3OUT &= ~BIT3;

    /*SPI Pins*/
    P1SEL = BIT7 + BIT6 + BIT5;
    P1SEL2 = BIT7 + BIT6 + BIT5;

    /*Clock Polarity: The inactive state is low MSB First,
     * 8-bit, Master, 3-pin mode, Synchronous*/
    UCB0CTL0 = UCCKPH + UCMSB + UCMST + UCSYNC;

    /*SMCLK*/
    UCB0CTL1 = UCSSEL_2;

    /*SMCLK / 2*/
    UCB0BR0 = 0x02;
    UCB0BR1 = 0;

    /*Initialize USCI state machine*/
    UCB0CTL1 &= ~UCSWRST;

    /*Set up the CS Pin*/
    P3DIR |= BIT7;
    P3OUT |= BIT7;
}

// Read <length> bytes into <rxData> (1 pt.)
void spi_read(unsigned char length, unsigned char *rxData)
{
    /*Set lenght and index*/
    SPI_RX_CTR = length + 1;
    SPI_RX_IDX = 0;

    /*Enable USCI0 RX interrupt*/
    IE2 |= UCB0RXIE;

    /*Send Dummy bytes*/
    spi_write(length, SPI_DUMMY);

    /*Copy received data into the passed in array*/
    if (SPI_RX_CTR == 0)
    {
        for (SPI_COPY_IDX = 0; SPI_COPY_IDX < length; SPI_COPY_IDX++)
        {
            rxData[SPI_COPY_IDX] = SPI_RX_BUFF[SPI_COPY_IDX + 1];
        }
    }
}

// Write <length> bytes from <txData> (1 pt.)
void spi_write(unsigned char length, unsigned char *txData)
{
    SPI_TX_CTR = length;
    SPI_TX_IDX = 0;

    /*Copy the contents of the passed in array to SPI_TX_BUFF*/
    for (SPI_COPY_IDX = 0; SPI_COPY_IDX < length; SPI_COPY_IDX++)
    {
        SPI_TX_BUFF[SPI_COPY_IDX] = txData[SPI_COPY_IDX];
    }

    /*Enable USCI0 TX interrupt*/
    IE2 |= UCB0TXIE;
}

/*Full duplex communication*/
void spi_transcieve(unsigned char w_length, unsigned char rd_length,
                    unsigned char *txData, unsigned char *rxData)
{
    P3OUT &= ~(BIT7);
    spi_write(w_length, txData);
    spi_read(rd_length, rxData);
    P3OUT |= BIT7;
}

/*SPI TX function*/
void SPI_TX(void)
{
    if (IFG2 & UCB0TXIFG)
    {
        if (SPI_TX_CTR)
        {
            /*Put data into TX Buffer*/
            UCB0TXBUF = SPI_TX_BUFF[SPI_TX_IDX++];
            SPI_TX_CTR--;
        }
        else
        {
            /*Disable USCI0 TX interrupt*/
            IE2 &= ~UCB0TXIE;
        }
    }
}

/*SPI RX function*/
void SPI_RX(void)
{
    if (IFG2 & UCB0RXIFG)
    {
        unsigned char SPI_RX_VAL;

        while (spi_busy())
        {

        }

        /*Read RX Buffer contents*/
        SPI_RX_VAL = UCB0RXBUF;

        if (SPI_RX_CTR)
        {
            SPI_RX_BUFF[SPI_RX_IDX++] = SPI_RX_VAL;
            SPI_RX_CTR--;
        }
        else
        {
            /*Disable USCI0 RX interrupt*/
            IE2 &= ~UCB0RXIE;
        }
    }
}

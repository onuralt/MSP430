/***************************************************************************//**
 * @file    i2c.c
 * @author  <Onur Altinordu>
 * @email   <onur.altinordu@mars.uni-freiburg.de>
 * @matno   <5250773>
 * @exno    <Exercise 4>
 * @date    <25.06.2022>
 *
 * @brief   <I2C library function implementations>
 *
 * Here goes a detailed description if required.
 ******************************************************************************/

#include "./i2c.h"
#include "./USCIAB0.h"

/******************************************************************************
 * VARIABLES
 *****************************************************************************/
static unsigned char *TX_BUFF;
static unsigned char TX_LENGTH;
static unsigned char *RX_BUFF;
static unsigned char RX_LENGTH;
static int transferFinished;
static int TX_STOP;
static int NACK;
/******************************************************************************
 * FUNCTION IMPLEMENTATION
 *****************************************************************************/

// TODO: Implement these functions:
void i2c_init(unsigned char addr)
{
    /*Empty TX and RX Buffers*/
    UCB0TXBUF = 0;
    UCB0RXBUF = 0;

    /*Hook I2C RX and TX functions to the ISR and unhook
     * SPI functions*/
    USCIAB0_InterruptEnable(I2C, RX, &I2C_RX);
    USCIAB0_InterruptEnable(I2C, TX, &I2C_TX);

    /*P.1 must be set in order to route the I2C bus*/
    P3DIR |= (BIT3);
    P3OUT |= (BIT3);

    /* disable USCI B */
    UCB0CTL1 |= UCSWRST;

    /* enable master mode with 7 bit slave addresses */
    UCB0CTL0 = UCSYNC | UCMODE_3 | UCMST;

    /* select SMCLK as clock source */

    UCB0CTL1 = UCSSEL_2;

    /* UCBR = f_SMCLK / f_BitClock = 1 MHz / 100 kHz = 10 */
    UCB0BR0 = 10; //degisti 80
    UCB0BR1 = 0;

    /*Slave Address*/
    UCB0I2CSA = addr;

    /*I2C Pins*/
    P1SEL = BIT7 + BIT6;
    P1SEL2 = BIT7 + BIT6;

    /* Enable USCI B */
    UCB0CTL1 &= ~UCSWRST;

    /* enable receive and transmit interrupts */
    IE2 |= UCB0TXIE | UCB0RXIE;

    /* disable STOP, START, arbitration lost interrupts */
    UCB0I2CIE &= ~(UCSTPIE | UCSTTIE | UCALIE);

    /* enable NACK interrupt */
    UCB0I2CIE |= UCNACKIE;

    transferFinished = 0;
    NACK = 0;
}

int i2c_busy(void)
{
    return transferFinished;
}

int i2c_nack_received(void)
{
    return NACK;
}

unsigned char i2c_write(unsigned char length, unsigned char *txData,
                        unsigned char stop)
{
    TX_BUFF = txData;
    TX_LENGTH = length;
    TX_STOP = stop;

    transferFinished = 1;
    NACK = 0;

    /* select transmitter mode */
    UCB0CTL1 |= UCTR;

    /* generate START condition and transmit slave address */
    UCB0CTL1 |= UCTXSTT;
    while (i2c_busy())
        ;
    return i2c_nack_received();
}

void i2c_read(unsigned char length, unsigned char *rxData)
{
    RX_BUFF = rxData;

    transferFinished = 1;
    NACK = 0;

    /* select receiver mode */
    UCB0CTL1 &= ~UCTR;

    if (length == 1)
    {
        RX_LENGTH = 0;

        /* generate START condition and transmit slave address */
        UCB0CTL1 |= UCTXSTT;

        /* wait for slave to acknowledge address */
        while (UCB0CTL1 & UCTXSTT)
            ;

        /* generate STOP condition */
        UCB0CTL1 |= UCTXSTP;
    }
    else
    {
        RX_LENGTH = length - 1;

        /* generate START condition and transmit slave address */
        UCB0CTL1 |= UCTXSTT;
    }
    while (i2c_busy())
        ;
}

void I2C_TX(void)
{
    /* transmit buffer empty? */
    if (IFG2 & UCB0TXIFG)
    {
        if (TX_LENGTH--)
        {
            /* load data into TX buffer */
            UCB0TXBUF = *TX_BUFF++;
        }
        else
        {
            /* reset TX interrupt flag */
            IFG2 &= ~UCB0TXIFG;
            if (TX_STOP)
            {
                /* generate STOP condition */
                UCB0CTL1 |= UCTXSTP;

                /* wait until STOP has been sent */
                while (UCB0CTL1 & UCTXSTP)
                    ;
            }
            transferFinished = 0;
        }
    }

    /* check if byte was received */
    if (IFG2 & UCB0RXIFG)
    {
        /* last byte? */
        if (RX_LENGTH == 0)
        {
            /* get data from receive buffer */
            *RX_BUFF = UCB0RXBUF;
            IFG2 &= ~UCB0RXIFG;
            transferFinished = 0;
        }
        else
        {
            /* second to last byte? */
            if (RX_LENGTH == 1)
            {
                /* generate STOP after reception of last byte */
                UCB0CTL1 |= UCTXSTP;
            }

            /* get data */
            *RX_BUFF++ = UCB0RXBUF;
            --RX_LENGTH;
        }
    }
}

void I2C_RX(void)
{
    /* check for NACK sent by slave */
    if (UCB0STAT & UCNACKIFG)
    {
        NACK = 1;

        /* reset NACK interrupt flag */
        UCB0STAT &= ~UCNACKIFG;

        /* generate STOP condition */
        UCB0CTL1 |= UCTXSTP;

        /* wait until STOP has been sent */
        while (UCB0CTL1 & UCTXSTP)
            ;
        transferFinished = 0;
    }
}

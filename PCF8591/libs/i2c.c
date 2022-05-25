/***************************************************************************//**
 * @file    i2c.c
 * @author  <Onur Altinordu>
 * @email   <onur.altinordu@mars.uni-freiburg.de>
 * @matno   <5250773>
 * @exno    <Exercise 2>
 * @date    <25.05.2022>
 *
 * @brief   <I2C library function implementations>
 *
 * Here goes a detailed description if required.
 ******************************************************************************/

#include "./i2c.h"

/******************************************************************************
 * VARIABLES
 *****************************************************************************/

// A variable to be set by your interrupt service routine:
// 1 if all bytes have been sent, 0 if transmission is still ongoing.
unsigned char transferFinished = 0;

unsigned char RX_BUFF[20] = { 0 };
unsigned char RX_LENGTH = 0;
unsigned char RX_IDX = 0;
unsigned char TX_BUFF[20] = { 0 };
unsigned char TX_LENGTH = 0;
unsigned char TX_IDX = 0;
unsigned char COPY_IDX;
unsigned char STP = 0;
unsigned char NACK = 0;

/******************************************************************************
 * LOCAL FUNCTION PROTOTYPES
 *****************************************************************************/

/******************************************************************************
 * LOCAL FUNCTION IMPLEMENTATION
 *****************************************************************************/

/******************************************************************************
 * FUNCTION IMPLEMENTATION
 *****************************************************************************/

// TODO: Implement these functions:
void i2c_init(unsigned char addr)
{
    P1DIR |= (BIT3);
    P1OUT |= (BIT3);                                    // P.1 must be set in order to route the I2C bus
    P1SEL |= BIT6 + BIT7;                               // USCI_B0
    P1SEL2 |= BIT6 + BIT7;                              // USCI_B0
    UCB0CTL1 |= UCSWRST;                                // Enable SW reset
    UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;               // I2C Master, synchronous mode
    UCB0CTL1 = UCSSEL_2 + UCSWRST;                      // Use SMCLK, SW reset
    UCB0BR0 = 160;                                      // fSCL = SMCLK/160 = ~100kHz
    UCB0BR1 = 0;
    UCB0I2CSA = addr;                                   // Slave Address
    UCB0CTL1 &= ~UCSWRST;                               // Clear SW reset
    UCB0I2CIE |= UCNACKIE;                              // Enable NACK interrupt
}
unsigned char i2c_write(unsigned char length, unsigned char *txData,
                        unsigned char stop)
{
    STP = stop;
    // Before writing, you should always check if the last STOP-condition has already been sent.
    while (UCB0CTL1 & UCTXSTP)
        ;
    //Copy register data to TX_BUFF
    for (COPY_IDX = 0; COPY_IDX < length; COPY_IDX++)
    {
        TX_BUFF[COPY_IDX] = txData[COPY_IDX];
    }

    TX_LENGTH = length;
    TX_IDX = 0;

    /* Initialize slave address and interrupts */
    IFG2 &= ~(UCB0TXIFG + UCB0RXIFG);                   // Clear pending interrupts
    IE2 &= ~UCB0RXIE;                                   // Disable RX interrupt
    IE2 |= UCB0TXIE;                                    // Enable TX interrupt

    UCB0CTL1 |= UCTR + UCTXSTT;                         // I2C Transmit, start condition
    __bis_SR_register(CPUOFF + GIE);                    // Enter LPM0 w/ interrupts

    // Wait for transfer to be finished.
    // Info: In TI's sample code, low-power mode statements are inserted,
    // also waiting for the transfer to be finished.
    while (!transferFinished)
        ;
    transferFinished = 0;
    if (NACK == 1)
    {
        NACK = 0;
        return 1;                                       // Return 1 if there exists a NACK
    }
    else
    {
        return 0;                                       // Return 0 if NACK doesn't exist
    }
}

void i2c_read(unsigned char length, unsigned char *rxData)
{
    // Before writing, you should always check if the last STOP-condition has already been sent.
    while (UCB0CTL1 & UCTXSTP)
        ;

    RX_LENGTH = length;
    RX_IDX = 0;

    /* Initialize slave address and interrupts */
    IFG2 &= ~(UCB0TXIFG + UCB0RXIFG);                   // Clear pending interrupts

    IE2 |= UCB0RXIE;                                    // Enable RX interrupt
    IE2 &= ~UCB0TXIE;                                   // Disable TX interrupt

    UCB0CTL1 &= ~UCTR;                                  // I2C Receive
    UCB0CTL1 |= UCTXSTT;                                // start condition

    if (length == 1)
    {
        //Must send stop since this is the N-1 byte
        while ((UCB0CTL1 & UCTXSTT))
            ;
        UCB0CTL1 |= UCTXSTP;                            // Stop condition
    }

    __bis_SR_register(CPUOFF + GIE);                    // Enter LPM0 w/ interrupts

    // Wait for transfer to be finished.
    // Info: In TI's sample code, low-power mode statements are inserted,
    // also waiting for the transfer to be finished.
    while (!transferFinished)
        ;
    //Copy register data to TX_BUFF
    for (COPY_IDX = 0; COPY_IDX < length; COPY_IDX++)   // Copy the read data into the passed in array
    {
        rxData[COPY_IDX] = RX_BUFF[COPY_IDX];
    }
    transferFinished = 0;
}

#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void)
{
    // TODO: Read RX-Buffer or write TX-Buffer, depending on what you'd like to do.
    if (IFG2 & UCB0RXIFG)                               // Receive Interrupt
    {
        unsigned char rx_val = UCB0RXBUF;               // Read from UCB0RXBUF

        if (RX_LENGTH)                                  // Copy buffer data into RX_BUFF until the end
        {
            RX_BUFF[RX_IDX++] = rx_val;
            RX_LENGTH--;
        }

        if (RX_LENGTH == 1)                             // Issue a Stop Condition at the end
        {
            UCB0CTL1 |= UCTXSTP;
        }
        else if (RX_LENGTH == 0)                        // Disable Receive Interrupt after reading all data
        {
            IE2 &= ~UCB0RXIE;
            __bic_SR_register_on_exit(CPUOFF);          // Exit LPM0
        }

    }
    else if (IFG2 & UCB0TXIFG)                          // Transmit Interrupt
    {
        if (TX_LENGTH)
        {
            UCB0TXBUF = TX_BUFF[TX_IDX++];
            TX_LENGTH--;
        }
        else if (STP == 1)
        {
            //Done with transmission
            STP = 0;
            UCB0CTL1 |= UCTXSTP;                        // Send stop condition
            IE2 &= ~UCB0TXIE;                           // disable TX interrupt
            __bic_SR_register_on_exit(CPUOFF);          // Exit LPM0
        }
        else
        {
            //Done with transmission
            IE2 &= ~UCB0TXIE;                           // disable TX interrupt
            __bic_SR_register_on_exit(CPUOFF);          // Exit LPM0
        }
    }
    IFG2 &= ~(UCB0TXIFG | UCB0RXIFG);
    // Exit waiting mode after this interrupt, i.e. set the transferFinished variable.
    // TODO: Call this only when necessary
    transferFinished = 1;
}

#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCIAB0RX_ISR(void)
{
    // TODO: Check for NACKs
    if (UCB0STAT & UCNACKIFG)
    {
        UCB0STAT &= ~UCNACKIFG;                         // Clear NACK Flags
        NACK = 1;
    }
    if (UCB0STAT & UCSTPIFG)                            // Stop or NACK Interrupt
    {
        UCB0STAT &= ~(UCSTTIFG + UCSTPIFG + UCNACKIFG); //Clear START/STOP/NACK Flags
    }
    if (UCB0STAT & UCSTTIFG)
    {
        UCB0STAT &= ~(UCSTTIFG);                        //Clear START Flags
    }
}

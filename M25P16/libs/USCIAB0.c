/***************************************************************************//**
 * @file    USCIAB0.c.c
 * @author  <Onur Altinordu>
 * @email   <onur.altinordu@mars.uni-freiburg.de>
 * @matno   <5250773>
 * @exno    <Exercise 4>
 * @date    <25.06.2022>
 *
 * @brief   <Function implementationsfor differentiating I2c and SPI>
 *
 * Here goes a detailed description if required.
 ******************************************************************************/

#include "./USCIAB0.h"

/******************************************************************************
 * VARIABLES
 *****************************************************************************/
void (*USCIAB0_SPI_RX)();
void (*USCIAB0_I2C_RX)();
void (*USCIAB0_SPI_TX)();
void (*USCIAB0_I2C_TX)();
/******************************************************************************
 * FUNCTION IMPLEMENTATION
 *****************************************************************************/

void USCIAB0_InterruptEnable(unsigned char comm_type, unsigned char txrx,
                             void (*functPtr)())
{
    /* associate the function pointer */
    switch (comm_type)
    {
    case 0:
        /*I2C*/
        switch (txrx)
        {
        case 0:
            /*TX*/
            USCIAB0_InterruptDisable(1);
            USCIAB0_I2C_TX = functPtr;
            break;
        case 1:
            /*RX*/
            USCIAB0_InterruptDisable(1);
            USCIAB0_I2C_RX = functPtr;
            break;
        default:
            /*Programmers Trap*/
            while (1)
            {

            }
        }
        break;
    case 1:
        /*SPI*/
        switch (txrx)
        {
        case 0:
            /*TX*/
            USCIAB0_InterruptDisable(0);
            USCIAB0_SPI_TX = functPtr;
            break;
        case 1:
            /*RX*/
            USCIAB0_InterruptDisable(0);
            USCIAB0_SPI_RX = functPtr;
            break;
        default:
            /*Programmers Trap*/
            while (1)
            {

            }
        }
        break;
    default:
        /*Programmers Trap*/
        while (1)
        {

        }
    }
}

void USCIAB0_InterruptDisable(unsigned char mode)
{
    /* associate the function pointer */
    switch (mode)
    {
    case 0:
        /*I2C*/
        USCIAB0_I2C_RX = 0;
        USCIAB0_I2C_TX = 0;
        break;
    case 1:
        /*SPI*/
        USCIAB0_SPI_RX = 0;
        USCIAB0_SPI_TX = 0;
        break;
    default:
        /*Programmers Trap*/
        while (1)
        {

        }
    }
}

/*USCIAB0RX ISR*/
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCIAB0RX_ISR()
{
    if (USCIAB0_I2C_RX != 0)
    {
        (*USCIAB0_I2C_RX)();
    }
    else if (USCIAB0_SPI_RX != 0)
    {
        (*USCIAB0_SPI_RX)();
    }
    else
    {
        while (1)
        {

        }
    }
}

/*USCIAB0TX ISR*/
#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR()
{
    if (USCIAB0_I2C_TX != 0)
    {
        (*USCIAB0_I2C_TX)();
    }
    else if (USCIAB0_SPI_TX != 0)
    {
        (*USCIAB0_SPI_TX)();
    }
    else
    {
        while (1)
        {

        }
    }
}

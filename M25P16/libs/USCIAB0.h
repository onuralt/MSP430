/***************************************************************************//**
 * @file    USCIAB0.h
 * @author  <Onur Altinordu>
 * @email   <onur.altinordu@mars.uni-freiburg.de>
 * @matno   <5250773>
 * @exno    <Exercise 4>
 * @date    <25.06.2022>
 *
 * @brief   <USCIAB0 header file>
 *
 * Here goes a detailed description if required.
 ******************************************************************************/

#ifndef LIBS_USCIAB0_H_
#define LIBS_USCIAB0_H_

/******************************************************************************
 * INCLUDES
 *****************************************************************************/

#include <msp430g2553.h>

/******************************************************************************
 * CONSTANTS
 *****************************************************************************/
#define I2C     0
#define SPI     1
#define TX      0
#define RX      1

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/

void I2C_RX(void);
void SPI_RX(void);
void I2C_TX(void);
void SPI_TX(void);

void USCIAB0_InterruptEnable(unsigned char comm_type, unsigned char txrx,
                             void (*functPtr)());
void USCIAB0_InterruptDisable(unsigned char mode);

#endif /* LIBS_USCIAB0_H_ */

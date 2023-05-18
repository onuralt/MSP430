/***************************************************************************//**
 * @file    i2c.h
 * @author  <Onur Altinordu>
 * @email   <onur.altinordu@mars.uni-freiburg.de>
 * @matno   <5250773>
 * @exno    <Exercise 4>
 * @date    <24.06.2022>
 *
 * @brief   <I2C header file>
 *
 * Here goes a detailed description if required.
 ******************************************************************************/

#ifndef EXERCISE_LIBS_I2C_H_
#define EXERCISE_LIBS_I2C_H_

/******************************************************************************
 * INCLUDES
 *****************************************************************************/

#include <msp430g2553.h>

/******************************************************************************
 * CONSTANTS
 *****************************************************************************/
#define PCF8591         0x48
#define ISL95811        0x28
#define AIN0            0x40
#define AIN1            0x41
#define AIN2            0x42
#define CONFIG_MODE     1
#define DATA_MODE       0
#define SINGLE_WRITE    1
#define DOUBLE_WRITE    2
#define READ_LENGTH     4
#define WITH_STOP       1
#define WITHOUT_STOP    0
#define READ_ONE_BYTE   1
/******************************************************************************
 * VARIABLES
 *****************************************************************************/

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/

// Returns 1 when there is a NACK, returns 0 otherwise
int i2c_nack_received(void);

// Returns 1 when read/write in progress, returns 0 otherwise
int i2c_busy(void);

// Initialize the I2C state machine. The speed should be 100 kBit/s.
// <addr> is the 7-bit address of the slave (MSB shall always be 0, i.e. "right alignment"). (2 pts.)
void i2c_init(unsigned char addr);

// Write a sequence of <length> characters from the pointer <txData>.
// Return 0 if the sequence was acknowledged, 1 if not. Also stop transmitting further bytes upon a missing acknowledge.
// Only send a stop condition if <stop> is not 0. (2 pts.)
unsigned char i2c_write(unsigned char length, unsigned char *txData,
                        unsigned char stop);

// Returns the next <length> characters from the I2C interface. (2 pts.)
void i2c_read(unsigned char length, unsigned char *rxData);

#endif /* EXERCISE_LIBS_I2C_H_ */

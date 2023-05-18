/***************************************************************************//**
 * @file    adac.h
 * @author  <Onur Altinordu>
 * @email   <onur.altinordu@mars.uni-freiburg.de>
 * @matno   <5250773>
 * @exno    <Exercise 4>
 * @date    <25.06.2022>
 *
 * @brief   <PCF8591 header file>
 *
 * Here goes a detailed description if required.
 ******************************************************************************/

#ifndef EXERCISE_LIBS_ADAC_H_
#define EXERCISE_LIBS_ADAC_H_

/******************************************************************************
 * INCLUDES
 *****************************************************************************/

#include "./i2c.h"

/******************************************************************************
 * CONSTANTS
 *****************************************************************************/
#define PCF8591         0x48
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

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/

// All functions return 0 if everything went fine
// and anything but 0 if not.
// Initialize the ADC / DAC
unsigned char adac_init(void);

// Read all ADC-values and write it into the passed values-array.
unsigned char adac_read(unsigned char lenght, unsigned char *values);

// Write a certain value to the DAC. (1 pt.)
unsigned char adac_write(unsigned char conf, unsigned char value);

#endif /* EXERCISE_LIBS_ADAC_H_ */

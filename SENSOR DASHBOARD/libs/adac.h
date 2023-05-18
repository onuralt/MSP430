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

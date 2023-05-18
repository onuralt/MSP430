/***************************************************************************//**
 * @file    joy.h
 * @author  <Onur Altinordu>
 * @email   <onur.altinordu@mars.uni-freiburg.de>
 * @matno   <5250773>
 * @exno    <Exercise 4>
 * @date    <25.06.2022>
 *
 * @brief   <Joystick header file>
 *
 * Here goes a detailed description if required.
 ******************************************************************************/

#ifndef LIBS_JOY_H_
#define LIBS_JOY_H_

/******************************************************************************
 * INCLUDES
 *****************************************************************************/

#include <msp430g2553.h>

/******************************************************************************
 * CONSTANTS
 *****************************************************************************/

/******************************************************************************
 * VARIABLES
 *****************************************************************************/

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/

/* Initialize joystick*/
unsigned char joy_init(void);

/* Return X axis value*/
unsigned char joy_read_X(void);

/* Return Y axis value*/
unsigned char joy_read_Y(void);

/* Check if the button is pressed*/
unsigned char joy_read_B(void);

/* Read all axes and put the into the passed in array (X, Y, B)*/
void joy_read_all(unsigned char *AXES);

#endif /* LIBS_JOY_H_ */

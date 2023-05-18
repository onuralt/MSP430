/***************************************************************************//**
 * @file    actuatorMain.h
 * @author  <Onur Altinordu>
 * @email   <onur.altinordu@mars.uni-freiburg.de>
 * @matno   <5250773>
 * @exno    <Exercise 5>
 * @date    <30.07.2022>
 *
 * @brief   <Actuator Control header file>
 *
 * Here goes a detailed description if required.
 ******************************************************************************/

#ifndef LIBS_ACTUATORMAIN_H_
#define LIBS_ACTUATORMAIN_H_

/******************************************************************************
 * INCLUDES
 *****************************************************************************/

#include <msp430g2553.h>

/******************************************************************************
 * CONSTANTS
 *****************************************************************************/

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/
void actuatorMain_init(void);
void light_lvl(int lvl);
void led_control(char led_array[4]);

#endif /* LIBS_ACTUATORMAIN_H_ */

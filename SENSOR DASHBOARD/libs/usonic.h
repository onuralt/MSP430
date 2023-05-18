/***************************************************************************//**
 * @file    usonic.h
 * @author  <Onur Altinordu>
 * @email   <onur.altinordu@mars.uni-freiburg.de>
 * @matno   <5250773>
 * @exno    <Exercise 5>
 * @date    <30.07.2022>
 *
 * @brief   <Ultrasonic sensor header file>
 *
 * Here goes a detailed description if required.
 ******************************************************************************/

#ifndef LIBS_USONIC_H_
#define LIBS_USONIC_H_

/******************************************************************************
 * INCLUDES
 *****************************************************************************/

#include <msp430g2553.h>

/******************************************************************************
 * CONSTANTS
 *****************************************************************************/
#define usonic_StartTA0()          TA0CTL |= MC_2; //START TIMER IN CONT MODE
#define usonic_StopTA0()           TA0CTL &= ~MC_2;
/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/

/* Initialize sensor*/
void usonic_init(void);

/* Read sensor data*/
void usonic_read(unsigned int cm);

/* Initialise Timer0 for PWM output*/
void usonic_TA0_init(void);

#endif /* LIBS_USONIC_H_ */

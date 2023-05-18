/***************************************************************************//**
 * @file    sensorMain.h
 * @author  <Onur Altinordu>
 * @email   <onur.altinordu@mars.uni-freiburg.de>
 * @matno   <5250773>
 * @exno    <Exercise 5>
 * @date    <30.07.2022>
 *
 * @brief   <Sensor control header file>
 *
 * Here goes a detailed description if required.
 ******************************************************************************/

#ifndef LIBS_SENSORMAIN_H_
#define LIBS_SENSORMAIN_H_

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

/* Initialize ADC*/
void sensorMain_init(void);

/* Read NTC value*/
int ntcRead(void);

/* Read potentiometer value*/
int potRead(void);

/* Read LDR value*/
int ldrRead(void);

/* Read pushbutton states*/
int pbRead(void);

#endif /* LIBS_SENSORMAIN_H_ */

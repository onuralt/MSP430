/***************************************************************************//**
 * @file    joy.c
 * @author  <Onur Altinordu>
 * @email   <onur.altinordu@mars.uni-freiburg.de>
 * @matno   <5250773>
 * @exno    <Exercise 5>
 * @date    <30.07.2022>
 *
 * @brief   <Joystick library function implementations>
 *
 * Here goes a detailed description if required.
 ******************************************************************************/

#include "./joy.h"
#include "./adac.h"

/******************************************************************************
 * VARIABLES
 *****************************************************************************/
unsigned char JOY_READ_AX[1];
unsigned char JOY_RD_ALL[3];
unsigned char *J_R_A;
unsigned char i;

/******************************************************************************
 * FUNCTION IMPLEMENTATION
 *****************************************************************************/

/* Initialize joystick*/
unsigned char joy_init(void)
{
    return adac_init();
}

/* Return X axis value*/
unsigned char joy_read_X(void)
{
    /*Read AIN0*/
    adac_write(CONFIG_MODE, AIN0);
    adac_read(1, JOY_READ_AX);
    return JOY_READ_AX[0];
}

/* Return Y axis value*/
unsigned char joy_read_Y(void)
{
    /*Read AIN1*/
    adac_write(CONFIG_MODE, AIN1);
    adac_read(1, JOY_READ_AX);
    return JOY_READ_AX[0];
}

/* Check if the button is pressed*/
unsigned char joy_read_B(void)
{
    /*Read AIN2*/
    adac_write(CONFIG_MODE, AIN2);
    adac_read(1, JOY_READ_AX);
    return JOY_READ_AX[0];
}

/* Read all axes and put the into the passed in array (X, Y, B)*/
void joy_read_all(unsigned char *AXES)
{
    for (i = 0; i < 2; i++)
    {
        JOY_RD_ALL[0] = joy_read_Y();
        __delay_cycles(1000);
        JOY_RD_ALL[1] = joy_read_B();
        __delay_cycles(1000);
        JOY_RD_ALL[2] = joy_read_X();
        __delay_cycles(1000);
    }
    for (i = 0; i < 3; i++)
    {
        *AXES++ = JOY_RD_ALL[i];
    }
}

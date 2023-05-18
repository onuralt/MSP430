/***************************************************************************//**
 * @file    joy.c
 * @author  <Onur Altinordu>
 * @email   <onur.altinordu@mars.uni-freiburg.de>
 * @matno   <5250773>
 * @exno    <Exercise 4>
 * @date    <25.06.2022>
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

/*Return X axis value*/
unsigned char JOY_READ_X(void)
{
    /*Read AIN0*/
    adac_write(CONFIG_MODE, AIN0);
    adac_read(1, JOY_READ_AX);
    return JOY_READ_AX[0];
}

/*Return Y axis value*/
unsigned char JOY_READ_Y(void)
{
    /*Read AIN1*/
    adac_write(CONFIG_MODE, AIN1);
    adac_read(1, JOY_READ_AX);
    return JOY_READ_AX[0];
}

/*Check if the button is pressed*/
unsigned char JOY_READ_B(void)
{
    /*Read AIN2*/
    adac_write(CONFIG_MODE, AIN2);
    adac_read(1, JOY_READ_AX);
    return JOY_READ_AX[0];
}

/*Read all axes and put the into the passed in array (X, Y, B)*/
void JOY_READ_ALL(unsigned char *AXES)
{
    for (i = 0; i < 2; i++)
    {
        JOY_RD_ALL[0] = JOY_READ_Y();
        __delay_cycles(1000);
        JOY_RD_ALL[1] = JOY_READ_B();
        __delay_cycles(1000);
        JOY_RD_ALL[2] = JOY_READ_X();
        __delay_cycles(1000);
    }
    for (i = 0; i < 3; i++)
    {
        *AXES++ = JOY_RD_ALL[i];
    }
}

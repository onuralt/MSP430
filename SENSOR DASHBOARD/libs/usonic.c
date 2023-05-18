/***************************************************************************//**
 * @file    usonic.c
 * @author  <Onur Altinordu>
 * @email   <onur.altinordu@mars.uni-freiburg.de>
 * @matno   <5250773>
 * @exno    <Exercise 5>
 * @date    <30.07.2022>
 *
 * @brief   <Ultrasonc sensor library function implementations>
 *
 * Here goes a detailed description if required.
 ******************************************************************************/

#include "./usonic.h"

/******************************************************************************
 * VARIABLES
 *****************************************************************************/

/******************************************************************************
 * FUNCTION IMPLEMENTATION
 *****************************************************************************/

void usonic_init(void)
{
    /* P3.5 is set as output*/
    P3DIR |= BIT3;

    /* P3.5 is set to low*/
    P3OUT &= ~BIT3;

    /* Pullup / Pulldown Resistor enabled for P1.0*/
    P1REN |= BIT0;

    /* Pullup enabled for P1.0*/
    P1OUT |= BIT0;

    /* Initialize Timer0 A*/
    usonic_TA0_init();

    // Clear interrupt flag
    P1IFG &= ~BIT0;

    /* High /Low - Edge*/
    P1IES |= BIT0;

    /* Enable interrupt*/
    P1IE |= BIT0;
}

void usonic_read(unsigned int cm)
{
    unsigned char pulse_count;

    /* Stop capture timer*/
    usonic_StopTA0();

    /* Wait for 1ms*/
    __delay_cycles(1000);

    /* Start sending pulses with 40 KHz*/
    for (pulse_count = 0; pulse_count < 8; pulse_count++)
    {
        P3OUT |= BIT3;
        __delay_cycles(7);
        P3OUT &= ~BIT3;
        __delay_cycles(1);
    }

    /* Reset TAR*/
    TA0CTL |= TACLR;

    /* Start capture timer*/
    usonic_StartTA0();

    return cm;
}

void usonic_TA0_init(void)
{
    /* Clock source = SMCLK(1 MHz)*/
    TA0CTL = TASSEL_2 + TACLR;

    /* Clear TA0CCR0 IFG*/
    TA0CCTL0 &= ~CCIFG;

    /* Capture mode, both edges, GND*/
    TA0CCTL0 |= CAP + CM_3 + CCIS_2;
}

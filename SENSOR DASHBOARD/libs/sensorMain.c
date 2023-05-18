/***************************************************************************//**
 * @file    sensorMain.c
 * @author  <Onur Altinordu>
 * @email   <onur.altinordu@mars.uni-freiburg.de>
 * @matno   <5250773>
 * @exno    <Exercise 5>
 * @date    <30.07.2022>
 *
 * @brief   <Sensor Control function implementations>
 *
 * Here goes a detailed description if required.
 */
/*****************************************************************************/

#include "./sensorMain.h"

/******************************************************************************
 * VARIABLES
 *****************************************************************************/

/******************************************************************************
 * FUNCTION IMPLEMENTATION
 *****************************************************************************/
void sensorMain_init(void)
{

    /* Modify XTAL pins to be I/O*/
    P2SEL &= ~(BIT6 | BIT7);

    /* Set pin direction for shift registers*/
    P2DIR |= (BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6);
    P2DIR &= ~BIT7;

    /* Clear / reset the shift register , i.e. turn all LEDs off*/
    //P2OUT &= ~BIT5;

    /* Stop the reset mode (/ CLR = 1)*/
    //P2OUT |= BIT5;

    /* Reset the clock signal to 0*/
    P2OUT &= ~BIT4;

    /* Set the shift register 2 mode right shift mode (S0 = 1, S1 = 0)*/
    P2OUT |= BIT0;
    P2OUT &= ~BIT1;

    /* Set the shift register 1 mode parallel mode (S0 = 1, S1 = 1)*/
    P2OUT |= BIT2;
    P2OUT |= BIT3;

    /* Set pin direction for pushbuttons*/
    P3DIR &= ~(BIT5 | BIT6);

    /* Pullup / Pulldown Resistor enabled for P3.2, P3.3*/
    P3REN |= (BIT5 | BIT6);

    /* Pullup enabled for P3.2, P3.3*/
    P3OUT |= (BIT5 | BIT6);

    /* Set pin direction*/
    //P3DIR |= (BIT0 | BIT1 | BIT4);
    /* Turn ADC on; use 16 clocks as sample & hold time*/
    ADC10CTL0 = ADC10ON + ADC10SHT_2;

    /* Enable P1.0, P1.4, P1.6 as AD input*/
    ADC10AE0 |= (BIT0 | BIT3 | BIT4);
}

int ntcRead(void)
{
    /* Stop conversion*/
    ADC10CTL0 &= ~(ENC);

    /* Select channel 5 as input for the following conversion(s)*/
    ADC10CTL1 &= ~INCH_3;
    ADC10CTL1 &= ~INCH_4;
    ADC10CTL1 |= INCH_0;

    /* Start conversion*/
    ADC10CTL0 |= (ENC | ADC10SC);

    /* Wait until result is ready*/
    while (ADC10CTL1 & ADC10BUSY)
    {

    }

    /* If result is ready , copy it to reg_1*/
    int reg_1 = ADC10MEM;

    /* Stop conversion*/
    ADC10CTL0 &= ~(ENC);

    /* Disable P1.0, P1.4, P1.6 as AD input*/
    ADC10AE0 &= ~(BIT0 | BIT3 | BIT4);

    return reg_1;
}

int potRead(void)
{
    /* Stop conversion*/
    ADC10CTL0 &= ~(ENC);

    /* Select channel 7 as input for the following conversion(s)*/
    ADC10CTL1 &= ~INCH_3;
    ADC10CTL1 &= ~INCH_0;
    ADC10CTL1 |= INCH_4;

    /* Start conversion*/
    ADC10CTL0 |= (ENC | ADC10SC);

    /* Wait until result is ready*/
    while (ADC10CTL1 & ADC10BUSY)
    {

    }

    /* If result is ready , copy it to reg_1*/
    int reg_1 = ADC10MEM;

    /* Stop conversion*/
    ADC10CTL0 &= ~(ENC);

    /* Disable P1.0, P1.4, P1.6 as AD input*/
    ADC10AE0 &= ~(BIT0 | BIT3 | BIT4);

    return reg_1;
}

int ldrRead(void)
{
    /* Stop conversion*/
    ADC10CTL0 &= ~(ENC);

    /* Select channel 4 as input for the following conversion(s)*/
    ADC10CTL1 &= ~INCH_4;
    ADC10CTL1 &= ~INCH_0;
    ADC10CTL1 |= INCH_3;

    /* Start conversion*/
    ADC10CTL0 |= (ENC | ADC10SC);

    /* Wait until result is ready*/
    while (ADC10CTL1 & ADC10BUSY)
    {

    }

    /* If result is ready , copy it to reg_1*/
    int reg_1 = ADC10MEM;

    /* Stop conversion*/
    ADC10CTL0 &= ~(ENC);

    /* Disable P1.0, P1.4, P1.6 as AD input*/
    ADC10AE0 &= ~(BIT0 | BIT3 | BIT4);

    return reg_1;
}

int pbRead(void)
{
    int pb_reg = 0;
    int i;

    /* Set the shift register 2 mode stop mode (S0 = 0, S1 = 0)*/
    P2OUT &= ~BIT0;
    P2OUT &= ~BIT1;

    /* Set the shift register 1 mode parallel mode (S0 = 1, S1 = 1)*/
    P2OUT |= BIT2;
    P2OUT |= BIT3;

    /* Reset the clock*/
    P2OUT &= ~BIT4;

    /* Apply a rising clock edge => shift data in*/
    P2OUT |= BIT4;

    /* Reset the clock*/
    P2OUT &= ~BIT4;

    /* Set the shift register 1 mode right shift mode (S0 = 1, S1 = 0)*/
    P2OUT |= BIT2;
    P2OUT &= ~BIT3;

    if (!(P3IN & BIT6))
    {
        pb_reg = (pb_reg << 1) + 1;
    }
    else
    {
        pb_reg = pb_reg << 1;
    }

    if (!(P3IN & BIT5))
    {
        pb_reg = (pb_reg << 1) + 1;
    }
    else
    {
        pb_reg = pb_reg << 1;
    }

    /* Check P2.7 and store the result after 4 shifts*/
    for (i = 0; i <= 2; i++)
    {
        if (P2IN & BIT7)
        {
            pb_reg = (pb_reg << 1) + 1;
        }
        else
        {
            pb_reg = pb_reg << 1;
        }

        /* Apply a rising clock edge => shift data in*/
        P2OUT |= BIT4;

        /* Reset the clock*/
        P2OUT &= ~BIT4;
    }
    if (P2IN & BIT7)
    {
        pb_reg = (pb_reg << 1) + 1;
    }
    else
    {
        pb_reg = pb_reg << 1;
    }

    return pb_reg;
}

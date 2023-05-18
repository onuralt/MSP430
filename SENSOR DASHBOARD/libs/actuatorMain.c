/***************************************************************************//**
 * @file    actuatorMain.c
 * @author  <Onur Altinordu>
 * @email   <onur.altinordu@mars.uni-freiburg.de>
 * @matno   <5250773>
 * @exno    <Exercise 5>
 * @date    <30.07.2022>
 *
 * @brief   <Actuator control header file>
 *
 * Here goes a detailed description if required.
 ******************************************************************************/

#include "./actuatorMain.h"
#include "string.h"

/******************************************************************************
 * VARIABLES
 *****************************************************************************/

/******************************************************************************
 * FUNCTION IMPLEMENTATION
 *****************************************************************************/

void actuatorMain_init(void)
{
    /* Modify XTAL pins to be I/O*/
    P2SEL &= ~(BIT6 | BIT7);

    /* Set pin direction for shift registers*/
    P2DIR |= (BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6);
    P2DIR &= ~BIT7;

    /* Clear / reset the shift register , i.e. turn all LEDs off*/
    P2OUT &= ~BIT5;

    /* Stop the reset mode (/ CLR = 1)*/
    P2OUT |= BIT5;

    /* Reset the clock signal to 0*/
    P2OUT &= ~BIT4;

    /* Set the shift register 2 mode right shift mode (S0 = 1, S1 = 0)*/
    P2OUT |= BIT0;
    P2OUT &= ~BIT1;

    /* Set the shift register 1 mode parallel mode (S0 = 1, S1 = 1)*/
    P2OUT |= BIT2;
    P2OUT |= BIT3;
}

void light_lvl(int lvl)
{
    int i;
    // Set the shift register 2 mode right shift mode (S0 = 1, S1 = 0) ,
    P2OUT |= BIT0;
    P2OUT &= ~ BIT1;
    // Clear / reset the shift register , i.e. turn all LEDs off.
    P2OUT &= ~ BIT5;
    // stop the reset mode (/ CLR = 1) and
    P2OUT |= BIT5;
    switch (lvl)
    {
    case 1:
        // Clear / reset the shift register , i.e. turn all LEDs off.
        P2OUT &= ~ BIT5;
        // stop the reset mode (/ CLR = 1) and
        P2OUT |= BIT5;
        break;
    case 2:
        // reset the clock
        P2OUT &= ~ BIT4;
        // apply the data to be shifted at SR (SR = 1 to insert a 1).
        P2OUT |= BIT6;
        // apply a rising clock edge => shift data in
        P2OUT |= BIT4;
        break;
    case 3:
        // reset the clock
        P2OUT &= ~ BIT4;
        // apply the data to be shifted at SR (SR = 1 to insert a 1).
        P2OUT |= BIT6;
        // apply a rising clock edge => shift data in
        P2OUT |= BIT4;
        // apply the data to be shifted at SR (SR = 0 to insert a 0).
        P2OUT &= ~BIT6;
        // reset the clock
        P2OUT &= ~ BIT4;
        // apply a rising clock edge => shift data in
        P2OUT |= BIT4;
        break;
    case 4:
        // reset the clock
        P2OUT &= ~ BIT4;
        // apply the data to be shifted at SR (SR = 1 to insert a 1).
        P2OUT |= BIT6;
        // apply a rising clock edge => shift data in
        P2OUT |= BIT4;
        // apply the data to be shifted at SR (SR = 0 to insert a 0).
        P2OUT &= ~BIT6;
        for (i = 0; i <= 1; i++)
        {
            // reset the clock
            P2OUT &= ~ BIT4;
            // apply a rising clock edge => shift data in
            P2OUT |= BIT4;
        }
        break;
    case 5:
        // reset the clock
        P2OUT &= ~ BIT4;
        // apply the data to be shifted at SR (SR = 1 to insert a 1).
        P2OUT |= BIT6;
        // apply a rising clock edge => shift data in
        P2OUT |= BIT4;
        // apply the data to be shifted at SR (SR = 0 to insert a 0).
        P2OUT &= ~BIT6;
        for (i = 0; i <= 2; i++)
        {
            // reset the clock
            P2OUT &= ~ BIT4;
            // apply a rising clock edge => shift data in
            P2OUT |= BIT4;
        }
        break;
    case 6:
        // reset the clock
        P2OUT &= ~ BIT4;
        // apply the data to be shifted at SR (SR = 1 to insert a 1).
        P2OUT |= BIT6;
        for (i = 0; i <= 1; i++)
        {
            // reset the clock
            P2OUT &= ~ BIT4;
            // apply a rising clock edge => shift data in
            P2OUT |= BIT4;
        }
        break;
    case 7:
        // reset the clock
        P2OUT &= ~ BIT4;
        // apply the data to be shifted at SR (SR = 1 to insert a 1).
        P2OUT |= BIT6;
        // apply a rising clock edge => shift data in
        P2OUT |= BIT4;
        // apply the data to be shifted at SR (SR = 0 to insert a 0).
        P2OUT &= ~BIT6;
        // reset the clock
        P2OUT &= ~ BIT4;
        // apply a rising clock edge => shift data in
        P2OUT |= BIT4;
        // apply the data to be shifted at SR (SR = 1 to insert a 1).
        P2OUT |= BIT6;
        // reset the clock
        P2OUT &= ~ BIT4;
        // apply a rising clock edge => shift data in
        P2OUT |= BIT4;
        break;
    case 8:
        // reset the clock
        P2OUT &= ~ BIT4;
        // apply the data to be shifted at SR (SR = 1 to insert a 1).
        P2OUT |= BIT6;
        // apply a rising clock edge => shift data in
        P2OUT |= BIT4;
        // apply the data to be shifted at SR (SR = 0 to insert a 0).
        P2OUT &= ~BIT6;
        for (i = 0; i <= 1; i++)
        {
            // reset the clock
            P2OUT &= ~ BIT4;
            // apply a rising clock edge => shift data in
            P2OUT |= BIT4;
        }
        // reset the clock
        P2OUT &= ~ BIT4;
        // apply the data to be shifted at SR (SR = 1 to insert a 1).
        P2OUT |= BIT6;
        // apply a rising clock edge => shift data in
        P2OUT |= BIT4;
        break;
    case 9:
        // reset the clock
        P2OUT &= ~ BIT4;
        // apply the data to be shifted at SR (SR = 1 to insert a 1).
        P2OUT |= BIT6;
        for (i = 0; i <= 1; i++)
        {
            // reset the clock
            P2OUT &= ~ BIT4;
            // apply a rising clock edge => shift data in
            P2OUT |= BIT4;
        }
        // apply the data to be shifted at SR (SR = 0 to insert a 0).
        P2OUT &= ~BIT6;
        // reset the clock
        P2OUT &= ~ BIT4;
        // apply a rising clock edge => shift data in
        P2OUT |= BIT4;
        break;
    case 10:
        // reset the clock
        P2OUT &= ~ BIT4;
        // apply the data to be shifted at SR (SR = 1 to insert a 1).
        P2OUT |= BIT6;
        // apply a rising clock edge => shift data in
        P2OUT |= BIT4;
        // apply the data to be shifted at SR (SR = 0 to insert a 0).
        P2OUT &= ~BIT6;
        // reset the clock
        P2OUT &= ~ BIT4;
        // apply a rising clock edge => shift data in
        P2OUT |= BIT4;
        // apply the data to be shifted at SR (SR = 1 to insert a 1).
        P2OUT |= BIT6;
        // reset the clock
        P2OUT &= ~ BIT4;
        // apply a rising clock edge => shift data in
        P2OUT |= BIT4;
        // apply the data to be shifted at SR (SR = 0 to insert a 0).
        P2OUT &= ~BIT6;
        // reset the clock
        P2OUT &= ~ BIT4;
        // apply a rising clock edge => shift data in
        P2OUT |= BIT4;
        break;
    case 11:
        // reset the clock
        P2OUT &= ~ BIT4;
        // apply the data to be shifted at SR (SR = 1 to insert a 1).
        P2OUT |= BIT6;
        for (i = 0; i <= 1; i++)
        {
            // reset the clock
            P2OUT &= ~ BIT4;
            // apply a rising clock edge => shift data in
            P2OUT |= BIT4;
        }
        // apply the data to be shifted at SR (SR = 0 to insert a 0).
        P2OUT &= ~BIT6;
        for (i = 0; i <= 1; i++)
        {
            // reset the clock
            P2OUT &= ~ BIT4;
            // apply a rising clock edge => shift data in
            P2OUT |= BIT4;
        }
        break;
    case 12:
        // reset the clock
        P2OUT &= ~ BIT4;
        // apply the data to be shifted at SR (SR = 1 to insert a 1).
        P2OUT |= BIT6;
        for (i = 0; i <= 2; i++)
        {
            // reset the clock
            P2OUT &= ~ BIT4;
            // apply a rising clock edge => shift data in
            P2OUT |= BIT4;
        }
        break;
    case 13:
        // reset the clock
        P2OUT &= ~ BIT4;
        // apply the data to be shifted at SR (SR = 1 to insert a 1).
        P2OUT |= BIT6;
        // apply a rising clock edge => shift data in
        P2OUT |= BIT4;
        // apply the data to be shifted at SR (SR = 0 to insert a 0).
        P2OUT &= ~BIT6;
        // reset the clock
        P2OUT &= ~ BIT4;
        // apply a rising clock edge => shift data in
        P2OUT |= BIT4;
        // apply the data to be shifted at SR (SR = 1 to insert a 1).
        P2OUT |= BIT6;
        for (i = 0; i <= 1; i++)
        {
            // reset the clock
            P2OUT &= ~ BIT4;
            // apply a rising clock edge => shift data in
            P2OUT |= BIT4;
        }
        break;
    case 14:
        // reset the clock
        P2OUT &= ~ BIT4;
        // apply the data to be shifted at SR (SR = 1 to insert a 1).
        P2OUT |= BIT6;
        for (i = 0; i <= 1; i++)
        {
            // reset the clock
            P2OUT &= ~ BIT4;
            // apply a rising clock edge => shift data in
            P2OUT |= BIT4;
        }
        // apply the data to be shifted at SR (SR = 0 to insert a 0).
        P2OUT &= ~BIT6;
        // reset the clock
        P2OUT &= ~ BIT4;
        // apply a rising clock edge => shift data in
        P2OUT |= BIT4;
        // apply the data to be shifted at SR (SR = 1 to insert a 1).
        P2OUT |= BIT6;
        // reset the clock
        P2OUT &= ~ BIT4;
        // apply a rising clock edge => shift data in
        P2OUT |= BIT4;
        break;
    case 15:
        // reset the clock
        P2OUT &= ~ BIT4;
        // apply the data to be shifted at SR (SR = 1 to insert a 1).
        P2OUT |= BIT6;
        for (i = 0; i <= 2; i++)
        {
            // reset the clock
            P2OUT &= ~ BIT4;
            // apply a rising clock edge => shift data in
            P2OUT |= BIT4;
        }
        // apply the data to be shifted at SR (SR = 0 to insert a 0).
        P2OUT &= ~BIT6;
        // reset the clock
        P2OUT &= ~ BIT4;
        // apply a rising clock edge => shift data in
        P2OUT |= BIT4;
        break;
    case 16:
        // reset the clock
        P2OUT &= ~ BIT4;
        // apply the data to be shifted at SR (SR = 1 to insert a 1).
        P2OUT |= BIT6;
        for (i = 0; i <= 3; i++)
        {
            // reset the clock
            P2OUT &= ~ BIT4;
            // apply a rising clock edge => shift data in
            P2OUT |= BIT4;
        }
        break;
    }
    // Set the shift register 2 mode stop mode (S0 = 0, S1 = 0) ,
    P2OUT &= ~ BIT0;
    P2OUT &= ~ BIT1;
}

void led_control(char led_array[4])
{
    if (strcmp(led_array, "0000") == 0)
    {
        light_lvl(1);
    }
    else if (strcmp(led_array, "1000") == 0)
    {
        light_lvl(2);
    }
    else if (strcmp(led_array, "0100") == 0)
    {
        light_lvl(3);
    }
    else if (strcmp(led_array, "0010") == 0)
    {
        light_lvl(4);
    }
    else if (strcmp(led_array, "0001") == 0)
    {
        light_lvl(5);
    }
    else if (strcmp(led_array, "1100") == 0)
    {
        light_lvl(6);
    }
    else if (strcmp(led_array, "1010") == 0)
    {
        light_lvl(7);
    }
    else if (strcmp(led_array, "1001") == 0)
    {
        light_lvl(8);
    }
    else if (strcmp(led_array, "0110") == 0)
    {
        light_lvl(9);
    }
    else if (strcmp(led_array, "0101") == 0)
    {
        light_lvl(10);
    }
    else if (strcmp(led_array, "0011") == 0)
    {
        light_lvl(11);
    }
    else if (strcmp(led_array, "1110") == 0)
    {
        light_lvl(12);
    }
    else if (strcmp(led_array, "1101") == 0)
    {
        light_lvl(13);
    }
    else if (strcmp(led_array, "1011") == 0)
    {
        light_lvl(14);
    }
    else if (strcmp(led_array, "0111") == 0)
    {
        light_lvl(15);
    }
    else if (strcmp(led_array, "1111") == 0)
    {
        light_lvl(16);
    }
}

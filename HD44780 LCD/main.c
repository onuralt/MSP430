/***************************************************************************//**
 * @file    main.c
 * @author  <Onur Altinordu>
 * @email   <onur.altinordu@mars.uni-freiburg.de>
 * @matno   <5250773>
 * @exno    <Exercise 1>
 * @date    <10.05.2022>
 *
 * @brief   Exercise 1 - Display Interface
 *
 * CON5:D4 -> CON3:P2.0
 * CON5:D5 -> CON3:P2.1
 * CON5:D6 -> CON3:P2.2
 * CON5:D7 -> CON3:P2.3
 * CON5:RS -> CON4:P3.0
 * CON5:R/W -> CON4:P3.1
 * CON5:E -> CON4:P3.2
 *
 * @note    The project was exported using CCS 8.0.0.
 ******************************************************************************/

#include "libs/templateEMP.h"
#include "libs/LCD.h"

int main(void)
{
    initMSP();
    // TODO: Add your initialization here.
    lcd_init();

    while (1)
    {
        // TODO: Show a funky demo of what you did.
        lcd_funky();
    }
}

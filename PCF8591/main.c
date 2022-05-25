/***************************************************************************//**
 * @file    main.c
 * @author  <Onur Altinordu>
 * @email   <onur.altinordu@mars.uni-freiburg.de>
 * @matno   <5250773>
 * @exno    <Exercise 2>
 * @date    <25.05.2022>
 *
 * @brief   Exercise 2 - I2C
 *
 * CON6:I2C_SPI -> CON2:P1.3
 * CON6:XSCL    -> CON2:P1.6
 * CON6:XSDA    -> CON2:P1.7
 * CON6:UDAC    -> CON5:BCKL
 *
 * @note    The project was exported using CCS 8.0.0.
 *          UART is disabled within templateEMP.h in order to avoid
 *          interference with your I2C routine!
 *
 ******************************************************************************/

#include "libs/templateEMP.h"       // UART disabled, see @note!
#include "libs/adac.h"

unsigned char READ[4] = { };
unsigned char i;

int main(void)
{
    initMSP();
    // TODO: Add your initialization here.
    adac_init();                    // Initialize PCF8591

    while (1)
    {
        adac_read(READ);            // Read AIN0
        for (i = 0; i < 3; i++)
        {
            adac_write(READ[i]);    // Write to UDAC
        }

    }
}

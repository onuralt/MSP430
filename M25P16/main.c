/***************************************************************************//**
 * @file    main.c
 * @author  <Onur Altinordu>
 * @email   <onur.altinordu@mars.uni-freiburg.de>
 * @matno   <5250773>
 * @exno    <Exercise 4>
 * @date    <25.06.2022>
 * @brief   Exercise 4 - SPI
 *
 * Description and pin connections go here.
 *
 * CON6:I2C_SPI -> CON2:P1.3
 * CON6:XSCL    -> CON2:P1.6
 * CON6:XSDA    -> CON2:P1.7
 * CON9:F_/CS   -> CON4:P3.7
 * CON9:F_CLK   -> CON2:P1.5
 * CON9:F_SO    -> CON6:CC_SO
 * CON9:F_SI    -> CON6:CC_SI
 * CON5:D4 -> CON3:P2.0
 * CON5:D5 -> CON3:P2.1
 * CON5:D6 -> CON3:P2.2
 * CON5:D7 -> CON3:P2.3
 * CON5:RS -> CON4:P3.0
 * CON5:R/W -> CON4:P3.1
 * CON5:E -> CON4:P3.2
 *
 * @note    The project was exported using CCS 8.0.0.
 *
 ******************************************************************************/

#include "libs/templateEMP.h"   // UART disabled, see @note!
#include "libs/lcd.h"
#include "libs/flash.h"
#include "libs/adac.h"
#include "libs/joy.h"
#include "libs/lcd.h"
#include "libs/USCIAB0.h"

/******************************************************************************
 * LOCAL FUNCTION PROTOTYPES
 *****************************************************************************/
void change_position(void);
void choose_letter(void);
void issue_letter(void);
/******************************************************************************
 * VARIABLES
 *****************************************************************************/
unsigned char address = 0x000000;
signed char LTR_IX = 0;
signed char MEM_IX = 0;
unsigned char sum = 0;
unsigned char letter;
unsigned char AX[3];
unsigned char i;
unsigned char WELCOME[16];
unsigned char LETTR_MEM[16] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
                                ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
unsigned char ALPHABET[] = { ' ', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I',
                             'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S',
                             'T', 'U', 'V', 'W', 'X', 'Y', 'Z' };
/******************************************************************************
 * FUNCTION IMPLEMENTATION
 *****************************************************************************/
int main(void)
{
    initMSP();

    /*Initialize M25P16*/
    flash_init();

    /*Read the Welcome Message from the flash memory*/
    flash_read(address, 16, WELCOME);

    /*Initialize LCD*/
    lcd_init();

    /*Display Welcome Message for 5 seconds*/
    lcd_cursorShow(1);
    lcd_cursorSet(0, 0);
    lcd_putText("MESSAGE:");
    lcd_cursorSet(0, 1);
    for (i = 0; i < 16; i++)
    {
        sum += WELCOME[i];
    }
    if (sum != 0)
    {
        for (i = 0; i < 16; i++)
        {
            lcd_putChar(WELCOME[i]);
            __delay_cycles(1000);
        }
        __delay_cycles(5000000);
    }
    lcd_clear();
    __delay_cycles(1000);
    lcd_cursorSet(0, 0);

    /*Initialize PCF8591*/
    adac_init();

    while (1)
    {
        /*Get all Joystick axis values (X, Y, B)*/
        JOY_READ_ALL(AX);

        /*Y axis chooses the letter*/
        choose_letter();

        /*Pressing the Joystick writes the message into the
         * flash memory and shifts the cursor to the right */
        issue_letter();

        /*Y axis shifts the cursor to the left and to the right*/
        change_position();
    }
}

/*Y Axis function*/
void choose_letter(void)
{
    if ((AX[1] == 0xFF) & (LTR_IX < 27) & (LTR_IX > -1))
    {
        letter = ALPHABET[LTR_IX];
        lcd_putChar(ALPHABET[LTR_IX++]);

        lcd_cursorSet(MEM_IX, 0);
        __delay_cycles(250000);
    }
    else if ((AX[1] == 0x00) & (LTR_IX < 27) & (LTR_IX > -1))
    {
        letter = ALPHABET[LTR_IX];
        lcd_putChar(ALPHABET[LTR_IX--]);
        lcd_cursorSet(MEM_IX, 0);
        __delay_cycles(250000);
    }
    if (LTR_IX == 27)
    {
        LTR_IX = 0;
    }
    else if (LTR_IX == -1)
    {
        LTR_IX = 24;
    }
}

/*Button function*/
void issue_letter(void)
{
    if ((AX[2] == 0xFF) & (MEM_IX < 16))
    {
        //__delay_cycles(500000);
        if ((AX[2] == 0xFF) & (MEM_IX < 16))
        {
            LETTR_MEM[MEM_IX++] = letter;
            flash_init();
            flash_SE(address);
            flash_write(address, 16, LETTR_MEM);
            LTR_IX = 0;
            lcd_cursorSet(MEM_IX, 0);
            adac_init();
        }
    }
    if (MEM_IX == 16)
    {
        MEM_IX = 0;
        lcd_cursorSet(MEM_IX, 0);
    }
}

/*X Axis function*/
void change_position(void)
{
    if ((AX[0] == 0x00) & (MEM_IX < 16))
    {
        lcd_putChar(LETTR_MEM[MEM_IX]);
        LETTR_MEM[MEM_IX++] = ' ';
        lcd_cursorSet(MEM_IX, 0);
        __delay_cycles(250000);
    }
    if ((AX[0] == 0xFF) & (MEM_IX > -1))
    {
        lcd_putChar(LETTR_MEM[MEM_IX]);
        LETTR_MEM[MEM_IX--] = ' ';
        lcd_cursorSet(MEM_IX, 0);
        __delay_cycles(250000);
    }
    if (MEM_IX == 16)
    {
        MEM_IX = 0;
        lcd_cursorSet(MEM_IX, 0);
        __delay_cycles(250000);
    }
    else if (MEM_IX == -1)
    {
        MEM_IX = 15;
        lcd_cursorSet(MEM_IX, 0);
        __delay_cycles(250000);
    }
}

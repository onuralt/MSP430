/***************************************************************************//**
 * @file    LCD.c
 * @author  <Onur Altinordu>
 * @email   <onur.altinordu@mars.uni-freiburg.de>
 * @matno   <5250773>
 * @exno    <Exercise 1>
 * @date    <10.05.2022>
 *
 * @brief   <Implementation of HD44780 LCD display functions>
 *
 * Here goes a detailed description if required.
 ******************************************************************************/

#include "./LCD.h"

/******************************************************************************
 * VARIABLES
 *****************************************************************************/
unsigned char disp_state = 0x00;
/******************************************************************************
 * LOCAL FUNCTION PROTOTYPES
 *****************************************************************************/

/******************************************************************************
 * LOCAL FUNCTION IMPLEMENTATION
 *****************************************************************************/

/******************************************************************************
 * FUNCTION IMPLEMENTATION
 *****************************************************************************/

// TODO: Implement the functions.
unsigned char str_len(char *string)
{
    unsigned char i;
    // increment i until a null character is found
    for (i = 0; *string != '\0'; ++i)
    {
        string++;
    }
    return i;
}
void lcd_SendNibble(unsigned char nibble, unsigned char mode)
{
    switch (mode)
    {
    case 0:
        // Command mode
        P3OUT &= ~(lcd_RS + lcd_RW);
        break;
    case 1:
        // Read busy flag mode
        P3OUT |= lcd_RW;
        P3OUT &= ~lcd_RS;
        break;
    case 2:
        // Write data
        P3OUT |= lcd_RS;
        P3OUT &= ~lcd_RW;
        break;
    case 3:
        // Read data
        P3OUT |= (lcd_RS + lcd_RW);
        break;
    }
    // E -> HIGH
    P3OUT |= lcd_E;
    __delay_cycles(250);

    // Clear lower 4 bits of Port 2
    P2OUT &= 0xF0;
    // Send out the nibble
    P2OUT |= nibble;

    // E -> LOW
    __delay_cycles(250);
    P3OUT &= ~lcd_E;

    // E -> HIGH
    P3OUT |= lcd_E;
    __delay_cycles(250);
}
void lcd_SendByte(unsigned char byte, unsigned char mode)
{
    unsigned char nibble;
    // Shift the upper 4 bits to the right
    nibble = ((byte & 0xF0) >> 4);
    // Send out high nibble
    lcd_SendNibble(nibble, mode);
    // Send out low nibble
    lcd_SendNibble(byte, mode);

    // E -> HIGH
    P3OUT |= lcd_E;
    __delay_cycles(250);
}
void lcd_pindir(unsigned char dir)
{
    P3DIR |= (lcd_RS + lcd_RW + lcd_E);
    switch (dir)
    {
    case 0:
        // input
        P2DIR &= ~(lcd_D4 + lcd_D5 + lcd_D6 + lcd_D7);
        break;
    case 1:
        // output
        P2DIR |= (lcd_D4 + lcd_D5 + lcd_D6 + lcd_D7);
        break;
    }
}
/*Busyfunction is unfinished and will be completed in the future.
 * This function will then be used instead of the delay functions*/
unsigned char lcd_busy(void)
{
    unsigned char BF;
    lcd_pindir(0);
    P3OUT &= ~(lcd_RS);
    P3OUT |= lcd_RW;
    __delay_cycles(250);

    // E -> HIGH
    P3OUT |= lcd_E;
    __delay_cycles(250);

    BF = (P2IN & BIT3);
    __delay_cycles(1);

    // E -> LOW
    P3OUT &= ~lcd_E;
    __delay_cycles(250);

    // E -> HIGH
    P3OUT |= lcd_E;
    __delay_cycles(250);

    // E -> LOW
    P3OUT &= ~lcd_E;
    __delay_cycles(250);

    // E -> HIGH
    P3OUT |= lcd_E;
    __delay_cycles(250);

    P3OUT &= ~(lcd_RS + lcd_RW);
    lcd_pindir(1);

    return BF;
}
void lcd_entry_mode(void)
{
    // I/D -> 1 S-> 0
    lcd_SendByte(0x06, 0);
}
void lcd_clear(void)
{
    // [00 0000] -> [00 0001]
    lcd_SendByte(0x01, 0);
}
void lcd_func_set(void)
{
    // Implementation of the flowchart at page 46 of HD44780 datasheet
    lcd_SendNibble(0x03, 0);
    __delay_cycles(5000);

    lcd_SendNibble(0x03, 0);
    __delay_cycles(200);

    lcd_SendNibble(0x03, 0);
    lcd_SendNibble(0x02, 0);

    lcd_SendByte(0x2C, 0);
}
void lcd_display_ctl(unsigned char d, unsigned char c, unsigned char b)
{
    // Create the byte to be sent according tothe given inputs
    unsigned char byte = 0x01;
    byte = (byte << 1) + d;
    byte = (byte << 1) + c;
    byte = (byte << 1) + b;
    lcd_SendByte(byte, 0);
    // Send byte
    disp_state = byte;
}
void lcd_cursorBlink(unsigned char on)
{
    // Retrieve the state of the parameters d and c
    unsigned char d = (disp_state >> 2) & 0x01;
    unsigned char c = (disp_state >> 1) & 0x01;
    // b -> 1
    lcd_display_ctl(d, c, on);
}
void lcd_cursorShow(unsigned char on)
{
    // Retrieve the state of the parameters d and b
    unsigned char d = (disp_state >> 2) & 0x01;
    unsigned char b = disp_state & 0x01;
    // c -> 1
    lcd_display_ctl(d, on, b);
}
void lcd_enable(unsigned char on)
{
    // Retrieve the state of the parameters c and b
    unsigned char c = (disp_state >> 1) & 0x01;
    unsigned char b = disp_state & 0x01;
    // d -> 1
    lcd_display_ctl(on, c, b);
}
void lcd_return(void)
{
    // [00 0000] -> [00 001_]
    lcd_SendByte(0x02, 0);
}

void lcd_cursorSet(unsigned char x, unsigned char y)
{
    /*When N is 1 (2-line display), AAAAAAA can be 00H to 27H for the first
     * line, and 40H to 67H for the second line.*/
    unsigned char pos = 0x81;
    if (x <= 39)
    {
        switch (y)
        {
        case 0:
            pos = 0x80 + x;
            break;
        case 1:
            pos = 0xC0 + x;
            break;
        }
    }
    else
    {
        /*This part is to be sure that the cursor is at the first position
         * if a mistake is made.*/
        pos = 0x80;
    }
    lcd_SendByte(pos, 0);
}
void lcd_putChar(char character)
{
    // Send a character using the write mode.
    lcd_SendByte(character, 2);
    // Return back to the command mode.
    P3OUT &= ~(lcd_RS + lcd_RW);
}
void lcd_putText(char *text)
{
    unsigned char i;
    while ((*text != '\0'))
    {
        /*If the text is longer that 16  characters, only display the first
         * 16 characters*/
        if (str_len(text) > 16)
        {
            for (i = 0; i < 16; i++)
            {
                lcd_putChar(*text++);
            }
            break;
        }
        // Otherwise send characters until a null character is hit
        else
        {
            lcd_putChar(*text++);
        }
    }
}
void lcd_putNumber(int number)
{
    int i = 0;
    int digits[50];
    while (number)
    {
        // Extract the first digit using the mod operator
        digits[i++] = (number % 10);
        // Discard first digit (second digit is now the first digit)
        number /= 10;
    }
    /*Convert to ASCII and start displaying the characters in the list in
     * reverse order*/
    for (i = (i - 1); i >= 0; i--)
    {
        lcd_putChar(0x30 + digits[i]);
    }
}
void lcd_init(void)
{
    // output
    lcd_pindir(1);
    __delay_cycles(1000);
    lcd_func_set();
    __delay_cycles(1000);
    lcd_enable(1);
    __delay_cycles(1000);
    lcd_cursorShow(0);
    __delay_cycles(1000);
    lcd_cursorBlink(0);
    __delay_cycles(1000);
    lcd_clear();
    __delay_cycles(1000);
    lcd_entry_mode();

    // E -> HIGH
    P3OUT |= lcd_E;

    __delay_cycles(50000);
}

void lcd_funky(void)
{
    // LCD demo function
    lcd_putChar('|');
    lcd_cursorSet(0, 0);
    __delay_cycles(250000);
    lcd_putChar('H');
    __delay_cycles(250000);
    lcd_putChar('|');
    lcd_cursorSet(1, 0);
    __delay_cycles(250000);
    lcd_putChar('E');
    __delay_cycles(250000);
    lcd_putChar('|');
    lcd_cursorSet(2, 0);
    __delay_cycles(250000);
    lcd_putChar('L');
    __delay_cycles(250000);
    lcd_putChar('|');
    lcd_cursorSet(3, 0);
    __delay_cycles(250000);
    lcd_putChar('L');
    __delay_cycles(250000);
    lcd_putChar('|');
    lcd_cursorSet(4, 0);
    __delay_cycles(250000);
    lcd_putChar('O');
    __delay_cycles(250000);

    lcd_enable(0);
    __delay_cycles(500000);
    lcd_enable(1);
    __delay_cycles(500000);
    lcd_enable(0);
    __delay_cycles(500000);
    lcd_enable(1);
    __delay_cycles(500000);
    lcd_enable(0);
    __delay_cycles(500000);
    lcd_enable(1);
    __delay_cycles(500000);

    lcd_clear();
    __delay_cycles(500000);

    lcd_cursorSet(0, 0);
    lcd_putChar('W');
    __delay_cycles(250000);
    lcd_cursorSet(1, 1);
    lcd_putChar('e');
    __delay_cycles(250000);
    lcd_cursorSet(2, 0);
    lcd_putChar('l');
    __delay_cycles(250000);
    lcd_cursorSet(3, 1);
    lcd_putChar('c');
    __delay_cycles(250000);
    lcd_cursorSet(4, 0);
    lcd_putChar('o');
    __delay_cycles(250000);
    lcd_cursorSet(5, 1);
    lcd_putChar('m');
    __delay_cycles(250000);
    lcd_cursorSet(6, 0);
    lcd_putChar('e');
    __delay_cycles(250000);
    lcd_cursorSet(7, 1);
    lcd_putChar('!');
    __delay_cycles(250000);
    lcd_cursorSet(8, 0);
    lcd_putChar('!');
    __delay_cycles(250000);
    lcd_cursorSet(9, 1);
    lcd_putChar('!');
    __delay_cycles(500000);

    lcd_clear();
    __delay_cycles(500000);

    lcd_putText("Now, watch the");
    lcd_cursorSet(0, 1);
    lcd_putText("cursor->");
    lcd_cursorBlink(1);
    __delay_cycles(5000000);
    lcd_cursorBlink(0);

    lcd_clear();
    __delay_cycles(500000);

    lcd_putText("IT BLINKS!!!");
    __delay_cycles(5000000);

    lcd_clear();
    __delay_cycles(500000);
    lcd_clear();
    __delay_cycles(500000);

    lcd_putText("I can also do");
    lcd_cursorSet(0, 1);
    lcd_putText("this:");
    lcd_cursorShow(1);
    __delay_cycles(5000000);
    lcd_cursorShow(0);

    lcd_clear();
    __delay_cycles(500000);

    lcd_putText("What an");
    lcd_cursorSet(0, 1);
    lcd_putText("anticlimax...");
    __delay_cycles(5000000);

    lcd_clear();
    __delay_cycles(500000);

    lcd_putText("Time to see");
    lcd_cursorSet(0, 1);
    lcd_putText("some numbers");
    __delay_cycles(5000000);

    lcd_clear();
    __delay_cycles(500000);

    lcd_putNumber(1234);
    __delay_cycles(5000000);

    lcd_clear();
    __delay_cycles(500000);

    lcd_putText("THE END");
    __delay_cycles(5000000);

    lcd_clear();
    __delay_cycles(500000);
}

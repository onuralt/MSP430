/***************************************************************************//**
 * @file    LCD.h
 * @author  <Onur Altinordu>
 * @email   <onur.altinordu@mars.uni-freiburg.de>
 * @matno   <5250773>
 * @exno    <Exercise 1>
 * @date    <10.05.2022>
 *
 * @brief   <HD44780 LCD display header file>
 *
 * Here goes a detailed description if required.
 ******************************************************************************/

#ifndef LIBS_LCD_H_
#define LIBS_LCD_H_

/******************************************************************************
 * INCLUDES
 *****************************************************************************/

#include <msp430g2553.h>

/******************************************************************************
 * CONSTANTS
 *****************************************************************************/

#define lcd_RS        BIT0
#define lcd_RW        BIT1
#define lcd_E         BIT2
#define lcd_D4        BIT0
#define lcd_D5        BIT1
#define lcd_D6        BIT2
#define lcd_D7        BIT3

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/
/** Extra */

// Find the length of a string
unsigned char str_len(char *string);

// Check busy flag
unsigned char lcd_busy(void);

// A funky demo
void lcd_funky(void);

/*Sets entire display (D) on/off, cursor on/off (C), and blinking of
 * cursor position character (B).*/
void lcd_display_ctl(unsigned char d, unsigned char c, unsigned char b);

/*Sets DDRAM address 0 in address counter. Also returns display from being
 * shifted to original position. DDRAM contents remain unchanged.*/
void lcd_return(void);

// Send 4 bits from the data lines
void lcd_SendNibble(unsigned char nibble, unsigned char mode);

// Send two nibbles conescutively
void lcd_SendByte(unsigned char byte, unsigned char mode);

// Set pin directions. 0: input 1: output
void lcd_pindir(unsigned char dir);

/*Implementation of all the "function set" portions of the flowchart at
 * page 46 of the hd44780 datasheet. [00 0011] 4.1ms -> [00 0011] 100us
 * -> [00 0011] -> [00 0010] -> [00 0010] -> [00 1100]*/
void lcd_func_set(void);

/*Sets cursor move direction and specifies display shift. These
 * operations are performed during data write and read. [00 0000] ->
 * [00 0110]*/
void lcd_entry_mode(void);

/** Initialization */

// Initialization of the LCD; set all pin directions,
// basic setup of the LCD, etc. (1 pt.)
void lcd_init(void);

/** Control functions */

// Enable (1) or disable (0) the display (i.e. hide all text) (0.5 pts.)
void lcd_enable(unsigned char on);

// Set the cursor to a certain x/y-position (0.5 pts.)
void lcd_cursorSet(unsigned char x, unsigned char y);

// Show (1) or hide (0) the cursor (0.5 pts.)
void lcd_cursorShow(unsigned char on);

// Blink (1) or don't blink (0) the cursor (0.5 pts.)
void lcd_cursorBlink(unsigned char on);

/** Data manipulation */

// Delete everything on the LCD (1 pt.)
void lcd_clear(void);

// Put a single character on the display at the cursor's current position (1 pt.)
void lcd_putChar(char character);

// Show a given string on the display. If the text is too long to display,
// don't show the rest (i.e. don't break into the next line) (1 pt.).
void lcd_putText(char *text);

// Show a given number at the cursor's current position.
// Note that this is a signed variable! (1 pt.)
void lcd_putNumber(int number);

#endif /* LIBS_LCD_H_ */

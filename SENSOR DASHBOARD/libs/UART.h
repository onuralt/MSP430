/***************************************************************************//**
 * @file    templateEMP.c
 * @author  Sebastian Sester, Marc Schink & Sebastian Stoecklin
 * @date    April 19, 2018
 *
 * @brief   A basic library for MSP430 init and serial interface.
 ******************************************************************************/

#ifndef TEMPLATEEMP_H_
#define TEMPLATEEMP_H_

/******************************************************************************
 * INCLUDES
 *****************************************************************************/

#include <msp430g2553.h>

/******************************************************************************
 * CONSTANTS
 *****************************************************************************/

#define BUFFER_SIZE 32  // receive buffer array size
#define abs(a) (a)<0 ? -(a) : (a)

/******************************************************************************
 * VARIABLES
 *****************************************************************************/

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/

/**
 * serialEchoBack
 * This determines if the user's input should be echoed back or not.
 *
 * @param e   0 if no echo is required, anything else if it is.
 */
void serialEchoBack(char e);

/**
 * This function can be used to check for an buffer-error such as a buffer
 * overflow. Calling this function will also reset the error-variable.
 *
 * @return 0 if there is no error, anything elese if there is one.
 */
char serialError(void);

/**
 * Echo one character to the serial connection. Please note that this
 * function will not work with UTF-8-characters so you should stick
 * to ANSI or ASCII.
 *
 * @param char The character to be displayed.
 */
void serialWrite(char tx);

void serialPrintFloat(float f);

/**
 * Print a given integer as a readable number to serial connection (using
 * the ASCII charmap).
 *
 * @param i   The number to be displayed; 16 bit max.
 */
void serialPrintInt(int i);

/**
 * Print a sequence of characters to the serial connection.
 *
 * @example     serialPrint("output");
 * @param tx    A pointer to the text that shall be printed. Has to be
 *              terminated by \0
 */
void serialPrint(char *tx);

/**
 * Print a sequence of characters to the serial connection and terminate
 * the string with a linebreak. (Note that you'll have to enable "Newline
 * at LF+CR" within HTerm - if you use HTerm.)
 *
 * @example     serialPrint("output");
 * @param tx    A pointer to the text that shall be printed. Has to be
 *              terminated by \0
 */
void serialPrintln(char *tx);

/**
 * Returns 1 if the serial buffer is not empty i.e. some data has been
 * received on the serial connection (e.g. by sending something with HTerm)
 *
 * @return 1 if there is data, 0 if not.
 */
char serialAvailable(void);

/**
 * Clear the serial buffer; all content will be lost.
 */
void serialFlush(void);

/**
 * Returns the first byte from the serial buffer without modifying the
 * same. Returns -1 if the buffer is empty.
 *
 * @return The first byte within the buffer or -1 if the buffer is empty.
 */
int serialPeek(void);

/**
 * Returns the first byte from the serial buffer and removes it from the
 * same. Returns -1 if the buffer is empty.
 *
 * @return The first byte within the buffer or -1 if the buffer is empty.
 */
int serialRead(void);

/**
 * Reads in a number from the serial interface, terminated by any
 * non-numeric character.
 *
 * WARNING: This is a *very basic* implementation and you might want to
 * write your own depending on your scenario and your needs.
 *
 * @return The read-in-number.
 */
int serialReadInt(void);

/*initialize UART*/
void UART_init(void);

/*UART hook function*/
void UART_RX(void);

#endif /* TEMPLATEEMP_H_ */

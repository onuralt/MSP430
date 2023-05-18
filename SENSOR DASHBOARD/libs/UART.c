/***************************************************************************//**
 * @file    UART.c
 * @author  <Sebastian Sester, Marc Schink & Sebastian Stoecklin>
 * @email   <onur.altinordu@mars.uni-freiburg.de>
 * @matno   <5250773>
 * @exno    <Exercise 5>
 * @date    <30.07.2022>
 *
 * @brief   <A basic library for MSP430 init and serial interface.>
 *
 * Here goes a detailed description if required.
 ******************************************************************************/

#include "./UART.h"
#include "./USCIAB0.h"
#include "string.h"

/******************************************************************************
 * VARIABLES
 *****************************************************************************/

// Buffer type definition:
typedef struct
{
    char data[BUFFER_SIZE];
    char start;
    char end;
    char error;
} Buffer_t;

// Ring buffer definition:
Buffer_t ringBuffer = { .start = 0, .end = 0, .error = 0, };

// Echo flag definition:
char echoBack = 0;

/******************************************************************************
 * LOCAL FUNCTION PROTOTYPES
 *****************************************************************************/

/******************************************************************************
 * LOCAL FUNCTION IMPLEMENTATION
 *****************************************************************************/

/******************************************************************************
 * FUNCTION IMPLEMENTATION
 *****************************************************************************/

void serialEchoBack(char e)
{
    /*
     * This is the ternary operator. If e is 0, echoBack will be set 0,
     * else it will be set to 1 (we only need this because you can pass
     * other values but 0 and 1 to the function).
     */

    echoBack = e ? 1 : 0;
}

char serialError()
{
    char r = ringBuffer.error;
    ringBuffer.error = 0;
    return r;
}

void serialWrite(char tx)
{
    /* Loop until the TX buffer is ready.*/
    while (!(IFG2 & UCA0TXIFG))
        ;
    /* Write the character into the TX-register */
    UCA0TXBUF = tx;
    /* And wait until it has been transmitted. */
    while (!(IFG2 & UCA0TXIFG))
        ;
}

void serialPrintFloat(float f)
{
    char result[100];
    int d, dec, i;

    if (f < 0)
    {
        serialWrite(0x2D);
    }
    f = abs(f);

    f += 0.005;   // added after a comment from Matt McNabb, see below.

    d = f;
    dec = (int) (f * 100) % 100;

    memset(result, 0, 100);
    result[0] = (dec % 10) + '0';
    result[1] = (dec / 10) + '0';
    result[2] = '.';

    i = 3;
    while (d > 0)
    {
        result[i] = (d % 10) + '0';
        d /= 10;
        i++;
    }

    for (i = strlen(result) - 1; i >= 0; i--)
        serialWrite(result[i]);
}

void serialPrintInt(int i)
{
    if (i < 0)
    {
        serialWrite(0x2D);
    }
    int j = abs(i);
    // If the number is between 10000 and 65535, print the 10000-
    // digit.
    if (j >= 10000)
    {
        serialWrite(0x30 + i / 10000);
    }
    // Remove the 10000-digit.
    i = i % 10000;
    // Print the 1000-digit, if the number bigger then 999.
    if (j >= 1000)
    {
        serialWrite(0x30 + i / 1000);
    }
    // Now remove the 1000-digit.
    i = i % 1000;
    // Print the 100-digit if the number is big enough ...
    if (j >= 100)
    {
        serialWrite(0x30 + i / 100);
    }
    // ... remove it ...
    i = i % 100;
    // ... same for 10-digit ...
    if (j >= 10)
    {
        serialWrite(0x30 + i / 10);
    }
    // ...
    i = i % 10;
    // Print the last digit, no matter how big the number is (so if the
    // number is 0, we'll just print that).
    serialWrite(0x30 + i / 1);
}

void serialPrint(char *tx)
{
    int b, i = 0;
    // Count the number of bytes we shall display.
    while (tx[i] != 0x00)
    {
        i++;
    }
    // Write each of the bytes we counted.
    for (b = 0; b < i; b++)
    {
        // We already implemented the "write-a-single-character"-function,
        // so we're going to use that function instead of implementing the
        // same stuff here again.
        serialWrite(tx[b]);
    }
}

void serialPrintln(char *tx)
{
    // We don't have to implement this again, just pass tx to the apropriate
    // function.
    serialPrint(tx);
    // Print \n
    serialWrite(0x0D);
    // Print \r
    serialWrite(0x0A);
}

char serialAvailable(void)
{
    // If the buffer's start is not the buffer's end, there's data (return 1)
    if (ringBuffer.start != ringBuffer.end)
    {
        return 1;
    }
    // Else there is none (return 0)
    return 0;
}

void serialFlush(void)
{
    // Set the buffer's start to the buffer's end.
    ringBuffer.start = ringBuffer.end;
}

int serialPeek(void)
{
    // If the buffer's start is the buffer's end, there's no data (return -1)
    if (ringBuffer.start == ringBuffer.end)
    {
        return -1;
    }
    // Return the first byte
    return ringBuffer.data[ringBuffer.start];
}

int serialRead(void)
{
    // If the buffer's start is the buffer's end, there's no data (return -1)
    if (ringBuffer.start == ringBuffer.end)
    {
        return -1;
    }
    // Save the first byte to a temporary variable, move the start-pointer
    char r = ringBuffer.data[ringBuffer.start++];
    ringBuffer.start %= BUFFER_SIZE;
    // and return the stored byte.
    return r;
}

int serialReadInt(void)
{
    int number = 0;
    char stop = 0;
    char negative = 0;
    // While we didn't meet any non-numeric character
    while (!stop)
    {
        // Wait for data
        while (!serialAvailable())
            ;
        // Read the character
        char letter = serialRead();
        // If it's a minus and this is the first figure, it's a negative number
        if (letter == '-' && number == 0)
        {
            negative = 1;
        }
        // If it's a number, add the it to the resulting number
        else if (letter >= '0' && letter <= '9')
        {
            number = number * 10 + (letter - '0');
        }
        // Stop the interpretation elsewise.
        else
        {
            stop = 1;
        }
    }
    if (negative)
    {
        return number * -1;
    }
    return number;
}

void UART_RX(void)
{
    // Store the received byte in the serial buffer. Since we're using a
    // ringbuffer, we have to make sure that we only use RXBUFFERSIZE bytes.
    ringBuffer.data[ringBuffer.end++] = UCA0RXBUF;
    ringBuffer.end %= BUFFER_SIZE;
    // If enabled, print the received data back to user.
    if (echoBack)
    {
        while (!(IFG2 & UCA0TXIFG))
            ;
        UCA0TXBUF = UCA0RXBUF;
    }
    // Check for an overflow and set the corresponding variable.
    if (ringBuffer.start == ringBuffer.end)
    {
        ringBuffer.error = 1;
    }
}

void UART_init(void)
{
    /*Hook UART_RX function to the ISR and unhook
     * I2C & SPI functions*/
    USCIAB0_InterruptEnable(UART, RX, &UART_RX);
    // Activate UART on 1.1 / 1.2
    // (fixed connection to PC, shows up there as COMx)
    P1SEL = BIT1 + BIT2;           // P1.1 = RXD, P1.2=TXD, set everything
    P1SEL2 = BIT1 + BIT2;           // else as a normal GPIO.
    UCA0CTL1 |= UCSSEL_2;           // Use the SMCLK
    UCA0BR0 = 104;                  // 9600 Baud at 1 MHz
    UCA0BR1 = 0;                    // 9600 Baud at 1 MHz
    UCA0MCTL = UCBRS0;              // Modulation UCBRSx = 1
    UCA0CTL1 &= ~UCSWRST;           // Initialize USCI state machine
    IE2 |= UCA0RXIE;                // Enable USCI_A0 RX interrupt
}

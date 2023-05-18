/***************************************************************************//**
 * @file    flash.h
 * @author  <Onur Altinordu>
 * @email   <onur.altinordu@mars.uni-freiburg.de>
 * @matno   <5250773>
 * @exno    <Exercise 4>
 * @date    <25.06.2022>
 *
 * @brief   <M25P16 Flash Memory header file>
 *
 * Here goes a detailed description if required.
 ******************************************************************************/

#ifndef LIBS_FLASH_H_
#define LIBS_FLASH_H_

/******************************************************************************
 * INCLUDES
 *****************************************************************************/

#include "./spi.h"
#include <msp430g2553.h>

/******************************************************************************
 * CONSTANTS
 *****************************************************************************/
#define WIP     0x01
#define WEL     0x02
#define BP0     0x04
#define BP1     0x08
#define BP2     0x10
#define SRWD    0x80
/******************************************************************************
 * VARIABLES
 *****************************************************************************/

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/

// Bulk Erase function
void flash_BE(void);

// Sector Erase function
void flash_SE(long int address);

// Writes to the status register
void flash_write_status(unsigned char BP_2, unsigned char BP_1,
                        unsigned char BP_0);

// Returns the status register content
unsigned char flash_read_status();

// Returns 1 if WEL bit of the status register is not set, 0 if reset
unsigned char flash_WEL_DIS();

// Initialise the flash chip (in case you need it, else leave this function
// empty).
void flash_init(void);

// Read <length> bytes into <rxData> starting from address <address> (1 pt.)
void flash_read(long int address, unsigned char length, unsigned char *rxData);

// Write <length> bytes from <txData>, starting at address <address> (1 pt.)
void flash_write(long int address, unsigned char length, unsigned char *txData);

// Returns 1 if the FLASH is busy or 0 if not.
// Note: this is optional. You will probably need this, but you don't have to
// implement this if you solve it differently.
unsigned char flash_busy(void);

#endif /* LIBS_FLASH_H_ */

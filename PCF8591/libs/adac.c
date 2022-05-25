/***************************************************************************//**
 * @file    adac.c
 * @author  <Onur Altinordu>
 * @email   <onur.altinordu@mars.uni-freiburg.de>
 * @matno   <5250773>
 * @exno    <Exercise 2>
 * @date    <25.05.2022>
 *
 * @brief   <PCF8591 library function implementations>
 *
 * Here goes a detailed description if required.
 ******************************************************************************/

#include "./adac.h"

/******************************************************************************
 * VARIABLES
 *****************************************************************************/
/* Channel 0, Auto-Increment off, Four single-ended inputs, Analog output
 * enabled*/
unsigned char config[1] = { 0x40 };
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
unsigned char adac_init(void)
{
    i2c_init(PCF8591);
    return i2c_write(SINGLE_WRITE, config, WITH_STOP);      // Write and return 1 if there exists a NACK
}
unsigned char adac_read(unsigned char *values)
{
    values[0] = '\0';                                       // Clear the first byte of the passed in array
    i2c_read(4, values);                                    // Read 4 bytes
    if(values[0] == '\0')                                   // Read operation is not done if there exists a NACK
    {
        return 1;                                           // Error
    }
    else
    {
        return 0;                                           // No error
    }
}
unsigned char adac_write(unsigned char value)
{
    unsigned char write_val[2] = { 0x40, value };           // First configure and then write data
    return i2c_write(DOUBLE_WRITE, write_val, WITH_STOP);   // Write and return 1 if there exists a NACK
}

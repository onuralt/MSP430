/***************************************************************************//**
 * @file    adac.c
 * @author  <Onur Altinordu>
 * @email   <onur.altinordu@mars.uni-freiburg.de>
 * @matno   <5250773>
 * @exno    <Exercise 4>
 * @date    <25.06.2022>
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
 * FUNCTION IMPLEMENTATION
 *****************************************************************************/

/*Initialize PCF8591*/
unsigned char adac_init(void)
{
    i2c_init(PCF8591);
    return i2c_write(SINGLE_WRITE, config, WITH_STOP); // Write and return 1 if there exists a NACK
}

/*Read bytes from PCF8591*/
unsigned char adac_read(unsigned char lenght, unsigned char *values)
{
    /*Clear the first byte of the passed in array*/
    values[0] = '\0';

    /*Read Bytes*/
    i2c_read(lenght, values);

    /*Read operation is not done if there exists a NACK*/
    if (values[0] == '\0')
    {
        /*Error*/
        return 1;
    }
    else
    {
        /*No Error*/
        return 0;
    }
}
/*Write Bytes to PCF8591*/
unsigned char adac_write(unsigned char conf, unsigned char value)
{
    unsigned char conf_val[1] = { value };

    /*First configure and then write data*/
    unsigned char write_val[2] = { 0x40, value };
    switch (conf)
    {
    case 0:
        /*Write and return 1 if there exists a NACK*/
        return i2c_write(DOUBLE_WRITE, write_val, WITH_STOP);
        break;
    case 1:
        /*Write and return 1 if there exists a NACK*/
        return i2c_write(SINGLE_WRITE, conf_val, WITH_STOP);
        break;
    default:
        /*Programmers trap*/
        while (1)
        {

        }
        break;
    }
}

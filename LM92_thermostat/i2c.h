/***********************************************************
****  Title: C include file for I2C library (i2c.c)     ****
****  Author: David Schwehr 2015                        ****
****  File:                                             ****
****  Software:                                         ****
****  Target:                                           ****
************************************************************/
/**
@mainpage
@author
@file
@code
@brief
@see
@version
@copyright
 */

#ifndef I2C_H
#define I2C_h

/**
* @name includes
*/
#include <avr/io.h>
#include "pinDefines.h"

/**
* @name definitions
*/


/**
* @name functions
*/
/**
 @brief
 @param
 @return   none
 @see      
*/
void initI2C(void);
    /* Sets pullups and initializes bus speed to 100kHz (at FCPU=8MHz) */

/**
 @brief
 @param
 @return   none
 @see      
*/
void i2cWaitForComplete(void);
                       /* Waits until the hardware sets the TWINT flag */

/**
 @brief
 @param
 @return   none
 @see      
*/
void i2cStart(void);
                               /* Sends a start condition (sets TWSTA) */

/**
 @brief
 @param
 @return   none
 @see      
*/
void i2cStop(void);
                                /* Sends a stop condition (sets TWSTO) */


/**
 @brief
 @param
 @return   none
 @see      
*/
void i2cSend(uint8_t data);
                   /* Loads data, sends it out, waiting for completion */

/**
 @brief
 @param
 @return   none
 @see      
*/
uint8_t i2cReadAck(void);
              /* Read in from slave, sending ACK when done (sets TWEA) */

/**
 @brief
 @param
 @return   none
 @see      
*/
uint8_t i2cReadNoAck(void);
              /* Read in from slave, sending NOACK when done (no TWEA) */


#endif  // I2C_H
/*****************************************************************************
 *   ssp.h:  Header file for Philips LPC214x Family Microprocessors
 *
 *   Copyright(C) 2006, Philips Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2005.10.01  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#ifndef __SSP_H__
#define __SSP_H__

#include "utils.h"

/* SPI read and write buffer size */
#define BUFSIZE		256
#define FIFOSIZE	8

/* don't use CS, set CS as GPIO that you have total control
of the sequence */
#define USE_CS		0

/* SPI select pin */
#define SSP_PORT_DATA_SET_REG FIO0SET 
#define SSP_DSS_8_BIT 0x7
#define SSP_SCR 35 /* Note: Earlier this was set to 31 (bit rate = .9375 Mhz), somehow at this rate
                      we have observed data corruption over the SPI lines. However if we set the 
                      value to 35 (bit rate = .81 Mhz) the issue seems to disappear
                    */

#define SSP_FRF 0

/* Set DSS data to 8-bit, Frame format SPI, CPOL = 1, CPHA = 1, and SCR is 31 */
#define SSP_CONFIGURATION0 (SSP_SCR << SSPCR0_SCR_BIT) | _BV(SSPCR0_CPHA_BIT) | _BV(SSPCR0_CPOL_BIT) | SSP_DSS_8_BIT 
#define SSP_CONFIGURATION1 

U32 spi1_init(void);
void spi1_send(U8 *buf, U32 Length);
void spi1_receive(U8 *buf, U32 Length);




#endif  /* __SSP_H__ */




/********************************************************************/
/*                                                                  */
/* Filename:  spi.h                                                 */
/*                                                                  */
/* Description:                                                     */
/* This header file contains prototypes and constants used to       */
/* access the SPI port for access to the FLASH card.                */
/*                                                                  */
/* Global Functions:                                                */
/*     SPI_Init()             - Initialize the SPI Port             */
/*     SPI_Read()             - Read from the SPI Port              */
/*     SPI_Write()            - Write to the SPI Port               */
/*     SPI_WriteRead()        - Write NULL then read the SPI Port   */
/*                                                                  */
/* Local Functions:                                                 */
/*     SPI_ClearBuffer()      - Clear Out the RX buffer             */
/*                                                                  */
/*                                                                  */
/********************************************************************/
/*Edit Log                                                          */
/*    DATE                   COMMENTS                               */
/* 05-JUL-2005  Buckymeter LPC2138.                                 */
/* 04-AUG-2005  Debug SPI Chip Selection.                           */
/* 29-AUG-2005  SPI Communications Testing and Debug.               */
/* 06-SEP-2005  Modified for PCB Layout.                            */
/********************************************************************/
#ifndef  SPI_H
#define  SPI_H


//Required System Include Files
//-----------------Include Common Header Files--------------------------
#include "lpc_2148.h"          /* LPC213x definitions  */
#include "type.h"
#include "utils.h"

#define CHIP_SELECT()   SPI_ChipSelect(1)
#define CHIP_DESELECT() SPI_ChipSelect(0)

#define bit_is_clear(sfr, bit) (!(sfr & _BV(bit)))
#define SPI_TIMEOUT 0x00100000     //Maximum wait on SPI transactions

//SPI Control Register Bits
#define CPHA 3
#define CPOL 4
#define MSTR 5
#define LSBF 6
#define SPIE 7

//SPI Status Register Bits
#define ABRT 3
#define MODF 4
#define ROVR 5
#define WCOL 6
#define SPIF 7                //SPI Transfer Complete

#define SPI_RDY   10          //SPI Ready Pin
#define SPI_SS    7           //SPI Slave Select Pin
#define SPI_CS    7           //SPI DataFlash Chip Select
#define SPI_MOSI  6           //SPI Master Out Pin
#define SPI_MISO  5           //SPI Master In  Pin
#define SPI_CLK   4           //SPI Clock

#define SPI_PORT_DIRECTION_REG   FIO0DIR
#define SPI_PORT_DIRECTION_MASK  ( _BV(SPI_CLK)|_BV(SPI_MOSI)|_BV(SPI_CS) )   //Configure 1 for output MOSI|SCK|SS
#define SPI_PORT_DATA_REG        FIO0PIN
#define SPI_PORT_DATA_SET_REG    FIO0SET
#define SPI_PORT_DATA_CLR_REG    FIO0CLR
#define SPI_PORT_DATA_REG_INIT   ( _BV(SPI_CS) )       //Pull Up Slave Select
#define SPI_PORT_CHIP_SELECT     _BV(SPI_CS)
#define SPI_PORT_CHIP_READY      _BV(SPI_RDY)

//SPI Configuration No Interrupt | MSB First | Master | Mode:3 | Reserved | Reserved | Reserved
#define SPI_CONFIGURATION        ( 0 | 0 | _BV(MSTR) | _BV(CPOL) | _BV(CPHA) | 0 | 0 | 0 )

#define SPI_CLOCK_DIVISOR        64  // n > 8 and even number  (64 = 234k)

//Local Prototypes
unsigned char SPI_ClearBuffer(void);

//Global Prototypes
//efine PULSE_SPI_CLOCK()  {sbi(SPI_PORT_DATA_REG,SPI_CLK); cbi(SPI_PORT_DATA_REG,SPI_CLK);}//Note, Clock is active high for SPI mode 0
#define PULSE_SPI_CLOCK()  {cbi(SPI_PORT_DATA_REG,SPI_CLK); sbi(SPI_PORT_DATA_REG,SPI_CLK);}//Note, Clock is active low  for SPI mode 3

void SPI_Init(void);
void SPI_ChipSelect(U8 Select);
U8 SPI_Read     (U8 *rx);
U8 SPI_Write    (U16 bytes, U8 tx[]);
U8 SPI_WriteRead(U16 bytes, U8 tx[]);


#endif







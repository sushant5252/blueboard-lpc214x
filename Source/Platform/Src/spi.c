/********************************************************************/
/*                                                                  */
/* Filename:  SPI.c                                                 */
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
/* 04-JUL-2005  Buckymeter LPC2138.                                 */
/* 04-AUG-2005  Debug SPI Chip Selection.                           */
/* 27-AUG-2005  SPI Communications Testing and Debug.               */
/* 06-SEP-2005  Modified for PCB Layout.                            */
/********************************************************************/
#ifndef  SPI_C
#define  SPI_C


//-----------------Include Common Header Files--------------------------
#include "lpc_2148.h"                       /* LPC213x definitions  */

//-----------------Include BuckyMeter Header Files----------------------
#include "SPI.h"
#include "type.h"

/********************************************************************/
/*                                                                  */
/* Global Function:  void SPI_ChipSelect(Select)                    */
/*                                                                  */
/* This function Sets or Clears the FLASH Memory Chip Select.       */
/*                                                                  */
/* The Actual memory chip uses inverted logic.  This routine        */
/* implements the reversed logic.  The caller specifies 1 to        */
/* Chip Select, and 0 to DeSelect.                                  */
/*                                                                  */
/* Parameters                                                       */
/*     uchar Select            - Chip Select                        */
/*                               0 = Low (Chip DeSelect)            */
/*                               1 = Hi  (Chip Select)              */
/*                                                                  */
/********************************************************************/
/*Edit Log                                                          */
/*    DATE                   COMMENTS                               */
/* 01-JUL-2005  Buckymeter LPC2138.                                 */
/********************************************************************/
void SPI_ChipSelect(U8 Select)
{
   if (Select)  {
      SPI_PORT_DATA_CLR_REG = SPI_PORT_CHIP_SELECT;   //Clear bit

   }  else  {
      SPI_PORT_DATA_SET_REG = SPI_PORT_CHIP_SELECT;   //Set   bit

   }
}

/********************************************************************/
/*                                                                  */
/* Local  Function:  void  SPI_Init(void)                           */
/*                                                                  */
/* This function initializes the SPI hardware and Chip Select port. */
/*                                                                  */
/* Parameters                                                       */
/*     NONE                                                         */
/*                                                                  */
/********************************************************************/
/*Edit Log                                                          */
/*    DATE                   COMMENTS                               */
/* 04-JUL-2005  Buckymeter LPC2138.                                 */
/* 04-AUG-2005  Debug SPI Chip Selection.                           */
/********************************************************************/
void SPI_Init(void)
{
   U8 Status;
   U8 Data;

  PINSEL0 = (PINSEL0 & ~(3 << 12)) | (1 << 12);
  PINSEL0 = (PINSEL0 & ~(3 << 10)) | (1 << 10);
  PINSEL0 = (PINSEL0 & ~(3 << 8)) | (1 << 8);

	/* initialize SPI UART */
   SPI_PORT_DATA_SET_REG  = SPI_PORT_DATA_REG_INIT;   /* Initialize Chip Select High */

   S0SPCCR = SPI_CLOCK_DIVISOR;                       /* SPI Clock Divisor        */
   S0SPCR  = SPI_CONFIGURATION;                       /* SPI Config Register      */
   Status  = S0SPSR;                                  /* Clear Pending Status     */
   Data    = S0SPDR;                                  /* Clear Read Data Register */
}





/********************************************************************/
/*                                                                  */
/* Local  Function:  uchar SPI_ClearBuffer(void)                    */
/*                                                                  */
/* This function clears out the SPI read buffer in preparation for  */
/* incoming data.                                                   */
/*                                                                  */
/* Parameters                                                       */
/*     uchar SPI_ClearBuffer   - Completion Status                  */
/*                               0 = Request Successfully Completed */
/*                               1 = Request Failed (Timeout)       */
/*                                                                  */
/********************************************************************/
/*Edit Log                                                          */
/*    DATE                   COMMENTS                               */
/* 04-JUL-2005  Buckymeter LPC2138.                                 */
/********************************************************************/
U8 SPI_ClearBuffer(void)
{
   U8 rbuf;     //Place holder to clear Rx Buffer
   U8 timeout;  //Read Function Completion Status
   timeout = SPI_Read (&rbuf);
   return(timeout);
}

/********************************************************************/
/*                                                                  */
/* Global Function:  uchar SPI_Write(uchar bytes, uchar tx[])       */
/*                                                                  */
/* This function Sends command bytes to the SPI BUS.                */
/*                                                                  */
/* Parameters                                                       */
/*     uchar SPI_Write         - Completion Status                  */
/*                               0 = Request Successfully Completed */
/*                               1 = Request Failed (Timeout)       */
/*     uchar bytes             - Number of bytes to transmit        */
/*     uchar tx[]              - Array of Bytes to Send             */
/*                                                                  */
/********************************************************************/
/*Edit Log                                                          */
/*    DATE                   COMMENTS                               */
/* 04-JUL-2005  Buckymeter LPC2138.                                 */
/* 27-AUG-2005  SPI Communications Testing and Debug.               */
/********************************************************************/
U8 SPI_Write(U16 bytes, U8 tx[])
{
   U8 timeout;  //Completion Status
   U8 junk;     //Dummy Variable to clear Rx Buffer
   U16 i;        //Local Byte Counter
   U32 j;        //Timeout Failure Counter


   if (bytes)  {
      timeout = 0;
      for (i=0; ( (!timeout) && (i<bytes) ); i++)  {
         junk = S0SPSR;       //Clear Transfer Complete Flag
         junk = S0SPDR;       //Clear RX buffer
	      S0SPDR = tx[i];      //Transmit Character(s)

         for (j=0; ( (j<SPI_TIMEOUT) && bit_is_clear(S0SPSR, SPIF) ); j++) {};
         if ( bit_is_clear(S0SPSR, SPIF) )  {
   	      timeout = 1;	
         }
      }

   }  else  {

      timeout = 1;
   }
   return(timeout);
}





/********************************************************************/
/*                                                                  */
/* Global Function:  uchar SPI_Read(uchar *rx)                      */
/*                                                                  */
/* This function Receives one data bytes from the SPI BUS.          */
/*                                                                  */
/* Parameters                                                       */
/*     uchar SPI_Read          - Completion Status                  */
/*                               0 = Request Successfully Completed */
/*                               1 = Request Failed (Timeout)       */
/*     uchar *rx               - Reference for received byte        */
/*                                                                  */
/********************************************************************/
/*Edit Log                                                          */
/*    DATE                   COMMENTS                               */
/* 04-JUL-2005  Buckymeter LPC2138.                                 */
/* 27-AUG-2005  SPI Communications Testing and Debug.               */
/********************************************************************/
U8 SPI_Read (U8 *rx)
{
   U8 timeout;  //Function Completion Status
   U32 i;        //Local Timeout Counter


   timeout = 0;
   for(i=0;( (i<SPI_TIMEOUT) && bit_is_clear(S0SPSR, SPIF) ); i++) {};
   if ( bit_is_clear(S0SPSR, SPIF) )  {
      timeout = 1;
   }

   *rx = S0SPDR;           //Get Receive Data Byte


   return(timeout);
}





/********************************************************************/
/*                                                                  */
/* Global Function:  uchar SPI_WriteRead(ushort bytes, uchar rx[])  */
/*                                                                  */
/* This function transmits NULL bytes to receive data back from     */
/* the SPI BUS.                                                     */
/*                                                                  */
/* Parameters                                                       */
/*     uchar SPI_Read          - Completion Status                  */
/*                               0 = Request Successfully Completed */
/*                               1 = Request Failed (Timeout)       */
/*     ushort bytes            - Number of bytes to receive         */
/*     uchar  rx[]             - Array of Bytes to Send             */
/*                                                                  */
/********************************************************************/
/*Edit Log                                                          */
/*    DATE                   COMMENTS                               */
/* 04-JUL-2005  Buckymeter LPC2138.                                 */
/* 27-AUG-2005  SPI Communications Testing and Debug.               */
/********************************************************************/
U8 SPI_WriteRead (U16 bytes, U8 rx[])
{
   U8 NullOpCode[1];
   U8 timeout;  //Function Completion Status
   U16 b;        //Local Byte Counter
   U32 i;        //Local Timeout Counter


   if (bytes)  {
      timeout = 0;
      NullOpCode[0] = 0x00;

      for (b=0; ( (!timeout) && (b<bytes) ); b++)  {
         timeout = SPI_Write(1, NullOpCode);
         for(i=0;( (i<SPI_TIMEOUT) && bit_is_clear(S0SPSR, SPIF) ); i++) {};
         if ( bit_is_clear(S0SPSR, SPIF) )  {
   	      timeout = 1;
         }

         *rx++ = S0SPDR; 
      }

   }  else  {
      timeout = 1;

   }


   return(timeout);
}





#undef  SPI_C
#endif







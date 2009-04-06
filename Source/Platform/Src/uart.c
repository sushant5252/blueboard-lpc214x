/******************************************************************************
 *
 * $RCSfile: uart.c,v $
 * $Revision: 1.1 $
 *
 * This module provides interface routines to the LPC ARM UARTs.
 * Copyright 2004, R O SoftWare
 * No guarantees, warrantees, or promises, implied or otherwise.
 * May be used for hobby or commercial purposes provided copyright
 * notice remains intact.
 *
 *****************************************************************************/
#include <limits.h>
#include "types.h"
#include "lpc_2148.h"
#include "uart.h"
#include "uart_isr.h"
#include "lpc_uart.h"
#include "lpc_scb.h"
#include "lpc_vic.h"
#include "circular_buf.h"

#define U0_PINSEL       (0x00000005)    /* PINSEL0 Value for UART0 */
#define U0_PINMASK      (0x0000000F)    /* PINSEL0 Mask for UART0 */
#define U1_PINSEL       (0x00050000)    /* PINSEL0 Value for UART1 */
#define U1_PINMASK      (0x000F0000)    /* PINSEL0 Mask for UART1 */

#define ENABLE_MASK      0x00000C00
#define REB          10
#define DE           11

#if UART0_SUPPORT
#ifdef UART0_TX_INT_MODE
U8  uart0_tx_buffer[UART0_TX_BUFFER_SIZE];
U16 uart0_tx_insert_idx, uart0_tx_extract_idx;
int      uart0_tx_running;
#endif
#endif

#if UART1_SUPPORT
#ifdef UART1_TX_INT_MODE
U8  uart1_tx_buffer[UART1_TX_BUFFER_SIZE];
U16 uart1_tx_insert_idx, uart1_tx_extract_idx;
int uart1_tx_running;
#endif
#endif


#define FIFO_SIZE 512

CIR_BUFFER uart0_rx_buf;
CIR_BUFFER uart1_rx_buf;
U8 uart0_rx_fifo_ptr[FIFO_SIZE];
U8 uart1_rx_fifo_ptr[FIFO_SIZE];


#if UART0_SUPPORT

/******************************************************************************
 *
 * Function Name: uart0Init()
 *
 * Description:  
 *    This function initializes the UART for async mode
 *
 * Calling Sequence: 
 *    baudrate divisor - use UART_BAUD macro
 *    mode - see typical modes (uart.h)
 *    fmode - see typical fmodes (uart.h)
 *
 * Returns:
 *    void
 *
 * NOTE: uart0Init(UART_BAUD(9600), UART_8N1, UART_FIFO_8);
 *
 *****************************************************************************/
void uart0Init(U16 baud, U8 mode, U8 fmode)
{
  // set port pins for UART0
  PINSEL0 = (PINSEL0 & ~U0_PINMASK) | U0_PINSEL;
  FIO0DIR |= ENABLE_MASK;
//  FIO0SET |=(1<<DE);
//  FIO0CLR |=(1<<REB);
  

  U0IER = 0x00;                         // disable all interrupts
  U0IIR;                                // clear interrupt ID
  U0RBR;                                // clear receive register
  U0LSR;                                // clear line status register

  // set the baudrate
  U0LCR = ULCR_DLAB_ENABLE;             // select divisor latches 
  U0DLL = (U8)baud;                // set for baud low byte
  U0DLM = (U8)(baud >> 8);         // set for baud high byte

  // set the number of characters and other
  // user specified operating parameters
  U0LCR = (mode & ~ULCR_DLAB_ENABLE);
  U0FCR = fmode;

#if defined(UART0_TX_INT_MODE) || defined(UART0_RX_INT_MODE)
  // initialize the interrupt vector
  VICIntSelect &= ~ VIC_BIT(VIC_UART0);  // UART0 selected as IRQ
  VICIntEnable |= VIC_BIT(VIC_UART0);    // UART0 interrupt enabled
  VICVectCntl4 = VIC_ENABLE | VIC_UART0;
  VICVectAddr4 = (U32)uart0ISR;    // address of the ISR

#ifdef UART0_TX_INT_MODE
  // initialize the transmit data queue
  uart0_tx_extract_idx = uart0_tx_insert_idx = 0;
  uart0_tx_running = 0;
#endif

#ifdef UART0_RX_INT_MODE
  // initialize the receive data queue

  // enable receiver interrupts
  U0IER = UIER_ERBFI;
#endif
#endif
}

/******************************************************************************
 *
 * Function Name: uart0Putch()
 *
 * Description:  
 *    This function puts a character into the UART output queue for
 *    transmission.
 *
 * Calling Sequence: 
 *    character to be transmitted
 *
 * Returns:
 *    ch on success, -1 on error (queue full)
 *
 *****************************************************************************/
int uart0Putch(int ch)
{
#ifdef UART0_TX_INT_MODE
  U16 temp;

  temp = (uart0_tx_insert_idx + 1) % UART0_TX_BUFFER_SIZE;

  if (temp == uart0_tx_extract_idx)
    return -1;                          // no room

  U0IER &= ~UIER_ETBEI;                 // disable TX interrupts

  // check if in process of sending data
  if (uart0_tx_running)
    {
    // add to queue
    uart0_tx_buffer[uart0_tx_insert_idx] = (U8)ch;
    uart0_tx_insert_idx = temp;
    }
  else
    {
    // set running flag and write to output register
    uart0_tx_running = 1;
    U0THR = (U8)ch;
    }

  U0IER |= UIER_ETBEI;                  // enable TX interrupts
#else
  while (!(U0LSR & ULSR_THRE))          // wait for TX buffer to empty
    continue;                           // also either WDOG() or swap()

  U0THR = (U8)ch;
#endif
  return (U8)ch;
}

/******************************************************************************
 *
 * Function Name: uart0Space()
 *
 * Description:  
 *    This function gets the available space in the transmit queue
 *
 * Calling Sequence: 
 *    void
 *
 * Returns:
 *    available space in the transmit queue
 *
 *****************************************************************************/
U16 uart0Space(void)
{
#ifdef UART0_TX_INT_MODE
  int space;

  if ((space = (uart0_tx_extract_idx - uart0_tx_insert_idx)) <= 0)
    space += UART0_TX_BUFFER_SIZE;

  return (U16)(space - 1);
#else
  return USHRT_MAX;
#endif
}

/******************************************************************************
 *
 * Function Name: uart0Puts()
 *
 * Description:  
 *    This function writes a NULL terminated 'string' to the UART output
 *    queue, returning a pointer to the next character to be written.
 *
 * Calling Sequence: 
 *    address of the string
 *
 * Returns:
 *    a pointer to the next character to be written
 *    (\0 if full string is written)
 *
 *****************************************************************************/
const char *uart0Puts(const char *string)
{
  register char ch;

  while ((ch = *string) && (uart0Putch(ch) >= 0))
    string++;

  return string;
}

/******************************************************************************
 *
 * Function Name: uart0Write()
 *
 * Description:  
 *    This function writes 'count' characters from 'buffer' to the UART
 *    output queue.
 *
 * Calling Sequence: 
 *    
 *
 * Returns:
 *    0 on success, -1 if insufficient room, -2 on error
 *    NOTE: if insufficient room, no characters are written.
 *
 *****************************************************************************/
int uart0Write(const char *buffer, U16 count)
{
#ifdef UART0_TX_INT_MODE
  if (count > uart0Space())
    return -1;
#endif
  while (count && (uart0Putch(*buffer++) >= 0)) 
    count--;

  return (count ? -2 : 0);
}

/******************************************************************************
 *
 * Function Name: uart0TxEmpty()
 *
 * Description:
 *    This function returns the status of the UART transmit data
 *    registers.
 *
 * Calling Sequence: 
 *    void
 *
 * Returns:
 *    FALSE - either the tx holding or shift register is not empty
 *   !FALSE - if both the tx holding & shift registers are empty
 *
 *****************************************************************************/
int uart0TxEmpty(void)
{
  return (U0LSR & (ULSR_THRE | ULSR_TEMT)) == (ULSR_THRE | ULSR_TEMT);
}

/******************************************************************************
 *
 * Function Name: uart0TxFlush()
 *
 * Description:  
 *    This function removes all characters from the UART transmit queue
 *    (without transmitting them).
 *
 * Calling Sequence: 
 *    void
 *
 * Returns:
 *    void
 *
 *****************************************************************************/
void uart0TxFlush(void)
{
#ifdef UART0_TX_INT_MODE
//  unsigned cpsr;

  U0FCR |= UFCR_TX_FIFO_RESET;          // clear the TX fifo

  // "Empty" the transmit buffer.
//  cpsr = disableIRQ();                  // disable global interrupts
  U0IER &= ~UIER_ETBEI;                 // disable TX interrupts
//  restoreIRQ(cpsr);                     // restore global interrupts
  uart0_tx_insert_idx = uart0_tx_extract_idx = 0;
#else
  U0FCR |= UFCR_TX_FIFO_RESET;          // clear the TX fifo
#endif
}

/******************************************************************************
 *
 * Function Name: uart0Getch()
 *
 * Description:  
 *    This function gets a character from the UART receive queue
 *
 * Calling Sequence: 
 *    void
 *
 * Returns:
 *    character on success, -1 if no character is available
 *
 *****************************************************************************/
#include "circular_buf.h"


S8 uart0Getch(U8 *ch)
{

#ifdef UART0_RX_INT_MODE
#if 0
if(!popFromCirBuf(&uart0_rx_buf,ch,1))
return -1;
else
return 0;
#endif
#else 

  if (U0LSR & ULSR_RDR)                 // check if character is available
    return U0RBR;                       // return character

  return -1;
#endif
return 0; 
}

#endif

#if UART1_SUPPORT

/******************************************************************************
 *
 * Function Name: uart1Init()
 *
 * Description:  
 *    This function initializes the UART for async mode
 *
 * Calling Sequence: 
 *    baudrate divisor - use UART_BAUD macro
 *    mode - see typical modes (uart.h)
 *    fmode - see typical fmodes (uart.h)
 *
 * Returns:
 *    void
 *
 * NOTE: uart1Init(UART_BAUD(9600), UART_8N1, UART_FIFO_8);
 *
 *****************************************************************************/
void uart1Init(U16 baud, U8 mode, U8 fmode)
{
  // set port pins for UART1
  PINSEL0 = (PINSEL0 & ~U1_PINMASK) | U1_PINSEL;
  FIO0DIR |= ENABLE_MASK;
//  FIO0SET |=(1<<DE);
//  FIO0CLR |=(1<<REB);
  
  U1IER = 0x00;                         // disable all interrupts
  U1IIR;                                // clear interrupt ID
  U1RBR;                                // clear receive register
  U1LSR;                                // clear line status register

  // set the baudrate
  U1LCR = ULCR_DLAB_ENABLE;             // select divisor latches 
  U1DLL = (U8)baud;                // set for baud low byte
  U1DLM = (U8)(baud >> 8);         // set for baud high byte

  // set the number of characters and other
  // user specified operating parameters
  U1LCR = (mode & ~ULCR_DLAB_ENABLE);
  U1FCR = fmode;

#if defined(UART1_TX_INT_MODE) || defined(UART1_RX_INT_MODE)
  // initialize the interrupt vector
  VICIntSelect &= ~VIC_BIT(VIC_UART1);  // UART1 selected as IRQ
  VICIntEnable |= VIC_BIT(VIC_UART1);    // UART1 interrupt enabled
  VICVectCntl3 = VIC_ENABLE | VIC_UART1;
  VICVectAddr3 = (U32)uart1ISR;    // address of the ISR

#ifdef UART1_TX_INT_MODE
  uart1_tx_extract_idx = uart1_tx_insert_idx = 0;
  uart1_tx_running = 0;
#endif

#ifdef UART1_RX_INT_MODE
  // initialize data queues

  // enable receiver interrupts
  U1IER |= UIER_ERBFI;
#endif
#endif
}

/******************************************************************************
 *
 * Function Name: uart1Putch()
 *
 * Description:  
 *    This function puts a character into the UART output queue for
 *    transmission.
 *
 * Calling Sequence: 
 *    ch - character to be transmitted
 *
 * Returns:
 *    ch on success, -1 on error (queue full)
 *
 *****************************************************************************/
int uart1Putch(int ch)
{

#ifdef UART1_TX_INT_MODE
  U16 temp;

  temp = (uart1_tx_insert_idx + 1) % UART1_TX_BUFFER_SIZE;

  if (temp == uart1_tx_extract_idx)
    return -1;                          // no room

  U1IER &= ~UIER_ETBEI;                 // disable TX interrupts

  // check if in process of sending data
  if (uart1_tx_running)
    {
    // add to queue
    uart1_tx_buffer[uart1_tx_insert_idx] = (U8)ch;
    uart1_tx_insert_idx = temp;
    }
  else
    {
    // set running flag and write to output register
    uart1_tx_running = 1;
    U1THR = (U8)ch;
    }

  U1IER |= UIER_ETBEI;                  // enable TX interrupts
#else
  while (!(U1LSR & ULSR_THRE))          // wait for TX buffer to empty
    continue;                           // also either WDOG() or swap()

  U1THR = (U8)ch;
#endif
  return (U8)ch;
}

/******************************************************************************
 *
 * Function Name: uart1Space()
 *
 * Description:  
 *    This function gets the available space in the transmit queue
 *
 * Calling Sequence: 
 *    void
 *
 * Returns:
 *    available space in the transmit queue
 *
 *****************************************************************************/
U16 uart1Space(void)
{
#ifdef UART1_TX_INT_MODE
  int space;

  if ((space = (uart1_tx_extract_idx - uart1_tx_insert_idx)) <= 0)
    space += UART1_TX_BUFFER_SIZE;

  return (U16)(space - 1);
#else
  return USHRT_MAX;
#endif
}

/******************************************************************************
 *
 * Function Name: uart1Puts()
 *
 * Description:  
 *    This function writes a NULL terminated 'string' to the UART output
 *    queue, returning a pointer to the next character to be written.
 *
 * Calling Sequence: 
 *    address of the string
 *
 * Returns:
 *    a pointer to the next character to be written
 *    (\0 if full string is written)
 *
 *****************************************************************************/
const char *uart1Puts(const char *string)
{
  register char ch;

  while ((ch = *string) && (uart1Putch(ch) >= 0))
    string++;

  return string;
}

/******************************************************************************
 *
 * Function Name: uart1Write()
 *
 * Description:  
 *    This function writes 'count' characters from 'buffer' to the UART
 *    output queue.
 *
 * Calling Sequence: 
 *    
 *
 * Returns:
 *    0 on success, -1 if insufficient room, -2 on error
 *    NOTE: if insufficient room, no characters are written.
 *
 *****************************************************************************/
int uart1Write(const char *buffer, U16 count)
{
#ifdef UART1_TX_INT_MODE
  if (count > uart1Space())
    return -1;
#endif
  while (count && (uart1Putch(*buffer++) >= 0))
    count--;

  return (count ? -2 : 0);
}

/******************************************************************************
 *
 * Function Name: uart1TxEmpty()
 *
 * Description:
 *    This function returns the status of the UART transmit data
 *    registers.
 *
 * Calling Sequence: 
 *    void
 *
 * Returns:
 *    FALSE - either the tx holding or shift register is not empty
 *   !FALSE - if both the tx holding & shift registers are empty
 *
 *****************************************************************************/
int uart1TxEmpty(void)
{
  return (U1LSR & (ULSR_THRE | ULSR_TEMT)) == (ULSR_THRE | ULSR_TEMT);
}

/******************************************************************************
 *
 * Function Name: uart1TxFlush()
 *
 * Description:  
 *    This function removes all characters from the UART transmit queue
 *    (without transmitting them).
 *
 * Calling Sequence: 
 *    void
 *
 * Returns:
 *    void
 *
 *****************************************************************************/
void uart1TxFlush(void)
{
#ifdef UART1_TX_INT_MODE
//  unsigned cpsr;

  U1FCR |= UFCR_TX_FIFO_RESET;          // clear the TX fifo

  // "Empty" the transmit buffer.
//  cpsr = disableIRQ();                  // disable global interrupts
  U1IER &= ~UIER_ETBEI;                 // disable TX interrupts
//  restoreIRQ(cpsr);                     // restore global interrupts
  uart1_tx_insert_idx = uart1_tx_extract_idx = 0;
#else
  U1FCR |= UFCR_TX_FIFO_RESET;          // clear the TX fifo
#endif
}

/******************************************************************************
 *
 * Function Name: uart1Getch()
 *
 * Description:  
 *    This function gets a character from the UART receive queue
 *
 * Calling Sequence: 
 *    void
 *
 * Returns:
 *    character on success, -1 if no character is available
 *
 *****************************************************************************/
S8 uart1Getch(U8 *ch)
{

#ifdef UART1_RX_INT_MODE
#if 0
if(!popFromCirBuf(&uart1_rx_buf,ch,1))
return -1;
else
return 0;
#endif
#else
  if (U1LSR & ULSR_RDR)                 // check if character is available
    return U1RBR;                       // return character

  return -1;
#endif
return 0;
}

#endif

/**
*******************************************************************************
  Function Name : uart0Receive()

  Description :	 

  Input :	

  Output :	none

  Note : 
*******************************************************************************
*/
S8 uart0Receive(U8 *buf, U16 size)
{
  S32 retval=0;
  
  retval = popFromCirBuf(&uart0_rx_buf, buf, size);

  if(retval!=size)
    return -1;
  else
    return 0;
}

/**
*******************************************************************************
  Function Name : uart1Receive()

  Description :	 

  Input :	

  Output :	none

  Note : 
*******************************************************************************
*/
S8 uart1Receive(U8 *buf, U16 size)
{
  S32 retval=0;
  
  retval = popFromCirBuf(&uart1_rx_buf, buf, size);

  if(retval!=size)
    return -1;
  else
    return 0;
}

/**
*******************************************************************************
  Function Name : init_uart0()

  Description :	 

  Input :	

  Output :	none

  Note : 
*******************************************************************************
*/
void init_uart0(void)
{
  uart0Init(UART_BAUD(HOST_BAUD), UART_8N1, UART_FIFO_8);
  CirBufInit(&uart0_rx_buf, FIFO_SIZE, uart0_rx_fifo_ptr);
}

/**
*******************************************************************************
  Function Name : init_uart1()

  Description :	 

  Input :	

  Output :	none

  Note : 
*******************************************************************************
*/
void init_uart1(void)
{
	uart1Init(UART_BAUD(HOST_BAUD), UART_8N1, UART_FIFO_8);
  CirBufInit(&uart1_rx_buf, FIFO_SIZE, uart1_rx_fifo_ptr);	
}




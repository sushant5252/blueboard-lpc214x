/******************************************************************************
 *
 * $RCSfile: uartISR.c,v $
 * $Revision: 1.1 $
 *
 * This module implements the ISRs for the UARTs on the LPC ARMs.
 * Copyright 2004, R O SoftWare
 * No guarantees, warrantees, or promises, implied or otherwise.
 * May be used for hobby or commercial purposes provided copyright
 * notice remains intact.
 *
 *****************************************************************************/
#include "types.h"
#include "lpc_2148.h"
#include "uart.h"
#include "uart_isr.h"
#include "lpc_uart.h"
#include "circular_buf.h"

extern CIR_BUFFER uart0_rx_buf;
extern CIR_BUFFER uart1_rx_buf;

#if UART0_SUPPORT
#ifdef UART0_TX_INT_MODE
extern U8  uart0_tx_buffer[UART0_TX_BUFFER_SIZE];
extern U16 uart0_tx_insert_idx, uart0_tx_extract_idx;
extern int      uart0_tx_running;
#endif // UART0_TX_INT_MODE

#if defined(UART0_TX_INT_MODE) || defined(UART0_RX_INT_MODE)
/******************************************************************************
 *
 * Function Name: uart0ISR()
 *
 * Description:
 *    This function implements the ISR for UART0.
 *
 * Calling Sequence: 
 *    void
 *
 * Returns:
 *    void
 *
 *****************************************************************************/
void uart0ISR(void)
{
  U8 iid,ch;

    // loop until not more interrupt sources
  while (((iid = U0IIR) & UIIR_NO_INT) == 0)
    {
    // identify & process the highest priority interrupt
    switch (iid & UIIR_ID_MASK)
      {
      case UIIR_RLS_INT:                // Receive Line Status
        U0LSR;                          // read LSR to clear
        break;

#ifdef UART0_RX_INT_MODE
      case UIIR_CTI_INT:                // Character Timeout Indicator
      case UIIR_RDA_INT:                // Receive Data Available
        do
          {
            ch = U0RBR;
            pushToCirBuf(&uart0_rx_buf, &ch, 1);            
          }
        while (U0LSR & ULSR_RDR);

        break;
#endif

#ifdef UART0_TX_INT_MODE
      case UIIR_THRE_INT:               // Transmit Holding Register Empty
        while (U0LSR & ULSR_THRE)
          {
          // check if more data to send
          if (uart0_tx_insert_idx != uart0_tx_extract_idx)
            {
            U0THR = uart0_tx_buffer[uart0_tx_extract_idx++];
            uart0_tx_extract_idx %= UART0_TX_BUFFER_SIZE;
            }
          else
            {
            // no
            uart0_tx_running = 0;       // clear running flag
            break;
            }
          }

        break;
#endif // UART0_TX_INT_MODE

      default:                          // Unknown
        U0LSR;
        U0RBR;
        break;
      }
    }

  VICVectAddr = 0x00000000;             // clear this interrupt from the VIC
 			                        
}
#endif // defined(UART0_TX_INT_MODE) || defined(UART0_RX_INT_MODE)
#endif // UART0_SUPPORT


#if UART1_SUPPORT


#ifdef UART1_TX_INT_MODE
extern U8  uart1_tx_buffer[UART1_TX_BUFFER_SIZE];
extern U16 uart1_tx_insert_idx, uart1_tx_extract_idx;
extern int      uart1_tx_running;
#endif // UART1_TX_INT_MODE

#if defined(UART1_TX_INT_MODE) || defined(UART1_RX_INT_MODE)
/******************************************************************************
 *
 * Function Name: uart1ISR()
 *
 * Description:
 *    This function implements the ISR for UART1.
 *
 * Calling Sequence: 
 *    void
 *
 * Returns:
 *    void
 *
 *****************************************************************************/
void uart1ISR(void)
{
  U8 iid,ch;
  
  // perform proper ISR entry so thumb-interwork works properly
  //ISR_ENTRY();

  // loop until not more interrupt sources
  while (((iid = U1IIR) & UIIR_NO_INT) == 0)
    {
    // identify & process the highest priority interrupt
    switch (iid & UIIR_ID_MASK)
      {
      case UIIR_RLS_INT:                // Receive Line Status
        U1LSR;                          // read LSR to clear
        break;

#ifdef UART1_RX_INT_MODE
      case UIIR_CTI_INT:                // Character Timeout Indicator
      case UIIR_RDA_INT:                // Receive Data Available
        do
          {
            ch = U1RBR;
            pushToCirBuf(&uart1_rx_buf, &ch, 1);
          }          
        while (U1LSR & ULSR_RDR);

        break;
#endif

#ifdef UART1_TX_INT_MODE
      case UIIR_THRE_INT:               // Transmit Holding Register Empty
        while (U1LSR & ULSR_THRE)
          {
          // check if more data to send
          if (uart1_tx_insert_idx != uart1_tx_extract_idx)
            {
            U1THR = uart1_tx_buffer[uart1_tx_extract_idx++];
            uart1_tx_extract_idx %= UART1_TX_BUFFER_SIZE;
            }
          else
            {
            // no
            uart1_tx_running = 0;       // clear running flag
            break;
            }
          }

        break;
#endif // UART1_TX_INT_MODE

      case UIIR_MS_INT:                 // MODEM Status
        U1MSR;                          // read MSR to clear
        break;

      default:                          // Unknown
        U1LSR;
        U1RBR;
        U1MSR;
        break;
      }
    }

  VICVectAddr = 0x00000000;             // clear this interrupt from the VIC
  //ISR_EXIT();                           // recover registers and return
}
#endif // defined(UART1_TX_INT_MODE) || defined(UART1_RX_INT_MODE)
#endif // UART1_SUPPORT




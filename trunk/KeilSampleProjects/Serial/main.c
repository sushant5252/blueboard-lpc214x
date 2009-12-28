#include <stdio.h>                /* prototype declarations for I/O functions */
#include <LPC214x.H>              /* LPC21xx definitions                      */
#include "Serial.h"

#define UART0_TEXT "\n\r Testing UART0 NGX's BlueBoard \n\r BlueBoard Revision : 1 \n\r Firmware Version: 1 \n\r For more information on BlueBoard visit www.ngxtechnologies.com"
#define UART1_TEXT "\n\r Testing UART1 NGX's BlueBoard \n\r BlueBoard Revision : 1 \n\r Firmware Version: 1 \n\r For more information on BlueBoard visit www.ngxtechnologies.com"

/**
**************************************************************************

****1111
  Function Name : delay()

  Description :This function suspends the tasks for specified ticks.	

  Input :  ticks:no of ticks in multiple of 1 usec
            task: task to be suspended

  Output : void

  Note :
*******************************************************************************
*/

void delay(int count)
{
  int j=0,i=0;

  for(j=0;j<count;j++)
  {
    /* At 60Mhz, the below loop introduces
    delay of 10 us */
    for(i=0;i<35;i++);
  }
}

/****************/
/* main program */
/****************/
int main (void)  {                /* execution starts here                    */

  uart0_init();					  // Initialize UART0

  while (1) {                          /* An embedded program does not stop */
  uart0_getkey();
  uart0_puts (UART0_TEXT);	  // Transffer data to PC through Serial
  delay(100000);
  }                               
}

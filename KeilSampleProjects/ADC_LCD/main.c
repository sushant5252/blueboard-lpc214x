#include <stdio.h>
#include <LPC214x.H>                       /* LPC214x definitions */
#include "lcd.h"
#include "adc.h"

/**
**************************************************************************

****1111
  Function Name : wait()

  Description :This function suspends the tasks for specified ticks.	

  Input :  ticks:no of ticks in multiple of 1 usec
            task: task to be suspended

  Output : void

  Note :
*******************************************************************************
*/

void wait(int count)
{
  int j=0,i=0;

  for(j=0;j<count;j++)
  {
    /* At 60Mhz, the below loop introduces
    delay of 10 us */
    for(i=0;i<35;i++);
  }
}

/**
********************************************************************************************
	Function Name :	process_adc()

	Description :
	
	Input :	Void

	Output : Void

	Note :
**********************************************************************************************
*/
void process_adc(void)
{

}

/**
********************************************************************************************
	Function Name :	main()

	Description :
	
	Input :	Void

	Output : 

	Note :
**********************************************************************************************
*/
int main (void) 
{
  init_adc0();						// Initialize ADC
  init_lcd();						// Initialize LCD

  wait(100000);
  lcd_clear();						// clear display
  while(1)
  {
    process_adc();					// Raed ADC value and display it on first line of LCD
    wait(30000);
  }
}

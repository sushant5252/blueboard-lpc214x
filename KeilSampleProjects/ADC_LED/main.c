#include <stdio.h>
#include "lpc_2148.h"                       /* LPC214x definitions */
#include "adc.h"
#include "type.h"
#include "74HC595.h"
#include "spi.h"

#define GET_SCALED_LED_MASK_BUF_INDEX(val) (val/114)


const U8 led_mask_buf[] = {0,1,3,7,0xF,0x1F,0x3F,0x7F,0xFF};

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
  U16 adc_value = 0;
  U8 index = 0;

  adc_value = adc_read(ADC0, CHANNEL_3);
  index = GET_SCALED_LED_MASK_BUF_INDEX(adc_value);
  if(index > sizeof(led_mask_buf))
    index = sizeof(led_mask_buf) -1;
  turn_on_led(led_mask_buf[index]);
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
  SCS = 3;    //sets port 1 and 0 to fast mode (not compatable with previous LPCs)
  LATCH_DIR();

  init_adc0();						// Initialize ADC
  SPI_Init();
  test_leds();

  while(1)
  {
    process_adc();					// Raed ADC value and display it on first line of LCD
    wait(30000);
  }
}

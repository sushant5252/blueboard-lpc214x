#include "74HC595.h"
#include "type.h"
#include "led.h"
#include "utils.h"

#define MAX_NUM_OF_LED_TOGGLE 3

#define MAX_INTENSITY_VALUE 1023
#define MAX_ON_PERIOD 20
#define GET_SCALED_VALUE(i) (i/50)

extern const U8 led_mask_buf[];

/**
*******************************************************************************
  Function Name : leds_on()

  Description :	 

  Input :	none
  
  Output :	none

  Note : 
*******************************************************************************
*/
void leds_on(void)
{
  parallel_data_output_high_74hc595();
}

/**
*******************************************************************************
  Function Name : leds_off()

  Description :	 

  Input :	none
  
  Output :	none

  Note : 
*******************************************************************************
*/
void turn_off_all_leds(void)
{
  parallel_data_output_low_74hc595();
}
/**
********************************************************************************************
	Function Name :	test_leds()

	Description :
	
	Input :	Void

	Output : Void

	Note :
**********************************************************************************************
*/
void test_leds(void)
{
  U8 led_num = 0;
  U8 index = 0;

  for(index = 0; index < MAX_NUM_OF_LED_TOGGLE; index++)
  {
    for(led_num = 0; led_num < MAX_NUMBER_OF_LEDS; led_num++)
    {
      turn_on_led(led_mask_buf[led_num]);
      wait(30000);
    }
    for(led_num = MAX_NUMBER_OF_LEDS; led_num > 0; led_num--)
    {
      turn_on_led(led_mask_buf[led_num]);
      wait(30000);
    }
		
  }
}



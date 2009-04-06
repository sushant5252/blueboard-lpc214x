#include "softtimer.h"
#include "74HC595.h"
#include "type.h"
#include "led.h"
#include "utils.h"

#define MAX_NUM_OF_LED_TOGGLE 3

#define MAX_INTENSITY_VALUE 1023
#define MAX_ON_PERIOD 20
#define GET_SCALED_VALUE(i) (i/50)

extern const U8 led_mask_buf[];

struct uclk led_timer;
static U16 led_intensity = 0;

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
*******************************************************************************
  Function Name : set_led_intensity()

  Description :	 

  Input :	none
  
  Output :	none

  Note : 
*******************************************************************************
*/
void set_led_intensity(U16 intensity)
{
  led_intensity = intensity;
}

/**
*******************************************************************************
  Function Name : get_led_intensity()

  Description :	 

  Input :	none
  
  Output :	none

  Note : 
*******************************************************************************
*/
U16 get_led_intensity(void)
{
  return led_intensity;
}

/**
*******************************************************************************
  Function Name : led_timer_function()

  Description :	 Call back function for refreshing LED's 

  Input :	none
  
  Output :	none

  Note : 
*******************************************************************************
*/
void led_timer_function()
{
  static int count=0;
  U16 intensity=0;
  U8 scaled_value=0;

  count++;
  intensity = get_led_intensity();

  if(intensity > MAX_INTENSITY_VALUE)
    {
      intensity = MAX_INTENSITY_VALUE;
    }    
    scaled_value = GET_SCALED_VALUE(intensity);    
    if(count < scaled_value)
    {
      leds_on();
    }
    else
    {
      turn_off_all_leds();
    }
  if(count >= MAX_ON_PERIOD)
  {
    count=0;
  }
}

/**
*******************************************************************************
  Function Name : LED_refresh_timer_init()

  Description :	 

  Input :	none
  
  Output :	none

  Note : 
*******************************************************************************
*/
void LED_refresh_timer_init()
{
  def_uclk(&led_timer);
  led_timer.function_callback = led_timer_function;
  led_timer.repeat = 1;
  led_timer.ticks =  1;
  reg_uclk(&led_timer);
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
      delay(5000);
    }
    for(led_num = MAX_NUMBER_OF_LEDS; led_num > 0; led_num--)
    {
      turn_on_led(led_mask_buf[led_num]);
      delay(5000);
    }
		
  }
}



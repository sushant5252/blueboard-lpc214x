#ifndef _74HC595_
#define _74HC595_

#include "utils.h"

#define STCP BIT7

#define LATCH_DIR() (FIO0DIR = STCP)

void test_leds(void);
void parallel_data_output_high_74hc595();
void parallel_data_output_low_74hc595();
void turn_on_led(U8 led_mask); 

#endif // _74HC595_


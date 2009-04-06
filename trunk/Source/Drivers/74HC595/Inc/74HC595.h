#ifndef _74HC595_
#define _74HC595_

#define STCP BIT20
#define OE 	BIT7

#define LATCH_DIR() (FIO1DIR = STCP)
#define OE_DIR() (FIO0DIR = OE)


void test_leds(void);
void parallel_data_output_high_74hc595();
void parallel_data_output_low_74hc595();
void turn_on_led(U8 led_mask); 

#endif // _74HC595_


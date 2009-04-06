#ifndef _LED_H
#define _LED_H

#define MAX_NUMBER_OF_LEDS 8

void set_led_intensity(U16 intensity);
void LED_refresh_timer_init();
void turn_off_all_leds(void);

#endif // _LED_H

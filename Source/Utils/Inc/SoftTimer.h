#ifndef _SOFTWARE_TIMER_H_
#define _SOFTWARE_TIMER_H_

#include "type.h"

#define TRUE 1
#define FALSE 0
#define is_timer_expired(x) (x.expired)

struct uclk {
 void (*function_callback)(void);
 int current_ticks;
 int ticks;
 int repeat;
 int expired;
 struct uclk *next;
 struct uclk *prev;
};


struct uclk *head, *tail;
struct uclk clk_head, clk_tail;

void dereg_uclk(struct uclk *luclk);
void reg_uclk(struct uclk *luclk);
void def_uclk(struct uclk *luclk);
U32 check_timeout(struct uclk *temp_head);
void uclk_handler(void);
void init_software_timer (void);
void task_SoftwareTimer(void *par);
int is_uclk_expired(struct uclk *luclk);

#endif

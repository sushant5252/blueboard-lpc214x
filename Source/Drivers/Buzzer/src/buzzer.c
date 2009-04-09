#include "pwm.h"
#include "lpc_2148.h"
#include "utils.h"
#include "softtimer.h"

#define BUZZER BIT25
#define BUZZER_DIR FIO1DIR
#define BUZZER_SET FIO1SET
#define BUZZER_CLR FIO1CLR
#define BUZZER_TIMEOUT 10

struct uclk buzzer_timer;

static void start_buzzer_timer(void);
static void stop_buzzer_timer(void);

/**
********************************************************************************************
  Function Name :	buzzer_on()
  
  Description   :	
  
  Input         :	
  
  Output        :	Void
  
  Note          :
********************************************************************************************
*/
void buzzer_on()
{    
  stop_buzzer_timer();// May be use dereg_uclk directly.
  BUZZER_DIR |= BUZZER;
  BUZZER_CLR |= BUZZER;	
  start_buzzer_timer();
}

/**
********************************************************************************************
 Function Name : turn_off_lcd_back_light()

 Description   :	

 Input         :	

 Output        : Void

 Note          :
********************************************************************************************
*/
void turn_off_buzzer(void)
{
   BUZZER_DIR |= BUZZER;
   BUZZER_SET |= BUZZER;
}

/**
********************************************************************************************
 Function Name : start_buzzer_timer()

 Description   :	

 Input         :	

 Output        : Void

 Note          :
********************************************************************************************
*/
static void start_buzzer_timer(void)
{
  def_uclk(&buzzer_timer);
  buzzer_timer.repeat = 0;
  buzzer_timer.expired= 0;
  buzzer_timer.ticks = BUZZER_TIMEOUT;
  buzzer_timer.function_callback = turn_off_buzzer;
  reg_uclk(&buzzer_timer);
}

/**
********************************************************************************************
 Function Name : stop_buzzer_timer()

 Description   :	

 Input         :	

 Output        : Void

 Note          :
********************************************************************************************
*/
static void stop_buzzer_timer(void)
{
  dereg_uclk(&buzzer_timer);
}


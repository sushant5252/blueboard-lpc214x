#include "lpc_vic.h"
#include "timer.h"
#include "tn.h"
#include "SoftTimer.h"


void (*timer1_cb)(void);

/**
*******************************************************************************
  Function Name :	init_timer0()

  Description :This function initialises the timer0 as system tick
              The minimum tick for which the timer0 is configured is 10msec

  Input :	
		            
  Output :	void

  Note :
*******************************************************************************
*/	
void init_timer0(void)
{

//--- Timer 0  - interrupt 10 ms
  T0PR = 0;  //-- Prscaler = 0
  T0PC = 0;
  //T0MR0 = 60000 * 10;  //clock rate of 60000 * 10ms ?
  T0MR0 = 60000 ; // 1ms
  T0MCR = 3; //-- bit 0=1 -int on MR0 , bit 1=1 - Reset on MR0
  T0TCR = 1; //-- Timer 0 - run
  VICIntSelect &= ~ VIC_BIT(VIC_TIMER0);
  VICVectAddr0 = (unsigned int)tn_timer0_int_func;   //-- Timer 0 int - priority top-1
  VICVectCntl0 = VIC_ENABLE | VIC_TIMER0;
  VICIntEnable |= VIC_BIT(VIC_TIMER0);
}


/**
*******************************************************************************
  Function Name : init_timer1()

  Description :This function initialises the timer1  for delays that are less
              then  delay configured for TNKernel.The minimum tick for which 
              the timer1 can be configured is '1usec'

  Input :	
  
  Output :	void

  Note :
*******************************************************************************
*/	
void init_timer1(void)
{
   //T1PR = 60; /* 1us */
  T1PR = 0; /* 1us */
  T1PC = 0;
 // T1MCR = TMCR_MR0_I | TMCR_MR0_R | TMCR_MR0_S;  //-- bit 0=1 -int on MR0 , bit 1=1 - Reset on MR0
  T1MR0 = 600000; // 10ms
  T1MCR=3;
  T1TCR=1;
  
  VICIntSelect &= ~ VIC_BIT(VIC_TIMER1);
  VICVectAddr1 = (unsigned int)timer1_int_func;   //-- Timer 0 int - priority top-1
  VICVectCntl1 = VIC_ENABLE | VIC_TIMER1;
  VICIntEnable |= VIC_BIT(VIC_TIMER1);

 // VICVectAddr = 0xFF;
}




/**
*******************************************************************************
  Function Name :	timer1_int_func()

  Description : This is the timer1 interrupt handler. It will invoke the 
                callback function if registered.

  Input : None           
  Output : None

  Note :
*******************************************************************************
*/
void timer1_int_func(void)
{
  T1IR = 0xFF;

  uclk_handler ( );

   VICVectAddr = 0xFF;
}




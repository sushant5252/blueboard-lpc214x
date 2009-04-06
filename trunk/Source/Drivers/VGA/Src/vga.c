#include "lpc_2148.h"
#include "type.h"
#include "pwm.h"
#include "utils.h"
#include "lpc_vic.h"
#include "softtimer.h"
#include "tn.h"

#define VGA_TIMEOUT 0x000FFFFF

#define HSYN	BIT21			//PWM5 P0.21
#define VSYN	BIT22			//GPIO P0.22

#define RED BIT17
#define GREEN BIT18
#define BLUE BIT19

#define RGB	(RED | GREEN | BLUE)

#define R_MASK 0x2
#define G_MASK 0x4
#define B_MASK 0x8
#define RG_MASK 0x6
#define RB_MASK 0xA
#define GB_MASK 0xC
#define RGB_MASK 0xE

#define HFREQ	0x0810				// Horz synch frequency
#define HSYNW	0xE2		// Horz synch width

#define SET_VGA_VSYN_DIR (FIO0DIR |= VSYN)
#define SET_VGA_RGB_DIR (FIO1DIR |= RGB)

#define CLR_VSYN (FIO0CLR = VSYN)
#define SET_VSYN (FIO0SET = VSYN)

#define CLR_RGB (FIO1CLR = RGB)
#define SET_RGB (FIO1SET = RGB)

#define CLR_RED (FIO1CLR = RED)
#define SET_RED (FIO1SET = RED)

#define CLR_GREEN (FIO1CLR = GREEN)
#define SET_GREEN (FIO1SET = GREEN)
#define CLR_BLUE (FIO1CLR = BLUE)
#define SET_BLUE (FIO1SET = BLUE)

enum VSYNC_STATES_E
{
  P_START = 1,
  Q_START = 3,
  R_START = 34,
  S_START = 514,
  S_END = 528
};

U16 vsyn_state = P_START;

/**
*****************************************************************************
	Function Name : vsync_timer_function()

	Description   :

	Input         : Void 

	Output        : None

	Note          :
******************************************************************************
*/
void vsync_timer_function(void)
{
  U32 regVal;
  regVal = PWMIR;

  switch(vsyn_state)
  {
    case P_START:
      CLR_VSYN;
      break;
    
    case Q_START:
      SET_VSYN;
      break;
      
    case S_END:
      vsyn_state = 0;
      break;
  }
  
  vsyn_state++;
 
  PWMIR |= regVal;		/* clear interrupt flag on match 0 */
  VICVectAddr = 0;		/* Acknowledge Interrupt */
}

/**
*****************************************************************************
	Function Name : init_Hsync()

	Description   :

	Input         : Void 

	Output        : None

	Note          :
******************************************************************************
*/
void init_Hsync(void)
{
  PINSEL1 = 0x00000400;
  PINSEL0 = 0x000A800A; /* set GPIOs for all PWMs */
  PWMPR = 0;  						  // prescale = 0
  PWMMCR = PWMMCR_PWMMR0I_MASK | PWMMCR_PWMMR4R_MASK ; //reset on MR0, int on MR4
  PWMPCR = PWMENA5; 					// PWM5 output enabled
  PWMTCR = 0x0A;	 					//CNT enabled
  PWMMR0 = HFREQ;  					//31.77KHz line freq
  PWMMR5 = HFREQ-HSYNW;			//PWM5 match value
  PWMMR4 = HFREQ;			//PWM4 match value
  PWMLER = 0x7F;						//use new values (all), latch enable
  PWMTC = 0;								//reset timer counter
  PWMTCR=9;									//run PWM
  
  VICIntSelect &= ~ VIC_BIT(VIC_PWM);
  VICVectAddr6 = (unsigned int)vsync_timer_function;  //-- Timer 0 int - priority top-1
  VICVectCntl6 = VIC_ENABLE | VIC_PWM;
  VICIntEnable |= VIC_BIT(VIC_PWM);	
}

/**
*****************************************************************************
	Function Name : init_vga()

	Description   :

	Input         : Void 

	Output        : None

	Note          :
******************************************************************************
*/
void init_vga(void)
{
  int i,j,temp, timeout = 0;

	SET_VGA_VSYN_DIR;
	SET_VGA_RGB_DIR;

  init_Hsync();
  
//  FIO1MASK2 = 0xF1;

  while(1 && timeout++ < VGA_TIMEOUT)
  {
    if(vsyn_state == R_START)
    {
      for(i = 0; i < 480; i++)
      {
        temp=vsyn_state;
        for(j = 0; j < 9; j++)
        {
          FIO1PIN2 = R_MASK;
        }
        for(j = 0; j < 9; j++)
        {
          FIO1PIN2 = G_MASK;
        }
        for(j = 0; j < 9; j++)
        {
          FIO1PIN2 = B_MASK;
        }
        for(j = 0; j < 9; j++)
        {
          FIO1PIN2 = RG_MASK;
        }
        for(j = 0; j < 9; j++)
        {
          FIO1PIN2 = RB_MASK;
        }
        for(j = 0; j < 12; j++)
        {
          FIO1PIN2 = GB_MASK;
        }
	for(j = 0; j < 12; j++)
	{
          FIO1PIN2 = RGB_MASK;
	}
	FIO1PIN2 = 0x00;
        while(temp==vsyn_state);
      }
    }
  }
	
}

 




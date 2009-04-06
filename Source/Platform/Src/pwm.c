/*****************************************************************************
 *   pwm.c:  PWM module file for Philips LPC214x Family Microprocessors
 *
 *   Copyright(C) 2006, Philips Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2005.10.01  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#include "lpc_2148.h"                        /* LPC21xx definitions */
#include "type.h"
#include "pwm.h"
#include "lpc_vic.h"

/******************************************************************************
** Function name:		PWMHandler
**
** Descriptions:		PWM interrupt handler
**				For now, it only deals with PWM match 0
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void PWMHandler (void)
{
  U32 regVal;
  
  regVal = PWMIR;
  if ( regVal & MR0_INT )
  {
  }
  
  PWMIR |= regVal;		/* clear interrupt flag on match 0 */
  VICVectAddr = 0;		/* Acknowledge Interrupt */
  
}

/******************************************************************************
** Function name:		PWM_Init
**
** Descriptions:		PWM initialization, setup all GPIOs to PWM0~6,
**				reset counter, all latches are enabled, interrupt
**				on PWMMR0, install PWM interrupt to the VIC table.
**
** parameters:			Duty cycle
** Returned value:		true or fase, if VIC table is full, return false
** 
******************************************************************************/
U32 PWM_Init( U32 cycle )
{
    PINSEL0 = (PINSEL0 & ~(3 << 16)) | (0x2 << 16);

    PWMTCR = TCR_RESET;		/* Counter Reset */ 
		
    PWMPR = 0x00;		/* count frequency:Fpclk */
//    PWMMCR = PWMMR0I | PWMMR0R;	/* interrupt on PWMMR0, reset on PWMMR0, reset 
//				TC if PWM0 matches */				
    PWMMR0 = cycle;		/* set PWM cycle */

    /* all PWM latch enabled */
    PWMLER = LER0_EN;

	VICIntSelect &= ~ VIC_BIT(VIC_PWM);
	VICVectAddr6 = (unsigned int)PWMHandler;   //-- Timer 0 int - priority top-1
	VICVectCntl6 = VIC_ENABLE | VIC_PWM;
	VICIntEnable |= VIC_BIT(VIC_PWM);

    return (TRUE);
}

/******************************************************************************
** Function name:		PWM_Set
**
** Descriptions:		PWM cycle setup
**
** parameters:			PWM cycle and offset
** Returned value:		None
** 
******************************************************************************/
void PWM_Set( U32 cycle)
{			
    PWMMR0 = cycle;		/* set PWM cycle */

    /* The LER will be cleared when the Match 0 takes place, in order to
    load and execute the new value of match registers, all the PWMLERs need to
    reloaded. all PWM latch enabled */
    PWMLER = LER0_EN;    
    return;
}

/******************************************************************************
** Function name:		PWM_Start
**
** Descriptions:		Enable PWM by setting the PCR, PTCR registers
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void PWM_Start( void )
{
    /* All single edge, all enable */
    PWMPCR = PWMENA1;
    PWMTCR = TCR_CNT_EN | TCR_PWM_EN;	/* counter enable, PWM enable */
    return;
}

/******************************************************************************
** Function name:		PWM_Stop
**
** Descriptions:		Stop all PWM channels
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void PWM_Stop( void )
{
    PWMPCR = 0;
    PWMTCR = 0x00;		/* Stop all PWMs */
    return;
}


#include "lpc_2148.h"
#include "lpc_vic.h"
#include "buzzer.h"
#include "lcd.h"

/**
*****************************************************************************
	Function Name : extISR2()

	Description   :

	Input         :  

	Output        : 

	Note          :
******************************************************************************
*/
void extISR2(void)
{
  buzzer_on();	

  EXTINT = EXTINT_EINT2_MASK;      //clear interrupt
  VICVectAddr = 0; 	
}

/**
*****************************************************************************
	Function Name : init_ext_interrupt()

	Description   :

	Input         :  

	Output        : 

	Note          :
******************************************************************************
*/
void init_ext_interrupt(void)
{
EXTMODE |= EXTMODE_EXTMODE2_MASK; 			 /* INT2 edge trigger */
EXTPOLAR |= EXTPOLAR_EXTPOLAR2_MASK; 			 /* INT2 is falling edge by default */

PINSEL0 = (PINSEL0 & ~(3 << 30)) | (1 << 31);

/* initialize the interrupt vector */
VICIntSelect &= ~ VIC_BIT(VIC_EINT2); 		 // EINT0 selected as IRQ
VICVectAddr7 = (unsigned int)extISR2; 		 // address of the ISR
VICVectCntl7 = VIC_ENABLE | VIC_EINT2;
VICIntEnable = VIC_BIT(VIC_EINT2);				 // EINT0 interrupt enabled

/* clear interrupt */
EXTINT &= ~EXTINT_EINT2_MASK; 
}




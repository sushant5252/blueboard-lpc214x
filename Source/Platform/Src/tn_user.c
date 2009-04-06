/*
TNKernel real-time kernel - USB examples

Copyright © 2004,2006 Yuri Tiomkin
All rights reserved.
 
Permission to use, copy, modify, and distribute this software in source
and binary forms and its documentation for any purpose and without fee
is hereby granted, provided that the above copyright notice appear
in all copies and that both that copyright notice and this permission
notice appear in supporting documentation.

THIS SOFTWARE IS PROVIDED BY THE YURI TIOMKIN AND CONTRIBUTORS ``AS IS'' AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL YURI TIOMKIN OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
SUCH DAMAGE.
*/


#include "lpc_2148.h"
#include "tn.h"
#include "Hardware.h"

typedef void (*int_func)(void);

/**
******************************************************************************
	Function Name : tn_cpu_irq_handler()

	Description   :

	Input         : Void

	Output        : None

	Note          :
*******************************************************************************
*/  
void tn_cpu_irq_handler(void)
{
   int_func ifunc;
   ifunc = (int_func)VICVectAddr;
   if(ifunc != NULL)
      (*ifunc)();
}



/**
********************************************************************************
	Function Name : tn_timer_int_func()

	Description   :

	Input         : Void

	Output	      : None

	Note          :
********************************************************************************
*/
void tn_timer0_int_func(void)
{
   T0IR = 0xFF;                //-- clear int source

  tn_tick_int_processing();    //-- OS timer ticks

   VICVectAddr = 0xFF;
}


/**
**********************************************************************************
	Function Name : tn_uart0_int_func()

	Description   :

	Input         : Void

	Output        : 

	Note          :
**********************************************************************************
*/

void tn_uart0_int_func(void)  //-- Not uses here
{
   volatile int data;

   data = U0IIR;               //-- clear int source

   VICVectAddr = 0xFF;
}


/**
***********************************************************************************
	Function Name : tn_default_func()

	Description  :

	Input	     : Void

	Output       : 

	Note         :
************************************************************************************
*/
void tn_int_default_func(void)
{
   VICVectAddr = 0xFF;
}

//----------------------------------------------------------------------------
// Processor specific routine
//
// For LPC2xxx, here we enable all int vectors that we use ( not just tick timer)
// and than enable IRQ int in ARM core
//
// In LPC214x USB example we need just enable IRQ int in ARM core
//----------------------------------------------------------------------------

/**
*************************************************************************************
	Function Name : tn_cpu_int_enable()

	Description  :

	Input	     : 

	Output       : 

	Note         :
************************************************************************************
*/
void tn_cpu_int_enable()
{
   tn_arm_enable_interrupts();
}









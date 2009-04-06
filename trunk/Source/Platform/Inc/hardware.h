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

//----------------------------------------------------------------------------
//    VALID ONLY FOR 'LPC2146 USB TEST' PROJECT
//----------------------------------------------------------------------------


#ifndef  _HARDWARE_H_
#define  _HARDWARE_H_

#include "utils.h"

#define PORT1_HIGH(bit)	FIO1SET |= (bit);
#define PORT1_LOW(bit)	FIO1CLR |= (bit);

#define IRQ_RAM_ADDR        0x40000018
#define FIQ_RAM_ADDR        0x4000001C
#define IRQ_RAM_FUNC_ADDR   0x40000038
#define FIQ_RAM_FUNC_ADDR   0x4000003C
#define RAM_START_ADDR      0x40000000

#define SPI_SSEL0 			BIT7
#define RESET_F				BIT22

#define	PORT0_DIRECTION_VALUE  (SPI_SSEL0 | RESET_F )   






void  HardwareInit(void);
void init_watchdog_timer(void);

//--- TNKernel core functions

void tn_arm_enable_interrupts(void);
void tn_arm_disable_interrupts(void);
void init_ports(void);



#endif


#include "lpc_config.h"
#include "lpc_2148.h"
#include "hardware.h"
#include "lpc_scb.h"
#include "timer.h"
#include "spi.h"
#include "uart.h"
#include "lpc_uart.h"
#include "pwm.h"
#include "softtimer.h"
#include "circular_buf.h"
#include "rtc.h"
#include "vga.h"
#include "vcom.h"
#include "adc.h"
#include "lcd.h"
#include "riffwave.h"			
#include "ps2kbd.h"
#include "ext_interrupt.h"
#include "74HC595.h"
#include "i2c.h"
#include "buzzer.h"
#include "led.h"

#define SECOND 100000

/**
*******************************************************************
	Function Name : init_ports()

	Description   :

	Input         : Void

	Output        : None

	Note          :
********************************************************************
*/
void init_ports(void)						
{
  SCS = 3;    //sets port 1 and 0 to fast mode (not compatable with previous LPCs)
  LATCH_DIR();
}

/**
*******************************************************************
	Function Name : lpc_pll_init()

	Description   :

	Input         : Void

	Output        : None

	Note          :
********************************************************************
*/
static void lpc_pll_init(void)
{
  // set PLL multiplier & divisor.
  // values computed from config
  PLLCFG = PLLCFG_MSEL | PLLCFG_PSEL;
  // enable PLL
  PLLCON = PLLCON_PLLE;
  PLLFEED = 0xAA;                       // Make it happen.  These two updates
  PLLFEED = 0x55;                       // MUST occur in sequence.
			
  // wait for PLL lock
  while (!(PLLSTAT & PLLSTAT_LOCK))
    continue;
	
  // enable & connect PLL
  PLLCON = PLLCON_PLLE | PLLCON_PLLC;
  PLLFEED = 0xAA;                       // Make it happen.  These two updates
  PLLFEED = 0x55;                       // MUST occur in sequence.
	
  // setup & enable the MAM
  MAMTIM = MAMTIM_CYCLES;
  MAMCR = MAMCR_FULL;	
  // set the peripheral bus speed
  // value computed from config.h
  VPBDIV = VPBDIV_VALUE;                // set the peripheral bus clock speed
}

/**
**********************************************************************
	Function Name : init_watchdog_timer()

	Description   :

	Input         : Void

	Output        : None

	Note          :
***********************************************************************
*/
void init_watchdog_timer(void)
{
  WDTC = 0xFF;
  WDMOD = 3;

  WDFEED = 0xAA;
  WDFEED = 0x55;
}

/**
*****************************************************************************
	Function Name : Hardwareinit()

	Description   :

	Input         : Void 

	Output        : None

	Note          :
******************************************************************************
*/
void  HardwareInit(void)
{
  lpc_pll_init();
  init_ports();	
  turn_off_buzzer();	
  VCOM_init();	
  init_lcd();
  tn_cpu_int_enable();
  init_vga();
  init_timer0_for_audio();
  delay(2 * SECOND); // 5 sec delay given (for test audio)		
  i2c_lpc_init(I2C_SPEED_100);
  SPI_Init();
  turn_off_all_leds();
  init_uart0();
  init_uart1();
  init_keyboard();
  init_adc0();
  init_timer0();
  init_timer1();
  init_software_timer();	
  init_ext_interrupt();
}





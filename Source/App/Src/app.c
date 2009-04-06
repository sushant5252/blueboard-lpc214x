#include "lpc_2148.h"
#include "hardware.h"
#include "tn.h"
#include "type.h"
#include "utils.h"
#include "uart.h"
#include "74HC595.h"
#include "led.h"
#include "ps2kbd.h"
#include "test.h"
#include "lcd.h"
#include "usbapi.h"
#include "vcom.h"

#define  TASK_HARDWARE_VERIFY_PRIORITY      4
#define  TASK_HARDWARE_VERIFY_STK_SIZE	    256

U32 task_hardware_verify_stack[TASK_HARDWARE_VERIFY_STK_SIZE];

TN_TCB  tcb_hardware_verify_task;

void hardware_verify_task(void *par);

/**
*****************************************************************	
	Function Name :	main()

	Description :	To initialise system.

	Input :	none

	Output : int

	Note :
*****************************************************************
*/	
S32 main()
{
  unsigned int * ptr;
  tn_arm_disable_interrupts();
  MEMMAP = 0x1;    //-- FLASHs

  //------ Clear FW loader marks
  ptr =(unsigned int *)RAM_START_ADDR;
  *ptr = 0;
  ptr++;
  *ptr = 0;
  RSIR=0x0f;
  
  //------ Set interrupt vectors
  ptr = (unsigned int *)IRQ_RAM_ADDR;
  *ptr = 0xE59FF018;                      //-- ldr pc, [pc, #24]
  ptr = (unsigned int *)FIQ_RAM_ADDR;
  *ptr = 0xE59FF018;                      //-- ldr pc, [pc, #24]
  
  //--- Put IRQ & FIQ vectors in RAM
  ptr = (unsigned int *)IRQ_RAM_FUNC_ADDR;
  *ptr = (unsigned int)&tn_cpu_irq_isr;
  ptr = (unsigned int *)FIQ_RAM_FUNC_ADDR;
  *ptr = (unsigned int)&tn_cpu_fiq_isr;

  HardwareInit();

  tn_start_system(); //-- Never return

  return 1;
}

/**
********************************************************************************************
	Function Name :	tn_app_init()

	Description :	In this function there are two tasks ( task_main and task_led1) 
				created and activated.

	Input :	Void

	Output : Void

	Note :
**********************************************************************************************
*/
void  tn_app_init()
{
  //  message_queue_init();
  //  malloc_init();
  tcb_hardware_verify_task.id_task = 0;
  tn_task_create(&tcb_hardware_verify_task,       //-- task TCB
                  hardware_verify_task,           //-- task function
                  TASK_HARDWARE_VERIFY_PRIORITY,  //-- task priority
                  &(task_hardware_verify_stack    //-- task stack first addr in memory
                  [TASK_HARDWARE_VERIFY_STK_SIZE-1]),
                  TASK_HARDWARE_VERIFY_STK_SIZE,  //-- task stack size (in int,not bytes)
                  NULL,                           //-- task function parameter
                  TN_TASK_START_ON_CREATION	//-- Creation option
                );
}

/**
********************************************************************************************
	Function Name :	hardware_verify_task()

	Description :
	
	Input :	Void

	Output : Void

	Note :
**********************************************************************************************
*/
void hardware_verify_task(void *par)
{
  U8 ch=0;

  if(is_usb_cable_connected())
  {
 //   while(!get_USB_enumeration_status());
  }
	
  test_leds();
  turn_off_all_leds();
  test_uart0();
  test_uart1();
  test_sd_card();
  test_i2c_at24c256_flash();
  if(get_USB_enumeration_status())
  {
    test_usb();
  }
  
  while(1)
  {
    if(keyhit())
    {
      process_lcd();
    }
    if ( uart0Receive(&ch, sizeof(ch)) != -1)
    {
      test_uart0();
    }
    if ( uart1Receive(&ch,sizeof(ch)) != -1)
    {
      test_uart1();
    }
    if(get_USB_enumeration_status())
    {
      if(VCOM_read(sizeof(ch),&ch))
      {
        test_usb();
      }
    }
    process_adc();
    tn_task_sleep(10);
  }
}







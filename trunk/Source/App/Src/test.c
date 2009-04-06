#include "adc.h"
#include "type.h"
#include "ps2kbd.h"
#include "lcd.h"
#include "led.h"
#include "uart.h"
#include "vcom.h"
#include "i2c.h"
#include "74HC595.h"
#include "fs_api.h"

#define AT24C256_I2C_ADDRESS 0xA0
#define FIRST_WORD_ADDRESS 0
#define SECOND_WORD_ADDRESS 0

#define UART0_TEXT "\n\r Testing UART0 NGX's BlueBoard \n\r BlueBoard Revision : 1 \n\r Firmware Version: 1 \n\r For more information on BlueBoard visit www.ngxtechnologies.com"
#define UART1_TEXT "\n\r Testing UART1 NGX's BlueBoard \n\r BlueBoard Revision : 1 \n\r Firmware Version: 1 \n\r For more information on BlueBoard visit www.ngxtechnologies.com"
#define USB_TEXT "\n\r Testing USB NGX's BlueBoard \n\r BlueBoard Revision : 1 \n\r Firmware Version: 1 \n\r For more information on BlueBoard visit www.ngxtechnologies.com"
#define SD_CARD_TEXT "\n\r Testing SD card NGX's BlueBoard \n\r BlueBoard Revision : 1 \n\r Firmware Version: 1 \n\r For more information on BlueBoard visit www.ngxtechnologies.com"

#define GET_SCALED_LED_MASK_BUF_INDEX(val) (val/114)


const U8 led_mask_buf[] = {0,1,3,7,0xF,0x1F,0x3F,0x7F,0xFF};

/**
********************************************************************************************
	Function Name :	is_valid_ascii()

	Description :
	
	Input :	Void

	Output : Void

	Note :
**********************************************************************************************
*/
U8 is_valid_ascii(S8 key)
{
  U8 retval = FALSE;
  
  if(key >= ' ' && key <= '~')
    retval = TRUE;

  return retval;
}

/**
********************************************************************************************
	Function Name :	process_lcd()

	Description :
	
	Input :	Void

	Output : Void

	Note :
**********************************************************************************************
*/
void process_lcd(void)
{
  S8 key;
  static U8 position = 0;
	
  key = xgetkey();
  if(is_valid_ascii(key))
  {
    lcd_gotoxy(LINE2, position);
    lcd_putchar(key);
    if(position == MAX_CHAR_IN_ONE_LINE)
    {
      lcd_clear();
      position = 0;
    }		
    position++;
  }
}

/**
********************************************************************************************
	Function Name :	process_adc()

	Description :
	
	Input :	Void

	Output : Void

	Note :
**********************************************************************************************
*/
void process_adc(void)
{
  U16 adc_value = 0;
  U8 index = 0;

  adc_value = adc_read(ADC0, CHANNEL_3);
  index = GET_SCALED_LED_MASK_BUF_INDEX(adc_value);
  if(index > sizeof(led_mask_buf))
    index = sizeof(led_mask_buf) -1;
  turn_on_led(led_mask_buf[index]);
}

/**
*******************************************************************************
  Function Name : test_uart0()

  Description :	 

  Input :	

  Output :	none

  Note : 
*******************************************************************************
*/
void test_uart0(void)
{
  U8 buffer[] = UART0_TEXT;
  
  uart0Write((char*)buffer, sizeof(buffer));
}

/**
*******************************************************************************
  Function Name : test_uart1()

  Description :	 

  Input :	

  Output :	none

  Note : 
*******************************************************************************
*/
void test_uart1(void)
{
  U8 buffer[] = UART1_TEXT;

  uart1Write((char*)buffer, sizeof(buffer));
}

/**
*******************************************************************
	Funtion Name : test_usb()

	Description  : 

	Input        : 

	Output       : None

	Note         :
*****************************************************************
*/
void test_usb(void)
{
  U8 buffer[] = USB_TEXT;

  while((VCOM_write(sizeof(buffer), buffer)) != sizeof(buffer));
}

/**
*******************************************************************
	Funtion Name : test_sd_card()

	Description  : 

	Input        : 

	Output       : None

	Note         :
*****************************************************************
*/
void test_sd_card(void)
{
  if(!fs_init())
  {
    set_file_system_status(TRUE);
    lcd_clear();
    lcd_putstring(LINE1,"SD card - OK");
  }
  else
  {
    lcd_clear();
    lcd_putstring(LINE1,"SD card - not OK");
  }
}

/**
*******************************************************************
	Funtion Name : check_at24c256_connction()

	Description  : 

	Input        : 

	Output       : None

	Note         :
*****************************************************************
*/
U8 check_at24c256_connction()
{
  U8 read_buf[5];
  U32 status=0;
  U8 slave_address = AT24C256_I2C_ADDRESS;
  
  read_buf[0] = slave_address | 0x1;
  
  if (i2c_master_read(I2C_0, read_buf,1) == OK)
  {
  /* A successful i2c read means that the  data flash is connected */
    status = 1;
  }
  
  return status;
}

/**
*******************************************************************
	Funtion Name : test_i2c_at24c256_flash()

	Description  : 

	Input        : 

	Output       : None

	Note         :
*****************************************************************
*/
void test_i2c_at24c256_flash(void)
{
  U8 wr_buffer[] = {AT24C256_I2C_ADDRESS, FIRST_WORD_ADDRESS, SECOND_WORD_ADDRESS, 1, 2, 3, 4, 5, 6, 7, 8, 10};
  U8 rd_buffer[11] = {0};
  
  if(check_at24c256_connction())
  {
    lcd_putstring(LINE2,"I2C EEPROM - OK");
    i2c_master_write(I2C_0, wr_buffer, sizeof(wr_buffer));
    delay(1000);
    i2c_master_write(I2C_0, wr_buffer, 3);
    rd_buffer[0] = AT24C256_I2C_ADDRESS | 1;
    i2c_master_read(I2C_0, rd_buffer, 10);
  }
  else
    lcd_putstring(LINE2,"I2C EEPROM-NOT OK");		
}


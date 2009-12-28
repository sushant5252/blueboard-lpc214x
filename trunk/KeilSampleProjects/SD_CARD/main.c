#include "lcd.h"
#include "type.h"

U8 init_sdcard(void)
{
 if(if_initInterface(0)==0)
 return 1;
 else
 return 0;
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
  if(init_sdcard())
  {
    lcd_clear();
    lcd_putstring(LINE1,"SD card - OK");
  }
  else
  {
    lcd_clear();
    lcd_putstring(LINE1,"SD card - not OK");
  }
}

int main (void) 
{
  init_lcd();
  test_sd_card();
  while (1)  
  {
  }
}

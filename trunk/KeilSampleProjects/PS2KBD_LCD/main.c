#include "lcd.h"
#include "ps2kbd.h"

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

int main (void) 
{
  init_lcd();
  init_keyboard();

  while (1)  
  {
    if(keyhit())
    {
      process_lcd();
    }
  }
}

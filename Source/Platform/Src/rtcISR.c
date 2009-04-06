#include "lpc_2148.h"
#include "rtc.h"
#include "lcd.h"

/**
********************************************************************************************
 Function Name : rtcISR()

 Description   :	

 Input         :	

 Output        : Void

 Note          :
********************************************************************************************
*/
void rtcISR (void)
{
  time_rec time;
  U8 buffer[3] = {0}; 

  CCR = (RTC_CCR_CLKEN | RTC_CCR_CLKSRC);
  PCONP |= SCB_PCONP_PCRTC;

  if (ILR & RTC_ILR_RTCCIF)
  {
    get_time(&time);
    buffer[0] = time.hour;
    buffer[1] = ':';
    buffer[2] = time.minute;
    lcd_putstring(LINE1, (char *)buffer);

    ILR = RTC_ILR_RTCCIF;
  }

  if (ILR & RTC_ILR_RTCALF)
  {
     // not using			
     ILR = RTC_ILR_RTCALF;
  }

  VICVectAddr = (unsigned int) 0;

  CCR = (RTC_CCR_CLKEN | RTC_CCR_CLKSRC);
  PCONP &= ~SCB_PCONP_PCRTC;
}



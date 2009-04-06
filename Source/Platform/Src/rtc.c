#include "rtcISR.h"
#include "rtc.h"
#include "lpc_2148.h"
#include "type.h"
#include "lpc_vic.h"
#include "type.h"
#include "tn.h"


#define PRESCALAR_INTIGER_VALUE 1830
#define PRESCALAR_FRACTION_VALUE 1792

/**
********************************************************************************************
 Function Name : rtcSleep()

 Description   :	

 Input         :	

 Output        : Void

 Note          :   Place RTC on 32kHz xtal and disconnect power.
********************************************************************************************
*/
static inline void rtcSleep (void)
{
  CCR = (RTC_CCR_CLKEN | RTC_CCR_CLKSRC);   /* conters are enabled and  Place RTC on 32kHz xtal and disconnect power.*/
  PCONP &= ~SCB_PCONP_PCRTC;
}

/**
********************************************************************************************
 Function Name : rtcWake()

 Description   :	

 Input         :	

 Output        : Void

 Note          :     Prepare clock for interactive use.
 
********************************************************************************************
*/
static inline void rtcWake (void)
{
  CCR = (RTC_CCR_CLKEN | RTC_CCR_CLKSRC);
  PCONP |= SCB_PCONP_PCRTC;
}
#if 0
//  Read clock registers and return tm structure.
static void rtcRead (struct tm *theTime, unsigned int *milliseconds)
{
  unsigned int ticks32Khz;
  rtcCTIME0_t ctime0;
  rtcCTIME1_t ctime1;
  rtcCTIME2_t ctime2;

  rtcWake ();
//  portENTER_CRITICAL ();

  do
  {
    ctime0.i = CTIME0;
    ctime1.i = CTIME1;
    ctime2.i = CTIME2;

    do
      ticks32Khz = (CTC & 0xfffe);
    while (ticks32Khz != (CTC & 0xfffe));
  }
  while (ctime0.i != CTIME0);

//  portEXIT_CRITICAL ();
  rtcSleep ();

  if (milliseconds)
    *milliseconds = (((ticks32Khz / 2) * 1000u) / 32768u);

  theTime->tm_sec   = ctime0.seconds;
  theTime->tm_min   = ctime0.minutes;
  theTime->tm_hour  = ctime0.hours;
  theTime->tm_mday  = ctime1.dom;
  theTime->tm_mon   = ctime1.month - 1;
  theTime->tm_year  = ctime1.year - 1900;
  theTime->tm_wday  = ctime0.dow;
  theTime->tm_yday  = ctime2.doy - 1;
  theTime->tm_isdst = 0;
}

//  Set clock to new values.
static void rtcWrite (struct tm *newTime)
{
  rtcWake ();
//  portENTER_CRITICAL ();

  CCR &= ~RTC_CCR_CLKEN;
  CCR |=  RTC_CCR_CTCRST;

  SEC   = newTime->tm_sec;
  MIN   = newTime->tm_min;
  HOUR  = newTime->tm_hour;
  DOM   = newTime->tm_mday;
  MONTH = newTime->tm_mon + 1;
  YEAR  = newTime->tm_year + 1900;
  DOW   = newTime->tm_wday;
  DOY   = newTime->tm_yday + 1;

  CCR &= ~RTC_CCR_CTCRST;
  CCR |=  RTC_CCR_CLKEN;

//  portEXIT_CRITICAL ();
  rtcSleep ();
}

time_t rtcGetEpochSeconds (unsigned int *milliseconds)
{
  struct tm tm;

  rtcRead (&tm, milliseconds);
  return mktime (&tm);
}

void rtcSetEpochSeconds (time_t now)
{
  struct tm tm;

  localtime_r (&now, &tm);
  rtcWrite (&tm);
}

//  Start clock so that the sytsem may use it.
static void rtcInitClockCalendar (void)
{
  int nonsense = 0;
  rtcCTIME0_t ctime0;
  rtcCTIME1_t ctime1;
  rtcCTIME2_t ctime2;

  rtcWake ();
  
  ctime0.i = CTIME0; 
  ctime1.i = CTIME1; 
  ctime2.i = CTIME2;

  //  Leisurely tear the packed time apart into individual time.
  if ((ctime0.seconds < 0) || (ctime0.seconds > 59))  nonsense = 1;
  if ((ctime0.minutes < 0) || (ctime0.minutes > 59))  nonsense = 1;
  if ((ctime0.hours < 0)   || (ctime0.hours > 23))    nonsense = 1;
  if ((ctime1.dom < 1)     || (ctime1.dom > 31))      nonsense = 1;
  if ((ctime1.month < 1)   || (ctime1.month > 12))    nonsense = 1;
  if ((ctime1.year < 1980) || (ctime1.year > 2050))   nonsense = 1;
  if ((ctime0.dow < 0)     || (ctime0.dow > 6))       nonsense = 1;
  if ((ctime2.doy < 0)     || (ctime2.doy > 366))     nonsense = 1;

  rtcSleep ();

  //  Set the clock to Jan 1, 2006 00:00:00
  if (nonsense) 
    rtcSetEpochSeconds ((time_t) 1136073600);
}
#endif

/**
********************************************************************************************
 Function Name : rtcInit()

 Description   :	

 Input         :	

 Output        : Void

 Note          :    
 
********************************************************************************************
*/
void init_rtc (void)
{
  PCONP |= SCB_PCONP_PCRTC;           /*when 9th bit is 0 it takes clk from external oscillator if 1 from internal prescalar*/
  
  PREINT  = PRESCALAR_INTIGER_VALUE;
  PREFRAC = PRESCALAR_FRACTION_VALUE;


  CCR = 0;

  CCR |= RTC_CCR_CLKSRC;             /*If 4th bit is 0, the Clock Tick Counter takes its clock from the Prescaler,
                                                                    If this bit is 1, the CTC takes its clock from the 32 kHz oscillator that?s connected to
                                                                    the RTCX1 and RTCX2 pins*/

  AMR = RTC_AMR_AMRMASK;
//  CIIR = 0;
  CIIR = CIIR_IMMIN_MASK;


  ILR = RTC_ILR_MASK;                /*if 1 CIIR generates an interrupt if 2 Alarm reg generates an interrupt*/

  VICIntSelect &= ~ VIC_BIT(VIC_RTC);
  VICIntEnable |= VIC_BIT(VIC_RTC);
  VICVectCntl9 = VIC_ENABLE | VIC_RTC;
  VICVectAddr9 = (unsigned int) rtcISR;
  
  CCR |= RTC_CCR_CLKEN;             /* When 1st bit is 1 the time counters are enabled.
                                                                    When it is 0, they are disabled so that they may be initialized.*/
 
}

//int rtcSetAlarm (struct tm *tm)
/**
********************************************************************************************
 Function Name : rtcSetAlarm()

 Description   :	

 Input         :	

 Output        : Void

 Note          :     
 
********************************************************************************************
*/
U32 rtcSetAlarm (U8 hour, U8 min)	
{
  rtcWake ();
  
  AMR = RTC_AMR_AMRMASK;
  
  ALYEAR = 0;
  ALMON = 0;
  ALDOM =0;
  ALHOUR =0;
  ALHOUR =hour ;
  ALMIN =min;
  ALSEC= 0;
  ALDOW = 0;
  ALDOY = 0;	
  
  AMR &=~(RTC_AMR_AMRHOUR|RTC_AMR_AMRMIN); 
  
  rtcSleep ();
  return 0;
}

#if 0
struct tm *rtcGetAlarmTm (struct tm *tm)
{
  if (tm)
  {
    memset (tm, 0, sizeof (* tm));

    rtcWake ();

    if (AMR != RTC_AMR_AMRMASK)
    {
      tm->tm_year = RTC_ALYEAR - 1900;
      tm->tm_mon = RTC_ALMON - 1;
      tm->tm_mday = RTC_ALDOM;
      tm->tm_hour = RTC_ALHOUR;
      tm->tm_min = RTC_ALMIN;
      tm->tm_sec = RTC_ALSEC;
    }
    else
    {
      time_t now = 0;
      memcpy (tm, localtime (&now), sizeof (* tm));
    }

    rtcSleep ();
  }

  return tm;
}

time_t rtcGetAlarmEpochSeconds (void)
{
  struct tm tm;

  return mktime (rtcGetAlarmTm (&tm));
}
#endif
/**
********************************************************************************************
 Function Name : rtcPeriodicAlarm()

 Description   :	

 Input         :	

 Output        : Void

 Note          :     
 
********************************************************************************************
*/
int rtcPeriodicAlarm (int mode)
{
  int state;
    
  rtcWake ();

  state = CIIR & RTC_CIIR_IMMIN;

  if (!mode)
    CIIR &= ~RTC_CIIR_IMMIN;
  else if (mode > 0)
    CIIR |= RTC_CIIR_IMMIN;

  rtcSleep ();

  return state ? 1 : 0;
}

/**
********************************************************************************************
 Function Name : set_time()

 Description   :	

 Input         :	

 Output        : Void

 Note          :   
 
********************************************************************************************
*/
void set_time(time_rec *time)
{
  YEAR  =  time->year; 
  MONTH =  time->month;
  DOM   =  time->day_of_month;
  DOY   =  time->day_of_year;
  DOW   =  time->day_of_week;
  HOUR  =  time->hour;
  MIN   =  time->minute;
  SEC   =  0;
}
/**
********************************************************************************************
 Function Name : get_time()

 Description   :	

 Input         :	

 Output        : Void

 Note          :
********************************************************************************************
*/
void get_time(time_rec *time)
{
  time->year          =YEAR;
  time->month         =MONTH;
  time->day_of_month  =DOM;
  time->day_of_year   =DOY;
  time->day_of_week   =DOW;
  time->hour          =HOUR;
  time->minute        =MIN;
  time->seconds       =SEC;  
}


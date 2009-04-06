#ifndef _RTC_H_
#define _RTC_H_

//#include <time.h>
#include <type.h>

#define RTC_ILR_RTCCIF  (0x00000001)
#define RTC_ILR_RTCALF  (0x00000002)
#define RTC_ILR_MASK    (0x00000003)

#define RTC_CCR_CLKEN   (0x00000001)
#define RTC_CCR_CTCRST  (0x00000002)
#define RTC_CCR_TEST    (0x0000000c)
#define RTC_CCR_CLKSRC  (0x00000010)

#define RTC_CIIR_IMSEC  (0x00000001)
#define RTC_CIIR_IMMIN  (0x00000002)
#define RTC_CIIR_IMHOUR (0x00000004)
#define RTC_CIIR_IMDOM  (0x00000008)
#define RTC_CIIR_IMDOW  (0x00000010)
#define RTC_CIIR_IMDOY  (0x00000020)
#define RTC_CIIR_IMMON  (0x00000040)
#define RTC_CIIR_IMYEAR (0x00000080)
#define RTC_CIIR_IMMASK (0x000000ff)

#define RTC_AMR_AMRSEC  (0x00000001)
#define RTC_AMR_AMRMIN  (0x00000002)
#define RTC_AMR_AMRHOUR (0x00000004)
#define RTC_AMR_AMRDOM  (0x00000008)
#define RTC_AMR_AMRDOW  (0x00000010)
#define RTC_AMR_AMRDOY  (0x00000020)
#define RTC_AMR_AMRMON  (0x00000040)
#define RTC_AMR_AMRYEAR (0x00000080)
#define RTC_AMR_AMRMASK (0x000000ff)
#define VIC_IntSelect_RTC       (0x00002000)
#define SCB_PCONP_PCRTC     (0x00000200)
#define VIC_Channel_RTC         (13)
#define VIC_VectCntl_ENABLE     (0x00000020)
#define VIC_IntEnable_RTC       (0x00002000)


/* Scheduler structure*/
typedef struct
{
 U16 year;
 U8 month;
 U8 day_of_month;
 U16 day_of_year;
 U8 day_of_week;
 U8 hour;
 U8 minute;
 U8 seconds; 
}
#if defined(GNU) 
__attribute__ ((packed))
#endif /* defined(GNU) */
time_rec;

//
//  Define this for using non-vectored IRQs.  Undef it if a regular vectored
//  IRQ is preferred.
//
#define RTC_NONVECTOREDIRQ

//
//
//
void init_rtc (void);
#if 0
time_t rtcGetEpochSeconds (unsigned int *milliseconds);
void rtcSetEpochSeconds (time_t now);
int rtcSetAlarm (struct tm *tm);
struct tm *rtcGetAlarmTm (struct tm *tm);
time_t rtcGetAlarmEpochSeconds (void);
int rtcPeriodicAlarm (int mode);
#endif

U32 rtcSetAlarm (U8 hour, U8 min);	
void get_time(time_rec *time);
void set_time(time_rec *time);

#endif

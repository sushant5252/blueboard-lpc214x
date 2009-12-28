/******************************************************************************
 *
 * $RCSfile: lpcVIC.h,v $
 * $Revision: 1.1 $
 *
 * Header file for Philips LPC ARM Processors.
 * Copyright 2004 R O SoftWare
 *
 * No guarantees, warrantees, or promises, implied or otherwise.
 * May be used for hobby or commercial purposes provided copyright
 * notice remains intact.
 *
 *****************************************************************************/
#ifndef INC_LPC_VIC_H
#define INC_LPC_VIC_H


// VIC Channel Assignments
#define VIC_WDT         0
#define VIC_TIMER0      4
#define VIC_TIMER1      5
#define VIC_UART0       6
#define VIC_UART1       7
#define VIC_PWM         8
#define VIC_PWM0        8
#define VIC_I2C         9
#define VIC_SPI         10
#define VIC_SPI0        10
#define VIC_SPI1        11
#define VIC_PLL         12
#define VIC_RTC         13
#define VIC_EINT0       14
#define VIC_EINT1       15
#define VIC_EINT2       16
#define VIC_EINT3       17
#define VIC_ADC0         18
#define VIC_ADC1         21
// Vector Control Register bit definitions
#define VIC_ENABLE      (1 << 5)

// Convert Channel Number to Bit Value
#define VIC_BIT(chan)   (1 << (chan))

#endif


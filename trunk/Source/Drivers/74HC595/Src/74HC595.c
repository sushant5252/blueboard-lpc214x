#include "type.h"
#include "lpc_2148.h"
#include "utils.h"
#include "spi.h"
#include "74HC595.h"
#include "led.h"

#define OE_LOW() (FIO0CLR = BIT7)
#define OE_HIGH() (FIO0SET = BIT7)

#define PARALLEL_DATA_HIGH_74HC595 0xFF
#define PARALLEL_DATA_LOW_74HC595 0x00

#define LATCH_SET() (FIO1SET = STCP)
#define LATCH_CLR() (FIO1CLR = STCP)

/**
********************************************************************************************
	Function Name :	parallel_data_output_high_74hc595()

	Description :
	
	Input :	Void

	Output : Void

	Note :
**********************************************************************************************
*/
void parallel_data_output_high_74hc595()
{
  U8 data = PARALLEL_DATA_HIGH_74HC595;

  OE_LOW();
  SPI_Write(sizeof(data), &data);
  LATCH_CLR();
  LATCH_SET();
}

/**
********************************************************************************************
	Function Name :	parallel_data_output_low_74hc595()

	Description :
	
	Input :	Void

	Output : Void

	Note :
**********************************************************************************************
*/
void parallel_data_output_low_74hc595()
{
  U8 data = PARALLEL_DATA_LOW_74HC595;

  OE_LOW();
  SPI_Write(sizeof(data), &data);
  LATCH_CLR();
  LATCH_SET();
}

/**
********************************************************************************************
	Function Name :	turn_on_led()

	Description :
	
	Input :	Void

	Output : Void

	Note :
**********************************************************************************************
*/
void turn_on_led(U8 led_mask) 
{
  OE_LOW();
  SPI_Write(sizeof(led_mask), &led_mask);
  LATCH_CLR();
  LATCH_SET();	
}


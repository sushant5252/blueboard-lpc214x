#include "type.h"
#include "lpc_2148.h"
#include "utils.h"
#include "spi.h"
#include "74HC595.h"
#include "led.h"

#define PARALLEL_DATA_HIGH_74HC595 0xFF
#define PARALLEL_DATA_LOW_74HC595 0x00

#define LATCH_SET() (FIO0SET = STCP)
#define LATCH_CLR() (FIO0CLR = STCP)

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

  LATCH_CLR();
  SPI_Write(sizeof(data), &data);
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

  LATCH_CLR();
  SPI_Write(sizeof(data), &data);  
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
  LATCH_CLR();
  SPI_Write(sizeof(led_mask), &led_mask);
  LATCH_SET();	
}


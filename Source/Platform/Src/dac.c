#include "dac.h"
#include "type.h"
#include "lpc_2148.h"

/**
********************************************************************************************
  Function Name :	init_dac()
  
  Description   :	
  
  Input         :	
  
  Output        :	Void
  
  Note          :
********************************************************************************************
*/
void init_dac(void)
{
  PINSEL1 = (PINSEL1 & ~(3 << 18)) | (1 << 19);
  DACR |= (1<<16);
}
/**
********************************************************************************************
  Function Name :	start_dac_conversion()
  
  Description   :	
  
  Input         :	
  
  Output        :	Void
  
  Note          :
********************************************************************************************
*/
void start_dac_conversion(U16 digital_value)
{
  volatile U8 i=0;
  U32 temp=0;

  temp = digital_value&0x03ff; 
  temp<<=6;
  DACR &=0x00010000;
  DACR |=temp;
  for(i=0;i<10;i++);                           //the settling time for the DAC   
}


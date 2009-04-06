
#include "i2c.h"    
#include "hardware.h"    
#include "lpc_2148.h"    




#define I2C1CONSET_I2C        0x00000040  /* I2C Control Set Register */
#define I2C1CONSET_I2C_MASK   0x0000007C  /* Used bits */
#define I2DAT_I2C             0x00000000  /* I2C Data Register */
#define I2DAT_I2C_MASK        0x000000FF  /* Used bits */
#define I2C1ADR_I2C           0x00000000  /* I2C Slave Address Register */
#define I2C1ADR_I2C_MASK      0x000000FF  /* Used bits */
#define I2C1SCLH_I2C          0x00000004  /* I2C SCL Duty Cycle High Register */
#define I2C1SCLH_I2C_MASK     0x0000FFFF  /* Used bits */
#define I2C1SCLL_I2C          0x00000004  /* I2C SCL Duty Cycle Low Register */
#define I2C1SCLL_I2C_MASK     0x0000FFFF  /* Used bits */

#define SCLH  75
#define SCLL  SCLH

char Info;
char StatusI2C;




void i2c_init(U8 i2c_num) 
{
if(i2c_num == I2C_1)
{
  PINSEL0 |= I2C_PINSEL0;
  I2C1CONCLR = I2C_CLEAR_ALL_FLAGS;
  I2C1CONSET = I2C1CONSET_I2EN; 
  /*I2C = 100KHz, PCLK = 60MHz*/
  I2C1SCLH = SCLH; 
  I2C1SCLL = SCLL;    
}
else if(i2c_num == I2C_0)
{
	PINSEL0 = (PINSEL0 & ~(3 << 4)) | (1 << 4);
	PINSEL0 = (PINSEL0 & ~(3 << 6)) | (1 << 6);
	I2C0CONCLR = I2C_CLEAR_ALL_FLAGS;
	I2C0CONSET = I2C0CONSET_I2EN; 
	/*I2C = 100KHz, PCLK = 60MHz*/
	I2C0SCLH = SCLH; 
	I2C0SCLL = SCLL;	
}
}

    
void i2c_reset( void ) 
{
  /*--- Clear flags ---*/
  I2C1CONCLR = 0x6c;
  /*I2C = 100KHz, PCLK = 60MHz*/
  I2C1SCLH = SCLH; 
  I2C1SCLL = SCLL;
  I2C1ADR    = ( I2C1ADR & ~I2C1ADR_I2C_MASK ) | I2C1ADR_I2C;
  I2C1CONSET = ( I2C1CONSET & ~I2C1CONSET_I2C_MASK ) | I2C1CONSET_I2C;
} 




S8 i2c_start(U8 i2c_num) 

{
  U8 Status, Flag;
  S8 ReturnCode;

  if(i2c_num == I2C_1)
  	{
  /* Set Start flag */ 
  I2C1CONSET =0x60;      

  /*--- Wait until START transmitted ---*/
  Flag = TRUE;
  while( Flag )
  {
    /*--- Get new status ---*/
    Status = i2c_check_status(i2c_num);
    if(( Status == 0x08 ) || ( Status == 0x10 ))
    {
      /*--- Start condition has been transmitted ---*/
      Flag = FALSE;
      ReturnCode = OK;
    }
    else if( Status != 0xf8 )
    {
    /*--- ERROR ---*/
    Flag = FALSE;
    ReturnCode = (S8) Status;
    }
    else
    {
      /*--- Clear SI flag ---*/
        I2C1CONCLR = 0x08;
    }
  }

  /*--- Clear Start flag ---*/
  I2C1CONCLR = 0x20;
  	}
  else if(i2c_num == I2C_0)
  	{
  	
	/* Set Start flag */ 
	I2C0CONSET =0x60;	   
	
	/*--- Wait until START transmitted ---*/
	Flag = TRUE;
	while( Flag )
	{
	  /*--- Get new status ---*/
	  Status = i2c_check_status(i2c_num);
	  if(( Status == 0x08 ) || ( Status == 0x10 ))
	  {
		/*--- Start condition has been transmitted ---*/
		Flag = FALSE;
		ReturnCode = OK;
	  }
	  else if( Status != 0xf8 )
	  {
	  /*--- ERROR ---*/
	  Flag = FALSE;
	  ReturnCode = (S8) Status;
	  }
	  else
	  {
		/*--- Clear SI flag ---*/
		  I2C0CONCLR = 0x08;
	  }
	}
	
	/*--- Clear Start flag ---*/
	I2C0CONCLR = 0x20;
  	}
  return ReturnCode;
} 

S8 i2c_repeat_start(U8 i2c_num) 
{
  U8 Status, Flag;
  S8 ReturnCode;

  if(i2c_num == I2C_1)
  	{
  /* Set Start flag */ 
  I2C1CONSET = 0x20;
  /* Clear SI flag */     
  I2C1CONCLR = 0x08;  

  /*--- Wait until START transmitted ---*/
  Flag = TRUE;
  while( Flag )
  {
    /*--- Get new status ---*/
    Status = i2c_check_status(i2c_num);
    if(( Status == 0x08 ) || ( Status == 0x10 ))
    {
      /*--- Start condition has been transmitted ---*/
      Flag = FALSE;
      ReturnCode = OK;
    }
    else if( Status != 0xf8 )
    {
      /*--- ERROR ---*/
      Flag = FALSE;
      ReturnCode = (S8) Status;
    }
    else
    {
      /*--- Clear SI flag ---*/
      I2C1CONCLR = 0x08;    
    }
  }
  
  /*--- Clear Start flag ---*/
  I2C1CONCLR = 0x20;
  	}

  else if(i2c_num == I2C_0)
  	{
	  /* Set Start flag */ 
	  I2C0CONSET = 0x20;
	  /* Clear SI flag */	  
	  I2C0CONCLR = 0x08;  
	  
	  /*--- Wait until START transmitted ---*/
	  Flag = TRUE;
	  while( Flag )
	  {
		/*--- Get new status ---*/
		Status = i2c_check_status(i2c_num);
		if(( Status == 0x08 ) || ( Status == 0x10 ))
		{
		  /*--- Start condition has been transmitted ---*/
		  Flag = FALSE;
		  ReturnCode = OK;
		}
		else if( Status != 0xf8 )
		{
		  /*--- ERROR ---*/
		  Flag = FALSE;
		  ReturnCode = (S8) Status;
		}
		else
		{
		  /*--- Clear SI flag ---*/
		  I2C0CONCLR = 0x08;	
		}
	  }
	  
	  /*--- Clear Start flag ---*/
	  I2C0CONCLR = 0x20;

  }
  return ReturnCode;
    
}


S8 i2c_stop(U8 i2c_num) 
{
if(i2c_num == I2C_1)
{
  /* Set Stop flag */
  I2C1CONSET = 0x10;

  /* Clear SI flag */
  I2C1CONCLR = 0x48;  
  
  while( I2C1CONSET&0x10 )
  {
    /*--- do nothing ---*/
    ;
  }
}
else if(i2c_num == I2C_0)
{
	/* Set Stop flag */
	I2C0CONSET = 0x10;
	
	/* Clear SI flag */
	I2C0CONCLR = 0x48;	
	
	while( I2C0CONSET&0x10 )
	{
	  /*--- do nothing ---*/
	  ;
	}

}
  return OK;    
}





S8 i2c_master_putchar(U8 i2c_num, U8 Data ) 
{
  S8 ReturnCode;

if(i2c_num == I2C_1)  
{
  if(  I2C1CONSET&0x08   )
  {
    /*--- Send data ---*/
    I2C1DAT = Data;
    I2C1CONCLR = 0x08;     /* Clear SI flag */
    ReturnCode = OK;
  }
  else
  {
	  /*--- Data register not ready ---*/
	  ReturnCode = BUSY;
	}
  
}
else if(i2c_num == I2C_0)
{
	if(  I2C0CONSET&0x08   )
	{
	  /*--- Send data ---*/
	  I2C0DAT = Data;
	  I2C0CONCLR = 0x08;	 /* Clear SI flag */
	  ReturnCode = OK;
	}
	else
	{
		/*--- Data register not ready ---*/
		ReturnCode = BUSY;
	  }
	

}
  
  return ReturnCode;    
}


S8 i2c_master_getchar(U8 i2c_num, U8 Mode, U8 *pData ) 
{
  S8 ReturnCode;
  ReturnCode = OK;

  if(i2c_num == I2C_1)
  	{
  if( Mode == I2C_ACK0 )
  {
    /*--- The operation mode is changed from master transmit to master receive ---*/
    /*--- Set ACK=0 (informs slave to send next byte) ---*/
    I2C1CONSET=0x04;
    I2C1CONCLR = 0x08;     /* Clear SI flag */
  }
  else if( Mode == I2C_ACK1 )
  {
    /*--- Set ACK=1 (informs slave to send last byte) ---*/
    I2C1CONCLR = 0x04;
    I2C1CONCLR = 0x08;     /* Clear SI flag */ 
  }
  else if( Mode == I2C_READ )
  {
    /*--- Check if I2C Data register can be accessed ---*/
    if( I2C1CONSET&0x08 )
    {
      /*--- Read data ---*/
      *pData = (U8) I2C1DAT;
    }
    else
    {
      /*--- No data available ---*/
      ReturnCode = EMPTY;
    }
  }
  	}

  else if (i2c_num == I2C_0)
  	{
	  if( Mode == I2C_ACK0 )
	  {
		/*--- The operation mode is changed from master transmit to master receive ---*/
		/*--- Set ACK=0 (informs slave to send next byte) ---*/
		I2C0CONSET=0x04;
		I2C0CONCLR = 0x08;	   /* Clear SI flag */
	  }
	  else if( Mode == I2C_ACK1 )
	  {
		/*--- Set ACK=1 (informs slave to send last byte) ---*/
		I2C0CONCLR = 0x04;
		I2C0CONCLR = 0x08;	   /* Clear SI flag */ 
	  }
	  else if( Mode == I2C_READ )
	  {
		/*--- Check if I2C Data register can be accessed ---*/
		if( I2C0CONSET&0x08 )
		{
		  /*--- Read data ---*/
		  *pData = (U8) I2C0DAT;
		}
		else
		{
		  /*--- No data available ---*/
		  ReturnCode = EMPTY;
		}
	  }

  }
  return ReturnCode;
    
}


S8 i2c_master_write(U8 i2c_num, U8 *pTxData, U16 Length ) 

{
  S8 ReturnCode=0;
  U8 Status=0, Flag=0, ii=0;
  
  Info=0;
  StatusI2C=0;
  
  /*--- Generate Start condition ---*/
  ReturnCode = i2c_start(i2c_num);
  
  /*--- Transmit address ---*/
  if(ReturnCode == OK)
  {
    /*--- Write SLA+W ---*/
    ReturnCode = i2c_master_putchar(i2c_num, *pTxData );
    while( ReturnCode == BUSY )
    {
      ReturnCode = i2c_master_putchar(i2c_num, *pTxData );
    }
    pTxData++;
  }

  if( ReturnCode == OK )
  {
    /*--- Wait until address transmitted and transmit data ---*/
    for( ii = 1; ii<Length; ii++ )
    {
       /*--- Wait until data transmitted ---*/
      Flag = TRUE;
      while( Flag )
      {
        /*--- Get new status ---*/
        Status = i2c_check_status(i2c_num);
        
        if( (Status == 0x18) || (Status == 0x28) )
        {
          /*--- Data transmitted and ACK received ---*/
          Flag = FALSE;
          
          /*--- Write data ---*/
          ReturnCode = i2c_master_putchar(i2c_num, *pTxData );
          
          while( ReturnCode == BUSY )
          {
            ReturnCode = i2c_master_putchar(i2c_num, *pTxData );
          }
          pTxData++;
        }
        else if( Status != 0xf8 )
        {
          /*--- ERROR ---*/
          Flag = FALSE;
          ii = Length;
          ReturnCode = ERROR;
          Info|=1;
          StatusI2C=Status;
        }
      }
    }
  }
  
  /*--- Wait until data transmitted ---*/
  Flag = TRUE;
  while( Flag )
    {
      /*--- Get new status ---*/
      Status = i2c_check_status(i2c_num);
      if( (Status == 0x18) || (Status == 0x28)||(Status == 0x30) )
        {
          /*--- Data transmitted and ACK received ---*/
          Flag = FALSE;
        }
        else if( Status != 0xf8 )
        {
          /*--- ERROR ---*/
          Flag = FALSE;
          ii = Length;
          Info|=2;
          ReturnCode = ERROR;
        }
    }

    /*--- Generate Stop condition ---*/
    i2c_stop(i2c_num);
  
    return ReturnCode;
} 

S8 i2c_master_read(U8 i2c_num, U8 *pRxData, U16 Length ) 
{
  S8 ReturnCode;
  U8 Status, Flag, ii;

   /*--- Generate Start condition ---*/
   ReturnCode = i2c_start(i2c_num);
   /*--- Transmit address ---*/
  if( ReturnCode == OK )
    {
      /*--- Write SLA+R ---*/
      ReturnCode = i2c_master_putchar(i2c_num, *pRxData );
      while( ReturnCode == BUSY )
       {
          ReturnCode = i2c_master_putchar(i2c_num, *pRxData );
       }
      pRxData++;
    }
  if( ReturnCode == OK )
  {
     /*--- wait until address transmitted and receive data ---*/
     for( ii = 1; ii<=Length; ii++ )
     {
      /*--- wait until data transmitted ---*/
      Flag = TRUE;
      while( Flag )
      {
        /*--- Get new status ---*/
        Status = i2c_check_status(i2c_num);
        
        if(( Status == 0x40 ) || ( Status == 0x48 ) || ( Status == 0x50 )||( Status == 0x58 ))
        {
          /*--- Data received ---*/
          Flag = FALSE;

          if(Status == 0x48)
            {
              ReturnCode = ERROR;
              break;
            }

          if( ii == Length )
          {
            /*--- Set generate NACK ---*/
            ReturnCode = i2c_master_getchar(i2c_num, I2C_ACK1, pRxData );
          }
          else
          {
            ReturnCode = i2c_master_getchar(i2c_num, I2C_ACK0, pRxData );
          }
          /*--- Read data ---*/
          ReturnCode = i2c_master_getchar(i2c_num, I2C_READ, pRxData );
          while( ReturnCode == EMPTY )
          {
            ReturnCode = i2c_master_getchar(i2c_num, I2C_READ, pRxData );
          }
          pRxData++;
        }
        else if( Status != 0xf8 )
        {
          /*--- ERROR ---*/
          Flag = FALSE;
          ii = Length;
          ReturnCode = ERROR;
        }
      }
    }
  }

  /*--- Generate Stop condition ---*/
  i2c_stop(i2c_num);
  
  return ReturnCode;    
} 

U8 i2c_check_status(U8 i2c_num) 
/*
**---------------------------------------------------------------------------
**  Returns:
**      00h Bus error
**      08h START condition transmitted
**      10h Repeated START condition transmitted
**      18h SLA + W transmitted, ACK received
**      20h SLA + W transmitted, ACK not received
**      28h Data byte transmitted, ACK received
**      30h Data byte transmitted, ACK not received
**      38h Arbitration lost
**      40h SLA + R transmitted, ACK received
**      48h SLA + R transmitted, ACK not received
**      50h Data byte received in master mode, ACK transmitted
**      58h Data byte received in master mode, ACK not transmitted
**      60h SLA + W received, ACK transmitted
**      68h Arbitration lost, SLA + W received, ACK transmitted
**      70h General call address received, ACK transmitted
**      78h Arbitration lost, general call addr received, ACK transmitted
**      80h Data byte received with own SLA, ACK transmitted
**      88h Data byte received with own SLA, ACK not transmitted
**      90h Data byte received after general call, ACK transmitted
**      98h Data byte received after general call, ACK not transmitted
**      A0h STOP or repeated START condition received in slave mode
**      A8h SLA + R received, ACK transmitted
**      B0h Arbitration lost, SLA + R received, ACK transmitted
**      B8h Data byte transmitted in slave mode, ACK received
**      C0h Data byte transmitted in slave mode, ACK not received
**      C8h Last byte transmitted in slave mode, ACK received
**      F8h No relevant status information, SI=0
**      FFh Channel error
**
**---------------------------------------------------------------------------
*/
{

  U8 Status;

  if(i2c_num == I2C_1)
  	{
  while(!(I2C1CONSET&0x8))
  {
    ;
  }
  
  /*--- Read I2C State ---*/
  Status = I2C1STAT;
  	}
  else if(i2c_num == I2C_0)
  	{
	  while(!(I2C0CONSET&0x8))
	  {
		;
	  }
	  
	  /*--- Read I2C State ---*/
	  Status = I2C0STAT;

  }
  return Status;
} 


S8 i2c_write_poll(U8 i2c_num) 
{
    S8 ReturnCode;
    U8 Status,BurnEnd;
    BurnEnd=FALSE;
      while (BurnEnd==FALSE)
	{
    	/*--- Generate Start condition ---*/
          ReturnCode = i2c_start(i2c_num);
    	/*--- Transmit SLA+W ---*/
    	if( ReturnCode == OK )
          {
            /*--- Write SLA+W ---*/
            ReturnCode = i2c_master_putchar(i2c_num, I2C_SLAVEADR_SEND );
            while( ReturnCode == BUSY )
              {
            	ReturnCode = i2c_master_putchar(i2c_num, I2C_SLAVEADR_SEND );
              }
          }
    	if( ReturnCode == OK )
          {
            /*--- Wait until SLA+W transmitted ---*/
            /*--- Get new status ---*/
	    Status = i2c_check_status(i2c_num);
            if (Status == 0x18)
              {
            	/*--- Data transmitted and ACK received ---*/
		BurnEnd=TRUE;
              }
            else if( Status == 0x20 )
              {
           	/*--- Data transmitted and ACK not received ---*/
		// send start bit, start again
                BurnEnd=FALSE;
              }
              else if( Status != 0xf8 )
              {
               /*--- ERROR ---*/
               BurnEnd=TRUE;
               ReturnCode = ERROR;
              }
          }
        /*--- Generate Stop condition ---*/
        i2c_stop(i2c_num);
        } // end of while (BurnEnd==FALSE)

      return ReturnCode;
    
} 

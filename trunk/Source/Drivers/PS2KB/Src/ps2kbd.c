#include "ps2kbd.h"
#include "lpc_vic.h"
#include "scancodes.h"
#include "lpc_2148.h"

/**
*****************************************************************************
	Function Name : extISR1()

	Description   :

	Input         :  

	Output        : 

	Note          :
******************************************************************************
*/
void extISR0(void);

U8	keyBuf[KBUFFSIZE];	 //PS2 Keyboard buffer, the register to store characters key in
U8	keyIn;			     //Index into PS/2 key buf where next scan code will be inserted
U8	keyOut;			     //Index into PS/2 key buf where next scan code will be removed
U8	keyRead;		     //Number of keys read from the PS/2 keyboard
U8 bitcount;             // 0 = neg.  1 = pos.

/**
*****************************************************************************
	Function Name : init_keyboard()

	Description   :

	Input         :  

	Output        : 

	Note          :
******************************************************************************
*/
U8 init_keyboard(void)
{
  FIO0CLR = KB_CLK;             //hold keyboard clock line untill ready
  EXTMODE |= EINT0_EDGE;	       /* INT0 edge trigger */
  EXTPOLAR |= 0x0;			   /* INT0 is falling edge by default */

  PINSEL1 |= KBD_PINSEL;
  
  /* initialize the interrupt vector */
  VICIntSelect &= ~ VIC_BIT(VIC_EINT0);      // EINT0 selected as IRQ
  VICVectAddr2 = (unsigned int)extISR0;      // address of the ISR
  VICVectCntl2 = VIC_ENABLE | VIC_EINT0;
  VICIntEnable = VIC_BIT(VIC_EINT0);         // EINT0 interrupt enabled
 
  bitcount = 0;
  keyIn =0;
  keyOut=0;
  keyRead=0;

  EXTINT = EINT0;	
  /* clear interrupt */

  return(TRUE);
  
}

/**
*****************************************************************************
	Function Name : decode()

	Description   :

	Input         :  

	Output        : 

	Note          :
******************************************************************************
*/
void decode(U8 sc)
{
  static unsigned char is_up=0, shift = 0;    //, mode = 0;
  unsigned char i;

  if (!is_up)                                 /* Last data received was the up-key identifier */
    {
      switch (sc)
        {
        case BREAKCHAR :                      /*  The up-key identifier */
          is_up = 1;
          break;
        case NONUMCODE :
          //is_up = 1;
          break;
        case LEFTSHIFT :
          shift = 1;
          break;
        case RIGHTSHIFT:
          shift = 1;
          break;
  //      case 0x58: //caps lock
  //          break;
        default:
          if (!shift)                          /* If shift not pressed */
            {
              // do a table look-up
              for (i = 0; unshifted[i][0]!=sc && unshifted[i][0]; i++);
              if (unshifted[i][0] == sc)
                {
                  putkeybuf((unshifted[i][1]));
                }
            }
          else
            {
              /* look-up */
              for (i = 0; shifted[i][0]!=sc && shifted[i][0]; i++);
              if (shifted[i][0] == sc)
                {
                  putkeybuf((shifted[i][1]));
                }
            }
          break;
        }
    }
  else
    {
      is_up = 0;
      switch (sc)
        {
        case LEFTSHIFT :
        case RIGHTSHIFT:
          shift = 0;
          break;
        }
    }
}

/**
*****************************************************************************
	Function Name : putkeybuf()

	Description   :

	Input         :  

	Output        : 

	Note          :
******************************************************************************
*/
void putkeybuf (U8 key)
{
  if (keyRead <KBUFFSIZE)  		    //make sure that we don't overflow the buffer
    {
      keyRead++;					    //Increment the number of keys read
      keyBuf[keyIn++] = key;		    //Store the scan code into the buffer
      if (keyIn >= KBUFFSIZE)  	        //Adjust index to the next scan code to put in buffer
        {
          keyIn = 0;
        }
    }
}

/**
*****************************************************************************
	Function Name : keyhit()

	Description   :

	Input         :  

	Output        : 

	Note          :
******************************************************************************
*/
U8 keyhit(void)
{
  unsigned char hit;
  hit = (unsigned char)(keyRead > 0)? TRUE : FALSE;
  return(hit);
}

/**
*****************************************************************************
	Function Name : xgetkey()

	Description   :

	Input         :  

	Output        : 

	Note          :
******************************************************************************
*/
U8 xgetkey(void)
{
  unsigned char key=0;
  if (keyRead > 0)
    {
      keyRead--;			            /* Decrement the number of keys in the buffer */
      key = keyBuf[keyOut];         	/* Get scan code from the buffer */
      keyOut++;
      if (keyOut >= KBUFFSIZE)
        {
          keyOut = 0;
        }

    }
	// else 
	 //{
      //return (0xFF);					     /* No scan codes in buffer, return -1 */
     //}
  return (key);						   /* return the scan code of the key pressed */
}


#include "tn.h"
#include "type.h"

#define QUEUE_MSG_SIZE 64

TN_DQUE  queueMsg;

void	* queue_msg_mem[QUEUE_MSG_SIZE];

/**
*****************************************************************
	Function Name :	message_queue_init()

	Description :	

	Input :	Void pointer

	Output :	Void

	Note :
*****************************************************************
*/
U8 message_queue_init()
{
  S32 rc = 0;
  
  rc = tn_queue_create(&queueMsg,queue_msg_mem,QUEUE_MSG_SIZE);

  if(rc != TERR_NO_ERR)
    return FALSE;
  else
    return TRUE;
}

/**
*****************************************************************
	Function Name :	post_imessage_queue()

	Description :	

	Input :	Void pointer

	Output :	Void

	Note :
*****************************************************************
*/
U8 post_imessage_queue(U8 *msg)
{
  S32 rc = 0;
  
  rc = tn_queue_isend_polling(&queueMsg,msg);
  
  if(rc != TERR_NO_ERR)
    return FALSE;
  else
    return TRUE;		
}

/**
*****************************************************************
	Function Name :	get_message_queue()

	Description :	

	Input :	Void pointer

	Output :	Void

	Note :
*****************************************************************
*/
U8 get_message_queue(U8 **ppmsg)
{
  S32 rc = 0;
	
  rc = tn_queue_receive_polling(&queueMsg,(void *)ppmsg);

  if(rc != TERR_NO_ERR)
    return FALSE;
  else
    return TRUE;	
}

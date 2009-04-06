#include "SoftTimer.h"
#include "type.h"
#include "hardware.h"
#include  "tn.h"
/**
*******************************************************************************
  Function Name : dereg_uclk()

  Description :	
   
  Input :	

  Output :	none

  Note : 
*******************************************************************************
*/	

void dereg_uclk(struct uclk *luclk) 
{
	TN_INTSAVE_DATA
	tn_disable_interrupt();
	if(luclk->next!=NULL)
	{
          if ( luclk->next != tail )
          {
            luclk->next->current_ticks+=luclk->current_ticks;
            luclk->next->prev=luclk->prev;
          }
	}
	if(luclk->prev!=NULL)
	{
          luclk->prev->next=luclk->next;
	}
	luclk->prev=NULL;
	luclk->next=NULL;
	tn_enable_interrupt();
}


/**
*******************************************************************************
  Function Name : reg_uclk()

  Description :	
   
  Input :	

  Output :	none

  Note : 
*******************************************************************************
*/	

void reg_uclk(struct uclk *luclk) 
{
    TN_INTSAVE_DATA
	struct uclk *temp_head,*node;
	int temp_ticks=0;

	tn_disable_interrupt();
	
        temp_head=&clk_head;
	    node=temp_head;
        dereg_uclk(luclk);
        luclk->current_ticks=luclk->ticks;
        luclk->expired = 0;
	
	do
	{
		temp_ticks=temp_ticks+temp_head->current_ticks;
		if(temp_ticks > luclk->current_ticks)
		{
			luclk->current_ticks=luclk->current_ticks-(temp_ticks-temp_head->current_ticks);
			luclk->next=temp_head;
			luclk->prev=temp_head->prev;
			temp_head->current_ticks=temp_head->current_ticks-luclk->current_ticks;
			temp_head->prev->next=luclk;
			temp_head->prev=luclk;
            tn_enable_interrupt();
			return;
		}
		node=temp_head;
		temp_head=temp_head->next;
	}
	while(temp_head!=tail);

	luclk->next=tail;
        tail->prev=luclk;
	luclk->prev=node;
        node->next=luclk;
	luclk->current_ticks=luclk->current_ticks-temp_ticks;

	tn_enable_interrupt();
}


/**
*******************************************************************************
  Function Name : def_uclk()

  Description :	
   
  Input :	

  Output :	none

  Note : 
*******************************************************************************
*/	

void def_uclk(struct uclk *luclk) 
{
    TN_INTSAVE_DATA
	tn_disable_interrupt();
	luclk->next=NULL;
	luclk->prev=NULL;
	luclk->ticks=0;
	luclk->current_ticks=0;
	luclk->repeat=0;
	luclk->function_callback = NULL;
	tn_enable_interrupt();
}

/**
*******************************************************************************
  Function Name : check_timeout()

  Description :	
   
  Input :	

  Output :	

  Note : 
*******************************************************************************
*/	

U32 check_timeout (struct uclk *temp_head)
{
	return !(temp_head->current_ticks&&1);
	
}

/**
*******************************************************************************
  Function Name : uclk_handler()

  Description :	
   
  Input :	none

  Output :	none

  Note : 
*******************************************************************************
*/	

void uclk_handler ( )
{
	struct uclk *temp_head,*node;
      
        temp_head=&clk_head;
	node = temp_head;
	temp_head = temp_head->next;

	if ( temp_head == tail )
	{
             return;
	}

        if ( temp_head->current_ticks > 0 )
	{
		temp_head->current_ticks--;
	}

	if ( temp_head->current_ticks == 0 )
	{
		while (  temp_head->current_ticks == 0 )
		{
			//temp_head->function_callback = function_node_2;
			if(temp_head->function_callback)
			{
			  (*temp_head->function_callback)( );
			}
			node = temp_head;
			temp_head = temp_head->next;
			dereg_uclk ( node );
                        node->expired = 1;
			if ( node->repeat == 1)
			{
				node->current_ticks = node->ticks;
                                reg_uclk( node );
			}
                        if ( temp_head == tail )
			{
                            temp_head=temp_head;
				break;
			}
		}
	}
}

/**
*******************************************************************************
  Function Name : init_software_timer()

  Description :	
   
  Input :	  none

  Output :        none

  Note : 
*******************************************************************************
*/	

void init_software_timer (void) 
{
        head=&clk_head;
	tail=&clk_tail;

	def_uclk(&clk_head);
        if ( clk_head.next == NULL )
	{
		clk_head.next=&clk_tail;
		clk_tail.next=NULL;
		clk_tail.prev=&clk_head;
		clk_tail.ticks=-1;
		clk_tail.current_ticks=-1;
		clk_tail.repeat=-1;
                clk_tail.expired=-1;
	}
        
}



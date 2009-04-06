#ifndef  _TN_MALLOC_H_
#define  _TN_MALLOC_H_
#include "type.h"
#include "tn.h"

#define TNGUI_MALLOC_HEAP_SIZE  2048+1024


typedef struct _TN_MFLENTRY
{
   unsigned int nbytes;          //-- number of bytes in block
   struct _TN_MFLENTRY * next;   //-- next entry in free list
}TN_MFLENTRY;

typedef struct _TN_MALLOCDATA
{
   TN_MFLENTRY * first_entry;    //-- first entry in free list
   unsigned char * mem_start;
   int  mem_size;
}TN_MALLOCDATA;

//-- Prototypes

void   tn_malloc_init(TN_MALLOCDATA * md,unsigned char * start,int size);
void * tn_malloc(int size);
void   tn_free(void * ptr_to_ret);
void * tn_realloc(void * mem_block, int new_size);
void * tn_calloc(int num_el, int size_of_el);


#endif






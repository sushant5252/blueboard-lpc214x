// Yuri Tiomkin (c) 2006
//
// (c) Copyright 1990 Conor P. Cahill (uunet!virtech!cpcahil).
// You may copy, distribute, and use this software as long as this
// copyright statement is not removed.
//
//-----------------------------------------------------------------------------


#include "malloc.h"
#include "type.h"
#include "utils.h"

//--- Global malloc data
TN_MALLOCDATA tn_malloc_data;


//----------------------------------------------------------------------------
// There are no malloc locking in this project
#define  DECLARE_STATUS_REG int tn_save_status_reg = 0;
#define  malloc_lock() (tn_save_status_reg = tn_cpu_save_sr())
#define  malloc_unlock() tn_cpu_restore_sr(tn_save_status_reg)

//----------------------------------------------------------------------------

#define  TN_MBLOCK_MIN_SIZE       8
#define  TN_MASK_MIN_MBLOCK     0x7
#define  TN_MASK_BLOCK_IN_USE   0x1


//----------------------------------------------------------------------------
// Free block list routines
//----------------------------------------------------------------------------
static void mfl_insert(TN_MALLOCDATA * md, TN_MFLENTRY * ptr)
{
   TN_MFLENTRY * curr;
   TN_MFLENTRY * prev;

   if(ptr == NULL || md == NULL)
      return;

   curr = md->first_entry;
   prev = NULL;

   if(md->first_entry == NULL)  //-- There were no free blocks
   {
      md->first_entry = ptr;
      md->first_entry->next = NULL;      //-- Now our block - single(and last) free block
      return;
   }

  //-- find a biggest block - free fist is sorted by size
   for(;;)
   {
      if(curr == NULL)
         break;
          else if(curr->nbytes >= ptr->nbytes)
                 break;
      prev = curr;
      curr = curr->next;
   }

   if(prev == NULL)                     //-- our ptr will be first
   {
      ptr->next  = md->first_entry;
      md->first_entry = ptr;
   }
   else if(curr == NULL)                //-- our ptr will be last
   {
      prev->next = ptr;
      ptr->next  = NULL;
   }
   else                                 //-- insert
   {
      ptr->next  = curr;
      prev->next = ptr;
   }
}

//----------------------------------------------------------------------------
static void mfl_remove(TN_MALLOCDATA * md, TN_MFLENTRY * ptr)
{
   TN_MFLENTRY * curr;
   TN_MFLENTRY * prev;

   if(ptr == NULL || md == NULL)
      return;

   curr = md->first_entry;
   prev = NULL;

   //-- find in free list
   for(;;)
   {
      if(curr == NULL || curr == ptr)
         break;
      prev = curr;
      curr = curr->next;
   }

   if(curr == NULL)                //-- not found
      md->first_entry = NULL;      //-- Now there are no free blocks
   else if(prev == NULL)           //-- Our ptr was first
      md->first_entry = ptr->next;  // first
   else
      prev->next = ptr->next;
}


//----------------------------------------------------------------------------
// split/join blocks  routines

//----------------------------------------------------------------------------
static void malloc_split(TN_MALLOCDATA * md,TN_MFLENTRY * ptr,int size_to_alloc)
{
   int prev_size;
   int rem_size;
   TN_MFLENTRY * next;

   //----- Processing
   prev_size = ptr->nbytes;
   mfl_remove(md,ptr);
   //-- if block is larger -> return remainder into free list
   rem_size = prev_size - size_to_alloc;
   if(rem_size >= TN_MBLOCK_MIN_SIZE + sizeof(TN_MFLENTRY))
   {
      next = (TN_MFLENTRY *)((char *)ptr + sizeof(TN_MFLENTRY) + size_to_alloc);
      next->nbytes = rem_size - sizeof(TN_MFLENTRY);
      mfl_insert(md,next);
      ptr->nbytes = size_to_alloc;
   }
}

//----------------------------------------------------------------------------
static void malloc_join(TN_MALLOCDATA * md, TN_MFLENTRY * prev,
                                                 TN_MFLENTRY * entry_to_free)
{
   unsigned char * ptr;
   TN_MFLENTRY * next;

   // get next block
   ptr  = (unsigned char *)entry_to_free;
   ptr += sizeof(TN_MFLENTRY) + entry_to_free->nbytes;
   next = (TN_MFLENTRY *)ptr;

   //--- Check next for out of heap
   ptr =  md->mem_start + md->mem_size;
   ptr -= sizeof(TN_MFLENTRY);
   if(next > (TN_MFLENTRY *)ptr)
      next = NULL;
   if(next != NULL && (next->nbytes & TN_MASK_BLOCK_IN_USE)) //-- Block not free
      next = NULL;

   //-- Check prev
   if(prev != NULL && (prev->nbytes & TN_MASK_BLOCK_IN_USE)) //-- Block not free
      prev = NULL;

    //-- join previous & current & next
   if(prev != NULL && next != NULL)
   {
      mfl_remove(md,prev);
      mfl_remove(md,next);
      prev->nbytes += entry_to_free->nbytes + next->nbytes +
                                                      (sizeof(TN_MFLENTRY)<<1);
      mfl_insert(md,prev);
      return;
   }
    //-- join previous & last
   if(prev != NULL)
   {
      mfl_remove(md,prev);
      prev->nbytes += entry_to_free->nbytes + sizeof(TN_MFLENTRY);
      mfl_insert(md,prev);
      return;
   }
    //-- join current & next
   if(next != NULL )
   {
      mfl_remove(md,next);
      entry_to_free->nbytes += next->nbytes + sizeof(TN_MFLENTRY);
      mfl_insert(md,entry_to_free);
      return;
   }
    // Nothing to join
   mfl_insert(md,entry_to_free);
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void tn_malloc_init(TN_MALLOCDATA * md,unsigned char * start,int size)
{
   unsigned int addr_aligment;
   DECLARE_STATUS_REG

   if(md == NULL)
      return;
   if(start == NULL || size < ((TN_MBLOCK_MIN_SIZE + sizeof(TN_MFLENTRY))<<3))
   {
      md->mem_start = NULL;
      return;
   }

   malloc_lock();

   //--- Make aligment to 8
   addr_aligment = (unsigned int)start;
   addr_aligment = (addr_aligment + TN_MASK_MIN_MBLOCK) & (~TN_MASK_MIN_MBLOCK);
   md->mem_start = (unsigned char *)addr_aligment;  //(start + TN_MASK_MIN_MBLOCK) & (~TN_MASK_MIN_MBLOCK);
   //---

   md->mem_size  = size;
   md->first_entry = (TN_MFLENTRY *) md->mem_start;
   md->first_entry->nbytes = md->mem_size - sizeof(TN_MFLENTRY);
   md->first_entry->next = NULL;

   malloc_unlock();
}

//----------------------------------------------------------------------------
void * tn_malloc(int size)
{
   TN_MFLENTRY * curr;
   char * ret_ptr;
   DECLARE_STATUS_REG

  //--- Uses global var tn_malloc_data
   if(size <= 0 || tn_malloc_data.mem_start == NULL)
      return NULL;

   malloc_lock();

   curr = tn_malloc_data.first_entry;
   size = (size + TN_MASK_MIN_MBLOCK) & (~TN_MASK_MIN_MBLOCK);

//   first fit strategy
   for(;;)
   {
      if(curr == NULL || curr->nbytes >= (unsigned int)size)
         break;
      curr = curr->next;
   }

   if(curr == NULL) //-- no blocks with requested size
   {
      malloc_unlock();
      return NULL;
   }
   malloc_split(&tn_malloc_data,curr,size);

   curr->nbytes |= TN_MASK_BLOCK_IN_USE;  //-- Set block as in use

   malloc_unlock();
   ret_ptr = (char *)curr;
   ret_ptr += sizeof(TN_MFLENTRY);

   return ret_ptr;
}

//----------------------------------------------------------------------------
void tn_free(void * ptr_to_ret)
{
   char * ptr;
   TN_MFLENTRY * prev;
   TN_MFLENTRY * curr;
//   TN_MFLENTRY * next;
   DECLARE_STATUS_REG

  //--- Uses global var tn_malloc_data
   if(ptr_to_ret == NULL || tn_malloc_data.mem_start == NULL)
      return;

   ptr  = (char *) ptr_to_ret;
   ptr -= sizeof(TN_MFLENTRY);     //-- Go to real start of memory block

   prev = NULL;
 //  next = NULL;

   malloc_lock();

   curr = (TN_MFLENTRY *)tn_malloc_data.mem_start;
   //--- search block through all heap  from start to ptr_to_ret addr
   for(;;)
   {
      if(curr >= (TN_MFLENTRY *) ptr)
         break;
      prev = curr;
      curr = (TN_MFLENTRY *)((char *)curr + sizeof(TN_MFLENTRY) +
                                 (curr->nbytes & (~TN_MASK_BLOCK_IN_USE)) );
   }
   //-- Avoid to free block already free or out of range
   if((curr != (TN_MFLENTRY *) ptr) || (!(curr->nbytes & TN_MASK_BLOCK_IN_USE)))
   {
      malloc_unlock();
      return;
   }
   curr->nbytes &= (~TN_MASK_BLOCK_IN_USE);   // Set block free

   malloc_join(&tn_malloc_data,prev,curr);

   malloc_unlock();
}
//----------------------------------------------------------------------------
void * tn_realloc(void * mem_block, int new_size)
{

   unsigned char * ptr;
   TN_MFLENTRY * curr;
   TN_MFLENTRY * next;
   int curr_size;
   int delta = 0;
   DECLARE_STATUS_REG

   //-- Get current block size
   ptr  = (unsigned char *)mem_block;
   ptr -= sizeof(TN_MFLENTRY);
   curr = (TN_MFLENTRY *)ptr;
   if((curr->nbytes & TN_MASK_BLOCK_IN_USE) != TN_MASK_BLOCK_IN_USE)
      return NULL;

   curr_size = curr->nbytes & (~TN_MASK_BLOCK_IN_USE);
   //--
   new_size = (new_size + TN_MASK_MIN_MBLOCK) & (~TN_MASK_MIN_MBLOCK);

   if(new_size < curr_size)
   {
      if(curr_size - new_size < (TN_MBLOCK_MIN_SIZE + sizeof(TN_MFLENTRY)))
         return  mem_block;

      curr->nbytes = new_size | TN_MASK_BLOCK_IN_USE;

      //--- Make free block from remainder
      ptr += new_size + sizeof(TN_MFLENTRY);
      curr = (TN_MFLENTRY *)ptr;
      curr->nbytes = (curr_size - new_size - sizeof(TN_MFLENTRY)) |
                                                        TN_MASK_BLOCK_IN_USE;
      ptr += sizeof(TN_MFLENTRY);
      tn_free(ptr);
      //--

      return mem_block;
   }
   else // >=
   {
     // check next memory block in list, see if memory block can be expanded
      ptr += curr_size + sizeof(TN_MFLENTRY);
      next = (TN_MFLENTRY *)ptr;

      malloc_lock();

      //--- Check next for out of heap
      ptr =  tn_malloc_data.mem_start + tn_malloc_data.mem_size;
      ptr -= sizeof(TN_MFLENTRY);
      if(next > (TN_MFLENTRY *)ptr)
         next = NULL;
      else
      {
         //-- Check next is free
         if((next->nbytes & TN_MASK_BLOCK_IN_USE) == TN_MASK_BLOCK_IN_USE)
            next = NULL;
         else //-- Check free space to fit
         {
            delta = curr_size + next->nbytes + sizeof(TN_MFLENTRY) - new_size;
            if(delta < 0)
               next = NULL;
         }
      }
      if(next != NULL) //-- Fit in next free block
      {
         mfl_remove(&tn_malloc_data,next);
         //-- Remainder is too small to split
         if(delta < TN_MBLOCK_MIN_SIZE + sizeof(TN_MFLENTRY))
         {
            curr->nbytes = (new_size + delta) | TN_MASK_BLOCK_IN_USE;
         }
         else
         {
            curr->nbytes = new_size | TN_MASK_BLOCK_IN_USE;
            ptr =(unsigned char*)curr;
            ptr += new_size + sizeof(TN_MFLENTRY);
            next = (TN_MFLENTRY *)ptr;
            next->nbytes = delta - sizeof(TN_MFLENTRY);
            mfl_insert(&tn_malloc_data,next);
         }

         malloc_unlock();
         return mem_block;
      //-----------------------------------------------------
      }
      else //-- Alloc new & copy
      {
         ptr = (unsigned char*) tn_malloc(new_size);
         if(ptr == NULL)
            return NULL;
         s_memcpy(ptr, mem_block, curr_size);
         tn_free(mem_block);

         malloc_unlock();
         return ptr;
      }
   }
}

//----------------------------------------------------------------------------
void * tn_calloc(int num_el, int size_of_el)
{
   int i;
   void * ret_ptr;
   unsigned int * ptr; //-- Optimized for 32 bit integer

   i = num_el * size_of_el;
   ret_ptr = tn_malloc(i);
   if(ret_ptr != NULL)     //-- Set to 0
   {
      ptr = (unsigned int *) ret_ptr;
      i = ((i + TN_MASK_BLOCK_IN_USE) & (~TN_MASK_BLOCK_IN_USE))>>2; //-- to int

      while(i--)
         *ptr++ = 0;
   }
   return ret_ptr;
}



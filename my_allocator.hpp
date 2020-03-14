/* 
    File: my_allocator.hpp

    Author: Joseph Renteria
            Department of Computer Science
            Texas A&M University
    Date  : 09-08-19

    Modified:

*/

#ifndef _my_allocator_hpp_                   // include file only once
#define _my_allocator_hpp_

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <cstdlib>
#include "free_list.hpp"
#include <vector>

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

typedef void * Addr; 
using namespace std;
/*--------------------------------------------------------------------------*/
/* CLASS   MyAllocator */
/*--------------------------------------------------------------------------*/

class MyAllocator {
   private:
      size_t available;
      size_t _basic_block_size;
      char* start;
      vector<FreeList*> fl;


  // YOU MAY NEED TO ADD YOUR OWN DATA STRUCTURES HERE.
  
   public:
      MyAllocator(size_t _basic_block_size, size_t _size); 
      /* This function initializes the memory allocator and makes a portion of 
         ’_size’ bytes available. The allocator uses a ’_basic_block_size’ as 
         its minimal unit of allocation. 
         NOTE: In the old days one used 'unsigned int' for the size. The C
         Standards Committee has added the 'size_t' data type to ensure to 
         represent memory sizes. This makes the code more portable. Depending
         on the architecture, 'size_t' may or may not be the same 
         as 'unsigned int'. 
      */ 

      ~MyAllocator(); 
      /* This function returns any allocated memory to the operating system. 
      */ 
      SegmentHeader* merge(SegmentHeader* block1, SegmentHeader* block2, size_t fibNum);
      Addr Malloc(size_t _length); 
      SegmentHeader* getBuddy(SegmentHeader* block);
      /* Allocate _length number of bytes of free memory and returns the 
         address of the allocated portion. Returns nullptr when out of memory. 
      */ 
      SegmentHeader* split(SegmentHeader* currSeg, int j, vector<FreeList*> fibNums);
      bool Free(Addr _a);
      int getIndex(size_t _size);
      /* Frees the section of physical memory previously allocated 
         using ’Malloc’. Returns true if everything ok. */ 
      void printFL();
};

#endif 

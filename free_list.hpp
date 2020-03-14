/* 
    File: free_list.hpp

    Author: Joseph Renteria
            Department of Computer Science
            Texas A&M University
    Date  : 09-08-19

    Modified:

*/

#ifndef _free_list_hpp_                   // include file only once
#define _free_list_hpp_

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <cstdlib>
#include <cassert>
#include <vector>

#include "my_allocator.hpp"

using namespace std;

/*--------------------------------------------------------------------------*/
/* CLASS  FreeList */
/*--------------------------------------------------------------------------*/


class SegmentHeader;

class FreeList {
  private:

  public:
    FreeList(size_t fibNum);
    SegmentHeader* head;
    size_t fibNum;
    ~FreeList(); 
    /* We probably don't need a destructor. */ 

    bool Remove(SegmentHeader * _segment); 

    bool Add(SegmentHeader * _segment);
};

class SegmentHeader {
  private:
    static const unsigned COOKIE_VALUE = 0xBAAB00;
    unsigned int cookie; /* To check whether this is a genuine header! */

  public:
    bool LRBuddy; // false means it is on the left, true means it is right
    bool Inheritance; // same as above
    bool is_free;
    SegmentHeader(size_t size, bool Inheritance = true, bool LRBuddy = false);
    //~SegmentHeader();
    SegmentHeader* next;
    SegmentHeader* prev;

    void CheckValid();
    size_t size;

    /* Check if the cookie is valid. */
};

#endif
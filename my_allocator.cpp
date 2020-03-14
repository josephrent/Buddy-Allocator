/* 
    File: my_allocator.cpp

    Author: Joseph Renteria
            Department of Computer Science
            Texas A&M University
    Date  : 09-08-19

    Modified: 

    This file contains the implementation of the class MyAllocator.

*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <cstdlib>
#include "my_allocator.hpp"
#include <assert.h>
#include <iostream>
#include "free_list.hpp"
#include <cmath>

/*--------------------------------------------------------------------------*/
/* NAME SPACES */ 
/*--------------------------------------------------------------------------*/

using namespace std;
/* I know, it's a bad habit, but this is a tiny program anyway... */

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FUNCTIONS FOR CLASS MyAllocator */
/*--------------------------------------------------------------------------*/

MyAllocator::MyAllocator(size_t _basic_block_size, size_t _size) { //DONE
  start = (char*)std::malloc(_size);
  SegmentHeader* nSeg = new(start)SegmentHeader(_size,true);
  l.Add(nSeg);
  this->available = _size;
  this->_basic_block_size = _basic_block_size;
}


MyAllocator::~MyAllocator() {
  std::free(this->start);
}

Addr MyAllocator::Malloc(size_t _length) {
  size_t blockSizeL = ((_length + _basic_block_size - 1) / _basic_block_size) * _basic_block_size;
  SegmentHeader* curr = this->l.head;
  //cout << "The size wanted by Malloc(): " << _length << endl;
  //cout << "The size rounded to because of basic block size: " << blockSizeL << endl;
  while((curr->getL() < blockSizeL) && (curr!=nullptr)) {//2 get segment
    curr = curr->next;
    //cout << "Iterating through fl... -> "" << curr << endl;
    if(curr==nullptr){
      return nullptr;
    }
  }
  // cout << "THIS IS Curr, it should equal null ptr for the last one" << curr << endl;
  this->l.Remove(curr);//3 fList remove(cSeg)
  SegmentHeader* nSeg = this->l.Split(curr, blockSizeL);//4 fList.Split
  this->l.Add(nSeg);//5 add 
  cout << "Malloc() executed" << endl;
  return (char*)curr + sizeof(SegmentHeader);
}
bool MyAllocator::Free(Addr _a) { //DONE
  // This empty implementation just uses C standard library free
  this->l.Add( (SegmentHeader*) ( (char*)_a-sizeof(SegmentHeader) ));
  cout << "Free() called" << endl;
  return true;
}
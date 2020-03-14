/* 
    File: free_list.cpp

    Author: Joseph Renteria
            Department of Computer Science
            Texas A&M University
    Date  : 09-08-19

    Modified: 

    This file contains the implementation of the class FreeList.

*/

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <iostream>
#include "my_allocator.hpp"

/*--------------------------------------------------------------------------*/
/* NAME SPACES */ 
/*--------------------------------------------------------------------------*/

using namespace std;
/* I know, it's a bad habit, but this is a tiny program anyway... */

/* FUNCTIONS FOR CLASS SegmentHeader */
/*--------------------------------------------------------------------------*/

SegmentHeader::SegmentHeader(size_t size, bool Inheritance, bool LRBuddy) {
  this->is_free = true;
  cookie = COOKIE_VALUE;
  this->next = nullptr;
  this->prev = nullptr;
  this->size = size;
  this->LRBuddy = LRBuddy;
  this->Inheritance = Inheritance;
}


/*SegmentHeader::~SegmentHeader() {
  // You may need to add code here.
}*/

void SegmentHeader::CheckValid() {
  if (cookie != COOKIE_VALUE) {
    cout << "INVALID SEGMENT HEADER!!" << endl;
    assert(false);
    // You will need to check with the debugger to see how we got into this
    // predicament.
  }
}

/*--------------------------------------------------------------------------*/
/* FUNCTIONS FOR CLASS FreeList */
/*--------------------------------------------------------------------------*/


FreeList::FreeList(size_t fibNum) {
  head = nullptr;
  this->fibNum = fibNum;
}

FreeList::~FreeList() {
		SegmentHeader* curr = head;
		while (curr->next) {
			SegmentHeader* temp = curr->next;
			delete curr;
			curr = temp;
		}
}

bool FreeList::Add(SegmentHeader * _segment) {//Adds a free block to free list. Places at head
  _segment->is_free = true;
  if (_segment->size != fibNum) cout << "PANIC " << endl;
  _segment->next = nullptr;
  _segment->prev = nullptr;
  if(head==nullptr) {
    head= _segment;
    return true;
  }
  if(_segment!=head) {
    head->prev = _segment;
    _segment->next = head;
    head = _segment;
  }
  return true;
}
bool FreeList::Remove(SegmentHeader * _segment) {
  //cout << "buddy is being removed somewhere here " << _segment->size << endl;
  //cout << "Seg pointer " << (size_t) _segment << endl;
  if(_segment==nullptr) {
    //cout << "this" << endl;
    return true;
  }
  
  _segment->is_free = false;
  if(_segment==head){//case when segment is the head
    head = head->next;
    if(head)
      head->prev = nullptr;
  }
  else {//case when segment is in the middle of two nodes
    if(_segment->next){
      _segment->next->prev = _segment->prev;
    }
    else {
      _segment->prev->next = _segment->next;
    }
  }
  _segment->next = nullptr;
  _segment->prev = nullptr;
  return true;
}
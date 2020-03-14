/* 
    File: my_allocator.cpp

    Author: Joseph Renteria
            Department of Computer Science
            Texas A&M University
    Date  : 09-08-19

    Modified: 

    This file contains the implementation of the class MyAllocator.

*/

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


/*--------------------------------------------------------------------------*/
/* FUNCTIONS FOR CLASS MyAllocator */
/*--------------------------------------------------------------------------*/

MyAllocator::MyAllocator(size_t _basic_block_size, size_t _size) { //DONE
  cout << "bbs: " << _basic_block_size << endl << "tot mem: " << _size << endl;
  start = (char*)std::malloc(_size);
  available = _size;
  vector<size_t> fibNumbers;
  int i=0;//i used as iterator and kept in scope in order to have index of final fib number
  fibNumbers.push_back(1);
  fibNumbers.push_back(2);
  for(i=2; (fibNumbers[i-2] + fibNumbers[i-1]) * _basic_block_size <= _size ; i++) {//gets fib numbers needed given desired size
    fibNumbers.push_back(fibNumbers[i-2] + fibNumbers[i-1]);
  }
  for(int j = 0; j < i; j++) {
    size_t temp = _basic_block_size * fibNumbers[j];
    fl.push_back(new FreeList(temp));
  }
  int j = fl.size()-1;
  cout << j << endl;
  SegmentHeader* nSeg = new(start)SegmentHeader(fl[j]->fibNum);
  fl.back()->Add(nSeg);

}

MyAllocator::~MyAllocator() {
  std::free(this->start);
}

SegmentHeader* MyAllocator::merge(SegmentHeader* block, SegmentHeader* buddy, size_t fibNum) {
	SegmentHeader* left = (block->size > buddy->size) ? buddy : block;
  left->LRBuddy = (block->size > buddy->size) ? buddy->Inheritance : block->Inheritance;//grab LR from smaller inh
  left->Inheritance = (block->size > buddy->size) ? block->Inheritance : buddy->Inheritance;//grab Inh from larger inh
	left->size = fibNum;
	buddy = nullptr;
  block = nullptr;
	return left;
}
int MyAllocator::getIndex(size_t _size) {
  int i;
  for(i=0;i<fl.size();i++) {
    if(fl[i]->fibNum >= _size) {
      break;
    }
  }
  if (i == fl.size()) return -1;
  return i;
}
SegmentHeader* MyAllocator::split(SegmentHeader* currSeg, int j, vector<FreeList*> fl) {
   // if j < 2 
  size_t smallerSize = fl[j-2]->fibNum;
  size_t largerSize = fl[j-1]->fibNum;
  SegmentHeader* nSeg = new((char*)currSeg + smallerSize)SegmentHeader(largerSize,currSeg->Inheritance,true);//final true is for isFree, false makes this the left buddy
  currSeg->size = smallerSize;
  currSeg->Inheritance = currSeg->LRBuddy;
  currSeg->LRBuddy = false;
  currSeg->is_free = true;
  nSeg->is_free = true;
  return nSeg;
}
Addr MyAllocator::Malloc(size_t _length) {
  size_t reqSize = _length + sizeof(SegmentHeader);
	int i = getIndex(reqSize);//index for desired block
  if(fl[i]->head) {
    SegmentHeader* temp = fl[i]->head;
    fl[i]->Remove(fl[i]->head);
    return (char*)temp + sizeof(SegmentHeader);
  }
  else {
    int j = i;
    while (j <= fl.size() - 1 && fl[j]->head == nullptr) ++j;//j has the index of the largest existing block above the desired block
    if (j > fl.size() - 1) return nullptr;//out of bounds of fl
    SegmentHeader* block;
    while(j!=i) {
      if(j==1) {//we do not want to ever split fib 2 into 1 and 1
        ++j;
        while (j <= fl.size() - 1 && fl[j]->head == nullptr) ++j;
      }
      block = fl[j]->head;
      fl[j]->Remove(block);
      SegmentHeader* buddy = split(block,j,fl);
      fl[j - 2]->Add(block);
      fl[j - 1]->Add(buddy);
      if(fl[j-1]->fibNum==fl[i]->fibNum)
        j-=1;
      else
        j -= 2;
    }
      SegmentHeader* temp = fl[j]->head;
      fl[j]->Remove(fl[j]->head);
      return (char*)temp + sizeof(SegmentHeader);
  }
}

SegmentHeader* MyAllocator::getBuddy(SegmentHeader* block) {
  int i = getIndex(block->size);
  if(block->LRBuddy) {//case where block is right buddy, meaning we need to get the left one 
    SegmentHeader* buddy = (SegmentHeader*)((char*)block - fl[i-1]->fibNum);
    return buddy;
  }
  else if(!block->LRBuddy){//case where block is left buddy
    SegmentHeader* buddy = (SegmentHeader*)((char*)block + fl[i]->fibNum );
    return buddy;
  }
  return NULL;
}
bool MyAllocator::Free(Addr _a) {
  SegmentHeader* block = (SegmentHeader*)((char*)_a - sizeof(SegmentHeader));
  size_t fibNum;
  int i = getIndex(block->size);
  while(block->size < fl.back()->fibNum) {
    int j = getIndex(block->size);
    SegmentHeader* buddy = getBuddy(block);
    // check buddy length, is free and block is not of largest size 
    if(buddy->is_free) {//means it is a left buddy
      if(block->LRBuddy && fl[j-1]->fibNum==buddy->size) {//block is right buddy, buddy is left and must be removed at i - 1
        fl[j-1]->Remove(buddy); 
        j = j + 1;
        fibNum = fl[j]->fibNum;
      }
      else if((!block->LRBuddy)&&(fl[j+1]->fibNum==buddy->size)){//block is left buddy, buddy is right and must be removed at i + 1
        fl[j+1]->Remove(buddy);
        j = j + 2;
        fibNum = fl[j]->fibNum;
      } 
      else {
        fl[j]->Add(block);
        break;
      }
      SegmentHeader* nBlock = merge(block,buddy,fibNum);
      block = nBlock;
    }
    else {
      fl[j]->Add(block);
      break;
      // add block to appropriate free list and break
    }
  }
	return true;
}

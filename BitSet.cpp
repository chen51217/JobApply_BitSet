#include "BitSet.h"
#include <stdlib.h>

#define CONTENT_SIZE 32   // how much bit in a content
#define LOCKER_SIZE 32  // how much bit controlled by this lock 

#define NUMS_OF_CONTENT(NumsOfBit) ((NumsOfBit+CONTENT_SIZE-1)/CONTENT_SIZE)
#define NUMS_OF_LOCK(NumbOfBit)    ((NUMS_OF_CONTENT(NumsOfBit)+LOCKER_SIZE-1)/LOCKER_SIZE)
#define INDICATER 0x00000001U

// locker //
#define GET_LOCKER_IDX(pos)     (pos/(CONTENT_SIZE*LOCKER_SIZE))
#define GET_IDX_IN_LOCKER(pos)  (pos/(LOCKER_SIZE))

// content set //
#define GET_CONTENT_IDX(pos)    (pos/CONTENT_SIZE)
#define GET_IDX_IN_CONTENT(pos) (pos%CONTENT_SIZE)

// basic bit operator //
#define GET_BIT(this, pos) ((*(this->data.pContent+GET_CONTENT_IDX(pos)) & INDICATER<<(GET_IDX_IN_CONTENT(pos)))?true:false)
#define SET_BIT(this, pos) (*(this->data.pContent+GET_CONTENT_IDX(pos)) |= INDICATER<<(GET_IDX_IN_CONTENT(pos)))
#define CLR_BIT(this, pos) (*(this->data.pContent+GET_CONTENT_IDX(pos)) &= ~(INDICATER<<(GET_IDX_IN_CONTENT(pos))))


// avoid function call, which risk might be higher.
// consider std::atomic 
#define CHECK_LOCK(this,pos)        (*(this->data.pLock+GET_LOCKER_IDX(pos)) & INDICATER<<(GET_IDX_IN_LOCKER(pos)))
#define LOCK(this, pos)             (*(this->data.pLock+GET_LOCKER_IDX(pos)) |= INDICATER<<(GET_IDX_IN_LOCKER(pos)))
#define CHECK_AND_LOCK(this, pos)   (CHECK_LOCK(this,pos)?-1:LOCK(this,pos))
#define UNLOCK(this, pos)           (*(this->data.pLock+GET_LOCKER_IDX(pos)) &= ~(INDICATER<<(GET_IDX_IN_LOCKER(pos))))


/////// public ///////
BitSet::BitSet(int NumsOfBit)
{
    this->NumsOfBit = NumsOfBit;
    this->data.pContent = (unsigned long*)calloc(NUMS_OF_CONTENT(NumsOfBit), sizeof(unsigned long));
    this->data.pLock = (unsigned long*)calloc(NUMS_OF_LOCK(NumsOfBit), sizeof(unsigned long));
}

BitSet::~BitSet()
{
    free(this->data.pContent);
    free(this->data.pLock);
}

int BitSet::setBit(int pos)
{
    if(pos > this->NumsOfBit) return -1;

    if( CHECK_AND_LOCK(this, pos) == -1)
    {
        // locked, set fail
        return -1;
    }
    else
    {
        // free to set
        SET_BIT(this,pos);
        UNLOCK(this, pos);
        return 0;
    }
}

int BitSet::clearBit(int pos)
{
    if(pos > this->NumsOfBit) return -1;

    if( CHECK_AND_LOCK(this, pos) == -1)
    {
        // locked, set fail
        return -1;
    }
    else
    {
        // free to set
        CLR_BIT(this,pos);
        UNLOCK(this, pos);
        return 0;
    }
}

bool BitSet::getBit(int pos)
{
    if(pos > this->NumsOfBit) return false;
    return GET_BIT(this, pos);
}
  

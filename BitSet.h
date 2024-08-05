#ifndef BITSET_H
#define BITSET_H

typedef struct bitset_base
{
    unsigned long *pLock;       // indicate is content currently using
    unsigned long *pContent;
} BIT_SET_BASE, *PBIT_SET_BASE;

class BitSet{
    private:
        int NumsOfBit;
        BIT_SET_BASE data;

    public:
        BitSet(int NumsOfBit);
        ~BitSet();
        int setBit(int pos);
        int clearBit(int pos);
        bool getBit(int pos);
  
};

#endif //BITSET_H
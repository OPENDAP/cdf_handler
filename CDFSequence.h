#ifndef _cdfsequence_h
#define _cdfsequence_h 1

#include <list>

#include "Sequence.h"

class CDFSequence: public Sequence {
private:
        
public:
    CDFSequence(const string &n = "");
    CDFSequence(const CDFSequence &rhs);
    virtual ~CDFSequence();

    CDFSequence &operator=(const CDFSequence &rhs);
    virtual BaseType *ptr_duplicate();
};

#endif


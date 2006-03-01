#ifndef _cdfint16_h
#define _cdfint16_h 1

#include "Int16.h"

class CDFInt16: public Int16 {
        
public:
    CDFInt16(const string &n = "");
    CDFInt16(const CDFInt16 &rhs);
    virtual ~CDFInt16();

    CDFInt16 &operator=(const CDFInt16 &rhs);
    virtual BaseType *ptr_duplicate();

    virtual bool read(const string &dataset);
};

#endif


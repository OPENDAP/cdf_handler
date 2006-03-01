#ifndef _cdfint32_h
#define _cdfint32_h 1

#include "Int32.h"

class CDFInt32: public Int32 {
        
public:
    CDFInt32(const string &n = "");
    CDFInt32(const CDFInt32 &rhs);
    virtual ~CDFInt32();

    CDFInt32 &operator=(const CDFInt32 &rhs);
    virtual BaseType *ptr_duplicate();

    virtual bool read(const string &dataset);
};

#endif


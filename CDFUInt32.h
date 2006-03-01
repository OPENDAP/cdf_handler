#ifndef _cdfuint32_h
#define _cdfuint32_h 1

#include "UInt32.h"

class CDFUInt32: public UInt32 {
        
public:
    CDFUInt32(const string &n = "");
    CDFUInt32(const CDFUInt32 &rhs);
    virtual ~CDFUInt32();

    CDFUInt32 &operator=(const CDFUInt32 &rhs);
    virtual BaseType *ptr_duplicate();

    virtual bool read(const string &dataset);
};

#endif


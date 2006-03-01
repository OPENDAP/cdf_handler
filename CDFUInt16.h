#ifndef _cdfuint16_h
#define _cdfuint16_h 1

#include "UInt16.h"

class CDFUInt16: public UInt16 {
        
public:
    CDFUInt16(const string &n = "");
    CDFUInt16(const CDFUInt16 &rhs);
    virtual ~CDFUInt16();

    CDFUInt16 &operator=(const CDFUInt16 &rhs);
    virtual BaseType *ptr_duplicate();

    virtual bool read(const string &dataset);
};

#endif


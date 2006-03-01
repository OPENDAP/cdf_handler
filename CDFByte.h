#ifndef _cdfbyte_h
#define _cdfbyte_h 1

#include "Byte.h"

class CDFByte: public Byte {
   
public:
    CDFByte(const string &n = "");
    CDFByte(const CDFByte &rhs);
    virtual ~CDFByte();

    CDFByte &operator=(const CDFByte &rhs);

    virtual BaseType *ptr_duplicate();

    virtual bool read(const string &dataset);
};

#endif


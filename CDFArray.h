#ifndef _cdfarray_h
#define _cdfarray_h 1

#include "Array.h"

class CDFArray: public Array {
public:
    CDFArray(const string &n = "", BaseType *v = 0);
    CDFArray(const CDFArray &cdf_array);
    CDFArray &operator=(const CDFArray &rhs);
    virtual ~CDFArray();

    virtual BaseType *ptr_duplicate();

    virtual bool read(const string &dataset);
};

#endif


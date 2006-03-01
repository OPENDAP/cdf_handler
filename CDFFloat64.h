#ifndef _cdffloat64_h
#define _cdffloat64_h 1

#include "Float64.h"

class CDFFloat64: public Float64 {

public:
    CDFFloat64(const string &n = "");
    CDFFloat64(const CDFFloat64 &rhs);
    virtual ~CDFFloat64();

    CDFFloat64 &operator=(const CDFFloat64 &rhs);
    virtual BaseType *ptr_duplicate();

    virtual bool read(const string &dataset);
};

#endif


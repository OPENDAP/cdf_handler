#ifndef _cdffloat32_h
#define _cdffloat32_h 1

#include "Float32.h"

class CDFFloat32: public Float32 {

public:
    CDFFloat32(const string &n = "");
    CDFFloat32(const CDFFloat32 &rhs);
    virtual ~CDFFloat32();

    CDFFloat32 &operator=(const CDFFloat32 &rhs);
    virtual BaseType *ptr_duplicate();

    virtual bool read(const string &dataset);
};

#endif


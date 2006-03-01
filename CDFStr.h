#ifndef _cdfstr_h
#define _cdfstr_h 1

#include "Str.h"

class CDFStr: public Str {
     
public:
    CDFStr(const string &n = "");
    CDFStr(const CDFStr &rhs);
    virtual ~CDFStr();

    CDFStr &operator=(const CDFStr &rhs);

    virtual BaseType *ptr_duplicate();
    
    virtual bool read(const string &dataset);
};

#endif


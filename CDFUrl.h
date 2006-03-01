#ifndef _cdfurl_h
#define _cdfurl_h 1

#include "Url.h"

class CDFUrl: public Url {
        
public:
    CDFUrl(const string &n = "");
    CDFUrl(const CDFUrl &rhs);
    virtual ~CDFUrl();

    CDFUrl &operator=(const CDFUrl &rhs);
    virtual BaseType *ptr_duplicate();
};

#endif


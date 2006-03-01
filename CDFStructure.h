#ifndef _cdfstructure_h
#define _cdfstructure_h 1

#include "Structure.h"

class CDFStructure: public Structure {
        
public:
    CDFStructure(const string &n = "");
    CDFStructure(const CDFStructure &rhs);
    virtual ~CDFStructure();

    CDFStructure &operator=(const CDFStructure &rhs);
    virtual BaseType *ptr_duplicate();
};

#endif


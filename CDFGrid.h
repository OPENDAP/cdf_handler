#ifndef _cdfgrid_h
#define _cdfgrid_h 1

#include "Grid.h"

class CDFGrid: public Grid {

public:
    CDFGrid(const string &n = "");
    CDFGrid(const CDFGrid &rhs);
    virtual ~CDFGrid();

    CDFGrid &operator=(const CDFGrid &rhs);
    virtual BaseType *ptr_duplicate();
};

#endif


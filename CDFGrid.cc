#include "config_cdf.h"

#include <cdf.h>

#include "CDFGrid.h"

BaseType *
CDFGrid::ptr_duplicate()
{
    return new CDFGrid(*this);
}

CDFGrid::CDFGrid(const string &n) : Grid(n)
{
}

CDFGrid::CDFGrid(const CDFGrid &rhs) : Grid(rhs)
{
}


CDFGrid::~CDFGrid()
{
}

CDFGrid &
CDFGrid::operator=(const CDFGrid &rhs)
{
    if (this == &rhs)
        return *this;

    dynamic_cast<CDFGrid&>(*this) = rhs;


    return *this;
}


#include "config_cdf.h"

#include <cdf.h>

#include "CDFStructure.h"

BaseType *
CDFStructure::ptr_duplicate()
{
    return new CDFStructure(*this);
}

CDFStructure::CDFStructure(const string &n) : Structure(n)
{
}

CDFStructure::CDFStructure(const CDFStructure &rhs) : Structure(rhs)
{
}

CDFStructure::~CDFStructure()
{
}

CDFStructure &
CDFStructure::operator=(const CDFStructure &rhs)
{
    if (this == &rhs)
        return *this;

    dynamic_cast<Structure&>(*this) = rhs;
    
    return *this;
}


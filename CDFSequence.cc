#include "config_cdf.h"

#include "CDFSequence.h"

BaseType *
CDFSequence::ptr_duplicate()
{
    return new CDFSequence(*this);
}

CDFSequence::CDFSequence(const string &n) : Sequence(n)
{
}

CDFSequence::CDFSequence(const CDFSequence &rhs) : Sequence(rhs)
{
}

CDFSequence::~CDFSequence()
{
}

CDFSequence &
CDFSequence::operator=(const CDFSequence &rhs)
{
    if (this == &rhs)
        return *this;

    dynamic_cast<Sequence &>(*this) = rhs; // run Sequence assignment
        
    return *this;
}


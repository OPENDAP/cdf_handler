#include "config_cdf.h"

#include "CDFUrl.h"

CDFUrl::CDFUrl(const string &n) : Url(n)
{
}

CDFUrl::CDFUrl(const CDFUrl &rhs) : Url(rhs)
{
}

CDFUrl::~CDFUrl()
{
}

CDFUrl &
CDFUrl::operator=(const CDFUrl &rhs)
{
    if (this == &rhs)
        return *this;

    dynamic_cast<CDFUrl&>(*this) = rhs;

    return *this;
}

BaseType *
CDFUrl::ptr_duplicate()
{
    return new CDFUrl(*this);
}


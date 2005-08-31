// cdf_version.h

#ifndef E_cdf_version_h
#define E_cdf_version_h 1

#include <string>

using std::string ;

#include "config_cdf.h"

string
cdf_version()
{
    return (string)CDF_VERSION + ": compiled on " + __DATE__ + ":" + __TIME__ ;
}

#endif // E_cdf_version_h

// $Log: cdf_version.h,v $

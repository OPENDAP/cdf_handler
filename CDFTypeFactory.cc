// CDFTypeFactory.cc

// -*- mode: c++; c-basic-offset:4 -*-

// This file is part of cdf-handler, a data handler for the OPeNDAP data
// server. 

// Copyright (c) 2004,2005 University Corporation for Atmospheric Research
// Author: Patrick West <pwest@ucar.edu>
//
// This is free software; you can redistribute it and/or modify it under the
// terms of the GNU Lesser General Public License as published by the Free
// Software Foundation; either version 2.1 of the License, or (at your
// option) any later version.
// 
// This software is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
// License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// You can contact University Corporation for Atmospheric Research at
// 3080 Center Green Drive
 
#include <string>

#include "CDFByte.h"
#include "CDFInt32.h"
#include "CDFUInt32.h"
#include "CDFFloat32.h"
#include "CDFFloat64.h"
#include "CDFStr.h"
#include "CDFArray.h"

#include "CDFTypeFactory.h"

Byte *
CDFTypeFactory::NewByte(const string &n ) const 
{ 
    return new CDFByte(n);
}

Int32 *
CDFTypeFactory::NewInt32(const string &n ) const 
{ 
    return new CDFInt32(n);
}

UInt32 *
CDFTypeFactory::NewUInt32(const string &n ) const 
{ 
    return new CDFUInt32(n);
}

Float32 *
CDFTypeFactory::NewFloat32(const string &n ) const 
{ 
    return new CDFFloat32(n);
}

Float64 *
CDFTypeFactory::NewFloat64(const string &n ) const 
{ 
    return new CDFFloat64(n);
}

Str *
CDFTypeFactory::NewStr(const string &n ) const 
{ 
    return new CDFStr(n);
}

Array *
CDFTypeFactory::NewArray(const string &n , BaseType *v) const 
{ 
    return new CDFArray(n, v);
}


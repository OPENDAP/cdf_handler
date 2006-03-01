
// -*- mode: c++; c-basic-offset:4 -*-

// This file is part of cdf-handler, a C++ server of OPeNDAP for access to cdf
// data

// Copyright (c) 2002,2003 OPeNDAP, Inc.
// Author: Patrick West <pwest@ucar.edu>
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// You can contact OPeNDAP, Inc. at PO Box 112, Saunderstown, RI. 02874-0112.
 
// Implementation for TypeFactory.
//
// pwest 05/08/03

#include <string>

#include "CDFByte.h"
#include "CDFInt16.h"
#include "CDFUInt16.h"
#include "CDFInt32.h"
#include "CDFUInt32.h"
#include "CDFFloat32.h"
#include "CDFFloat64.h"
#include "CDFStr.h"
#include "CDFUrl.h"
#include "CDFArray.h"
#include "CDFStructure.h"
#include "CDFSequence.h"
#include "CDFGrid.h"

#include "CDFTypeFactory.h"
#include "debug.h"

Byte *
CDFTypeFactory::NewByte(const string &n ) const 
{ 
    return new CDFByte(n);
}

Int16 *
CDFTypeFactory::NewInt16(const string &n ) const 
{ 
    return new CDFInt16(n); 
}

UInt16 *
CDFTypeFactory::NewUInt16(const string &n ) const 
{ 
    return new CDFUInt16(n);
}

Int32 *
CDFTypeFactory::NewInt32(const string &n ) const 
{ 
    DBG(cerr << "Inside CDFTypeFactory::NewInt32" << endl);
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

Url *
CDFTypeFactory::NewUrl(const string &n ) const 
{ 
    return new CDFUrl(n);
}

Array *
CDFTypeFactory::NewArray(const string &n , BaseType *v) const 
{ 
    return new CDFArray(n, v);
}

Structure *
CDFTypeFactory::NewStructure(const string &n ) const 
{ 
    return new CDFStructure(n);
}

Sequence *
CDFTypeFactory::NewSequence(const string &n ) const 
{ 
    DBG(cerr << "Inside CDFTypeFactory::NewSequence" << endl);
    return new CDFSequence(n);
}

Grid *
CDFTypeFactory::NewGrid(const string &n ) const 
{ 
    return new CDFGrid(n);
}


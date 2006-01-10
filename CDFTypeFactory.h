// CDFTypeFactory.h

// -*- mode: c++; c-basic-offset:4 -*-

// This file is part of cdf-handler, a C++ server of OPeNDAP for access to cdf
// data. 

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
 
#ifndef cdf_type_factory_h
#define cdf_type_factory_h

#include <string>

// Class declarations; Make sure to include the corresponding headers in the
// implementation file.

#include "BaseTypeFactory.h"

class CDFByte;
class CDFInt32;
class CDFUInt32;
class CDFFloat32;
class CDFFloat64;
class CDFStr;
class CDFArray;

/** A factory for the cdf client library types.

    @author Patrick West
    @see DDS */
class CDFTypeFactory: public BaseTypeFactory {
public:
    			CDFTypeFactory() {} 
    virtual		~CDFTypeFactory() {}

    virtual Byte *	NewByte(const string &n = "") const;
    virtual Int32 *	NewInt32(const string &n = "") const;
    virtual UInt32 *	NewUInt32(const string &n = "") const;
    virtual Float32 *	NewFloat32(const string &n = "") const;
    virtual Float64 *	NewFloat64(const string &n = "") const;

    virtual Str *	NewStr(const string &n = "") const;

    virtual Array *	NewArray(const string &n = "", BaseType *v = 0) const;
};

#endif // cdf_type_factory_h

// $Log: CDFTypeFactory.h,v $

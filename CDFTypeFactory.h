// CDFTypeFactory.h

// -*- mode: c++; c-basic-offset:4 -*-

// This file is part of cdf_handler, a C++ server of OPeNDAP for access to cdf
// data

// Copyright (c) 2004,2005 University Corporation for Atmospheric Research
// Author: Patrick West <pwest@ucar.org>
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
// You can contact University Corporation for Atmospheric Research at
// 3080 Center Green Drive, Boulder, CO 80301
 
// (c) COPYRIGHT University Corporation for Atmostpheric Research 2004-2005
// Please read the full copyright statement in the file COPYRIGHT_UCAR.
//
// Authors:
//      pwest       Patrick West <pwest@ucar.edu>

#ifndef I_CDFTypeFactory_h
#define I_CDFTypeFactory_h 1

#include <string>

// Class declarations; Make sure to include the corresponding headers in the
// implementation file.

#include "BaseTypeFactory.h"

#if 0
class CDFByte;
class CDFInt16;
class CDFUInt16;
class CDFInt32;
class CDFUInt32;
class CDFFloat32;
class CDFFloat64;
class CDFStr;
class CDFUrl;
class CDFArray;
class CDFStructure;
class CDFSequence;
class CDFGrid;
#endif

/** A factory for the netCDF client library types.

    @author James Gallagher
    @see DDS */
class CDFTypeFactory: public BaseTypeFactory {
public:
    CDFTypeFactory() {} 
    virtual ~CDFTypeFactory() {}

    virtual Byte *NewByte(const string &n = "") const;
    virtual Int16 *NewInt16(const string &n = "") const;
    virtual UInt16 *NewUInt16(const string &n = "") const;
    virtual Int32 *NewInt32(const string &n = "") const;
    virtual UInt32 *NewUInt32(const string &n = "") const;
    virtual Float32 *NewFloat32(const string &n = "") const;
    virtual Float64 *NewFloat64(const string &n = "") const;

    virtual Str *NewStr(const string &n = "") const;
    virtual Url *NewUrl(const string &n = "") const;

    virtual Array *NewArray(const string &n = "", BaseType *v = 0) const;
    virtual Structure *NewStructure(const string &n = "") const;
    virtual Sequence *NewSequence(const string &n = "") const;
    virtual Grid *NewGrid(const string &n = "") const;
};

#endif // I_CDFTypeFactory_h

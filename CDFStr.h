
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
 
// cdf-handler implementation of OPeNDAP Str class used to access variables
// in the cdf files.
//
// pwest 05/08/03

#ifndef _CDFStr_h
#define _CDFStr_h 1

#ifdef __GNUG__
#pragma interface
#endif

#include "Str.h"

extern Str * NewStr(const string &n = "");

class CDFStr : public Str {
public:
    CDFStr(const string &n = (char *)0);

    virtual ~CDFStr() ;

    virtual BaseType *ptr_duplicate();

    virtual bool read(const string &dataset);
};

typedef CDFStr *CDFStrPtr;

#endif // _CDFStr_h

// $Log: CDFStr.h,v $
// Revision 1.2  2003/05/08 16:59:20  pwest
// cdf-dods server implementation
//

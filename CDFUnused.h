
// -*- mode: c++; c-basic-offset:4 -*-

// This file is part of cdf-dap, a C++ server of OPeNDAP for access to cdf
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
 
// Represents the data model objects that are not used in the cdf-dods
// implementation of a dods server
//
// pwest 05/08/03

#ifndef _CDFUnused_h
#define _CDFUnused_h 1

#ifdef __GNUG__
#pragma interface
#endif

#include <string>

using std::string ;

class Structure ;
class Function ;
class Grid ;
class Int16 ;
class List ;
class Sequence ;
class UInt16 ;
class Url ;
class BaseType ;

extern Structure *NewStructure( const string &n = "" ) ;
extern Function *NewFunction( const string &n = "" ) ;
extern Grid *NewGrid( const string &n = "" ) ;
extern Int16 *NewInt16( const string &n = "" ) ;
extern List *NewList( const string &n = "", BaseType *v = 0 ) ;
extern Sequence *NewSequence( const string &n = "" ) ;
extern UInt16 *NewUInt16( const string &n = "" ) ;
extern Url *NewUrl( const string &n = "" ) ;

#endif // _CDFUnused_h

// $Log: CDFUnused.h,v $
// Revision 1.1  2003/05/08 16:59:20  pwest
// cdf-dods server implementation
//

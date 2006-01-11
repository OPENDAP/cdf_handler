
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
 
// Implementation for CDFUInt32.
//
// pwest 05/08/03

#ifdef __GNUG__
#pragma implementation
#endif

#include "CDFUInt32.h"

UInt32 *NewUInt32( const string &n )
{
    return new CDFUInt32( n ) ;
}

CDFUInt32::CDFUInt32( const string &n ) : UInt32( n )
{
}

CDFUInt32::~CDFUInt32( )
{
}

BaseType *CDFUInt32::ptr_duplicate( )
{
    return new CDFUInt32( *this ) ;
}

bool CDFUInt32::read( const string &dataset )
{
    if ( read_p( ) )
	return true ;

    set_read_p( true ) ;  
    return true ;
}

// $Log: CDFUInt32.cc,v $
// Revision 1.1  2003/05/08 16:59:20  pwest
// cdf-dods server implementation
//

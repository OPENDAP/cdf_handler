
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
 
// Implementation for CDFUtilities.
//
// pwest 05/08/03

#include <string>
#include <fstream>

using std::string ;
using std::endl ;
using std::ofstream ;
using std::ios ;

#include <stdio.h>

#include "CDFutilities.h"
#include "CDFByte.h"
#include "CDFInt32.h"
#include "CDFUInt32.h"
#include "CDFFloat32.h"
#include "CDFFloat64.h"
#include "CDFStr.h"

/*****************************************************************************
* Status handler.
*****************************************************************************/
bool StatusHandler ( CDFstatus status )
{
    char message[CDF_ERRTEXT_LEN+1] ;

    if ( status < CDF_WARN )
    {
	printf ( "An error has occurred, halting...\n" ) ;
	CDFerror ( status, message ) ;
	printf ( "%s\n", message ) ;
	return false ;
    }
    else if ( status < CDF_OK )
    {
	printf ( "Warning, function may not have completed as expected...\n" ) ;
	CDFerror ( status, message ) ;
	printf ( "%s\n", message ) ;
	return false ;
    }
    else if ( status > CDF_OK )
    {
	printf ( "Function completed successfully, but be advised that...\n" ) ;
	CDFerror ( status, message ) ;
	printf ( "%s\n", message ) ;
	return true ;
    }       

    return true ;
}

/*****************************************************************************
* Return DODS DataType string representation given CDF data type
*****************************************************************************/
char *DodsDataType( long cdfDataType )
{
    switch( cdfDataType )
    {
	case CDF_UCHAR:
	case CDF_CHAR:
	    return "STRING" ;
	    break ;
	case CDF_BYTE:
	    return "BYTE" ;
	    break ;
	case CDF_INT1:
	case CDF_INT2:
	case CDF_INT4:
	    return "INT32" ;
	    break ;
	case CDF_UINT1:
	case CDF_UINT2:
	case CDF_UINT4:
	    return "UINT32" ;
	    break ;
	case CDF_REAL4:
	case CDF_FLOAT:
	    return "FLOAT32" ;
	    break ;
	case CDF_REAL8:
	case CDF_DOUBLE:
	case CDF_EPOCH:
	    return "FLOAT64" ;
	    break ;
    }

    return "STRING" ;
}

/*****************************************************************************
* Return CDF DataType string representation given cdf data type
*****************************************************************************/
char *DataType( long cdfDataType )
{
    switch( cdfDataType )
    {
	case CDF_BYTE:
	    return "CDF_BYTE" ;
	    break ;
	case CDF_CHAR:
	    return "CDF_CHAR" ;
	    break ;
	case CDF_INT1:
	    return "CDF_INT1" ;
	    break ;
	case CDF_UCHAR:
	    return "CDF_UCHAR" ;
	    break ;
	case CDF_UINT1:
	    return "CDF_UINT1" ;
	    break ;
	case CDF_INT2:
	    return "CDF_INT2" ;
	    break ;
	case CDF_UINT2:
	    return "CDF_UINT2" ;
	    break ;
	case CDF_INT4:
	    return "CDF_INT4" ;
	    break ;
	case CDF_UINT4:
	    return "CDF_UINT4" ;
	    break ;
	case CDF_REAL4:
	    return "CDF_REAL4" ;
	    break ;
	case CDF_FLOAT:
	    return "CDF_FLOAT" ;
	    break ;
	case CDF_REAL8:
	    return "CDF_REAL8" ;
	    break ;
	case CDF_DOUBLE:
	    return "CDF_DOUBLE" ;
	    break ;
	case CDF_EPOCH:
	    return "CDF_EPOCH" ;
	    break ;
    }

    return "UNKNOWN" ;
}

/*****************************************************************************
* Return DODS BaseType given CDF data type
*****************************************************************************/
BaseType *DodsBaseType( char *varName, long cdfDataType )
{
    switch( cdfDataType )
    {
	case CDF_BYTE:
	    return NewByte( varName ) ;
	    break ;
	case CDF_UCHAR:
	case CDF_CHAR:
	    return NewStr( varName ) ;
	    break ;
	case CDF_INT1:
	case CDF_INT2:
	case CDF_INT4:
	    return NewInt32( varName ) ;
	    break ;
	case CDF_UINT1:
	case CDF_UINT2:
	case CDF_UINT4:
	    return NewUInt32( varName ) ;
	    break ;
	case CDF_REAL4:
	case CDF_FLOAT:
	    return NewFloat32( varName ) ;
	    break ;
	case CDF_REAL8:
	case CDF_DOUBLE:
	case CDF_EPOCH:
	    return NewFloat64( varName ) ;
	    break ;
    }

    return NewStr( varName ) ;
}

// $Log: CDFutilities.cc,v $
// Revision 1.1  2003/05/08 16:59:20  pwest
// cdf-dods server implementation
//

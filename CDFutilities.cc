// CDFutilities.cc

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

#include <string>
#include <fstream>
#include <memory.h>

using std::string ;
using std::endl ;
using std::ofstream ;
using std::ios ;

#include <stdio.h>

#include "config_cdf.h"
#include "CDFutilities.h"
#include "CDFByte.h"
#include "CDFInt16.h"
#include "CDFUInt16.h"
#include "CDFInt32.h"
#include "CDFUInt32.h"
#include "CDFFloat32.h"
#include "CDFFloat64.h"
#include "CDFStr.h"

/*****************************************************************************
* Status handler.
*****************************************************************************/
bool
CDFutilities::StatusHandler( CDFstatus status, const string &file, long line )
{
    char message[CDF_ERRTEXT_LEN+1] ;

    if ( status < CDF_WARN )
    {
	cerr << "An error has occurred in " << file << " on line " << line
	     << ", halting..." << endl ;
	CDFerror ( status, message ) ;
	cerr << message << endl ;
	return false ;
    }
    else if ( status < CDF_OK )
    {
	cerr << "Warning, " << file << ": " << line
	     << ": Function may not have completed as expected..." << endl ;
	CDFerror ( status, message ) ;
	cerr << message << endl ;
	return false ;
    }
    else if ( status > CDF_OK )
    {
	cerr << file << ": " << line
	     << ": Function completed successfully, but be advised...\n" ;
	CDFerror ( status, message ) ;
	cerr << message << endl ;
	return true ;
    }       

    return true ;
}

/*****************************************************************************
* Return DODS DataType string representation given CDF data type
*****************************************************************************/
char *
CDFutilities::DodsDataType( long cdfDataType )
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
	    return "INT16" ;
	    break ;
	case CDF_UINT1:
	case CDF_UINT2:
	    return "UINT16" ;
	    break ;
	case CDF_INT4:
	    return "INT32" ;
	    break ;
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
	case CDF_EPOCH16:
	default:
	    return "UNKNOWN" ;
	    break ;
    }

    return "UNKNOWN" ;
}

/*****************************************************************************
* Return CDF DataType string representation given cdf data type
*****************************************************************************/
char *
CDFutilities::DataType( long cdfDataType )
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
	case CDF_EPOCH16:
	    return "CDF_EPOCH16" ;
	    break ;
	default:
	    return "UNKNOWN" ;
	    break ;
    }

    return "UNKNOWN" ;
}

/*****************************************************************************
* Return DODS BaseType given CDF data type
*****************************************************************************/
BaseType *
CDFutilities::DodsBaseType( const string &varName,
			    const string &dataset,
			    long cdfDataType )
{
    switch( cdfDataType )
    {
	case CDF_BYTE:
	    return new CDFByte( varName, dataset ) ;
	    break ;
	case CDF_UCHAR:
	case CDF_CHAR:
	    return new CDFStr( varName, dataset ) ;
	    break ;
	case CDF_INT1:
	case CDF_INT2:
	    return new CDFInt16( varName, dataset ) ;
	    break ;
	case CDF_UINT1:
	case CDF_UINT2:
	    return new CDFUInt16( varName, dataset ) ;
	    break ;
	case CDF_INT4:
	    return new CDFInt32( varName, dataset ) ;
	    break ;
	case CDF_UINT4:
	    return new CDFUInt32( varName, dataset ) ;
	    break ;
	case CDF_REAL4:
	case CDF_FLOAT:
	    return new CDFFloat32( varName, dataset ) ;
	    break ;
	case CDF_REAL8:
	case CDF_DOUBLE:
	case CDF_EPOCH:
	    return new CDFFloat64( varName, dataset ) ;
	    break ;
	case CDF_EPOCH16:
	    return NULL ;
	    break ;
	default:
	    return NULL ;
	    break ;
    }

    return NULL ;
}

/*****************************************************************************
* read data into buffer
*****************************************************************************/
void
CDFutilities::read_record( void *cdf_buf, void *arrbuf,
			   unsigned int &arrindex, long numElements,
			   long varType, long charSize )
{
    long anindex = 0 ;

    switch( varType )
    {
	case CDF_CHAR:
	case CDF_UCHAR:
	    {
		char *my_buf = (char *)cdf_buf ;
		char buf[charSize+1] ;
		string *vbuf = (string *)arrbuf ;
		if( numElements == 0 )
		{
		    vbuf[arrindex] = "" ;
		    arrindex++ ;
		}
		else
		{
		    for( anindex = 0; anindex < numElements; anindex++ )
		    {
			memcpy(buf, my_buf, charSize ) ;
			buf[charSize] = '\0' ;
			my_buf += charSize ;
			vbuf[arrindex] = buf ;
			arrindex++ ;
		    }
		}
	    }
	    break ;
	case CDF_BYTE:
	    {
		dods_byte *my_buf = (dods_byte *)arrbuf ;
		unsigned char *my_cdf = (unsigned char *)cdf_buf ;
		if( numElements == 0 )
		{
		    my_buf[arrindex] = 0 ;
		    arrindex++ ;
		}
		else
		{
		    for( anindex = 0; anindex < numElements; anindex++ )
		    {
			my_buf[arrindex] = my_cdf[anindex] ;
			arrindex++ ;
		    }
		}
	    }
	    break ;
	case CDF_INT1:
	    {
		dods_int16 *my_buf = (dods_int16 *)arrbuf ;
		char *my_cdf = (char *)cdf_buf ;
		if( numElements == 0 )
		{
		    my_buf[arrindex] = 0 ;
		    arrindex++ ;
		}
		else
		{
		    for( anindex = 0; anindex < numElements; anindex++ )
		    {
			my_buf[arrindex] = (dods_int16)my_cdf[anindex] ;
			arrindex++ ;
		    }
		}
	    }
	    break ;
	case CDF_UINT1:
	    {
		dods_uint16 *my_buf = (dods_uint16 *)arrbuf ;
		unsigned char *my_cdf = (unsigned char *)cdf_buf ;
		if( numElements == 0 )
		{
		    my_buf[arrindex] = 0 ;
		    arrindex++ ;
		}
		else
		{
		    for( anindex = 0; anindex < numElements; anindex++ )
		    {
			my_buf[arrindex] = (dods_uint16)my_cdf[anindex] ;
			arrindex++ ;
		    }
		}
	    }
	    break ;
	case CDF_INT2:
	    {
		dods_int16 *my_buf = (dods_int16 *)arrbuf ;
		short *my_cdf = (short *)cdf_buf ;
		if( numElements == 0 )
		{
		    my_buf[arrindex] = 0 ;
		    arrindex++ ;
		}
		else
		{
		    for( anindex = 0; anindex < numElements; anindex++ )
		    {
			my_buf[arrindex] = my_cdf[anindex] ;
			arrindex++ ;
		    }
		}
	    }
	    break ;
	case CDF_UINT2:
	    {
		dods_uint16 *my_buf = (dods_uint16 *)arrbuf ;
		unsigned short *my_cdf = (unsigned short *)cdf_buf ;
		if( numElements == 0 )
		{
		    my_buf[arrindex] = 0 ;
		    arrindex++ ;
		}
		else
		{
		    for( anindex = 0; anindex < numElements; anindex++ )
		    {
			my_buf[arrindex] = my_cdf[anindex] ;
			arrindex++ ;
		    }
		}
	    }
	    break ;
	case CDF_INT4:
	    {
		dods_int32 *my_buf = (dods_int32 *)arrbuf ;
		long *my_cdf = (long *)cdf_buf ;
		if( numElements == 0 )
		{
		    my_buf[arrindex] = 0 ;
		    arrindex++ ;
		}
		else
		{
		    for( anindex = 0; anindex < numElements; anindex++ )
		    {
			my_buf[arrindex] = my_cdf[anindex] ;
			arrindex++ ;
		    }
		}
	    }
	    break ;
	case CDF_UINT4:
	    {
		dods_uint32 *my_buf = (dods_uint32 *)arrbuf ;
		unsigned long *my_cdf = (unsigned long *)cdf_buf ;
		if( numElements == 0 )
		{
		    my_buf[arrindex] = 0 ;
		    arrindex++ ;
		}
		else
		{
		    for( anindex = 0; anindex < numElements; anindex++ )
		    {
			my_buf[arrindex] = my_cdf[anindex] ;
			arrindex++ ;
		    }
		}
	    }
	    break ;
	case CDF_FLOAT:
	case CDF_REAL4:
	    {
		dods_float32 *my_buf = (dods_float32 *)arrbuf ;
		float *my_cdf = (float *)cdf_buf ;
		if( numElements == 0 )
		{
		    my_buf[arrindex] = 0.0 ;
		    arrindex++ ;
		}
		else
		{
		    for( anindex = 0; anindex < numElements; anindex++ )
		    {
			my_buf[arrindex] = my_cdf[anindex] ;
			arrindex++ ;
		    }
		}
	    }
	    break ;
	case CDF_REAL8:
	case CDF_DOUBLE:
	case CDF_EPOCH:
	    {
		dods_float64 *my_buf = (dods_float64 *)arrbuf ;
		double *my_cdf = (double *)cdf_buf ;
		if( numElements == 0 )
		{
		    my_buf[arrindex] = 0.0 ;
		    arrindex++ ;
		}
		else
		{
		    for( anindex = 0; anindex < numElements; anindex++ )
		    {
			my_buf[arrindex] = my_cdf[anindex] ;
			arrindex++ ;
		    }
		}
	    }
	    break ;
    }
}

void
CDFutilities::write_record( void *arrbuf,
                            unsigned int numElements,
			    long varType )
{
    unsigned int anindex = 0 ;

    switch( varType )
    {
	case CDF_INT4:
	{
	    dods_int32 *my_buf = (dods_int32 *)arrbuf ;
	    for( anindex = 0; anindex < numElements; anindex++ )
	    {
		BESDEBUG( "cdf", "    val = " << my_buf[anindex] << endl ) ;
	    }
	}
	break ;
    }
}


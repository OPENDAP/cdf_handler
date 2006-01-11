
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
 
// Implementation for CDFArray.
//
// pwest 05/08/03

#ifdef __GNUG__
#pragma implementation
#endif

#include <iostream>
#include <fstream>

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>

using std::cerr ;
using std::endl ;
using std::fstream ;
using std::ios ;

#include "CDFArray.h"
#include "CDFutilities.h"
#include "cdf.h"

//static fstream dbgThing("cdfarray_thingy.out", ios::out);

Array *NewArray( const string &n, BaseType *v )
{
    return new CDFArray( n, v ) ;
}

CDFArray::CDFArray( const string &n, BaseType *v )
    : Array( n, v ),
      _arrbuf( 0 ),
      _arrindex( 0 )
{
}

CDFArray::~CDFArray( )
{
}

BaseType *CDFArray::ptr_duplicate( )
{
    return new CDFArray( *this ) ;
}

bool CDFArray::read( const string &dataset )
{
    if( read_p( ) )
    {
	return true ;
    }

    CDFid id;            /* CDF identifier. */
    CDFstatus status;    /* CDF completion status. */

    long anindex = 0 ;

    long varNum = 0 ;
    long varType = 0 ;
    long varTypeSize = 0 ;
    long numDims = 0 ;
    long dimSizes[CDF_MAX_DIMS] ;
    long dimVarys[CDF_MAX_DIMS] ;
    long maxRec = 0 ;
    long numRecs = 0 ;
    long recVary = 0 ;
    long numElems = 0 ;

    void *thingy = 0 ;
    long indices[CDF_MAX_DIMS] ;
    long intervals[CDF_MAX_DIMS] ;
    long counts[CDF_MAX_DIMS] ;

    for( anindex = 0; anindex < CDF_MAX_DIMS; anindex++ )
    {
	indices[anindex] = 0 ;
	intervals[anindex] = 1 ;
	counts[anindex] = 0 ;
    }

    /*************************************************************************
    * Open the CDF.
    *************************************************************************/
    status = CDFopen ( dataset.c_str(), &id ) ;
    if ( status != CDF_OK )
    {
	if( StatusHandler ( status ) == false )
	{
	    return false ;
	}
    }

    /*************************************************************************
    * Set to readonly and zmode
    *************************************************************************/
    status = CDFlib( SELECT_, CDF_READONLY_MODE_, READONLYon,
			      CDF_zMODE_, zMODEon1,
		     NULL_ ) ;

    if ( status != CDF_OK )
    {
	if( StatusHandler ( status ) == false )
	{
	    return false ;
	}
    }

    /*************************************************************************
    * Read variables by hyper read
    *************************************************************************/
    status = CDFlib( GET_, zVAR_NUMBER_, name().c_str(), &varNum,
		     NULL_ ) ;
    if ( status != CDF_OK )
    {
	if( StatusHandler ( status ) == false )
	{
	    return false ;
	}
    }

    status = CDFlib( SELECT_, zVAR_, varNum,
		     NULL_ ) ;
    if ( status != CDF_OK )
    {
	if( StatusHandler ( status ) == false )
	{
	    return false ;
	}
    }

    status = CDFlib( GET_, zVAR_DATATYPE_, &varType,
			   zVAR_NUMDIMS_, &numDims,
			   zVAR_DIMSIZES_, dimSizes,
			   zVAR_DIMVARYS_, dimVarys,
			   zVAR_MAXREC_, &maxRec,
			   zVAR_NUMRECS_, &numRecs,
			   zVAR_RECVARY_, &recVary,
			   zVAR_NUMELEMS_, &numElems,
		     NULL_ ) ;
    if ( status != CDF_OK )
    {
	if( StatusHandler ( status ) == false )
	{
	    return false ;
	}
    }

    if( varType == CDF_CHAR || varType == CDF_UCHAR )
    {
	varTypeSize = numElems ;
    } else {
	status = CDFlib( GET_, DATATYPE_SIZE_, varType, &varTypeSize,
			 NULL_ ) ;
	if ( status != CDF_OK )
	{
	    if( StatusHandler ( status ) == false )
	    {
		return false ;
	    }
	}
    }

    unsigned long relements = 1 ;
    for( anindex = 0; anindex < numDims; anindex++ )
    {
	if( dimVarys[anindex] )
	{
	    counts[anindex] = dimSizes[anindex] ;
	} else {
	    counts[anindex] = 1 ;
	}
	relements = relements * counts[anindex] ;
    }

    // telements is the total number of elements to read
    unsigned long telements = relements * numRecs ;

    // rsize is the record size given the number of elements per record and
    // the size of each element
    unsigned long rsize = relements * varTypeSize ;

    // allocate the buffer that will read each of the records
    thingy = malloc( rsize+1 ) ;

    // allocate the dods buffer that will hold all the data
    switch( varType )
    {
	case CDF_BYTE:
	    _arrbuf = (void *)(new dods_byte[telements]);
	    break ;
	case CDF_UCHAR:
	case CDF_CHAR:
	    _arrbuf = (void *)(new string[telements]);
	    break ;
	case CDF_INT1:
	case CDF_INT2:
	case CDF_INT4:
	    _arrbuf = (void *)(new dods_int32[telements]);
	    break ;
	case CDF_UINT1:
	case CDF_UINT2:
	case CDF_UINT4:
	    _arrbuf = (void *)(new dods_uint32[telements]);
	    break ;
	case CDF_REAL4:
	case CDF_FLOAT:
	    _arrbuf = (void *)(new dods_float32[telements]);
	    break ;
	case CDF_REAL8:
	case CDF_DOUBLE:
	case CDF_EPOCH:
	    _arrbuf = (void *)(new dods_float64[telements]);
	    break ;
    }
    fflush( stdout ) ;

    for( anindex = 0; anindex < numRecs; anindex++ )
    {
	status = CDFlib( SELECT_, zVAR_RECCOUNT_, (long)1,
				  zVARs_RECNUMBER_, anindex,
				  zVAR_RECINTERVAL_, (long)1,
				  zVAR_DIMINDICES_, indices,
				  zVAR_DIMCOUNTS_, counts,
				  zVAR_DIMINTERVALS_, intervals,
			 NULL_ ) ;
	if ( status != CDF_OK )
	{
	    if( StatusHandler ( status ) == false )
	    {
		return false ;
	    }
	}

	status = CDFlib( GET_, zVAR_HYPERDATA_, thingy,
			 NULL_ ) ;
	if ( status != CDF_OK )
	{
	    if( StatusHandler ( status ) == false )
	    {
		return false ;
	    }
	}

	read_record( thingy, relements, varType, numElems ) ;
    }

    val2buf( _arrbuf ) ;

    /*************************************************************************
    * Close CDF.
    *************************************************************************/
    CDFclose ( id ) ;

    set_read_p( true ) ;
    return true ;
}

/*****************************************************************************
* Print the Values
*****************************************************************************/
void CDFArray::read_record( void *thingy, long numElements,
			    long varType, long charSize )
{
    long anindex = 0 ;

    switch( varType )
    {
	case CDF_CHAR:
	case CDF_UCHAR:
	    {
		char *vthingy = (char *)thingy ;
		char buf[charSize+1] ;
		string *vbuf = (string *)_arrbuf ;
		for( anindex = 0; anindex < numElements; anindex++ )
		{
		    memcpy(buf, vthingy, charSize ) ;
		    buf[charSize] = '\0' ;
		    //dbgThing << "val = " << buf << endl ;
		    vthingy += charSize ;
		    vbuf[_arrindex] = buf ;
		    _arrindex++ ;
		}
	    }
	    break ;
	case CDF_BYTE:
	    {
		dods_byte *dthingy = (dods_byte *)_arrbuf ;
		char *vthingy = (char *)thingy ;
		for( anindex = 0; anindex < numElements; anindex++ )
		{
		    dthingy[_arrindex] = vthingy[anindex] ;
		    //dbgThing << "val = " << dthingy[_arrindex] << endl ;
		    _arrindex++ ;
		}
	    }
	    break ;
	case CDF_INT1:
	    {
		dods_int32 *dthingy = (dods_int32 *)_arrbuf ;
		char *vthingy = (char *)thingy ;
		for( anindex = 0; anindex < numElements; anindex++ )
		{
		    dthingy[_arrindex] = vthingy[anindex] ;
		    //dbgThing << "val = " << dthingy[_arrindex] << endl ;
		    _arrindex++ ;
		}
	    }
	    break ;
	case CDF_INT2:
	    {
		dods_int32 *dthingy = (dods_int32 *)_arrbuf ;
		short *vthingy = (short *)thingy ;
		for( anindex = 0; anindex < numElements; anindex++ )
		{
		    dthingy[_arrindex] = vthingy[anindex] ;
		    //dbgThing << "val = " << dthingy[_arrindex] << endl ;
		    _arrindex++ ;
		}
	    }
	    break ;
	case CDF_INT4:
	    {
		long *vthingy = (long *)thingy ;
		dods_int32 *dthingy = (dods_int32 *)_arrbuf ;
		for( anindex = 0; anindex < numElements; anindex++ )
		{
		    dthingy[_arrindex] = vthingy[anindex] ;
		    //dbgThing << "val = " << dthingy[_arrindex] << endl ;
		    _arrindex++ ;
		}
	    }
	    break ;
	case CDF_UINT1:
	    {
		unsigned char *vthingy = (unsigned char *)thingy ;
		dods_uint32 *dthingy = (dods_uint32 *)_arrbuf ;
		for( anindex = 0; anindex < numElements; anindex++ )
		{
		    dthingy[_arrindex] = vthingy[anindex] ;
		    //dbgThing << "val = " << dthingy[_arrindex] << endl ;
		    _arrindex++ ;
		}
	    }
	    break ;
	case CDF_UINT2:
	    {
		unsigned short *vthingy = (unsigned short *)thingy ;
		dods_uint32 *dthingy = (dods_uint32 *)_arrbuf ;
		for( anindex = 0; anindex < numElements; anindex++ )
		{
		    dthingy[_arrindex] = vthingy[anindex] ;
		    //dbgThing << "val = " << dthingy[_arrindex] << endl ;
		    _arrindex++ ;
		}
	    }
	    break ;
	case CDF_UINT4:
	    {
		unsigned long *vthingy = (unsigned long *)thingy ;
		dods_uint32 *dthingy = (dods_uint32 *)_arrbuf ;
		for( anindex = 0; anindex < numElements; anindex++ )
		{
		    dthingy[_arrindex] = vthingy[anindex] ;
		    //dbgThing << "val = " << dthingy[_arrindex] << endl ;
		    _arrindex++ ;
		}
	    }
	    break ;
	case CDF_FLOAT:
	case CDF_REAL4:
	    {
		float *vthingy = (float *)thingy ;
		dods_float32 *dthingy = (dods_float32 *)_arrbuf ;
		for( anindex = 0; anindex < numElements; anindex++ )
		{
		    dthingy[_arrindex] = vthingy[anindex] ;
		    //dbgThing << "val = " << dthingy[_arrindex] << endl ;
		    _arrindex++ ;
		}
	    }
	    break ;
	case CDF_REAL8:
	case CDF_DOUBLE:
	case CDF_EPOCH:
	    {
		double *vthingy = (double *)thingy ;
		dods_float64 *dthingy = (dods_float64 *)_arrbuf ;
		for( anindex = 0; anindex < numElements; anindex++ )
		{
		    dthingy[_arrindex] = vthingy[anindex] ;
		    //dbgThing << "val = " << dthingy[_arrindex] << endl ;
		    _arrindex++ ;
		}
	    }
	    break ;
    }
}

// $Log: CDFArray.cc,v $
// Revision 1.2  2003/05/08 16:59:20  pwest
// cdf-dods server implementation
//

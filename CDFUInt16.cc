// CDFUInt16.cc

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
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//
// You can contact University Corporation for Atmospheric Research at
// 3080 Center Green Drive, Boulder, CO 80301
 
// (c) COPYRIGHT University Corporation for Atmostpheric Research 2004-2005
// Please read the full copyright statement in the file COPYRIGHT_UCAR.
//
// Authors:
//      pwest       Patrick West <pwest@ucar.edu>
 
#include "config_cdf.h"

#include <cdf.h>

#include "CDFUInt16.h"
#include "CDFutilities.h"

CDFUInt16::CDFUInt16(const string &n, const string &d) : UInt16(n, d)
{
}

CDFUInt16::CDFUInt16(const CDFUInt16 &rhs) : UInt16(rhs)
{
}

CDFUInt16::~CDFUInt16()
{
}

CDFUInt16 &
CDFUInt16::operator=(const CDFUInt16 &rhs)
{
    if (this == &rhs)
        return *this;

    dynamic_cast<CDFUInt16&>(*this) = rhs;

    return *this;
}


BaseType *
CDFUInt16::ptr_duplicate()
{
    return new CDFUInt16(*this);
}


bool
CDFUInt16::read()
{
    if( read_p( ) )
    {
	return false ;
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

    void *cdf_buf = 0 ;
    void *arrbuf = 0 ;

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
    string ds_name = dataset() ;
    BESDEBUG( "cdf", "CDFUInt16: " << name() << ": Opening "
		     << ds_name << endl ) ;
    status = CDFopen ( ds_name.c_str(), &id ) ;
    if ( status != CDF_OK )
    {
	if( CDFutilities::StatusHandler( status, __FILE__, __LINE__ ) == false )
	{
	    return false ;
	}
    }

    /*************************************************************************
    * Set to readonly and zmode
    *************************************************************************/
    status = CDFlib( SELECT_, CDF_READONLY_MODE_, READONLYon,
			      CDF_zMODE_, zMODEon2,
		     NULL_ ) ;

    if ( status != CDF_OK )
    {
	if( CDFutilities::StatusHandler( status, __FILE__, __LINE__ ) == false )
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
	if( CDFutilities::StatusHandler( status, __FILE__, __LINE__ ) == false )
	{
	    return false ;
	}
    }

    status = CDFlib( SELECT_, zVAR_, varNum,
		     NULL_ ) ;
    if ( status != CDF_OK )
    {
	if( CDFutilities::StatusHandler( status, __FILE__, __LINE__ ) == false )
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
	if( CDFutilities::StatusHandler( status, __FILE__, __LINE__ ) == false )
	{
	    return false ;
	}
    }

    if( BESISDEBUG( "cdf" ) )
    {
	BESDEBUG( "cdf", "  varType = "
			 << CDFutilities::DataType( varType ) << endl ) ;
	BESDEBUG( "cdf", "  numDims = " << numDims << endl ) ;
	BESDEBUG( "cdf", "  maxRec = " << maxRec << endl ) ;
	BESDEBUG( "cdf", "  numRecs = " << numRecs << endl ) ;
	BESDEBUG( "cdf", "  recVary = " << recVary << endl ) ;
	BESDEBUG( "cdf", "  numElems = " << numElems << endl ) ;
    }

    if( varType != CDF_UINT2 && varType != CDF_UINT1)
    {
	cerr << "CDFUInt16: the type of data should be CDF_UINT2 or CDF_UINT1"
	     << endl ;
    }

    if( numDims != 0 )
    {
	cerr << "CDFUInt16: number of dimensions should be 0" << endl ;
	return false ;
    }

    if( numRecs > 1 )
    {
	cerr << "CDFUInt16: number of records should be 1" << endl ;
	return false ;
    }

    status = CDFlib( GET_, DATATYPE_SIZE_, varType, &varTypeSize,
		     NULL_ ) ;
    if ( status != CDF_OK )
    {
	if( CDFutilities::StatusHandler( status, __FILE__, __LINE__ ) == false )
	{
	    return false ;
	}
    }
    BESDEBUG( "cdf", "  varTypeSize = " << varTypeSize << endl ) ;

    cdf_buf = malloc( varTypeSize ) ;
    arrbuf = (void *)&d_buf ;

    if( numRecs == 0 ) numRecs = 1 ;
    status = CDFlib( SELECT_, zVAR_RECCOUNT_, (long)1,
			      zVARs_RECNUMBER_, 0,
			      zVAR_RECINTERVAL_, (long)1,
			      zVAR_DIMINDICES_, indices,
			      zVAR_DIMCOUNTS_, counts,
			      zVAR_DIMINTERVALS_, intervals,
		     NULL_ ) ;
    if ( status != CDF_OK )
    {
	if( CDFutilities::StatusHandler( status, __FILE__, __LINE__ ) == false )
	{
	    return false ;
	}
    }

    status = CDFlib( GET_, zVAR_HYPERDATA_, cdf_buf,
		     NULL_ ) ;
    if ( status != CDF_OK )
    {
	if( CDFutilities::StatusHandler( status, __FILE__, __LINE__ ) == false )
	{
	    return false ;
	}
    }

    numElems = numRecs*numElems ;
    unsigned int arrindex = 0 ;
    CDFutilities::read_record( cdf_buf, arrbuf, arrindex,
                               numElems, varType, numElems ) ;
    BESDEBUG( "cdf", "  _buf = " << d_buf << endl ) ;

    /*************************************************************************
    * Close CDF.
    *************************************************************************/
    CDFclose ( id ) ;

    set_read_p( true ) ;
    return false ;
}


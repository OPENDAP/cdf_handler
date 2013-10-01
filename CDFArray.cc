// CDFArray.cc

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

#include "CDFArray.h"
#include "CDFutilities.h"

BaseType *
CDFArray::ptr_duplicate()
{
    return new CDFArray(*this);
}

CDFArray::CDFArray(const string &n, const string &d, BaseType *v)
    : Array(n, d, v)
{
}

CDFArray::CDFArray(const CDFArray &rhs) : Array(rhs)
{
}

CDFArray::~CDFArray()
{
}

CDFArray &
CDFArray::operator=(const CDFArray &rhs)
{
    if (this == &rhs)
        return *this;

    dynamic_cast<Array &>(*this) = rhs;

    return *this;
}

bool
CDFArray::read()
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
    BESDEBUG( "cdf", "CDFArray: " << name() << ": Opening "
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
	for( unsigned int i_numDims = 0; i_numDims < numDims; i_numDims++ )
	{
	    BESDEBUG( "cdf", "    dimSizes[" << i_numDims << "] = "
			     << dimSizes[i_numDims] << endl ) ;
	    if( dimVarys[i_numDims] == VARY )
		BESDEBUG( "cdf", "    dimVarys[" << i_numDims << "] = VARY"
				 << endl ) ;
	    else
		BESDEBUG( "cdf", "    dimVarys[" << i_numDims << "] = NOVARY"
				 << endl ) ;
	}
	BESDEBUG( "cdf", "  maxRec = " << maxRec << endl ) ;
	BESDEBUG( "cdf", "  numRecs = " << numRecs << endl ) ;
	if( recVary == VARY ) BESDEBUG( "cdf", "  recVary = VARY" << endl ) ;
	else BESDEBUG( "cdf", "  recVary = VARY" << endl ) ;
	BESDEBUG( "cdf", "  numElems = " << numElems << endl ) ;
    }
    if( numRecs == 0 )
    {
	numRecs = 1 ;
    }

    if( varType == CDF_CHAR || varType == CDF_UCHAR )
    {
	varTypeSize = numElems ;
    } else {
	status = CDFlib( GET_, DATATYPE_SIZE_, varType, &varTypeSize,
			 NULL_ ) ;
	if ( status != CDF_OK )
	{
	    if( CDFutilities::StatusHandler( status, __FILE__, __LINE__ ) == false )
	    {
		return false ;
	    }
	}
    }
    BESDEBUG( "cdf", "  varTypeSize = " << varTypeSize << endl ) ;

    unsigned long relements = 1 ;
    for( anindex = 0; anindex < numDims; anindex++ )
    {
	if( dimVarys[anindex] )
	{
	    counts[anindex] = dimSizes[anindex] ;
	}
	else
	{
	    counts[anindex] = 1 ;
	}
	BESDEBUG( "cdf", "  counts[" << anindex << "] = "
			 << counts[anindex] << endl ) ;
	relements = relements * counts[anindex] ;
    }
    BESDEBUG( "cdf", "  relements = " << relements << endl ) ;

    // telements is the total number of elements to read
    unsigned long telements = relements * numRecs ;
    BESDEBUG( "cdf", "  telements = " << telements << endl ) ;

    // rsize is the record size given the number of elements per record and
    // the size of each element
    unsigned long rsize = relements * varTypeSize ;
    BESDEBUG( "cdf", "  rsize = " << rsize << endl ) ;

    // allocate the buffer that will read each of the records
    cdf_buf = malloc( rsize+1 ) ;

    // allocate the dods buffer that will hold all the data
    void *arrbuf = 0 ;
    switch( varType )
    {
	case CDF_BYTE:
	    arrbuf = (void *)(new dods_byte[telements]);
	    break ;
	case CDF_UCHAR:
	case CDF_CHAR:
	    arrbuf = (void *)(new string[telements]);
	    break ;
	case CDF_INT1:
	case CDF_INT2:
	case CDF_INT4:
	    arrbuf = (void *)(new dods_int32[telements]);
	    break ;
	case CDF_UINT1:
	case CDF_UINT2:
	case CDF_UINT4:
	    arrbuf = (void *)(new dods_uint32[telements]);
	    break ;
	case CDF_REAL4:
	case CDF_FLOAT:
	    arrbuf = (void *)(new dods_float32[telements]);
	    break ;
	case CDF_REAL8:
	case CDF_DOUBLE:
	case CDF_EPOCH:
	    arrbuf = (void *)(new dods_float64[telements]);
	    break ;
    }
    fflush( stdout ) ;

    status = CDFlib( SELECT_, zVAR_RECCOUNT_, (long)1,
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
    unsigned int arrindex = 0 ;
    for( anindex = 0; anindex < numRecs; anindex++ )
    {

	status = CDFlib( SELECT_, zVARs_RECNUMBER_, anindex,
	                 GET_, zVAR_HYPERDATA_, cdf_buf,
			 NULL_ ) ;
	if ( status != CDF_OK )
	{
	    if( CDFutilities::StatusHandler( status, __FILE__, __LINE__ ) == false )
	    {
		return false ;
	    }
	}

	CDFutilities::read_record( cdf_buf, arrbuf, arrindex, relements,
				   varType, numElems ) ;
    }

    if( BESISDEBUG( "cdf" ) )
    {
	CDFutilities::write_record( arrbuf, telements, varType ) ;
    }

    val2buf( arrbuf ) ;

    /*************************************************************************
    * Close CDF.
    *************************************************************************/
    CDFclose ( id ) ;

    set_read_p( true ) ;
    return false ;
}


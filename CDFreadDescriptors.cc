// CDFreadDescriptors.cc

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

#include <stdlib.h>
#include <stdio.h>

#include <iostream>

using std::cout ;
using std::endl ;

#include "config_cdf.h"
#include "CDFreadDescriptors.h"
#include "CDFutilities.h"
#include "CDFArray.h"
#include "cgi_util.h"

/*
 * Current method of reading the data from  a CDF file:
 *
 * The current code stores all variables into a single array, except in the
 * case where there is only one record and no dimensions. In this case the
 * variable is a scaler. If there is more than one record and/or one or more
 * dimensions than the entire variable is stored into a single array.
 *
 * If there is more than one record with data for a particular variable then
 * a dimension is added for the records. So, if a variable has two
 * dimensions and has data stored in 2 records, then a third dimension is
 * prepended to the dimension list to represent the records.
 */
bool
readDescriptors( DDS &dds, const string &filename, const string &name )
{
    dds.set_dataset_name( name ) ;

    CDFid id;            /* CDF identifier. */
    CDFstatus status;    /* CDF completion status. */

    long numVars = 0 ;

    long anindex = 0 ;
    long subindex = 0 ;

    char varName[CDF_VAR_NAME_LEN + 1] ;
    long varType = 0 ;
    long numDims = 0 ;
    long dimSizes[CDF_MAX_DIMS] ;
    long dimVarys[CDF_MAX_DIMS] ;
    long maxRec = 0 ;
    unsigned long numRecs = 0 ;
    long recVary = 0 ;
    long numElems = 0 ;

    /*************************************************************************
    * Open the CDF.
    *************************************************************************/
    BESDEBUG( "cdf", "Opening CDF file " << filename << endl )
    status = CDFopen ( filename.c_str(), &id ) ;
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
    * Get number of variables
    *************************************************************************/
    status = CDFlib( GET_, CDF_NUMzVARS_, &numVars,
		     NULL_ ) ;
    if ( status != CDF_OK )
    {
	if( CDFutilities::StatusHandler( status, __FILE__, __LINE__ ) == false )
	{
	    return false ;
	}
    }
    BESDEBUG( "cdf", "  numVars = " << numVars << endl )

    /*************************************************************************
    * Get and display variable information
    *************************************************************************/
    for( anindex = 0; anindex < numVars; anindex++ )
    {
	status = CDFlib( SELECT_, zVAR_, anindex,
			 NULL_ ) ;
	if ( status != CDF_OK )
	{
	    if( CDFutilities::StatusHandler( status, __FILE__, __LINE__ ) == false )
	    {
		return false ;
	    }
	}
	if( status == CDF_OK )
	{
	    status = CDFlib( GET_, zVAR_NAME_, varName,
				   zVAR_DATATYPE_, &varType,
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
		BESDEBUG( "cdf", "varName: " << varName << endl )
		BESDEBUG( "cdf", "  varType = " << CDFutilities::DataType( varType ) << endl )
		BESDEBUG( "cdf", "  numDims = " << numDims << endl )
		unsigned int i_numDims = 0 ;
		for( i_numDims = 0; i_numDims < numDims; i_numDims++ )
		{
		    BESDEBUG( "cdf", "    dimSizes[" << i_numDims << "] = " << dimSizes[i_numDims] << endl )
		    BESDEBUG( "cdf", "    dimVarys[" << i_numDims << "] = " << dimVarys[i_numDims] << endl )
		}
		BESDEBUG( "cdf", "  numRecs = " << numRecs << endl )
		BESDEBUG( "cdf", "  recVary = " << recVary << endl )
		BESDEBUG( "cdf", "  numElems = " << numElems << endl )
	    }

	    BaseType *var = 0 ;
	    // if numDims is 0  AND numRecs less than 2 then it is a scaler,
	    // otherwise it's an array
	    if( numDims == 0 && numRecs < 2 )
	    {
		var = CDFutilities::DodsBaseType( varName, filename, varType ) ;
	    }
	    else
	    {
		BaseType *bt = CDFutilities::DodsBaseType( varName,
							   filename,
							   varType ) ;
		Array *ar = new CDFArray( varName, filename, bt ) ;
		delete bt ;
		var = ar ;
		if( recVary && numRecs > 0 )
		{
		    ar->append_dim( numRecs, "RecDim" ) ;
		}

		for( subindex = 0; subindex < numDims; subindex++ )
		{
		    if( dimVarys[subindex] )
		    {
			char dimName[64] ;
			sprintf( dimName, "Dim%ld", subindex ) ;
			ar->append_dim( dimSizes[subindex], dimName ) ;
		    }
		}
	    }

	    dds.add_var( var ) ;
	}
    }

    /*************************************************************************
    * Close CDF.
    *************************************************************************/
    CDFclose ( id ) ;

    return true ;
}


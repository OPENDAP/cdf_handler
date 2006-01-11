
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
 
// Implementation for CDFreadDescriptors.
//
// pwest 05/08/03

#include <stdlib.h>
#include <stdio.h>

#include "CDFreadDescriptors.h"
#include "CDFutilities.h"
#include "CDFArray.h"
#include "cgi_util.h"

bool readDescriptors( DDS &dds, const string &filename, const string &name )
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
    status = CDFopen ( filename.c_str(), &id ) ;
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
    * Get number of variables
    *************************************************************************/
    status = CDFlib( GET_, CDF_NUMzVARS_, &numVars,
		     NULL_ ) ;
    if ( status != CDF_OK )
    {
	if( StatusHandler ( status ) == false )
	{
	    return false ;
	}
    }

    /*************************************************************************
    * Get and display variable information
    *************************************************************************/
    for( anindex = 0; anindex < numVars; anindex++ )
    {
	status = CDFlib( SELECT_, zVAR_, anindex,
			 NULL_ ) ;
	if ( status != CDF_OK )
	{
	    if( StatusHandler ( status ) == false )
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
		if( StatusHandler ( status ) == false )
		{
		    return false ;
		}
	    }

	    Array *ar = NewArray( varName ) ;
	    ar->add_var( DodsBaseType( varName, varType ) ) ;
	    if( recVary )
		ar->append_dim( numRecs, "RecDim" ) ;

	    for( subindex = 0; subindex < numDims; subindex++ )
	    {
		if( dimVarys[subindex] )
		{
		    char dimName[64] ;
		    sprintf( dimName, "Dim%ld", subindex ) ;
		    ar->append_dim( dimSizes[subindex], dimName ) ;
		}
	    }

	    dds.add_var( ar ) ;
	}
    }

    /*************************************************************************
    * Close CDF.
    *************************************************************************/
    CDFclose ( id ) ;

    return true ;
}

// $Log: CDFreadDescriptors.cc,v $
// Revision 1.2  2004/07/02 20:10:19  pwest
// Added dataset name to the dds, updated the INSTALL, fixed configure.in
//
// Revision 1.1  2003/05/08 16:59:20  pwest
// cdf-dods server implementation
//

// CDFreadAttributes.cc

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
#include <string.h>

#include "CDFreadAttributes.h"
#include "CDFutilities.h"

#if 0
static int CDFelemSize(long dataType);
#endif

static int CDFelemSize (long dataType)
{
    switch (dataType) 
    {
	case CDF_BYTE: return 1;
	case CDF_INT1: return 1;
	case CDF_INT2: return 2;
	case CDF_INT4: return 4;
	case CDF_INT8: return 8;
	case CDF_UINT1: return 1;
	case CDF_UINT2: return 2;
	case CDF_UINT4: return 4;
	case CDF_REAL4: return 4;
	case CDF_REAL8: return 8;
	case CDF_FLOAT: return 4;
	case CDF_DOUBLE: return 8;
	case CDF_EPOCH: return 8;
	case CDF_EPOCH16: return 16;
	case CDF_TIME_TT2000: return 8;
	case CDF_CHAR: return 1;
	case CDF_UCHAR: return 1;
    }
    return 0;
}

bool readAttributes( DAS &das, string filename )
{
    CDFid id;            /* CDF identifier. */
    CDFstatus status;    /* CDF completion status. */

    long numAttrs = 0 ;

    /*************************************************************************
    * Open the CDF.
    *************************************************************************/
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
	    CDFclose( id ) ;
	    return false ;
	}
    }

    /*************************************************************************
    * Get attribute
    *************************************************************************/
    status = CDFlib( GET_, CDF_NUMATTRS_, &numAttrs,
		     NULL_ ) ;
    if ( status != CDF_OK )
    {
	if( CDFutilities::StatusHandler( status, __FILE__, __LINE__ ) == false )
	{
	    CDFclose( id ) ;
	    return false ;
	}
    }

    /*************************************************************************
    * Get and display attributes
    *************************************************************************/
    long anindex = 0 ;
    long subindex = 0 ;

    char varName[CDF_VAR_NAME_LEN + 1] ;

    char attrName[CDF_ATTR_NAME_LEN+1] ;
    long attrScope = 0 ;
    long maxEntry = 0 ;
    long numEntry = 0 ;
    long attrDataType = 0 ;
    long attrNumElems = 0 ;
    void *attrData;

    for( anindex = 0; anindex < numAttrs; anindex++ )
    {
	status = CDFlib( SELECT_, ATTR_, anindex,
			 GET_, ATTR_NAME_, attrName,
			       ATTR_SCOPE_, &attrScope,
			 NULL_ ) ;
	if ( status != CDF_OK )
	{
	    if( CDFutilities::StatusHandler( status, __FILE__, __LINE__ ) == false )
	    {
		CDFclose( id ) ;
		return false ;
	    }
	}

	if( attrScope == GLOBAL_SCOPE )
	{
	    status = CDFlib( GET_, ATTR_MAXgENTRY_, &maxEntry,
				   ATTR_NUMgENTRIES_, &numEntry,
			     NULL_ ) ;
	    if ( status != CDF_OK )
	    {
		if( CDFutilities::StatusHandler( status, __FILE__, __LINE__ ) == false )
		{
		    CDFclose( id ) ;
		    return false ;
		}
	    }

	    strcpy( varName, "CDF_GLOBAL" ) ;

	    for( subindex = 0; subindex < maxEntry+1; subindex++ )
	    {
		status = CDFlib( SELECT_, gENTRY_, subindex,
				 GET_, gENTRY_DATATYPE_, &attrDataType,
				       gENTRY_NUMELEMS_, &attrNumElems,
				 NULL_ ) ;
		if( status == NO_SUCH_ENTRY ) continue ;
		if ( status != CDF_OK )
		{
		    if( CDFutilities::StatusHandler( status, __FILE__, __LINE__ ) == false )
		    {
			CDFclose( id ) ;
			return false ;
		    }
		}
		if (attrDataType == CDF_CHAR || attrDataType == CDF_UCHAR)
                {
		    attrData = malloc((size_t)attrNumElems+1) ;
                }
		else
                {
		    attrData = malloc((size_t)CDFelemSize(attrDataType) * attrNumElems);
                }
		status = CDFlib( GET_, gENTRY_DATA_, attrData,
				 NULL_ ) ;
		if ( status != CDF_OK )
		{
		    if( CDFutilities::StatusHandler( status, __FILE__, __LINE__ ) == false )
		    {
			free (attrData) ;
			CDFclose( id ) ;
			return false ;
		    }
		}
		AddValue( das, attrName, attrData, attrDataType,
			  attrNumElems, varName) ;
		free (attrData) ;
	    }
	}
	else
	{
	    status = CDFlib( GET_, ATTR_MAXzENTRY_, &maxEntry,
				   ATTR_NUMzENTRIES_, &numEntry,
			     NULL_ ) ;
	    if ( status != CDF_OK )
	    {
		if( CDFutilities::StatusHandler( status, __FILE__, __LINE__ ) == false )
		{
		    CDFclose( id ) ;
		    return false ;
		}
	    }

	    for( subindex = 0; subindex < maxEntry+1; subindex++ )
	    {
		status = CDFlib( SELECT_, zENTRY_, subindex,
				 GET_, zENTRY_DATATYPE_, &attrDataType,
				       zENTRY_NUMELEMS_, &attrNumElems,
				 SELECT_, zVAR_, subindex,
				 GET_, zVAR_NAME_, varName,
				 NULL_ ) ;
		if( status == NO_SUCH_ENTRY ) continue ;
		if ( status != CDF_OK )
		{
		    if( CDFutilities::StatusHandler( status, __FILE__, __LINE__ ) == false )
		    {
			CDFclose( id ) ;
			return false ;
		    }
		}
		if (attrDataType == CDF_CHAR || attrDataType == CDF_UCHAR)
		{
		  attrData = malloc((size_t)attrNumElems+1) ;
		}
		else
		{
		  attrData = malloc((size_t)CDFelemSize(attrDataType) * attrNumElems);
		}
		status = CDFlib( GET_, zENTRY_DATA_, attrData,
				 NULL_ ) ;
		if ( status != CDF_OK )
		{
		    if( CDFutilities::StatusHandler( status, __FILE__, __LINE__ ) == false )
		    {
			free (attrData) ;
			CDFclose( id ) ;
			return false ;
		    }
		}

		AddValue( das, attrName, attrData, attrDataType,
			  attrNumElems, varName) ;
		free (attrData) ;
	    }
	}
    }

    /*************************************************************************
    * Close CDF.
    *************************************************************************/
    CDFclose( id ) ;

    return true ;
}

/*****************************************************************************
* Print a value
*****************************************************************************/
bool AddValue( DAS &das, char *attrName, void *attrData, long attrDataType,
	       long attrNumElems, char *varName )
{
    AttrTable *attrTable = das.get_table( varName ) ;
    if( !attrTable )
	attrTable = das.add_table( varName, new AttrTable ) ;

    if( attrDataType == CDF_CHAR || attrDataType == CDF_UCHAR )
    {
	char *charAttrData = (char *)attrData ;
	attrNumElems-- ;
	while( *(charAttrData + attrNumElems) == ' ' )
	    attrNumElems-- ;
	attrNumElems++ ;
	*((char *)attrData + attrNumElems) = '\0' ;
	attrNumElems = 1 ;
    }

    CDFstatus status;    /* CDF completion status. */

    for( long anindex = 0; anindex < attrNumElems; anindex++ )
    {
	long attrDataTypeSize = 0 ;
	status = CDFlib( GET_, DATATYPE_SIZE_, attrDataType, &attrDataTypeSize,
			 NULL_ ) ;
	if ( status != CDF_OK )
	{
	    if( CDFutilities::StatusHandler( status, __FILE__, __LINE__ ) == false )
	    {
		return false ;
	    }
	}

	int loc = anindex * attrDataTypeSize ;
	char *stringVal = ValToString( attrDataType, (char *)attrData + loc ) ;
	attrTable->append_attr( attrName,
				CDFutilities::DodsDataType( attrDataType ),
				stringVal ) ;
	delete stringVal ;
    }

    return true ;
}

char *ValToString( long attrDataType, void *attrData )
{
    char *retVal = 0 ;
    if( attrDataType == CDF_CHAR || attrDataType == CDF_UCHAR )
	retVal = new char[strlen((const char *)attrData) + 3] ;
    else
	retVal = new char[32] ;

    switch( attrDataType )
    {
	case CDF_CHAR:
	case CDF_UCHAR:
	    {
		sprintf( retVal, "\"%s\"", (char *)attrData ) ;
	    }
	    break ;
	case CDF_INT1:
	    {
		char c = *(char *)attrData ;
		sprintf( retVal, "%d", c ) ;
	    }
	    break ;
	case CDF_BYTE:
	case CDF_UINT1:
	    {
		unsigned char c = *(unsigned char *)attrData ;
		sprintf( retVal, "%u", c ) ;
	    }
	    break ;
	case CDF_INT2:
	    {
		short s = *(short *)attrData ;
		sprintf( retVal, "%d", s ) ;
	    }
	    break ;
	case CDF_UINT2:
	    {
		unsigned short s = *(unsigned short *)attrData ;
		sprintf( retVal, "%u", s ) ;
	    }
	    break ;
	case CDF_INT4:
	    {
		long l = *(long *)attrData ;
		sprintf( retVal, "%ld", l ) ;
	    }
	    break ;
	case CDF_UINT4:
	    {
		unsigned long l = *(unsigned long *)attrData ;
		sprintf( retVal, "%lu", l ) ;
	    }
	    break ;
	case CDF_FLOAT:
	case CDF_REAL4:
	    {
		float f = *(float *)attrData ;
		sprintf( retVal, "%.10g", f ) ;
		if( !strchr( retVal, '.' ) && !strchr( retVal, 'e' ) )
		    strcat( retVal, ".0" ) ;
	    }
	    break ;
	case CDF_REAL8:
	case CDF_DOUBLE:
	case CDF_EPOCH:
	    {
		double d = *(double *)attrData ;
		sprintf( retVal, "%.17g", d ) ;
		if( !strchr( retVal, '.' ) && !strchr( retVal, 'e' ) )
		    strcat( retVal, ".0" ) ;
	    }
	    break ;
    }

    return retVal ;
}


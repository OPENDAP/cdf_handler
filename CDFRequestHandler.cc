
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
 
// Implementation for CDFRequestHandler.
//
// pwest 05/08/03

#ifdef __GNUG__
#pragma implementation
#endif

#include <iostream>
#include <fstream>

using std::ifstream ;
using std::cerr ;
using std::endl ;

#include "CDFRequestHandler.h"
#include "CDFTypeFactory.h"
#include "DODSResponseHandler.h"
#include "DODSResponseNames.h"
#include "CDFreadAttributes.h"
#include "DAS.h"
#include "CDFreadDescriptors.h"
#include "DDS.h"
#include "DODSConstraintFuncs.h"
#include "DODSInfo.h"
#include "TheDODSKeys.h"
#include "DODSResponseException.h"
#include "cdf_version.h"

CDFRequestHandler::CDFRequestHandler( string name )
    : DODSRequestHandler( name )
{
    add_handler( DAS_RESPONSE, CDFRequestHandler::cdf_build_das ) ;
    add_handler( DDS_RESPONSE, CDFRequestHandler::cdf_build_dds ) ;
    add_handler( DATA_RESPONSE, CDFRequestHandler::cdf_build_data ) ;
    add_handler( HELP_RESPONSE, CDFRequestHandler::cdf_build_help ) ;
    add_handler( VERS_RESPONSE, CDFRequestHandler::cdf_build_version ) ;
}

CDFRequestHandler::~CDFRequestHandler()
{
}

bool
CDFRequestHandler::cdf_build_das( DODSDataHandlerInterface &dhi )
{
    DAS *das = (DAS *)dhi.response_handler->get_response_object() ;
    if( !readAttributes( *das, dhi.container->get_real_name() ) )
    {
	throw DODSResponseException( "CDF could not build the DAS response" ) ;
    }
    return true ;
}

bool
CDFRequestHandler::cdf_build_dds( DODSDataHandlerInterface &dhi )
{
    DDS *dds = (DDS *)dhi.response_handler->get_response_object() ;
    CDFTypeFactory *factory = new CDFTypeFactory ;
    dds->set_factory( factory ) ;
    if( !readDescriptors( *dds, dhi.container->get_real_name(),
			  dhi.container->get_symbolic_name() ) )
    {
	throw DODSResponseException( "CDF could not build the DDS response" ) ;
    }
    DODSConstraintFuncs::post_append( dhi ) ;

    return true ;
}

bool
CDFRequestHandler::cdf_build_data( DODSDataHandlerInterface &dhi )
{
    DDS *dds = (DDS *)dhi.response_handler->get_response_object() ;
    CDFTypeFactory *factory = new CDFTypeFactory ;
    dds->set_factory( factory ) ;
    if( !readDescriptors( *dds, dhi.container->get_real_name(),
			  dhi.container->get_symbolic_name() ) )
    {
	throw DODSResponseException( "CDF could not build the DDS response" ) ;
    }
    DODSConstraintFuncs::post_append( dhi ) ;

    return true ;
}

bool
CDFRequestHandler::cdf_build_help( DODSDataHandlerInterface &dhi )
{
    DODSInfo *info = (DODSInfo *)dhi.response_handler->get_response_object() ;
    info->add_data( (string)"cdf-handler help: " + cdf_version() + "\n" ) ;
    bool found = false ;
    string key = (string)"CDF.Help." + dhi.transmit_protocol ;
    string file = TheDODSKeys::TheKeys()->get_key( key, found ) ;
    if( found == false )
    {
	info->add_data( "no help information available for cdf-handler\n" ) ;
    }
    else
    {
	ifstream ifs( file.c_str() ) ;
	if( !ifs )
	{
	    info->add_data( "cdf-handler help file not found, help information not available\n" ) ;
	}
	else
	{
	    char line[4096] ;
	    while( !ifs.eof() )
	    {
		ifs.getline( line, 4096 ) ;
		if( !ifs.eof() )
		{
		    info->add_data( line ) ;
		    info->add_data( "\n" ) ;
		}
	    }
	    ifs.close() ;
	}
    }

    return true ;
}

bool
CDFRequestHandler::cdf_build_version( DODSDataHandlerInterface &dhi )
{
    DODSInfo *info = (DODSInfo *)dhi.response_handler->get_response_object() ;
    info->add_data( (string)"    " + cdf_version() + "\n" ) ;
    info->add_data( (string)"        libcdf3.0\n" ) ;
    return true ;
}

// $Log: CDFRequestHandler.cc,v $

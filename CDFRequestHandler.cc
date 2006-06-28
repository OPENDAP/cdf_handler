
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
#include "BESResponseHandler.h"
#include "BESResponseNames.h"
#include "CDFreadAttributes.h"
#include "DAS.h"
#include "CDFreadDescriptors.h"
#include "DDS.h"
#include "BESConstraintFuncs.h"
#include "BESVersionInfo.h"
#include "TheBESKeys.h"
#include "BESResponseException.h"
#include "config_cdf.h"

CDFRequestHandler::CDFRequestHandler( string name )
    : BESRequestHandler( name )
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
CDFRequestHandler::cdf_build_das( BESDataHandlerInterface &dhi )
{
    DAS *das = (DAS *)dhi.response_handler->get_response_object() ;
    if( !readAttributes( *das, dhi.container->get_real_name() ) )
    {
	string s = "CDF could not build the DAS response" ;
	throw BESResponseException( s, __FILE__, __LINE__ ) ;
    }
    return true ;
}

bool
CDFRequestHandler::cdf_build_dds( BESDataHandlerInterface &dhi )
{
    DDS *dds = (DDS *)dhi.response_handler->get_response_object() ;
    CDFTypeFactory *factory = new CDFTypeFactory ;
    dds->set_factory( factory ) ;
    if( !readDescriptors( *dds, dhi.container->get_real_name(),
			  dhi.container->get_symbolic_name() ) )
    {
	string s = "CDF could not build the DDS response" ;
	throw BESResponseException( s, __FILE__, __LINE__ ) ;
    }
    BESConstraintFuncs::post_append( dhi ) ;

    return true ;
}

bool
CDFRequestHandler::cdf_build_data( BESDataHandlerInterface &dhi )
{
    DDS *dds = (DDS *)dhi.response_handler->get_response_object() ;
    CDFTypeFactory *factory = new CDFTypeFactory ;
    dds->set_factory( factory ) ;
    if( !readDescriptors( *dds, dhi.container->get_real_name(),
			  dhi.container->get_symbolic_name() ) )
    {
	string s = "CDF could not build the Data DDS response" ;
	throw BESResponseException( s, __FILE__, __LINE__ ) ;
    }
    BESConstraintFuncs::post_append( dhi ) ;

    return true ;
}

bool
CDFRequestHandler::cdf_build_help( BESDataHandlerInterface &dhi )
{
    BESInfo *info = dynamic_cast<BESInfo *>(dhi.response_handler->get_response_object());
    string handles = (string)DAS_RESPONSE
                     + "," + DDS_RESPONSE
                     + "," + DATA_RESPONSE
                     + "," + HELP_RESPONSE
                     + "," + VERS_RESPONSE ;
    info->add_tag( "handles", handles ) ;
    info->add_tag( "version", PACKAGE_STRING ) ;
    return true ;
}

bool
CDFRequestHandler::cdf_build_version( BESDataHandlerInterface &dhi )
{
    BESVersionInfo *info = dynamic_cast<BESVersionInfo *>(dhi.response_handler->get_response_object() ) ;
    info->addHandlerVersion( PACKAGE_NAME, PACKAGE_VERSION ) ;
    return true ;
}


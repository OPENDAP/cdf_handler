// CDFRequestHandler.cc

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
#include "BESDataNames.h"
#include "CDFreadAttributes.h"
#include "BESDASResponse.h"
#include "CDFreadDescriptors.h"
#include "BESDDSResponse.h"
#include "BESDataDDSResponse.h"
#include "BESConstraintFuncs.h"
#include "BESVersionInfo.h"
#include "TheBESKeys.h"
#include "BESHandlerException.h"
#include "config_cdf.h"

CDFRequestHandler::CDFRequestHandler( const string &name )
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
    BESDASResponse *bdas =
	dynamic_cast<BESDASResponse *>(dhi.response_handler->get_response_object() ) ;
    DAS *das = bdas->get_das() ;

    if( !readAttributes( *das, dhi.container->access() ) )
    {
	string s = "CDF could not build the DAS response" ;
	throw BESHandlerException( s, __FILE__, __LINE__ ) ;
    }
    return true ;
}

bool
CDFRequestHandler::cdf_build_dds( BESDataHandlerInterface &dhi )
{
    BESDDSResponse *bdds =
	dynamic_cast<BESDDSResponse *>( dhi.response_handler->get_response_object() ) ;
    DDS *dds = bdds->get_dds() ;

    CDFTypeFactory *factory = new CDFTypeFactory ;
    dds->set_factory( factory ) ;
    if( !readDescriptors( *dds, dhi.container->access(),
			  dhi.container->get_symbolic_name() ) )
    {
	string s = "CDF could not build the DDS response" ;
	throw BESHandlerException( s, __FILE__, __LINE__ ) ;
    }
    dhi.data[POST_CONSTRAINT] = dhi.container->get_constraint();

    dds->set_factory( NULL ) ;

    return true ;
}

bool
CDFRequestHandler::cdf_build_data( BESDataHandlerInterface &dhi )
{
    BESDataDDSResponse *bdds =
	dynamic_cast<BESDataDDSResponse *>( dhi.response_handler->get_response_object() ) ;
    DataDDS *dds = bdds->get_dds() ;

    CDFTypeFactory *factory = new CDFTypeFactory ;
    dds->set_factory( factory ) ;
    if( !readDescriptors( *dds, dhi.container->access(),
			  dhi.container->get_symbolic_name() ) )
    {
	string s = "CDF could not build the Data DDS response" ;
	throw BESHandlerException( s, __FILE__, __LINE__ ) ;
    }
    dhi.data[POST_CONSTRAINT] = dhi.container->get_constraint();

    dds->set_factory( NULL ) ;

    return true ;
}

bool
CDFRequestHandler::cdf_build_help( BESDataHandlerInterface &dhi )
{
    BESInfo *info = dynamic_cast<BESInfo *>(dhi.response_handler->get_response_object());
    info->begin_tag( "Handler" ) ;
    info->add_tag( "name", PACKAGE_NAME ) ;
    string handles = (string)DAS_RESPONSE
                     + "," + DDS_RESPONSE
                     + "," + DATA_RESPONSE
                     + "," + HELP_RESPONSE
                     + "," + VERS_RESPONSE ;
    info->add_tag( "handles", handles ) ;
    info->add_tag( "version", PACKAGE_STRING ) ;
    info->end_tag( "Handler" ) ;
    return true ;
}

bool
CDFRequestHandler::cdf_build_version( BESDataHandlerInterface &dhi )
{
    BESVersionInfo *info = dynamic_cast<BESVersionInfo *>(dhi.response_handler->get_response_object() ) ;
    info->addHandlerVersion( PACKAGE_NAME, PACKAGE_VERSION ) ;
    return true ;
}

/** @brief dumps information about this object
 *
 * Displays the pointer value of this instance, the name of the request
 * handler, and the names of all registered handler functions
 *
 * @param strm C++ i/o stream to dump the information to
 */
void
CDFRequestHandler::dump( ostream &strm ) const
{
    strm << BESIndent::LMarg << "CDFRequestHandler::dump - ("
			     << (void *)this << ")" << endl ;
    BESIndent::Indent() ;
    BESRequestHandler::dump( strm ) ;
    BESIndent::UnIndent() ;
}


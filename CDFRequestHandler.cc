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
#include <BESResponseHandler.h>
#include <BESResponseNames.h>
#include <BESDapNames.h>
#include "CDFResponseNames.h"
#include <BESDataNames.h>
#include "CDFreadAttributes.h"
#include <BESDASResponse.h>
#include "CDFreadDescriptors.h"
#include <BESDDSResponse.h>
#include <BESDataDDSResponse.h>
#include <BESConstraintFuncs.h>
#include <BESVersionInfo.h>
#include <TheBESKeys.h>
#include <BESInternalError.h>
#include <Ancillary.h>
#include <BESUtil.h>
#include <BESServiceRegistry.h>
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
    BESResponseObject *response = dhi.response_handler->get_response_object() ;
    BESDASResponse *bdas = dynamic_cast < BESDASResponse * >(response) ;
    if( !bdas )
	throw BESInternalError( "cast error", __FILE__, __LINE__ ) ;

    bdas->set_container( dhi.container->get_symbolic_name() ) ;
    DAS *das = bdas->get_das() ;

    // read the attributes for this container, including ancillary
    // attributes
    string accessed = dhi.container->access() ;
    if( !readAttributes( *das, accessed ) )
    {
	string s = "CDF could not build the DAS response" ;
	throw BESInternalError( s, __FILE__, __LINE__ ) ;
    }

    Ancillary::read_ancillary_das( *das, accessed ) ;

    bdas->clear_container( ) ;

    return true ;
}

bool
CDFRequestHandler::cdf_build_dds( BESDataHandlerInterface &dhi )
{
    BESResponseObject *response = dhi.response_handler->get_response_object();
    BESDDSResponse *bdds = dynamic_cast < BESDDSResponse * >(response);
    if( !bdds )
	throw BESInternalError( "cast error", __FILE__, __LINE__ ) ;
  
    bdds->set_container( dhi.container->get_symbolic_name() ) ;
    DDS *dds = bdds->get_dds();

    // read the data structure including any ancillary structure
    string accessed = dhi.container->access() ;
    string symbolic = dhi.container->get_symbolic_name() ;
    dds->filename( accessed );
    if( !readDescriptors( *dds, accessed, symbolic ) )
    {
	string s = "CDF could not build the DDS response" ;
	throw BESInternalError( s, __FILE__, __LINE__ ) ;
    }
    Ancillary::read_ancillary_dds( *dds, accessed ) ;

    // The dds now includes attribute information. Grab the attributes and
    // any ancillary attributes.
    DAS *das = new DAS ;
    BESDASResponse bdas( das ) ;
    bdas.set_container( dhi.container->get_symbolic_name() ) ;
    if( !readAttributes( *das, accessed ) )
    {
	string s = "CDF could not build the DAS response" ;
	throw BESInternalError( s, __FILE__, __LINE__ ) ;
    }
    Ancillary::read_ancillary_das( *das, accessed ) ;

    // transfer the attributes to the dds.
    dds->transfer_attributes(das);

    bdds->set_constraint( dhi ) ;

    bdds->clear_container( ) ;

    return true ;
}

bool
CDFRequestHandler::cdf_build_data( BESDataHandlerInterface &dhi )
{
    BESResponseObject *response = dhi.response_handler->get_response_object();
    BESDataDDSResponse *bdds = dynamic_cast < BESDataDDSResponse * >(response);
    if( !bdds )
	throw BESInternalError( "cast error", __FILE__, __LINE__ ) ;
  
    bdds->set_container( dhi.container->get_symbolic_name() ) ;
    DataDDS *dds = bdds->get_dds();

    // read the data structure including any ancillary structure
    string accessed = dhi.container->access() ;
    string symbolic = dhi.container->get_symbolic_name() ;
    dds->filename( accessed );
    if( !readDescriptors( *dds, accessed, symbolic ) )
    {
	string s = "CDF could not build the Data DDS response" ;
	throw BESInternalError( s, __FILE__, __LINE__ ) ;
    }
    Ancillary::read_ancillary_dds( *dds, accessed ) ;

    // The dds now includes attribute information. Grab the attributes and
    // any ancillary attributes.
    DAS *das = new DAS ;
    BESDASResponse bdas( das ) ;
    bdas.set_container( dhi.container->get_symbolic_name() ) ;
    if( !readAttributes( *das, accessed ) )
    {
	string s = "CDF could not build the DAS response" ;
	throw BESInternalError( s, __FILE__, __LINE__ ) ;
    }
    Ancillary::read_ancillary_das( *das, accessed ) ;

    // transfer the attributes to the dds.
    dds->transfer_attributes(das);

    bdds->set_constraint( dhi ) ;

    bdds->clear_container( ) ;

    return true ;
}

bool
CDFRequestHandler::cdf_build_help( BESDataHandlerInterface &dhi )
{
    BESInfo *info =
	dynamic_cast<BESInfo *>(dhi.response_handler->get_response_object());
    if( !info )
	throw BESInternalError( "cast error", __FILE__, __LINE__ ) ;

    map<string,string> attrs ;
    attrs["name"] = PACKAGE_NAME ;
    attrs["version"] = PACKAGE_VERSION ;
    list<string> services ;
    BESServiceRegistry::TheRegistry()->services_handled( CDF_NAME, services );
    if( services.size() > 0 )
    {
	string handles = BESUtil::implode( services, ',' ) ;
	attrs["handles"] = handles ;
    }
    info->begin_tag( "module", &attrs ) ;
    info->end_tag( "module" ) ;

    return true ;
}

bool
CDFRequestHandler::cdf_build_version( BESDataHandlerInterface &dhi )
{
    BESResponseObject *response = dhi.response_handler->get_response_object();
    BESVersionInfo *info = dynamic_cast < BESVersionInfo * >(response);
    if( !info )
	throw BESInternalError( "cast error", __FILE__, __LINE__ ) ;
  
    info->add_module( PACKAGE_NAME, PACKAGE_VERSION ) ;

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


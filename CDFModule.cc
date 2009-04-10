// CDFModule.cc

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

#include <iostream>

using std::endl ;

#include "CDFModule.h"
#include <BESRequestHandlerList.h>
#include "CDFRequestHandler.h"
#include "CDFResponseNames.h"
#include <BESDapService.h>
#include <BESContainerStorageList.h>
#include <BESContainerStorageCatalog.h>
#include <BESCatalogDirectory.h>
#include <BESCatalogList.h>
#include <BESDebug.h>

#define CDF_CATALOG "catalog"

void
CDFModule::initialize( const string &modname )
{
    BESDEBUG( "cdf", "Initializing CDF module " << modname << endl )

    BESDEBUG( "cdf", "    adding " << modname << " request handler" << endl )
    BESRequestHandler *handler = new CDFRequestHandler( modname ) ;
    BESRequestHandlerList::TheList()->add_handler( modname, handler ) ;

    BESDEBUG( "cdf", modname << " handles dap services" << endl )
    BESDapService::handle_dap_service( modname ) ;

    BESDEBUG( "cdf", "    adding " << CDF_CATALOG << " catalog" << endl )
    if( !BESCatalogList::TheCatalogList()->ref_catalog( CDF_CATALOG ) )
    {
	BESCatalogList::TheCatalogList()->
	    add_catalog( new BESCatalogDirectory( CDF_CATALOG ) ) ;
    }
    else
    {
	BESDEBUG( "cdf", "    catalog already exists, skipping" << endl )
    }

    BESDEBUG( "cdf", "    adding catalog container storage " << CDF_CATALOG
		    << endl )
    if( !BESContainerStorageList::TheList()->ref_persistence( CDF_CATALOG ) )
    {
	BESContainerStorageCatalog *csc =
	    new BESContainerStorageCatalog( CDF_CATALOG ) ;
	BESContainerStorageList::TheList()->add_persistence( csc ) ;
    }
    else
    {
	BESDEBUG( "cdf", "    storage already exists, skipping" << endl )
    }

    BESDEBUG( "cdf", "    adding cdf debug context" << endl )
    BESDebug::Register( "cdf" ) ;

    BESDEBUG( "cdf", "Done Initializing CDF module " << modname << endl )
}

void
CDFModule::terminate( const string &modname )
{
    BESDEBUG( "cdf", "Cleaning CDF module " << modname << endl )

    BESDEBUG( "cdf", "    removing CDF Handler" << modname << endl )
    BESRequestHandler *rh = BESRequestHandlerList::TheList()->remove_handler( modname ) ;
    if( rh ) delete rh ;

    BESDEBUG( "cdf", "    removing catalog container storage"
                    << CDF_CATALOG << endl )
    BESContainerStorageList::TheList()->deref_persistence( CDF_CATALOG ) ;

    BESDEBUG( "cdf", "    removing " << CDF_CATALOG << " catalog" << endl )
    BESCatalogList::TheCatalogList()->deref_catalog( CDF_CATALOG ) ;

    BESDEBUG( "cdf", "Done Cleaning CDF module " << modname << endl )
}

/** @brief dumps information about this object
 *
 * Displays the pointer value of this instance
 *
 * @param strm C++ i/o stream to dump the information to
 */
void
CDFModule::dump( ostream &strm ) const
{
    strm << BESIndent::LMarg << "CDFModule::dump - ("
			     << (void *)this << ")" << endl ;
}

extern "C"
{
    BESAbstractModule *maker()
    {
	return new CDFModule ;
    }
}


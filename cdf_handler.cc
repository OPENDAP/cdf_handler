
// -*- mode: c++; c-basic-offset:4 -*-

// This file is part of libdap, A C++ implementation of the OPeNDAP Data
// Access Protocol.

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
 
#include "config_cdf.h"

static char not_used rcsid[]={"$Id: cdf_handler.cc,v 1.2 2004/07/02 20:10:19 pwest Exp $"};

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>

#include "CDFutilities.h"
#include "CDFreadDescriptors.h"
#include "CDFreadAttributes.h"
#include "DDS.h"
#include "DAS.h"
#include "DODSFilter.h"
#include "debug.h"
#include "cgi_util.h"
#include "Error.h"

using std::cerr ;
using std::endl ;
using std::string ;
using std::stringstream ;
using std::ends ;
using std::fstream ;
using std::ios ;

//static fstream dbgThing( "cdf_thingy.out", ios::out ) ;

const string cgi_version = DODS_SERVER_VERSION ;

int 
main( int argc, char *argv[] )
{
    try { 
	DODSFilter df( argc, argv ) ;

	switch ( df.get_response() ) {
	  case DODSFilter::DAS_Response: {
	    DAS das ;
	    if( !readAttributes( das, df.get_dataset_name() ) )
	    {
		stringstream s ;
		s << "cdf_dods: Failed to build DAS." << ends ;
		ErrMsgT( s.str( ) ) ;
		Error e( unknown_error, "Failed to build DAS." ) ;
		set_mime_text( stdout, dods_error, df.get_cgi_version( ) ) ;
		e.print( stdout ) ;
		return 1 ;
	    }
	    df.read_ancillary_das( das ) ;
	    df.send_das( das ) ;
	    break ;
	  }

	  case DODSFilter::DDS_Response: {
	    DDS dds ;
	    if( !readDescriptors( dds, df.get_dataset_name(),
				  name_path( df.get_dataset_name() ) ) )
	    {
		stringstream s ;
		s << "cdf_dods: Failed to build DDS." << ends ;
		ErrMsgT( s.str( ) ) ;
		Error e( unknown_error, "Failed to build DDS." ) ;
		set_mime_text( stdout, dods_error, df.get_cgi_version( ) ) ;
		e.print( stdout ) ;
		return 1 ;
	    }
	    df.read_ancillary_dds( dds ) ;
	    df.send_dds( dds, true ) ;
	    break ;
	  }

	  case DODSFilter::DataDDS_Response: {
	    DDS dds ;
	    if( !readDescriptors( dds, df.get_dataset_name() ,
				  name_path( df.get_dataset_name() ) ) )
	    {
		stringstream s ;
		s << "cdf_dods: Failed to build DataDDS." << ends ;
		ErrMsgT( s.str( ) ) ;
		Error e( unknown_error, "Failed to build DataDDS." ) ;
		set_mime_text( stdout, dods_error, df.get_cgi_version( ) ) ;
		e.print( stdout ) ;
		return 1 ;
	    }
	    df.read_ancillary_dds( dds ) ;
	    df.send_data( dds, stdout ) ;
	    break ;
	  }

	  case DODSFilter::DDX_Response: {
	    DDS dds ;
	    DAS das ;

	    dds.filename( df.get_dataset_name() ) ;

	    if( !readDescriptors( dds, df.get_dataset_name() ,
				  name_path( df.get_dataset_name() ) ) )
	    {
		stringstream s ;
		s << "cdf_dods: Failed to build DDS." << ends ;
		ErrMsgT( s.str( ) ) ;
		Error e( unknown_error, "Failed to build DDS." ) ;
		set_mime_text( stdout, dods_error, df.get_cgi_version( ) ) ;
		e.print( stdout ) ;
		return 1 ;
	    }
	    df.read_ancillary_dds( dds ) ;

	    if( !readAttributes( das, df.get_dataset_name() ) )
	    {
		stringstream s ;
		s << "cdf_dods: Failed to build DAS." << ends ;
		ErrMsgT( s.str( ) ) ;
		Error e( unknown_error, "Failed to build DAS." ) ;
		set_mime_text( stdout, dods_error, df.get_cgi_version( ) ) ;
		e.print( stdout ) ;
		return 1 ;
	    }
	    df.read_ancillary_das( das ) ;

	    dds.transfer_attributes( &das ) ;

	    df.send_ddx( dds, stdout ) ;
	    break ;
	  }

	  case DODSFilter::Version_Response: {
	    if ( df.get_cgi_version() == "" )
		df.set_cgi_version( cgi_version ) ;
	    df.send_version_info() ;

	    break ;
	  }

	  default:
	    df.print_usage();	// Throws Error
	}
    }
    catch ( Error &e ) {
	set_mime_text( cout, dods_error, cgi_version ) ;
	e.print( cout ) ;
	return 1 ;
    }

    return 0 ;
}

// $Log: cdf_handler.cc,v $
// Revision 1.2  2004/07/02 20:10:19  pwest
// Added dataset name to the dds, updated the INSTALL, fixed configure.in
//
// Revision 1.1  2003/05/22 18:07:16  pwest
// moved from cdf_dods to cdf_handler using new filter methods
//

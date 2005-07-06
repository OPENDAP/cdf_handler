
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
 
#include <iostream>

using std::cerr ;
using std::endl ;

#include "DODSCgi.h"
#include "DODSFilter.h"
#include "DODSGlobalIQ.h"
#include "DODSException.h"
#include "config_cdf.h"

int 
main(int argc, char *argv[])
{
    try
    {
	putenv( "DODS_INI=/project/cedar/bin/apache_1.3.29/conf/opendap.ini" ) ;
	DODSGlobalIQ::DODSGlobalInit( argc, argv ) ;
    }
    catch( DODSException &e )
    {
	cerr << "Error initializing application" << endl ;
	cerr << "    " << e.get_error_description() << endl ;
	return 1 ;
    }

    DODSFilter df(argc, argv);

    DODSCgi d( "cdf", df ) ;
    d.execute_request() ;

    DODSGlobalIQ::DODSGlobalQuit() ;

    return 0;
}

// $Log: cdf_handler.cc,v $
// Revision 1.2  2004/07/02 20:10:19  pwest
// Added dataset name to the dds, updated the INSTALL, fixed configure.in
//
// Revision 1.1  2003/05/22 18:07:16  pwest
// moved from cdf_dods to cdf_handler using new filter methods
//

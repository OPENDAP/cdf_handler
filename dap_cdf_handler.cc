
// -*- mode: c++; c-basic-offset:4 -*-

// This file is part of cdf_handler, a data handler for the OPeNDAP data
// server. 

// Copyright (c) 2002,2003 OPeNDAP, Inc.
// Author: James Gallagher <jgallagher@opendap.org>
//
// This is free software; you can redistribute it and/or modify it under the
// terms of the GNU Lesser General Public License as published by the Free
// Software Foundation; either version 2.1 of the License, or (at your
// option) any later version.
// 
// This software is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
// License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// You can contact OPeNDAP, Inc. at PO Box 112, Saunderstown, RI. 02874-0112.
 
#include "config_cdf.h"

#include <iostream>
#include <string>

#include "DODSFilter.h"
#include "DDS.h"
#include "DAS.h"
#include "DataDDS.h"

#include "ObjectType.h"
#include "cgi_util.h"
#include "ConstraintEvaluator.h"

#include "CDFTypeFactory.h"
#include "CDFreadAttributes.h"
#include "CDFreadDescriptors.h"
#include "CDFDebug.h"

const string cgi_version = PACKAGE_VERSION;

int 
main(int argc, char *argv[])
{
    CDFDebug::off() ;

    CDFTypeFactory *cdftf = new CDFTypeFactory;

    try { 
	DODSFilter df(argc, argv);
	if (df.get_cgi_version() == "")
	    df.set_cgi_version(cgi_version);

	switch (df.get_response()) {
	  case DODSFilter::DAS_Response: {
	    DAS das;

	    readAttributes(das, df.get_dataset_name());
	    df.read_ancillary_das(das);
	    df.send_das(das);
	    break;
	  }

	  case DODSFilter::DDS_Response: {
	    DDS dds(cdftf);
	    ConstraintEvaluator ce ;

	    string dsn = df.get_dataset_name();
	    readDescriptors( dds, dsn, name_path( dsn ) ) ;
	    df.read_ancillary_dds(dds);
	    df.send_dds(dds, ce, true);
	    break;
	  }

	  case DODSFilter::DataDDS_Response: {
	    DDS dds(cdftf);
	    ConstraintEvaluator ce ;

	    string dsn = df.get_dataset_name();
	    dds.filename( dsn ) ;
	    readDescriptors( dds, dsn, name_path( dsn ) ) ;
	    df.read_ancillary_dds(dds);
	    df.send_data(dds, ce, stdout);
	    break;
	  }

	  case DODSFilter::DDX_Response: {
	    DDS dds(cdftf);
	    DAS das;
	    ConstraintEvaluator ce ;

	    string dsn = df.get_dataset_name();

	    dds.filename( dsn ) ;

	    readDescriptors(dds, dsn, name_path( dsn ) ) ;
	    df.read_ancillary_dds(dds);

	    readAttributes( das, dsn ) ;
	    df.read_ancillary_das(das);

	    dds.transfer_attributes(&das);

	    df.send_ddx(dds, ce, stdout);
	    break;
	  }

	  case DODSFilter::Version_Response: {
	    df.send_version_info();

	    break;
	  }

	  default:
	    df.print_usage();	// Throws Error
	}
    }
    catch (Error &e) {
	delete cdftf; cdftf = 0;
	set_mime_text(stdout, dods_error, cgi_version);
	e.print(stdout);
	return 1;
    }

    delete cdftf; cdftf = 0;
    return 0;
}

// $Log: dap_cdf_handler.cc,v $
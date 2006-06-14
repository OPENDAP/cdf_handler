// cdf_module.cc

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
 
// modules required by the cdf-handler for use with OPeNDAP BES
//
// pwest 05/08/03

#ifdef __GNUG__
#pragma implementation
#endif

#include <iostream>

using std::endl ;

#include "BESInitList.h"
#include "BESRequestHandlerList.h"
#include "CDFRequestHandler.h"
#include "CDFResponseNames.h"
#include "BESLog.h"

static bool
CDFInit(int, char**)
{
    if( BESLog::TheLog()->is_verbose() )
	(*BESLog::TheLog()) << "Initializing CDF:" << endl ;

    if( BESLog::TheLog()->is_verbose() )
	(*BESLog::TheLog()) << "    adding " << CDF_NAME << " request handler" << endl ;
    BESRequestHandlerList::TheList()->add_handler( CDF_NAME, new CDFRequestHandler( CDF_NAME ) ) ;

    return true ;
}

static bool
CDFTerm(void)
{
    if( BESLog::TheLog()->is_verbose() )
	(*BESLog::TheLog()) << "Removing CDF Handlers" << endl;
    BESRequestHandler *rh = BESRequestHandlerList::TheList()->remove_handler( CDF_NAME ) ;
    if( rh ) delete rh ;
    return true ;
}

FUNINITQUIT( CDFInit, CDFTerm, 3 ) ;

// $Log: cdf_module.cc,v $

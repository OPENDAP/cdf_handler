// CDFHandlerApp.cc

#include <signal.h>
#include <unistd.h>

#include <iostream>

using std::cout ;
using std::cerr ;
using std::endl ;
using std::flush ;

#include "CDFHandlerApp.h"
#include "CDFResponseNames.h"
#include "DODSFilter.h"
#include "DODSCgi.h"

CDFHandlerApp::CDFHandlerApp()
    : _df( 0 )
{
}

CDFHandlerApp::~CDFHandlerApp()
{
    if( _df )
    {
	delete _df ;
	_df = 0 ;
    }
}

int
CDFHandlerApp::initialize( int argc, char **argv )
{
    OPeNDAPBaseApp::initialize( argc, argv ) ;

    _df = new DODSFilter( argc, argv ) ;

    return 0 ;
}

int
CDFHandlerApp::run()
{
    DODSCgi d( CDF_NAME, *_df ) ;
    d.execute_request() ;

    return 0 ;
}


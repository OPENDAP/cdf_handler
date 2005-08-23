// cdf_module.cc

#include <iostream>

using std::endl ;

#include "DODSInitList.h"
#include "TheRequestHandlerList.h"
#include "CDFRequestHandler.h"
#include "CDFResponseNames.h"
#include "TheDODSLog.h"

static bool
CDFInit(int, char**)
{
    if( TheDODSLog->is_verbose() )
	(*TheDODSLog) << "Initializing CDF:" << endl ;

    if( TheDODSLog->is_verbose() )
	(*TheDODSLog) << "    adding " << CDF_NAME << " request handler" << endl ;
    TheRequestHandlerList->add_handler( CDF_NAME, new CDFRequestHandler( CDF_NAME ) ) ;

    return true ;
}

static bool
CDFTerm(void)
{
    if( TheDODSLog->is_verbose() )
	(*TheDODSLog) << "Removing CDF Handlers" << endl;
    DODSRequestHandler *rh = TheRequestHandlerList->remove_handler( CDF_NAME ) ;
    if( rh ) delete rh ;
    return true ;
}

FUNINITQUIT( CDFInit, CDFTerm, 3 ) ;


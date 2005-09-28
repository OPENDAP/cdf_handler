// cdf_module.cc

#include <iostream>

using std::endl ;

#include "DODSInitList.h"
#include "DODSRequestHandlerList.h"
#include "CDFRequestHandler.h"
#include "CDFResponseNames.h"
#include "DODSLog.h"

static bool
CDFInit(int, char**)
{
    if( DODSLog::TheLog()->is_verbose() )
	(*DODSLog::TheLog()) << "Initializing CDF:" << endl ;

    if( DODSLog::TheLog()->is_verbose() )
	(*DODSLog::TheLog()) << "    adding " << CDF_NAME << " request handler" << endl ;
    DODSRequestHandlerList::TheList()->add_handler( CDF_NAME, new CDFRequestHandler( CDF_NAME ) ) ;

    return true ;
}

static bool
CDFTerm(void)
{
    if( DODSLog::TheLog()->is_verbose() )
	(*DODSLog::TheLog()) << "Removing CDF Handlers" << endl;
    DODSRequestHandler *rh = DODSRequestHandlerList::TheList()->remove_handler( CDF_NAME ) ;
    if( rh ) delete rh ;
    return true ;
}

FUNINITQUIT( CDFInit, CDFTerm, 3 ) ;


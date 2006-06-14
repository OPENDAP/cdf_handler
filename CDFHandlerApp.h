#ifndef I_CDFHandlerApp_h
#define I_CDFHandlerApp_h 1

#include "BESBaseApp.h"

class DODSFilter ;

class CDFHandlerApp : public BESBaseApp
{
private:
    DODSFilter *		_df ;
public:
    				CDFHandlerApp() ;
    virtual			~CDFHandlerApp() ;
    virtual int			initialize( int argc, char **argv ) ;
    virtual int			run() ;
} ;

#endif // I_CDFHandlerApp_h


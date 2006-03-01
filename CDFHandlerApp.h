#ifndef I_OPeNDAPBaseApp_h
#define I_OPeNDAPBaseApp_h 1

#include "OPeNDAPBaseApp.h"

class DODSFilter ;

class CDFHandlerApp : public OPeNDAPBaseApp
{
private:
    DODSFilter *		_df ;
public:
    				CDFHandlerApp() ;
    virtual			~CDFHandlerApp() ;
    virtual int			initialize( int argc, char **argv ) ;
    virtual int			run() ;
} ;

#endif // I_OPeNDAPBaseApp_h


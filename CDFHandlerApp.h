// CDFHandlerApp.h

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


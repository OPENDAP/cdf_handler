// CDFRequestHandler.h

#ifndef I_CDFRequestHandler_H
#define I_CDFRequestHandler_H 1

#include "DODSRequestHandler.h"

class CDFRequestHandler : public DODSRequestHandler {
public:
			CDFRequestHandler( string name ) ;
    virtual		~CDFRequestHandler( void ) ;

    static bool		cdf_build_das( DODSDataHandlerInterface &dhi ) ;
    static bool		cdf_build_dds( DODSDataHandlerInterface &dhi ) ;
    static bool		cdf_build_data( DODSDataHandlerInterface &dhi ) ;
    static bool		cdf_build_help( DODSDataHandlerInterface &dhi ) ;
    static bool		cdf_build_version( DODSDataHandlerInterface &dhi ) ;
};

#endif


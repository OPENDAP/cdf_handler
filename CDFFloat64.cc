#include "config_cdf.h"

#include <cdf.h>

#include "CDFFloat64.h"
#include "CDFutilities.h"
#include "CDFDebug.h"

CDFFloat64::CDFFloat64(const string &n) : Float64(n)
{
}

CDFFloat64::CDFFloat64(const CDFFloat64 &rhs) : Float64(rhs)
{
}

CDFFloat64::~CDFFloat64()
{
}

CDFFloat64 &
CDFFloat64::operator=(const CDFFloat64 &rhs)
{
    if (this == &rhs)
        return *this;

    dynamic_cast<CDFFloat64&>(*this) = rhs;

    return *this;
}

BaseType *
CDFFloat64::ptr_duplicate()
{
    return new CDFFloat64(*this);
}
 

bool
CDFFloat64::read(const string &dataset)
{
    if( read_p( ) )
    {
	return false ;
    }

    CDFid id;            /* CDF identifier. */
    CDFstatus status;    /* CDF completion status. */

    long anindex = 0 ;

    long varNum = 0 ;
    long varType = 0 ;
    long varTypeSize = 0 ;
    long numDims = 0 ;
    long dimSizes[CDF_MAX_DIMS] ;
    long dimVarys[CDF_MAX_DIMS] ;
    long maxRec = 0 ;
    long numRecs = 0 ;
    long recVary = 0 ;
    long numElems = 0 ;

    void *cdf_buf = 0 ;
    void *arrbuf = 0 ;

    long indices[CDF_MAX_DIMS] ;
    long intervals[CDF_MAX_DIMS] ;
    long counts[CDF_MAX_DIMS] ;

    for( anindex = 0; anindex < CDF_MAX_DIMS; anindex++ )
    {
	indices[anindex] = 0 ;
	intervals[anindex] = 1 ;
	counts[anindex] = 0 ;
    }

    /*************************************************************************
    * Open the CDF.
    *************************************************************************/
    if( CDFDebug::debug() )
    {
	cout << "CDFFloat64: " << name() << ": Opening " << dataset << endl ;
    }
    status = CDFopen ( dataset.c_str(), &id ) ;
    if ( status != CDF_OK )
    {
	if( CDFutilities::StatusHandler( status, __FILE__, __LINE__ ) == false )
	{
	    return false ;
	}
    }

    /*************************************************************************
    * Set to readonly and zmode
    *************************************************************************/
    status = CDFlib( SELECT_, CDF_READONLY_MODE_, READONLYon,
			      CDF_zMODE_, zMODEon2,
		     NULL_ ) ;

    if ( status != CDF_OK )
    {
	if( CDFutilities::StatusHandler( status, __FILE__, __LINE__ ) == false )
	{
	    return false ;
	}
    }

    /*************************************************************************
    * Read variables by hyper read
    *************************************************************************/
    status = CDFlib( GET_, zVAR_NUMBER_, name().c_str(), &varNum,
		     NULL_ ) ;
    if ( status != CDF_OK )
    {
	if( CDFutilities::StatusHandler( status, __FILE__, __LINE__ ) == false )
	{
	    return false ;
	}
    }

    status = CDFlib( SELECT_, zVAR_, varNum, NULL_ ) ;
    if ( status != CDF_OK )
    {
	if( CDFutilities::StatusHandler( status, __FILE__, __LINE__ ) == false )
	{
	    return false ;
	}
    }

    status = CDFlib( GET_, zVAR_DATATYPE_, &varType,
			   zVAR_NUMDIMS_, &numDims,
			   zVAR_DIMSIZES_, dimSizes,
			   zVAR_DIMVARYS_, dimVarys,
			   zVAR_MAXREC_, &maxRec,
			   zVAR_NUMRECS_, &numRecs,
			   zVAR_RECVARY_, &recVary,
			   zVAR_NUMELEMS_, &numElems,
		     NULL_ ) ;
    if ( status != CDF_OK )
    {
	if( CDFutilities::StatusHandler( status, __FILE__, __LINE__ ) == false )
	{
	    return false ;
	}
    }
    if( CDFDebug::debug() )
    {
	cout << "  varType = " << CDFutilities::DataType( varType ) << endl ;
	cout << "  numDims = " << numDims << endl ;
	cout << "  maxRec = " << maxRec << endl ;
	cout << "  numRecs = " << numRecs << endl ;
	cout << "  recVary = " << recVary << endl ;
	cout << "  numElems = " << numElems << endl ;
    }

    if( varType != CDF_REAL8 && varType != CDF_DOUBLE && varType != CDF_EPOCH )
    {
	cerr << "CDFFloat64: the type of data should be "
	     << "CDF_REAL8, CDF_DOUBLE or CDF_EPOCH," << endl ;
    }

    if( numDims != 0 )
    {
	cerr << "CDFFloat64: number of dimensions should be 0" << endl ;
	return false ;
    }

    if( numRecs > 1 )
    {
	cerr << "CDFFloat64: number of records should be 1" << endl ;
	return false ;
    }

    status = CDFlib( GET_, DATATYPE_SIZE_, varType, &varTypeSize,
		     NULL_ ) ;
    if ( status != CDF_OK )
    {
	if( CDFutilities::StatusHandler( status, __FILE__, __LINE__ ) == false )
	{
	    return false ;
	}
    }
    if( CDFDebug::debug() )
    {
	cout << "  varTypeSize = " << varTypeSize << endl ;
    }

    cdf_buf = malloc( varTypeSize ) ;
    arrbuf = (void *)&_buf ;

    if( !numRecs ) numRecs = 1 ;
    status = CDFlib( SELECT_, zVAR_RECCOUNT_, (long)1,
			      zVARs_RECNUMBER_, 0,
			      zVAR_RECINTERVAL_, (long)1,
			      zVAR_DIMINDICES_, indices,
			      zVAR_DIMCOUNTS_, counts,
			      zVAR_DIMINTERVALS_, intervals,
		     NULL_ ) ;
    if ( status != CDF_OK )
    {
	if( CDFutilities::StatusHandler( status, __FILE__, __LINE__ ) == false )
	{
	    return false ;
	}
    }

    status = CDFlib( GET_, zVAR_HYPERDATA_, cdf_buf,
		     NULL_ ) ;
    if ( status != CDF_OK )
    {
	if( CDFutilities::StatusHandler( status, __FILE__, __LINE__ ) == false )
	{
	    return false ;
	}
    }

    numElems = numRecs*numElems ;
    unsigned int arrindex = 0 ;
    CDFutilities::read_record( cdf_buf, arrbuf, arrindex,
                               numElems, varType, numElems ) ;
    if( CDFDebug::debug() )
    {
	cout << "  _buf = " << _buf << endl ;
    }

    /*************************************************************************
    * Close CDF.
    *************************************************************************/
    CDFclose ( id ) ;

    set_read_p( true ) ;
    return false ;
}


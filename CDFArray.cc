#include "config_cdf.h"

#include <cdf.h>

#include "CDFArray.h"
#include "CDFutilities.h"
#include "CDFDebug.h"

BaseType *
CDFArray::ptr_duplicate()
{
    return new CDFArray(*this);
}

CDFArray::CDFArray(const string &n, BaseType *v) : Array(n, v)
{
}

CDFArray::CDFArray(const CDFArray &rhs) : Array(rhs)
{
}

CDFArray::~CDFArray()
{
}

CDFArray &
CDFArray::operator=(const CDFArray &rhs)
{
    if (this == &rhs)
        return *this;

    dynamic_cast<Array &>(*this) = rhs;

    return *this;
}

bool
CDFArray::read(const string &dataset)
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
	cout << "CDFArray: " << name() << ": Opening " << dataset << endl ;
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

    status = CDFlib( SELECT_, zVAR_, varNum,
                     NULL_ ) ;
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
	for( unsigned int i_numDims = 0; i_numDims < numDims; i_numDims++ )
	{
	    cout << "    dimSizes[" << i_numDims << "] = " << dimSizes[i_numDims] << endl ;
	    if( dimVarys[i_numDims] == VARY )
		cout << "    dimVarys[" << i_numDims << "] = VARY" << endl ;
	    else
		cout << "    dimVarys[" << i_numDims << "] = NOVARY" << endl ;
	}
	cout << "  maxRec = " << maxRec << endl ;
	cout << "  numRecs = " << numRecs << endl ;
	if( recVary == VARY ) cout << "  recVary = VARY" << endl ;
	else cout << "  recVary = VARY" << endl ;
	cout << "  numElems = " << numElems << endl ;
    }
    if( numRecs == 0 )
    {
	numRecs = 1 ;
    }

    if( varType == CDF_CHAR || varType == CDF_UCHAR )
    {
	varTypeSize = numElems ;
    } else {
	status = CDFlib( GET_, DATATYPE_SIZE_, varType, &varTypeSize,
			 NULL_ ) ;
	if ( status != CDF_OK )
	{
	    if( CDFutilities::StatusHandler( status, __FILE__, __LINE__ ) == false )
	    {
		return false ;
	    }
	}
    }

    unsigned long relements = 1 ;
    for( anindex = 0; anindex < numDims; anindex++ )
    {
	if( dimVarys[anindex] )
	{
	    counts[anindex] = dimSizes[anindex] ;
	}
	else
	{
	    counts[anindex] = 1 ;
	}
	if( CDFDebug::debug() )
	{
	    cout << "  counts[" << anindex << "] = " << counts[anindex] << endl;
	}
	relements = relements * counts[anindex] ;
    }
    if( CDFDebug::debug() )
    {
	cout << "  relements = " << relements << endl ;
    }

    // telements is the total number of elements to read
    unsigned long telements = relements * numRecs ;
    if( CDFDebug::debug() )
    {
	cout << "  telements = " << telements << endl ;
    }

    // rsize is the record size given the number of elements per record and
    // the size of each element
    unsigned long rsize = relements * varTypeSize ;
    if( CDFDebug::debug() )
    {
	cout << "  rsize = " << rsize << endl ;
    }

    // allocate the buffer that will read each of the records
    cdf_buf = malloc( rsize+1 ) ;

    // allocate the dods buffer that will hold all the data
    void *arrbuf = 0 ;
    switch( varType )
    {
	case CDF_BYTE:
	    arrbuf = (void *)(new dods_byte[telements]);
	    break ;
	case CDF_UCHAR:
	case CDF_CHAR:
	    arrbuf = (void *)(new string[telements]);
	    break ;
	case CDF_INT1:
	case CDF_INT2:
	case CDF_INT4:
	    arrbuf = (void *)(new dods_int32[telements]);
	    break ;
	case CDF_UINT1:
	case CDF_UINT2:
	case CDF_UINT4:
	    arrbuf = (void *)(new dods_uint32[telements]);
	    break ;
	case CDF_REAL4:
	case CDF_FLOAT:
	    arrbuf = (void *)(new dods_float32[telements]);
	    break ;
	case CDF_REAL8:
	case CDF_DOUBLE:
	case CDF_EPOCH:
	    arrbuf = (void *)(new dods_float64[telements]);
	    break ;
    }
    fflush( stdout ) ;

    status = CDFlib( SELECT_, zVAR_RECCOUNT_, (long)1,
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
    unsigned int arrindex = 0 ;
    for( anindex = 0; anindex < numRecs; anindex++ )
    {

	status = CDFlib( SELECT_, zVARs_RECNUMBER_, anindex,
	                 GET_, zVAR_HYPERDATA_, cdf_buf,
			 NULL_ ) ;
	if ( status != CDF_OK )
	{
	    if( CDFutilities::StatusHandler( status, __FILE__, __LINE__ ) == false )
	    {
		return false ;
	    }
	}

	CDFutilities::read_record( cdf_buf, arrbuf, arrindex, relements,
				   varType, numElems ) ;
    }

    if( CDFDebug::debug() )
    {
	CDFutilities::write_record( arrbuf, telements, varType ) ;
    }

    val2buf( arrbuf ) ;

    /*************************************************************************
    * Close CDF.
    *************************************************************************/
    CDFclose ( id ) ;

    set_read_p( true ) ;
    return false ;
}


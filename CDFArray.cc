
/*
  Copyright 1995 The University of Rhode Island and The Massachusetts
  Institute of Technology

  Portions of this software were developed by the Graduate School of
  Oceanography (GSO) at the University of Rhode Island (URI) in collaboration
  with The Massachusetts Institute of Technology (MIT).

  Access and use of this software shall impose the following obligations and
  understandings on the user. The user is granted the right, without any fee
  or cost, to use, copy, modify, alter, enhance and distribute this software,
  and any derivative works thereof, and its supporting documentation for any
  purpose whatsoever, provided that this entire notice appears in all copies
  of the software, derivative works and supporting documentation.  Further,
  the user agrees to credit URI/MIT in any publications that result from the
  use of this software or in any product that includes this software. The
  names URI, MIT and/or GSO, however, may not be used in any advertising or
  publicity to endorse or promote any products or commercial entity unless
  specific written permission is obtained from URI/MIT. The user also
  understands that URI/MIT is not obligated to provide the user with any
  support, consulting, training or assistance of any kind with regard to the
  use, operation and performance of this software nor to provide the user
  with any updates, revisions, new versions or "bug fixes".

  THIS SOFTWARE IS PROVIDED BY URI/MIT "AS IS" AND ANY EXPRESS OR IMPLIED
  WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
  EVENT SHALL URI/MIT BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL
  DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
  PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTUOUS
  ACTION, ARISING OUT OF OR IN CONNECTION WITH THE ACCESS, USE OR PERFORMANCE
  OF THIS SOFTWARE.
*/

// Convert to CDF server
//
// csf 4/19/96

// Implementation for TestArray. See TestByte.cc
//
// jhrg 1/12/95

// $Log: CDFArray.cc,v $
// Revision 1.1  1996/06/10 20:31:56  jimg
// Added.
//
// Revision 1.7  1995/07/09  21:29:07  jimg
// Added copyright notice.
//
// Revision 1.6  1995/05/10  13:45:34  jimg
// Changed the name of the configuration header file from `config.h' to
// `config_dap.h' so that other libraries could have header files which were
// installed in the DODS include directory without overwriting this one. Each
// config header should follow the convention config_<name>.h.
//
// Revision 1.5  1995/03/16  17:32:27  jimg
// Fixed bugs with read()s new & delete calls.
//
// Revision 1.4  1995/03/04  14:38:00  jimg
// Modified these so that they fit with the changes in the DAP classes.
//
// Revision 1.3  1995/02/10  02:33:37  jimg
// Modified Test<class>.h and .cc so that they used to new definitions of
// read_val().
// Modified the classes read() so that they are more in line with the
// class library's intended use in a real subclass set.
//
// Revision 1.2  1995/01/19  21:58:50  jimg
// Added read_val from dummy_read.cc to the sample set of sub-class
// implementations.
// Changed the declaration of readVal in BaseType so that it names the
// mfunc read_val (to be consistant with the other mfunc names).
// Removed the unnecessary duplicate declaration of the abstract virtual
// mfuncs read and (now) read_val from the classes Byte, ... Grid. The
// declaration in BaseType is sufficient along with the decl and definition
// in the *.cc,h files which contain the subclasses for Byte, ..., Grid.
//
// Revision 1.1  1995/01/19  20:20:34  jimg
// Created as an example of subclassing the class hierarchy rooted at
// BaseType.
//

#ifdef __GNUG__
#pragma implementation
#endif

#include "config_dap.h"

#include <assert.h>
#include <stdio.h>

#include "netio.h"
#include "CDFArray.h"
#include "cdf.h"
#include "cdfUtil.h"

#ifdef TRACE_NEW
#include "trace_new.h"
#endif

Array *
NewArray(const String &n, BaseType *v)
{
    return new CDFArray(n, v);
}

BaseType *
CDFArray::ptr_duplicate()
{
    return new CDFArray(*this);
}

CDFArray::CDFArray(const String &n, BaseType *v) : Array(n, v)
{
}

CDFArray::~CDFArray()
{
}

/*****************************************************************************
  parse hyperslab and coordinate point location from the constraint expr.
*****************************************************************************/
long
Get_constraint(int num_dim, String constraint, long *cor, long *edg)
{
  long nels = 1;
	
  // Contraints not yet implemented
  return -1;

    if ((!(constraint.contains("hyper")) || constraint.contains("point"))
		|| ((constraint.contains("hyper"))
			&&(num_dim*2 != constraint.freq(",")+1))
		|| ((constraint.contains("point"))
			&&(num_dim != constraint.freq(",")+1)))
	  return -1;

  String cons_buf = (String) constraint.after("(");
    
  int id;
  for (id = 0; id < num_dim; id++){
	if((id+1 == num_dim)&&(constraint.contains("point"))){
	  cor[id] = atol((const char*)(String)cons_buf.before(")"));
	}
	else {
	  cor[id] = atol((const char*)(String)cons_buf.before(","));
	  cons_buf = (String) cons_buf.after(",");
	}
  }

  if (constraint.contains("hyper")){ // get edge values for hyper case
	for (id = 0; id < num_dim; id++) {
	  if(id+1 == num_dim){
		edg[id] = atol((const char*)(String)cons_buf.before(")"));
	  }
	  else {
		edg[id] = atol((const char*)(String)cons_buf.before(","));
		cons_buf = (String) cons_buf.after(",");
	  }
	  nels *= edg[id];      // total number of values for variable
	}
  }

  return nels;
}
/****************************************************************************
This routine returns the length in bytes of the dods type converted
from the the standard CDF types
****************************************************************************/
int
dodsTypeLen(long cdfType)
{
  switch (cdfType) {

	// byte or string
  case CDF_BYTE:	
	return 1;

  case CDF_CHAR:	
  case CDF_UCHAR:	
	//kpvt kluge	return sizeof(String*);
	return 1;

	// int32
  case CDF_INT1:
  case CDF_UINT1:
  case CDF_INT2:
  case CDF_UINT2:	
  case CDF_INT4:
  case CDF_UINT4:	
	return 4;

	// float64
  case CDF_REAL4:	
  case CDF_FLOAT:	
  case CDF_REAL8:	
  case CDF_DOUBLE:
  case CDF_EPOCH:	
	return 8;
  }

  // Type not found
  return 0;
}

/*****************************************************************************
  Read CDF file and load DODS model
*****************************************************************************/
bool
CDFArray::read(String fileName, String varName, String constraint)
{
  CDFid cdfid;
  CDFstatus status;
  long numDims, numVars, numAtts, encoding, majority, maxRec;
  long cdfDimSizes[CDF_MAX_DIMS];
  long varType;
  long recVary;
  long dimVarys[CDF_MAX_DIMS];
  long numElts;
  char varNameO[CDF_VAR_NAME_LEN+1];

  char Msgt[255];

  long rcdEdge;
  long corner[CDF_MAX_DIMS];      /* corner coordinates */
  long edges[CDF_MAX_DIMS];       /* lengths edges of hypercube */
  long intervals[CDF_MAX_DIMS];   /* intervals of hypercube */

  // Initialize the hyper get parms
  for (int i = 0; i<CDF_MAX_DIMS; i++) {
	corner[i] = 0;
	edges[i] = 0;
	intervals[i] = 0;
  }

  status = CDFopen(fileName.chars(), &cdfid);
  if (status != CDF_OK) {
	sprintf (Msgt,"cdfArray: Could not open file %s",fileName.chars());
	ErrMsgT(Msgt);
	return false;
  }

  // how many variables? how many global attributes? 
  if (CDFinquire(cdfid, &numDims, cdfDimSizes, &encoding, &majority, 
				 &maxRec, &numVars, &numAtts) != CDF_OK) {
	ErrMsgT("CDFArray: Could not inquire about CDF file");
	return false;
  }

  long varId = CDFvarNum(cdfid, (char*)varName.chars());
  if (CDFvarInquire(cdfid, varId,
					varNameO, &varType, &numElts, &recVary, dimVarys) == -1) {
	sprintf (Msgt,"CDFArray: Could not get variable number %d", varId);
	ErrMsgT(Msgt);
	return false;
  }

  // kpvt kluge
  // set the dim size for the reused dim needed by the kpvt tools
  int kpvtDimSize;
  if (numElts > 3) 
	kpvtDimSize = 3;
  else
	kpvtDimSize = numElts;


  long nels = Get_constraint(numDims, constraint, corner, edges);
  //without hyper get 
  if(nels == -1){

	// Set nbr of elts and the extent in the record dimension
	if (recVary) {
	  nels = maxRec+1;
	  rcdEdge = maxRec+1;
	} 
	else {
	  nels = 1;
	  rcdEdge = 1;
	}
	
	for (int id = 0; id < numDims; id++) {
	  corner[id] = 0;
	  edges[id] = cdfDimSizes[id];
	  intervals[id] = 1;
	  nels *= cdfDimSizes[id];    /* total number of values for variable */
	}
  }
  else {
	const_length(nels);  //set size for transmition
  }

#ifdef NEVER
  void *convbuf = malloc(nels*cdfTypeLen(datatype));
#endif

  // Allocate CDF buffer and assign to pointer to all CDF types
  void * cdfBuf = new char [(numElts*nels*cdfTypeLen(varType))];
  char * cdfChar           = (char*)cdfBuf;
  unsigned char * cdfUChar = (unsigned char*)cdfBuf;
  int * cdfInt             = (int*)cdfBuf;
  unsigned int * cdfUInt   = (unsigned int*)cdfBuf;
  long * cdfLong           = (long int*)cdfBuf;
  unsigned long * cdfULong = (unsigned long*)cdfBuf;
  float * cdfFloat         = (float*)cdfBuf;
  double * cdfDouble       = (double*)cdfBuf;

  // Allocate CDF buffer and assign to pointer to all DODS types
  // kpvt kluge  void * dodsBuf = new char [(nels*dodsTypeLen(varType))];
  void * dodsBuf = new char [(kpvtDimSize*nels*dodsTypeLen(varType))];
  byte * dodsByte          = (byte*)dodsBuf;
  String ** dodsString      = (String**)dodsBuf;
  int32 * dodsInt32        = (int32*)dodsBuf;
  float64 * dodsFloat64    = (float64*)dodsBuf;

  // Retrieve CDF data into CDF buffer
  status = CDFvarHyperGet (cdfid, varId, 
						   0, rcdEdge, 1,
						   corner, edges, intervals, cdfBuf);
  if (status != CDF_OK) {
	sprintf (Msgt,"cdfArray: Could not read variable.  File %s, Variable %s",
			 fileName.chars(), varNameO);
	ErrMsgT(Msgt);
	return false;
  }

  // Allocate temporary string
  char* tempStr = new char[numElts+1];

  String * ds;

  // Copy CDF buffer to Dods Buffer
  switch (varType) {
  case CDF_BYTE: 	
	for (int i = 0; i<nels; i++) 
	  dodsByte[i] = (byte)cdfChar[i];
	break;

  case CDF_CHAR:	
  case CDF_UCHAR:	
	// Strings have not been work out entirely
	// This includes the kpvt kluge

	/** kpvt kluge
	for (int i = 0; i<nels; i++) {
	  memcpy(tempStr, &(cdfChar[(i*numElts)]), numElts);
	  tempStr[numElts] = '\0';
	  ds = new String(tempStr);
	  //cout << *ds << endl;
	  dodsString[i] = ds;
	}
	**/
	//for (int i = 0; i<nels; i++) {
	//  cout << *dodsString[i] << endl;
	//}

	// kpvt kluge
	// This is the code to copy cdf char arrays to the kpvt char array in dods
	for (int i = 0; i<nels; i++) {
	  int cdfIdx = i*numElts;
	  int dodsIdx = i*kpvtDimSize;

	  for (int j = 0; j<kpvtDimSize; j++) {
		dodsByte[dodsIdx+j] = (byte)cdfChar[cdfIdx+j];
	  }
	}

	break;

  case CDF_INT1: 
	for (int i = 0; i<nels; i++)
	  dodsInt32[i] = (int32)cdfChar[i];
	break;

  case CDF_UINT1:
	for (int i = 0; i<nels; i++)
	  dodsInt32[i] = (int32)cdfUChar[i];
	break;

  case CDF_INT2:
	for (int i = 0; i<nels; i++)
	  dodsInt32[i] = (int32)cdfInt[i];
	break;

  case CDF_UINT2:	
	for (int i = 0; i<nels; i++)
	  dodsInt32[i] = (int32)cdfUInt[i];
	break;

  case CDF_INT4:
	for (int i = 0; i<nels; i++)
	  dodsInt32[i] = (int32)cdfLong[i];
	break;

  case CDF_UINT4:	
	for (int i = 0; i<nels; i++)
	  dodsInt32[i] = (int32)cdfULong[i];
	break;

  case CDF_REAL4:	
  case CDF_FLOAT:	
	for (int i = 0; i<nels; i++) 
	  dodsFloat64[i] = (float64)cdfFloat[i];
	break;

  case CDF_REAL8:	
  case CDF_DOUBLE:
  case CDF_EPOCH:	
	for (int i = 0; i<nels; i++)
	  dodsFloat64[i] = (float64)cdfDouble[i];
	break;
  }

  // Store dods buffer
  store_val(dodsBuf,FALSE);

  // Free buffers an close file
  delete [] tempStr;
  delete [] cdfBuf;
  delete [] dodsBuf;
  (void) CDFclose(cdfid);  

  return true;
}


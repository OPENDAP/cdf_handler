
// -*- C++ -*-

// TestByte is a demonstration of subclassing a class in the hierarchy of
// DODS data types. It does not do much of anything - a real subclass would
// add specifics for a API or format (e.g., a read mfunc for netcdf, HDF,
// ...).  The class is used by some of the test code (hence the name) as well
// as serving as a template for others who need to subclass the hierarchy. 
//
// NB: since the class Byte is an abstract class (as are all the other
// `variable type' classes), the hierarchy *must* be subclassed in order to
// be used.
//
// jhrg 1/12/95

/* $Log: CDFByte.h,v $
/* Revision 1.1  1996/06/10 20:33:37  jimg
/* Added.
/*
 * Revision 1.3  1995/02/10  02:33:52  jimg
 * Modified Test<class>.h and .cc so that they used to new definitions of
 * read_val().
 * Modified the classes read() so that they are more in line with the
 * class library's intended use in a real subclass set.
 *
 * Revision 1.2  1995/01/19  21:59:30  jimg
 * Added read_val from dummy_read.cc to the sample set of sub-class
 * implementations.
 * Changed the declaration of readVal in BaseType so that it names the
 * mfunc read_val (to be consistant with the other mfunc names).
 * Removed the unnecessary duplicate declaration of the abstract virtual
 * mfuncs read and (now) read_val from the classes Byte, ... Grid. The
 * declaration in BaseType is sufficient along with the decl and definition
 * in the *.cc,h files which contain the subclasses for Byte, ..., Grid.
 *
 * Revision 1.1  1995/01/19  20:20:38  jimg
 * Created as an example of subclassing the class hierarchy rooted at
 * BaseType.
 *
 */

#ifndef _CDFByte_h
#define _CDFByte_h 1

#ifdef __GNUG__
#pragma interface
#endif

#include "Byte.h"
extern Byte * NewByte(const String &n = (char *)0);

class CDFByte: public Byte {
public:
    CDFByte(const String &n = (char *)0);
    virtual ~CDFByte() {}

    virtual BaseType *ptr_duplicate();

    virtual bool read(String dataset, String var_name, String constraint);
};

typedef CDFByte * CDFBytePtr;

#endif


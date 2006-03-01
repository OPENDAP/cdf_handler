#ifndef S_CDFDebug_h
#define S_CDFDebug_h 1

class CDFDebug
{
private:
    static bool			_debug ;
public:
    static void			on( ) { _debug = true ; }
    static void			off( ) { _debug = false ; }
    static bool			debug( ) { return _debug ; }
} ;

#endif // S_CDFDebug_h


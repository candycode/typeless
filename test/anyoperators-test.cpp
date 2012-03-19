//#SRCHEADER#
#include <cassert>
#include <stdexcept>
#include <cmath>
#include <complex>
#include <map>
#define ANY_OSTREAM
#include <Any.h>


// specialize for complex float to allow any1 < any2 and any1 == any2 expressions
// note that due to Koenig lookup functions have to belong to the same namespace
// as their parameter types(at least on g++).
namespace std {
bool AnyLess( const std::complex< float >& c1, const std::complex< float >& c2 ) {
    return std::abs( c1 ) < std::abs( c2 ); 
}

bool AnyEqual( const std::complex< float >& c1, const std::complex< float >& c2 ) {
    return std::abs( c1 ) == std::abs( c2 ); 
}
}
Any AnyGen() { return 3.0; }

int main( int, char** )
{
    try {
        Any any1 = 2;
        Any any2 = 4;
        const float f1 = 1.0f;
        const float f2 = 2.0f;

        assert( any1 < any2 );
        assert( any2 > any1 );
        any2 = any1;
        assert( any1 == any2 );
        any1 = &f1;
        any2 = &f1;
        assert( any1 == any2 );
        any2 = &f2;
        if( &f1  < &f2 ) assert( any1 < any2 );
        else assert( any1 > any2 );

        any1 = size_t( 2 );
        any2 = size_t( 23 );
        assert( any1 < any2 );

        any1 = std::complex< float >( 1.f, 1.f );
        any2 = std::complex< float >( 2.f, 2.f );
        assert( any1 < any2 );

        assert( any1 != any2 );

        typedef std::map< Any, Any > AnyMap;
        AnyMap anyMap;
        anyMap[ 5 ] = 10.;
        assert( anyMap[ 5 ] == 10. );
        anyMap[ 3 ] = std::string( "hey" );
        assert( anyMap[ 3 ] == std::string( "hey" ) );

        Any any3( AnyMove( AnyGen() ) );
        assert( any3 == 3.0 );
        std::cout << "OK" << std::endl;

    } catch( const std::exception& e ) {
        std::cerr << e.what() << std::endl;
    }
    
    return 0;
}

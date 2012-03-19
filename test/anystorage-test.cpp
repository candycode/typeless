//#SRCHEADER#
#include <cassert>
#include <stdexcept>
#include <cmath>
#include <complex>
#include <map>
#include <string>

#define ANY_OSTREAM

#include <Any.h>
#include <AnyStorage.h>


int main( int, char** )
{
    try {

        {
        boost::intrusive_ptr< SingleAnyStorage > ss = new SingleAnyStorage;
        ss->Put( 123 );
        assert( int( ss->Get() ) == 123 );
        }       
        {
        boost::intrusive_ptr< MultiAnyStorage > ms = new MultiAnyStorage;
        typedef MultiAnyStorage::Key Key;
        const Key k1 = ms->Put( 123 );
        const Key k2 = ms->Put( std::string( "321" ) );
        assert( ms->Get( k1 ) == 123 );
        assert( ms->Get( k2 ) == std::string( "321" ) );
        ms->Put( 321, k1 );
        assert( ms->Get( k1 ) == 321 );
        }
        {
        boost::intrusive_ptr< MapAnyStorage > ms = new MapAnyStorage;
        const Any k1 = ms->Put( 123, 1 );
        const Any k2 = ms->Put( std::string( "321" ), 4 );
        assert( ms->Get( k1 ) == 123 );
        assert( ms->Get( k2 ) == std::string( "321" ) );
        ms->Put( 321, k1 );
        assert( ms->Get( k1 ) == 321 );
        }
        {
        typedef SyncAnyStorage< MapAnyStorage > Storage;
        boost::intrusive_ptr< Storage > ms = new Storage;
        const Any k1 = ms->Put( 123, 1 );
        const Any k2 = ms->Put( std::string( "321" ), 4 );
        assert( ms->Get( k1 ) == 123 );
        assert( ms->Get( k2 ) == std::string( "321" ) );
        ms->Put( 321, k1 );
        assert( ms->Get( k1 ) == 321 );
        }
        std::cout << "OK" << std::endl;

    } catch( const std::exception& e ) {
        std::cerr << e.what() << std::endl;
    }
    
    return 0;
}

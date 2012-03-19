#pragma once
//Author: Ugo Varetto


#include <string>
#include <stdexcept>

/// \defgroup any_ops Any 
/// @{

    /// \defgroup lt lower than
    /// @{

template < typename T >
struct AnyLt {
    static bool Op( const T&, const T& ) {
	throw std::runtime_error( std::string( "AnyLess not implemented for type " ) + typeid( T ).name() );
	return false;
    }
};

template < typename T >
struct AnyLt< T* > {
    static bool Op( const T* p1, const T* p2) {
	return p1 < p2;
    }
};

template < typename T > bool AnyLess( const T& v1, const T& v2 ) { return AnyLt< T >::Op( v1, v2 ); }
bool AnyLess( char v1, char v2 ) { return v1 < v2; }
bool AnyLess( unsigned char v1, unsigned char v2 ) { return v1 < v2; }
bool AnyLess( short v1, short v2 ) { return v1 < v2; }
bool AnyLess( unsigned short v1, unsigned short v2 ) { return v1 < v2; }
bool AnyLess( int v1, int v2 ) { return v1 < v2; }
bool AnyLess( unsigned int v1, unsigned int v2 ) { return v1 < v2; }
bool AnyLess( long v1, long v2 ) { return v1 < v2; }
bool AnyLess( unsigned long v1, unsigned long v2 ) { return v1 < v2; }
bool AnyLess( long long v1, long long v2 ) { return v1 < v2; }
bool AnyLess( unsigned long long v1, unsigned long long v2 ) { return v1 < v2; }
bool AnyLess( float v1, float v2 ) { return v1 < v2; }
bool AnyLess( double v1, double v2 ) { return v1 < v2; }
bool AnyLess( const std::string& v1, const std::string& v2 ) { return v1 < v2; }
bool AnyLess( const std::wstring& v1, const std::wstring& v2 ) { return v1 < v2; }
bool AnyLess( const void* v1, const void* v2 ) { return v1 < v2; }

/// @}

    /// \defgroup eq Equal
    /// {@

template < typename T >
struct AnyEq {
    static bool Op( const T&, const T& ) {
	throw std::runtime_error( std::string( "AnyEqual not implemented for type " ) + typeid( T ).name() );
	return false;
    }
};

template < typename T >
struct AnyEq< T* > {
    static bool Op( const T* p1, const T* p2) {
	return p1 == p2;
    }
};

template < typename T > bool AnyEqual( const T& v1, const T& v2 ) { return AnyEq< T >::Op( v1, v2 ); }
bool AnyEqual( char v1, char v2 ) { return v1 == v2; }
bool AnyEqual( unsigned char v1, unsigned char v2 ) { return v1 == v2; }
bool AnyEqual( short v1, short v2 ) { return v1 == v2; }
bool AnyEqual( unsigned short v1, unsigned short v2 ) { return v1 == v2; }
bool AnyEqual( int v1, int v2 ) { return v1 == v2; }
bool AnyEqual( unsigned int v1, unsigned int v2 ) { return v1 == v2; }
bool AnyEqual( long v1, long v2 ) { return v1 == v2; }
bool AnyEqual( unsigned long v1, unsigned long v2 ) { return v1 == v2; }
bool AnyEqual( long long v1, long long v2 ) { return v1 == v2; }
bool AnyEqual( unsigned long long v1, unsigned long long v2 ) { return v1 == v2; }
bool AnyEqual( float v1, float v2 ) { return v1 == v2; }
bool AnyEqual( double v1, double v2 ) { return v1 == v2; }
bool AnyEqual( const std::string& v1, const std::string& v2 ) { return v1 == v2; }
bool AnyEqual( const std::wstring& v1, const std::wstring& v2 ) { return v1 == v2; }
bool AnyEqual( const void* v1, const void* v2 ) { return v1 == v2; }

    /// @}

/// @}

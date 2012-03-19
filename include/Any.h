#pragma once
//#SRCHEADER#

/// @file Any.h Implementation of class to hold instances of any type.

#include <typeinfo>
#include <vector>
#include <iterator>
#include <iostream>
#include <string>


#include "any-operators.h"

class AnyMover;

//------------------------------------------------------------------------------
/// @brief Class that can hold instances of any type. Uses defaule new/delete
/// operators.
/// @todo allow client code to specify allocator
/// @ingroup utility
class Any
{
    friend class AnyMover;
public:
    /// Type used by Any::Type() method to signal an empty @c Any instance.
    struct EMPTY_ {};
    /// Default constructor, sets the internal pointer to @c NULL
    Any() : pval_( 0 ) {}
    /// Constructor accepting a parameter copied into internal type instance.
    template < class ValT >
    Any( const ValT& v )
        : pval_( new ValHandler< ValT >( v ) )
    {}
    /// Move constructor
    Any( const AnyMover& );
    /// Copy constructor.
    Any( const Any& a ) : pval_( a.pval_ ? a.pval_->Clone() : 0 ) {}
    /// Destructor: deletes the contained data type.
    ~Any() { delete pval_; }
public:
    /// Returns @c true if instance empty.
    bool Empty() const { return pval_ == 0; }
    /// Returns type of contained data or Any::EMPTY_ if instance empty.
    const std::type_info& Type() const
    {
        //note gcc requires typeid(C) with C != void; compiles on vc++ 2008
        return !Empty() ? pval_->GetType() : typeid( EMPTY_ ); //
    }
    /// Swap two Any instances by swapping the internal pointers.
    Any& Swap( Any& a ) { std::swap( pval_, a.pval_ ); return *this; }
    /// Assignment
    Any& operator=( const Any& a )
    { 
        if( pval_ != 0 && a.pval_ != 0 && a.Type() == this->Type() )
        {
            pval_->Assign( *a.pval_ );
            return *this;
        }
        Any( a ).Swap( *this ); return *this;
    }
    /// Assignment from non - @c Any value.
    template < class ValT >
    Any& operator=( const ValT& v )
    {
        if( typeid( ValT ) == this->Type() )
        {
            static_cast< ValHandler<ValT>* >( pval_ )->val_ = v;
            return *this;
        }
        //CheckAnyTypeAndThrow< ValT >( *this );
        Any( v ).Swap( *this ); return *this;
    }
    /// Equality: check by converting value to contained value type then
    /// invoking equality operator on converted type.
    template < class ValT >
    bool operator==( const ValT& v ) const
    {
        CheckAndThrow< ValT >();
        return ( static_cast< ValHandler<ValT>* >( pval_ )->val_ ) == v;
    }
public:
    ///Convert to const reference.
    template < class ValT > operator const ValT&() const
    {
        CheckAndThrow< ValT >();
        return ( static_cast< ValHandler<ValT>* >( pval_ )->val_ );
    }
    ///Convert to reference.
    template < class ValT > operator ValT&() const
    {
        CheckAndThrow< ValT >();
        return ( static_cast< ValHandler<ValT>* >( pval_ )->val_ );
    }

    ///Overloaded reference operator: required when a reference to Any
    ///instead of to the containded data is required. If this operator
    ///is not defined the <tt>template \< class ValT \> operator ValT&()</tt>
    ///will be used.
    operator Any&() { return *this; }
    ///Overloaded reference operator: required when a reference to Any
    ///instead of to the containded data is required. If this operator
    ///is not defined the <tt>template \< class ValT \> operator const ValT&()</tt>
    ///will be used.
    operator const Any&() const { return *this; } 
    /// Check if contained data is convertible to specific type.
    /// @note although this function does not use any private data
    /// it is required to be defined withing the Any class since:
    /// - it cannot be defined before the class because it uses
    ///   Any's member functions
    /// - it cannot be defined after the class because in this case
    ///   other inline friends cannot invoke it
    template < class ValT >
    friend void CheckAnyTypeAndThrow( const Any& any )
    {
//#ifdef ANY_CHECK_TYPE
        if( typeid( ValT ) != any.Type() )
            throw std::logic_error( 
                    ( std::string( " Attempt to convert from ")
                    + any.Type().name()
                    + std::string( " to " )
                    + typeid( ValT ).name() ).c_str() );
//#endif
    }
    /// Check if contained data is convertible to specific type.
    /// @note although this function does not use any private data
    /// it is required to be defined withing the Any class since:
    /// - it cannot be defined before the class because it uses
    ///   Any's member functions
    /// - it cannot be defined after the class because in this case
    ///   other inline friends cannot invoke it
    friend void CheckAnyTypeAndThrow( const Any& any1, const Any& any2 )
    {
//#ifdef ANY_CHECK_TYPE
        if( any1.Type() != any2.Type() )
            throw std::logic_error( 
                    ( std::string( " Attempt to convert between ")
                    + any1.Type().name()
                    + std::string( " and " )
                    + any2.Type().name() ).c_str() );
//#endif
    }
    

    /// Return size of contained data.
    friend size_t AnySizeOf( const Any& any )
    {
        return any.pval_->SizeofData();
    }
    ///Give access to address of contained data.
    friend void* AnyAddress( Any& any )
    {
        return any.pval_->GetDataAddress();
    }
    ///Get alignment of contained data.
    friend size_t AnyAlignment( Any& any )
    {
        return any.pval_->GetAlignment();
    }
    ///Give access to address of contained data.
    friend const void* AnyAddress( const Any& any )
    {
        return any.pval_->GetDataAddress();
    }
    /// Give access to address of contained data.
    template < class AnyT >
    friend AnyT* AnyPtr( Any& any )
    {
        CheckAnyTypeAndThrow< AnyT >( any );
        return ( &static_cast< Any::ValHandler<AnyT>* >( any.pval_ )->val_ );
    }
    /// Give access to address of contained const data.
    template < class AnyT >
    friend const AnyT* AnyPtr( const Any& any )
    {
        CheckAnyTypeAndThrow< AnyT >( any );
        return ( &static_cast< Any::ValHandler<AnyT>* >( any.pval_ )->val_ );
    }
    /// Give access to reference to contained data.
    template < class AnyT >
    friend AnyT& AnyRef( Any& any )
    {
        CheckAnyTypeAndThrow< AnyT >( any );
        return ( static_cast< Any::ValHandler<AnyT>* >( any.pval_ )->val_ );
    }
    /// Give access to const reference to contained data.
    template < class AnyT >
    friend const AnyT& AnyRef( const Any& any )
    {
        CheckAnyTypeAndThrow< AnyT >( any );
        return ( static_cast< Any::ValHandler<AnyT>* >( any.pval_ )->val_ );
    }
    /// Return value.
    template < class AnyT >
    friend AnyT AnyVal( const Any& any )
    {
        CheckAnyTypeAndThrow< AnyT >( any );
        return ( static_cast< Any::ValHandler<AnyT>* >( any.pval_ )->val_ );
    }
    /// Less than operator
    bool friend operator<( const Any& a1, const Any& a2 ) {
        CheckAnyTypeAndThrow( a1, a2 );
        if( a1.Empty() ) throw std::logic_error( "Attempt to compare empty values" );
        return a1.pval_->LessThan( a2.pval_->GetDataAddress() );
    }
    /// Greater than operator
    bool friend operator>( const Any& a1, const Any& a2 ) {
        CheckAnyTypeAndThrow( a1, a2 );
        if( a1.Empty() ) throw std::logic_error( "Attempt to compare empty values" );
        return a2.pval_->LessThan( a1.pval_->GetDataAddress() );
    }
    /// Equality operator
    bool friend operator==( const Any& a1, const Any& a2 ) {
        CheckAnyTypeAndThrow( a1, a2 );
        if( a1.Empty() ) throw std::logic_error( "Attempt to compare empty values" );
        return a1.pval_->EqualTo( a2.pval_->GetDataAddress() );
    }
    /// Inequality operator
    bool friend operator!=( const Any& a1, const Any& a2 ) {
        CheckAnyTypeAndThrow( a1, a2 );
        if( a1.Empty() ) throw std::logic_error( "Attempt to compare empty values" );
        return !a1.pval_->EqualTo( a2.pval_->GetDataAddress() );
    }

private:
    /// Check if contained data is convertible to specific type.
    template < class ValT > void CheckAndThrow() const
    {
        CheckAnyTypeAndThrow< ValT >( *this );
    }

    /// @interface HandlerBase Wrapper for data storage.
    struct HandlerBase // hint: use small object allocator
    {
        virtual const std::type_info& GetType() const = 0;
        virtual HandlerBase* Clone() const = 0;
        virtual ~HandlerBase() {}
        virtual size_t GetAlignment() const  = 0;
#ifdef ANY_OSTREAM
        virtual std::ostream& Serialize( std::ostream& ) const = 0;
#endif        
#ifdef ANY_ISTREAM
        virtual std::istream& DeSerialize( std::istream& ) const = 0;
#endif        
        virtual size_t SizeofData() const = 0;
        virtual void* GetDataAddress() = 0;
        virtual const void* GetDataAddress() const  = 0;
        virtual void Assign( const HandlerBase& ) = 0;
        virtual bool LessThan( const void* ) const  = 0; // type checked before call
        virtual bool EqualTo( const void* ) const = 0;   // type checked before call
    };


    /// Struct for computing alignment of data.
    template < typename T > struct Align
    {
        struct D
        {
            T d;
            char c[ 1 ];
        };
        enum {Value = sizeof( D ) - sizeof( T )};
    };

    /// HandlerBase actual data container class.
    template < class T > struct ValHandler :  HandlerBase
    {
        typedef T Type;
        T val_;
        size_t alignment_;
        ValHandler( const T& v ) : val_( v ), alignment_( Align< T >::Value )
        {}
        const std::type_info& GetType() const { return typeid( T ); }
        ValHandler* Clone() const { return new ValHandler( val_ ); }
#ifdef ANY_OSTREAM
        std::ostream& Serialize( std::ostream& os ) const
        {
            os << val_;
            return os;
        }
#endif
#ifdef ANY_ISTREAM
        std::istream& DeSerialize( std::ostream& is ) const
        {
            is >> val_;
            return is;
        }
#endif
        size_t SizeofData() const { return sizeof( Type ); }
        void* GetDataAddress() { return &val_; }
        const void* GetDataAddress() const { return &val_; }
        size_t GetAlignment() const { return alignment_; }
        void Assign( const HandlerBase& rhs )
        {
            val_ = *( reinterpret_cast< const T* >( rhs.GetDataAddress() ) );
        }
        virtual bool LessThan( const void* pv ) const { // type checked before call
            return AnyLess( val_, *reinterpret_cast< const T* >( pv ) ); //use Koenig lookup to find specialization
        }
        virtual bool EqualTo( const void* pv ) const { // type checked before call
            return AnyEqual( val_, *reinterpret_cast< const T* >( pv ) ); //use Koenig lookup to find specialization
        }
    };

    ///Pointer to contained data: deleted when Any instance deleted.
    HandlerBase* pval_;
#ifdef ANY_OSTREAM
    ///Overloaded operator to serialize data to output streams.
    friend inline std::ostream& operator<<( std::ostream& os, const Any& any )
    {
        if( any.Empty() ) return os;
        return any.pval_->Serialize( os );
    }
#endif
#ifdef ANY_ISTREAM
    ///Overloaded operator to serialize data to output streams.
    friend inline std::istream& operator>>( std::istream& is, const Any& any )
    {
        return any.pval_->DeSerialize( os );
    }
#endif

};

class AnyMover {
    mutable Any* anyRef_;
    friend class Any;
    AnyMover();
public:
    AnyMover( Any& anyRef ) : anyRef_( &anyRef ) {}
};

inline Any::Any( const AnyMover& ma ) : pval_( 0 ) {
    std::swap( pval_, ma.anyRef_->pval_ );
}

//inline Any& Any::operator=( const AnyMover& ma ) {
//  std::swap( pval_, ma.anyRef_->pval_ );
//}

inline AnyMover AnyMove( Any m ) { 
    return AnyMover( m ); 
} 


///Utility function to print the content of an std::vector of Any objects.
inline std::ostream& operator<<( std::ostream& os, const std::vector< Any >& av )
{
    std::copy( av.begin(), av.end(), std::ostream_iterator< Any >( os, ", " ) );
    return os;
}

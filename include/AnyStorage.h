#pragma once
//Author: Ugo Varetto

#include <vector>
#include <map>
#include <boost/utility.hpp>
#include <typeinfo>
#include <boost/thread.hpp>

#include <Referenced.h>
#include <ICloneable.h>
#include <Any.h>

struct IAnyStorage : Referenced,
                     ICloneable< IAnyStorage > {
    virtual const Any& Get( const Any& key = Any() ) const = 0;
    virtual Any Put( const Any& value, const Any& key = Any() ) = 0;
    virtual IAnyStorage* Clone() const = 0;
    virtual const std::type_info& KeyType() const = 0;
    virtual ~IAnyStorage() {}
};

class SingleAnyStorage : public IAnyStorage {
public:
    virtual const Any& Get( const Any& key = Any() ) const {
        return anyVal_;
    }
    virtual Any Put( const Any& value, const Any& key = Any() ) {
        anyVal_ = value;
        return Any();
    }
    virtual SingleAnyStorage* Clone() const { 
        SingleAnyStorage* sp = new SingleAnyStorage;
        sp->anyVal_ = anyVal_;
        return sp;
    }
    virtual const std::type_info& KeyType() const { return typeid( Any() ); }
private:
    Any anyVal_;
};


class MultiAnyStorage : public IAnyStorage {
public:
    typedef std::vector< Any > AnyVector;
    typedef AnyVector::size_type Key;

    virtual const Any& Get( const Any& key = Any() ) const {
        return anyArray_[ key ];
    }
    virtual Any Put( const Any& value, const Any& key = Any() ) {
        if( key.Empty() ) {
            anyArray_.push_back( value );
            return Key( anyArray_.size() - 1 );
        }
        const AnyVector::size_type k = AnyVector::size_type( Key( key ) );
        if( k >= anyArray_.size() ) anyArray_.resize( k );
        anyArray_[ key ] = value;
        return key;
    }
    virtual MultiAnyStorage* Clone() const { 
        MultiAnyStorage* mp = new MultiAnyStorage;
        mp->anyArray_ = anyArray_;
        return mp;
    }
    virtual const std::type_info& KeyType() const { return typeid( Key ); }
private:
    AnyVector anyArray_;
};

class MapAnyStorage : public IAnyStorage {
public:
    typedef std::map< Any, Any > AnyMap;
    typedef AnyMap::key_type Key;
    typedef AnyMap::const_iterator ConstIterator;

    virtual const Any& Get( const Any& key = Any() ) const {
        ConstIterator ci = anyMap_.find( key );
        if( ci != anyMap_.end() ) return ci->second;
        return emptyAny_;
    }
    virtual Any Put( const Any& value, const Any& key = Any() ) {
        anyMap_[ key ] = value;
        return key;
    }
    virtual MapAnyStorage* Clone() const { 
        MapAnyStorage* mp = new MapAnyStorage;
        mp->anyMap_ = anyMap_;
        return mp;
    }
    virtual const std::type_info& KeyType() const { 
        return anyMap_.empty() ? typeid( Key ) : typeid( anyMap_.begin()->first );
    }
private:
    AnyMap anyMap_;
    Any emptyAny_;
};


template < typename AnyStorageT >
class SyncAnyStorage : public IAnyStorage {
public:
    typedef AnyStorageT StorageType;
    typedef SyncAnyStorage< AnyStorageT > ThisType;

    SyncAnyStorage() : data_ready_( false ), storage_( new StorageType ) {}
    
    virtual const Any& Get( const Any& key = Any() ) const {
        boost::unique_lock<boost::mutex> lock(mutex_);
        while( !data_ready_ ) cond_.wait( lock ); 
        return storage_->Get( key );
    }
    virtual Any Put( const Any& value, const Any& key = Any() ) {
        data_ready_ = false;
        Any k;
        {
            boost::lock_guard< boost::mutex > lock( mutex_ );
            k = storage_->Put( value, key );
            data_ready_ = true;
        }
        cond_.notify_one();
        return k;
    }
    virtual ThisType* Clone() const {
        boost::lock_guard< boost::mutex > lock( mutex_ );
        ThisType* tt = new ThisType;
        tt->storage_ = tt->storage_->Clone();
        return tt;
    }
    virtual const std::type_info& KeyType() const { return storage_->KeyType(); }
    ~SyncAnyStorage() { delete storage_; }
private:
    mutable boost::condition_variable cond_;
    mutable boost::mutex mutex_;
    bool data_ready_;
    StorageType* storage_;
};

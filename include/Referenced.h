#pragma once
//Author: Ugo Varetto


#include <cassert>
#include <boost/atomic.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/utility.hpp>

///Referenced
class Referenced : boost::noncopyable
{
public:
  typedef ::boost::intrusive_ptr<Referenced> pointer;
  Referenced() : refcount_( 0 ) {}
  virtual void Destroy() const { delete this; } //ovveride for classes supporting custom deallocators
  virtual ~Referenced()  {}
private:
  friend int get_count( const Referenced& r ) { return r.refcount_; }
  friend int get_count( const Referenced* pr ) { assert( pr ); return pr->refcount_; }
  friend void intrusive_ptr_add_ref( const Referenced* pr )
  {
    assert( pr );
    pr->refcount_.fetch_add( 1, boost::memory_order_relaxed );
  }
  friend void intrusive_ptr_release( const Referenced* pr )
  {
    assert( pr );
    if( pr->refcount_.fetch_sub( 1, boost::memory_order_release ) == 1 ) {
      boost::atomic_thread_fence( boost::memory_order_acquire );
      pr->Destroy();
    }
  }
private:
  mutable boost::atomic< int > refcount_;
};

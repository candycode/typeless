#pragma once
//#SRCHEADER#

template < typename ClonedT >
struct ICloneable
{
  virtual ClonedT* Clone() const = 0;
  virtual ~ICloneable() {}
};

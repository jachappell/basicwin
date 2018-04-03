#ifndef __XOBJECT_H__
#define __XOBJECT_H__

#include "xviewport.h"
#include "xgc.h"

template<class T> class CXObject
{
public:
  CXObject(const CXViewPort<T>& win, const CXGC& gc,
           const T& x_ = 0, const T& y_ = 0)
    : win_(win)
    , gc_(gc)
  {}

  virtual ~CXObject() {}

  virtual void Draw() = 0 ;

protected:
 CXObject() {}

private:
 T x_ ;
 T y_ ;
 
 CXViewPort<T>& win_ ;
 CXGC& gc_ ;
} ;


#endif

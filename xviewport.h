#ifndef __XVIEWPORT_H__
#define __XVIEWPORT_H__

#include "xwindow.h"

template<class T> class CXViewPort
{
public:
  CXViewPort(CXWindowPtr& window,
             T xmax, T ymax, T xmin, T ymin)
    : window_(window)
    , Xmax_(xmax)
    , Ymax_(ymax)
    , Xmin_(xmin)
    , Ymin_(ymin)
  {
    Resize() ;
  }
  
  CXViewPort(CXWindowPtr& window,
             T xmax, T ymax, T xmin, T ymin,
             unsigned int width, unsigned int height)
    : window_(window)
    , Xmax_(xmax)
    , Ymax_(ymax)
    , Xmin_(xmin)
    , Ymin_(ymin)
    , width_(width)
    , height_(height)
  {
  }
  
  ~CXViewPort() {}

  unsigned int height() const { return height_ ; }
  unsigned int width() const { return width_ ; }
             
  void ViewPort(T xmax, T ymax, T xmin, T ymin)
  {
    Xmax_ = xmax ;
    Ymax_ = ymax ;
    Xmin_ = xmin ;
    Ymin_ = ymin ;
  }

  void Resize()
  {
    XWindowAttributes attr ;

    window_->GetWindowAttributes(attr) ;

    width_ = attr.width ;
    height_ = attr.height ;
  }

  T x(int ix) const
  {
    return ((T)ix * (Xmax_ - Xmin_) / (T)width_) + Xmin_ ;
  }

  T y(int iy) const
  {
    return ((T)iy * (Ymin_ - Ymax_) / (T)height_) + Ymax_ ;
  }

  int ix(T x) const
  {
    return (int)(((x - Xmin_) * (T)width_) / (Xmax_ - Xmin_)) ;
  }

  int iy(T y) const
  {
    return (int)(((y - Ymax_) * (T)height_) / (Ymin_ - Ymax_)) ;
  }

  CXWindowPtr& WindowPtr() { return window_ ; }
  CXDisplayPtr& DisplayPtr() { return window_->Display() ; }

  operator Window () const { return *window_ ; }

private:
  CXWindowPtr window_ ;

  unsigned int width_ ;
  unsigned int height_ ;
  T Xmax_ ;
  T Ymax_ ;
  T Xmin_ ;
  T Ymin_ ;
} ;


#endif
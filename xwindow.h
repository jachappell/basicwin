/*******************************************************************
 *
 * xwindow.h -- Wrap Window with a boost::shared_ptr
 *
 *  Copyright (C) 2007 by James A. Chappell (rlrrlrll@gmail.com)
 *
 *  Permission is hereby granted, free of charge, to any person
 *  obtaining a copy of this software and associated documentation
 *  files (the "Software"), to deal in the Software without
 *  restriction, including without limitation the rights to use,
 *  copy, modify, merge, publish, distribute, sublicense, and/or
 *  sell copies of the Software, and to permit persons to whom the
 *  Software is furnished to do so, subject to the following
 *  condition:
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *  OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *  OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef __XWINDOW_H__
#define __XWINDOW_H__

#include "xdisplay.h"

class bad_CXWindow : public std::exception
{
public:
  virtual const char* what() const throw()
  {
    return "bad_CXWindow" ; // for now
  }
} ;


class CXWindow ;

typedef boost::shared_ptr<CXWindow> CXWindowPtr ;

class CXWindow : private boost::noncopyable
{
public:

  static CXWindowPtr CreateWindow(CXDisplayPtr& display,
                                  int x, int y,
                                  unsigned int width,
                                  unsigned int height,
                                  unsigned int border_width,
                                  unsigned long border,
                                  unsigned long background,
                                  int screen_num = USE_DEFAULT_SCREEN)
  {
    Window parent(display->GetRootWindow(screen_num)) ;
    return CXWindowPtr(new CXWindow(display, parent,
                                    x, y, width, height, 
                                    border_width, border, background)) ;
  }
  
  
  static CXWindowPtr CreateWindow(CXDisplayPtr& display,
                                  CXWindowPtr& parent,
                                  int x, int y,
                                  unsigned int width,
                                  unsigned int height,
                                  unsigned int border_width,
                                  unsigned long border,
                                  unsigned long background)
  {
    return CXWindowPtr(new CXWindow(display, *parent,
                                    x, y, width, height, 
                                    border_width, border, background)) ;
  }

  ~CXWindow()
  {
    XDestroyWindow(*display_, window_) ;
  }

  operator Window () const { return window_ ; }

  CXDisplayPtr& Display() { return display_ ; }

  void GetWindowAttributes(XWindowAttributes &attr) const
  {
    if (!XGetWindowAttributes(*display_, window_, &attr))
    {
      throw bad_CXWindow() ;
    }
  }

  void Map(bool map_sub = true) const
  {
    XMapWindow(*display_, window_);
    if (map_sub)
    {
      XMapSubwindows(*display_, window_);
    }
  }

  void Unmap() const
  {
    XUnmapWindow(*display_, window_) ;
  }

private:
  Window window_ ;
  CXDisplayPtr display_ ;

  CXWindow(CXDisplayPtr& display, Window parent,
           int x, int y,
           unsigned int width, unsigned int height,
           unsigned int border_width,
           unsigned long border,
           unsigned long background)
    : display_(display)
  {
    window_ = XCreateSimpleWindow(*display_, parent,
                                   x, y, width, height, 
                                   border_width, border, background) ;
  }

  CXWindow() {} 
} ;

#endif
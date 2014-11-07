/*******************************************************************
 *
 * xdisplay.h -- Wrap Display with a boost::shared_ptr
 *
 *  Copyright (C) 2006 by James A. Chappell (rlrrlrll@gmail.com)
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

#ifndef __xdisplay_h__
#define __xdisplay_h__

#include <X11/Xlib.h>

#include <exception>

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

#define USE_DEFAULT_SCREEN -1

class bad_CXDisplay : public std::exception
{
public:
  virtual const char* what() const throw()
  {
    return "bad_CXDisplay" ; // for now
  }
} ;

class CXDisplay ;

typedef boost::shared_ptr<CXDisplay> CXDisplayPtr ;

class CXDisplay : private boost::noncopyable
{
public:
  static CXDisplayPtr OpenDisplay(const char *display_name = NULL) ;

  ~CXDisplay() ;

  Display* operator->() { return display_ ; }
  operator Display* () const { return display_ ; }

  int GetScreenCount() const { return ScreenCount(display_) ; }
  int GetDefaultScreen() const { return DefaultScreen(display_) ; }
  Window GetRootWindow(int screen_num = USE_DEFAULT_SCREEN) const ;
  int GetDisplayWidth(int screen_num = USE_DEFAULT_SCREEN) const ;
  int GetDisplayHeight(int screen_num = USE_DEFAULT_SCREEN) const ;
  int GetDefaultDepth(int screen_num = USE_DEFAULT_SCREEN) const ;
  unsigned long GetBlackPixel(int screen_num = USE_DEFAULT_SCREEN) const ;
  unsigned long GetWhitePixel(int screen_num = USE_DEFAULT_SCREEN) const ;

  int GetDisplayWidthMM(int screen_num = USE_DEFAULT_SCREEN) const ;
  int GetDisplayHeightMM(int screen_num = USE_DEFAULT_SCREEN) const ;

  unsigned long GetColor(const char *color_name,
                         int screen_num = USE_DEFAULT_SCREEN) const ;

private:
  Display *display_ ;
  //
  // Only call via static member function OpenDisplay
  //
  CXDisplay(const char *display_name) ;
 
  //
  // see if s indicates using the default screen, otherwise
  // return s
  int screen(int s) const
  {
    return (s == USE_DEFAULT_SCREEN) ? GetDefaultScreen() : s ;
  }

  // no default constructor
  CXDisplay() {}
} ;


inline CXDisplayPtr CXDisplay::OpenDisplay(const char *display_name)
{
  return CXDisplayPtr(new CXDisplay(display_name)) ;
}


inline CXDisplay::CXDisplay(const char *display_name)
{
  display_ = XOpenDisplay(display_name) ;
  if (!display_)
  {
    throw bad_CXDisplay() ;
  }
}


inline CXDisplay::~CXDisplay()
{
  XCloseDisplay(display_);
}


inline Window CXDisplay::GetRootWindow(int screen_num) const
{
  return RootWindow(display_, screen(screen_num)) ;
}


inline int CXDisplay::GetDisplayWidth(int screen_num) const
{
  return DisplayWidth(display_, screen(screen_num)) ;
}


inline int CXDisplay::GetDisplayHeight(int screen_num) const
{
  return DisplayHeight(display_, screen(screen_num)) ;
}


inline int CXDisplay::GetDisplayWidthMM(int screen_num) const
{
  return DisplayWidthMM(display_, screen(screen_num)) ;
}


inline int CXDisplay::GetDisplayHeightMM(int screen_num) const
{
  return DisplayHeightMM(display_, screen(screen_num)) ;
}


inline int CXDisplay::GetDefaultDepth(int screen_num) const 
{
  return DefaultDepth(display_, screen(screen_num)) ;
}


inline unsigned long CXDisplay::GetBlackPixel(int screen_num) const
{
  return BlackPixel(display_, screen(screen_num)) ;
}


inline unsigned long CXDisplay::GetWhitePixel(int screen_num) const
{
  return WhitePixel(display_, screen(screen_num)) ;
}


inline unsigned long
CXDisplay::GetColor(const char *color_name, int screen_num) const
{
  Colormap cmap;

  XColor near_color, true_color;

  cmap = DefaultColormap(display_,  screen(screen_num)) ;

  XAllocNamedColor(display_, cmap, color_name, &near_color, &true_color);

  return near_color.pixel ;
}

#endif

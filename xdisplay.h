/*******************************************************************
 *
 * xdisplay.h -- Wrap Display with a shared_ptr
 *
 *  Copyright (C) 2018 by James A. Chappell (rlrrlrll@gmail.com)
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
#include <memory>

#define USE_DEFAULT_SCREEN -1

class CXDisplay ;

typedef std::shared_ptr<CXDisplay> CXDisplayPtr ;

class CXDisplay
{
public:
  static CXDisplayPtr OpenDisplay(const char *display_name = NULL) ;

  ~CXDisplay();

  // no default constructor
  CXDisplay() = delete;
  // no copy
  CXDisplay(const CXDisplay&) = delete;
  CXDisplay& operator=(const CXDisplay&) = delete;

  class Exception : public std::exception
  {
  public:
    virtual const char* what() const throw()
    {
      return "bad_CXDisplay"; // for now
    }
  };

  Display* operator->() { return _display ; }
  operator Display* () const { return _display ; }

  int GetScreenCount() const { return ScreenCount(_display) ; }
  int GetDefaultScreen() const { return DefaultScreen(_display) ; }
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

  double AspectRatio(int screen_num = USE_DEFAULT_SCREEN);
  //
  // Only call via static member function OpenDisplay
  //
private:
  struct _private_constructor_tag
    { explicit _private_constructor_tag() = default; };
public:
  CXDisplay(const char *display_name, _private_constructor_tag);

private:
  Display *_display ;
 
  //
  // see if s indicates using the default screen, otherwise
  // return s
  int screen(int s) const
  {
    return (s == USE_DEFAULT_SCREEN) ? GetDefaultScreen() : s ;
  }
};


inline CXDisplayPtr CXDisplay::OpenDisplay(const char *display_name)
{
  return std::make_shared<CXDisplay>(display_name, _private_constructor_tag{});
}


inline CXDisplay::CXDisplay(const char *display_name, _private_constructor_tag)
{
  _display = XOpenDisplay(display_name) ;
  if (!_display)
  {
    throw CXDisplay::Exception();
  }
}


inline CXDisplay::~CXDisplay()
{
  XCloseDisplay(_display);
}


inline Window CXDisplay::GetRootWindow(int screen_num) const
{
  return RootWindow(_display, screen(screen_num)) ;
}


inline int CXDisplay::GetDisplayWidth(int screen_num) const
{
  return DisplayWidth(_display, screen(screen_num)) ;
}


inline int CXDisplay::GetDisplayHeight(int screen_num) const
{
  return DisplayHeight(_display, screen(screen_num)) ;
}


inline int CXDisplay::GetDisplayWidthMM(int screen_num) const
{
  return DisplayWidthMM(_display, screen(screen_num)) ;
}


inline int CXDisplay::GetDisplayHeightMM(int screen_num) const
{
  return DisplayHeightMM(_display, screen(screen_num)) ;
}


inline int CXDisplay::GetDefaultDepth(int screen_num) const 
{
  return DefaultDepth(_display, screen(screen_num)) ;
}


inline unsigned long CXDisplay::GetBlackPixel(int screen_num) const
{
  return BlackPixel(_display, screen(screen_num)) ;
}


inline unsigned long CXDisplay::GetWhitePixel(int screen_num) const
{
  return WhitePixel(_display, screen(screen_num)) ;
}


inline unsigned long
CXDisplay::GetColor(const char *color_name, int screen_num) const
{
  Colormap cmap;

  XColor near_color, true_color;

  cmap = DefaultColormap(_display,  screen(screen_num)) ;

  XAllocNamedColor(_display, cmap, color_name, &near_color, &true_color);

  return near_color.pixel ;
}


inline double CXDisplay::AspectRatio(int screen_num)
{
  int s = screen(screen_num);

  double aspect_ratio = static_cast<double>(GetDisplayHeight(s)
                                         + GetDisplayHeightMM(s)) /
                        static_cast<double>(GetDisplayWidth(s)
                                         + GetDisplayWidthMM(s));

  return aspect_ratio;
}


#endif

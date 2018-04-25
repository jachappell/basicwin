/*******************************************************************
 *
 * xwindow.h -- Wrap Window with a simple class
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

#ifndef __XWINDOW_H__
#define __XWINDOW_H__

#include "xdisplay.h"

class CXWindow
{
public:
  CXWindow(CXDisplayPtr& display, Window parent,
           int x, int y,
           unsigned int width, unsigned int height,
           unsigned int border_width,
           unsigned long border,
           unsigned long background)
    : _display(display)
  {
    _window = XCreateSimpleWindow(*_display, parent,
                                   x, y, width, height, 
                                   border_width, border, background);
  }

  CXWindow(CXDisplayPtr& display,
           int x, int y,
           unsigned int width, unsigned int height,
           unsigned int border_width,
           unsigned long border,
           unsigned long background,
           int screen_num = USE_DEFAULT_SCREEN)
    : _display(display)
  {
    Window parent(display->GetRootWindow(screen_num));
    _window = XCreateSimpleWindow(*_display, parent,
                                   x, y, width, height, 
                                   border_width, border, background);
  }

  ~CXWindow()
  {
    XDestroyWindow(*_display, _window) ;
  }

  CXWindow() = delete;
  // no copy
  CXWindow(const CXWindow&) = delete;
  CXWindow& operator=(const CXWindow&) = delete;

  class Exception : public std::exception
  {
  public:
    virtual const char* what() const throw()
    {
      return "bad_CXWindow"; // for now
    }
  };

  operator Window () const { return _window ; }

  CXDisplayPtr& Display() { return _display ; }

  void GetWindowAttributes(XWindowAttributes &attr) const
  {
    if (!XGetWindowAttributes(*_display, _window, &attr))
    {
      throw CXWindow::Exception();
    }
  }

  void Map(bool map_sub = true) const
  {
    XMapWindow(*_display, _window);
    if (map_sub)
    {
      XMapSubwindows(*_display, _window);
    }
  }

  void Unmap() const
  {
    XUnmapWindow(*_display, _window);
  }

private:
  Window _window;
  CXDisplayPtr _display;
};

#endif

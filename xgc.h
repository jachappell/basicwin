/*******************************************************************
 *
 * xgc.h -- Define wrapper around a Graphic Context
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

#ifndef __XGC_H__
#define __XGC_H__

#include "xwindow.h"

class CXGC
{
public:
  CXGC(CXDisplayPtr& display, Window win,
       unsigned long valuemask, XGCValues *values)
    : _display(display)
  {
    _gc = XCreateGC(*_display, win, valuemask, values);
  }

  ~CXGC()
  {
    XFreeGC(*_display, _gc);
  }

  CXGC() = delete;  
  // no copy
  CXGC(const CXGC&) = delete;
  CXGC& operator=(const CXGC&) = delete;

  void SetFont(Font fid) const
  {
    XSetFont(*_display, _gc, fid);
  }
  
  void SetForeground(unsigned long foreground) const
  {
    XSetForeground(*_display, _gc, foreground);
  }

  void SetLineAttributes(unsigned int line_width, int line_style, 
                         int cap_style, int join_style) const
  {
    XSetLineAttributes(*_display, _gc, line_width, line_style, 
                       cap_style, join_style) ;
  }

  void SetDashes(int dash_offset, char *dash_list, int n) const
  {
    XSetDashes(*_display, _gc, dash_offset, dash_list, n) ;
  }

  GC operator->() { return _gc ; }
  operator GC () const { return _gc ; }

private:
  GC _gc ;
  CXDisplayPtr _display ;
} ;

#endif

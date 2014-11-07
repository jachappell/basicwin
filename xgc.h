/*******************************************************************
 *
 * xgc.h -- Define wrapper around a Graphic Context
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

#ifndef __XGC_H__
#define __XGC_H__

#include "xwindow.h"

class CXGC : private boost::noncopyable
{
public:
  CXGC(CXDisplayPtr& display, Window win,
       unsigned long valuemask, XGCValues *values)
    : display_(display)
  {
    gc_ = XCreateGC(*display_, win, valuemask, values) ;
  }

  ~CXGC()
  {
    XFreeGC(*display_, gc_);
  }

  void SetFont(Font fid) const
  {
    XSetFont(*display_, gc_, fid);
  }
  
  void SetForeground(unsigned long foreground) const
  {
    XSetForeground(*display_, gc_, foreground);
  }

  void SetLineAttributes(unsigned int line_width, int line_style, 
                         int cap_style, int join_style) const
  {
    XSetLineAttributes(*display_, gc_, line_width, line_style, 
                       cap_style, join_style) ;
  }

  void SetDashes(int dash_offset, char *dash_list, int n) const
  {
    XSetDashes(*display_, gc_, dash_offset, dash_list, n) ;
  }

  GC operator->() { return gc_ ; }
  operator GC () const { return gc_ ; }

private:
  GC gc_ ;
  CXDisplayPtr display_ ;

  CXGC() {}
} ;

typedef boost::shared_ptr<CXGC> CXGCPtr ;

#endif

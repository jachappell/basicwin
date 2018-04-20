/*******************************************************************
 *
 * xlloc.h -- Define wrappers around pointers to ClassHint, IconSize, 
 *            SizeHints, and WMHints
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
#ifndef __xalloc_h__
#define __xalloc_h__

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <exception>


template<class T, T (*alloc())> class _XAlloc 
{
public:
//
// Default Constructor
//
  _XAlloc()
  {
    _p = alloc();
    if (!_p)
    {
      throw _XAlloc::Exception();
    }
  }
//
// No copy
//
  _XAlloc(const _XAlloc& a) = delete;
  _XAlloc& operator= (const _XAlloc& a) =delete;

//
// Destructor
//
  ~_XAlloc()
  {
    XFree(_p) ; 
  }

  class Exception : public std::exception
  {
  public:
    virtual const char* what() const throw()
    {
      return "bad_XAlloc"; // for now
    }
  };
//
// More operators
//
  T* operator->() { return _p; }
  operator T* () const { return _p; }

private:
  
  T *_p ;
} ;

//
// Hide some of the nastiness above with typedefs...
//
typedef _XAlloc<XClassHint, &XAllocClassHint> CXClassHint ;
typedef _XAlloc<XIconSize, &XAllocIconSize> CXIconSize ;
typedef _XAlloc<XSizeHints, &XAllocSizeHints> CXSizeHints ;
typedef _XAlloc<XWMHints, &XAllocWMHints> CXWMHints ;

#endif

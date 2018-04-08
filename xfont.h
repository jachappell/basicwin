/*******************************************************************
 *
 * xfont.h -- Define a wrapper around a XFontStruct pointer 
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

#ifndef __XFONT_H__
#define __XFONT_H__

#include "xdisplay.h"

class CXFont
{
public:
  CXFont(CXDisplayPtr display, const char *fontname) :
    _display(display)
  {
    _font = XLoadQueryFont(*_display, fontname) ;
    if (!_font)
    {
      throw CXFont::Exception();
    }
  }

  ~CXFont()
  {
    XFreeFont(*_display, _font);
  }

   // no copy
  CXFont(const CXFont&) = delete;
  CXFont& operator=(const CXFont&) = delete;

  class Exception : public std::exception
  {
  public:
    virtual const char* what() const throw()
    {
      return "bad_CXFont"; // for now
    }
  };

  XFontStruct* operator->() { return _font ; }
  operator XFontStruct* () const { return _font ; }

  Font Id() const { return _font->fid ; }

  int FontHeight() const { return _font->ascent + _font->descent ; }
  short MaxCharWidth() const { return _font->max_bounds.width ; }

private:
  XFontStruct *_font ;
  CXDisplayPtr _display ;
  
  CXFont() {} ;
} ;

typedef std::shared_ptr<CXFont> CXFontPtr ;

#endif

#include "xwindow.h"
#include "xgc.h"
#include "xfont.h"
#include "xalloc.h"

#include <X11/Xos.h>
#include <X11/Xatom.h>

#include <iostream>

#include <boost/lexical_cast.hpp>

#include "bitmaps/icon_bitmap"

using namespace std ;

#define BITMAPDEPTH 1
#define TOO_SMALL 0
#define BIG_ENOUGH 1

static void draw_text(CXDisplay::Ptr& display, Window win, CXGC& gc,
                      XFontStruct *font_info,
                      unsigned int win_width, unsigned int win_height)
{
  string string1("Hi! I'm a window, who are you?") ;
  string string2("To terminate program; Press any key") ;
  string string3("or button while in this window.");
  string string4("Screen Dimensions:");
  int width1, width2, width3;
  int font_height;
  int initial_y_offset, x_offset;

  /* get string widths for centering */
  width1 = XTextWidth(font_info, string1.c_str(), string1.length());
  width2 = XTextWidth(font_info, string2.c_str(), string2.length());
  width3 = XTextWidth(font_info, string3.c_str(), string3.length());

  font_height = font_info->ascent + font_info->descent;

  /* output text, centered on each line */
  XDrawString(*display, win, gc, (win_width - width1)/2, 
              font_height,
              string1.c_str(), string1.length());
  XDrawString(*display, win, gc, (win_width - width2)/2, 
              (int)(win_height - (2 * font_height)),
              string2.c_str(), string2.length());
  XDrawString(*display, win, gc, (win_width - width3)/2, 
              (int)(win_height - font_height),
              string3.c_str(), string3.length());

  /* copy numbers into string variables */

  string cd_height(" Height - "
                   + boost::lexical_cast<string>(display->GetDisplayHeight())
                   + " pixels") ;

  string cd_width(" Width  - "
                  + boost::lexical_cast<string>(display->GetDisplayWidth()) 
                  + " pixels") ;

  string cd_depth(" Depth  - "
                  + boost::lexical_cast<string>(display->GetDefaultDepth())
                  + " plane(s)") ;

  /* To center strings vertically, we place the first string
   * so that the top of it is two font_heights above the center
   * of the window.  Since the baseline of the string is what we
   * need to locate for XDrawString, and the baseline is one
   * font_info->ascent below the top of the character,
   * the final offset of the origin up from the center of the 
   * window is one font_height + one descent. */

  initial_y_offset = win_height/2 - font_height - font_info->descent;
  x_offset = (int) win_width/4;
  XDrawString(*display, win, gc, x_offset, (int)initial_y_offset, 
              string4.c_str(), string4.length());

  XDrawString(*display, win, gc, x_offset, (int)initial_y_offset + 
              font_height, cd_height.c_str(), cd_height.length());
  XDrawString(*display, win, gc, x_offset, (int) initial_y_offset + 
              2 * font_height, cd_width.c_str(), cd_width.length());
  XDrawString(*display, win, gc, x_offset, (int) initial_y_offset + 
              3 * font_height, cd_depth.c_str(), cd_depth.length());
}


static void draw_graphics(CXDisplay::Ptr& display, Window win, CXGC& gc,
                          unsigned int window_width,
                          unsigned int window_height)
{
  int x, y;
  int width, height;

  height = window_height/2;
  width = 3 * window_width/4;
  x = window_width/2 - width/2;  /* center */
  y = window_height/2 - height/2;

  XDrawRectangle(*display, win, gc, x, y, width, height);
}


static void TooSmall(CXDisplay::Ptr& display, Window win, CXGC& gc,
                     XFontStruct *font_info)
{
  string string1("Too Small");
  int y_offset, x_offset;

  y_offset = font_info->ascent + 2;
  x_offset = 2;

  /* output text, centered on each line */
  XDrawString(*display, win, gc, x_offset, y_offset, string1.c_str(), 
              string1.length());
}


int main(int argc, char **argv)
{
  string window_name("Basic Window Program");
  string icon_name("basicwin");
  XIconSize *size_list;
  int count;
  int window_size = BIG_ENOUGH; // or TOO_SMALL to display contents

  try
  {
    auto display(CXDisplay::OpenDisplay()) ;

    /* get screen size from display structure macro */
    int display_width(display->GetDisplayWidth());
    int display_height(display->GetDisplayHeight());

    /* Note that in a real application, x and y would default to 0
     * but would be settable from the command line or resource database.  
     */
    int x(0); 
    int y(0);     /* window position */

    /* size window with enough room for text */
    int width(display_width / 3);
    int height(display_height / 4);
  
    unsigned int border_width(4);    /* four pixels */

    /* create opaque window */
    CXWindow win(display,
                    x, y, width, height, border_width,
                    display->GetBlackPixel(),
                    display->GetWhitePixel()) ;

    /* Get available icon sizes from Window manager */

    if (!XGetIconSizes(*display, display->GetRootWindow(), 
                       &size_list, &count))
    {
      cerr
        << argv[0] 
        << ": Window manager didn't set icon sizes - using default."
        << endl;
    }
    else
    {
      ;
      /* A real application would search through size_list
       * here to find an acceptable icon size, and then
       * create a pixmap of that size.  This requires
       * that the application have data for several sizes
       * of icons. */
    }

    /* Create pixmap of depth 1 (bitmap) for icon */
    Pixmap icon_pixmap = XCreateBitmapFromData(*display, win,
                                        icon_bitmap_bits, 
                                        icon_bitmap_width,
                                        icon_bitmap_height);

    /* These calls store window_name and icon_name into
     * XTextProperty structures and set their other 
     * fields properly. */
    XTextProperty windowName;
    char *tmp = const_cast<char *>(window_name.c_str());
    if (!XStringListToTextProperty(&tmp, 1, &windowName))
    {
      cerr << argv[0]
         << ": structure allocation for windowName failed."
         << endl ;
      return -1 ;
    }
    
    XTextProperty iconName;
    tmp = const_cast<char *>(icon_name.c_str());
    if (!XStringListToTextProperty(&tmp, 1, &iconName))
    {
      cerr << argv[0]
         << ": structure allocation for iconName failed."
         << endl ;
      return -1 ;
    }

    /* Set size hints for window manager.  The window manager may
     * override these settings.  Note that in a real
     * application if size or position were set by the user
     * the flags would be UPosition and USize, and these would
     * override the window manager's preferences for this window. */

    /* x, y, width, and height hints are now taken from
     * the actual settings of the window when mapped. Note
     * that PPosition and PSize must be specified anyway. */

    CXSizeHints size_hints;
    size_hints->flags = PPosition | PSize | PMinSize;
    size_hints->min_width = 300;
    size_hints->min_height = 200;

    CXWMHints wm_hints;
    wm_hints->initial_state = NormalState;
    wm_hints->input = True;
    wm_hints->icon_pixmap = icon_pixmap;
    wm_hints->flags = StateHint | IconPixmapHint | InputHint;

    CXClassHint class_hints;
    class_hints->res_name = argv[0] ;
    string res_class("Basicwin");
    class_hints->res_class = const_cast<char *>(res_class.c_str());

    XSetWMProperties(*display, win, &windowName, &iconName, 
                   argv, argc, size_hints, wm_hints, 
                   class_hints);

    /* Select event types wanted */
    XSelectInput(*display, win, ExposureMask | KeyPressMask | 
               ButtonPressMask | StructureNotifyMask);

    CXFont font_info(display, "9x15");

    unsigned long valuemask = 0; /* ignore XGCvalues and use defaults */
    XGCValues values;

    CXGC gc(display, win, valuemask, &values) ;
  
    /* specify font */
    gc.SetFont(font_info.Id());

   /* specify black foreground since default window background is 
    * white and default foreground is undefined. */
    gc.SetForeground(display->GetBlackPixel());

    /* set line attributes */
    gc.SetLineAttributes(6, LineOnOffDash, CapRound, JoinRound);

    int dash_offset = 0;
    char dash_list[] = {12, 24};
    int list_length = 2;

    /* set dashes */
    gc.SetDashes(dash_offset, dash_list, list_length);

    /* Display window */
    win.Map();

    /* get events, use first to display text and graphics */
    XEvent report;
    while(true)
    {
      XNextEvent(*display, &report);
      switch(report.type)
      {
        case Expose:
          /* unless this is the last contiguous expose,
           * don't draw the window */
          if (report.xexpose.count != 0)
            break;

          /* if window too small to use */
          if (window_size == TOO_SMALL)
            TooSmall(display, win, gc, font_info);
          else
          {
            /* place text in window */
            draw_text(display, win, gc, font_info,
                      width, height);

            /* place graphics in window, */
            draw_graphics(display, win, gc, width, height);
          }
          break;

        case ConfigureNotify:
          /* window has been resized, change width and
           * height to send to draw_text and draw_graphics
           * in next Expose */
          width = report.xconfigure.width;
          height = report.xconfigure.height;
          if ((width < size_hints->min_width) || 
              (height < size_hints->min_height))
            window_size = TOO_SMALL;
          else
            window_size = BIG_ENOUGH;
          break;

        case ButtonPress:
          /* trickle down into KeyPress (no break) */
        case KeyPress:
          return 0 ;

        default:
          /* all events selected by StructureNotifyMask
           * except ConfigureNotify are thrown away here,
           * since nothing is done with them */
          break;
      } /* end switch */
    } /* end while */
  }
  catch(exception const & e)
  {
    cerr << e.what() << endl ;
  }
      
  return -1 ;
}

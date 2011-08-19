#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/XKBlib.h>
#include <X11/Xlibint.h>
#include <X11/Xatom.h>
#include <X11/xpm.h>

#include "xxkb.h"

#define	NAME	"XXkb"

XImage *pic;

Display *dpy; int scr;
GC gc;
XEvent ev;
Window root, MainWin;
XWMHints *wm_hints;
XSizeHints *size_hints;
XClassHint class_hints;
char *name=NAME;
Atom wm_del_win;
XErrorHandler DefErrHandler;

int load_image(Display *dpy, char *name, XImage **pixmap)
{
  int res;
  res = XpmReadFileToImage(dpy, name, pixmap, NULL, NULL);
  switch (res)
  {
    case XpmOpenFailed:
      printf("Xpm file open failed: %s\n", name);
      break;
    case XpmFileInvalid:
      printf("Xpm file is invalid: %s\n", name);
      break;
    case XpmNoMemory:
      printf("No memory for open xpm file: %s\n", name);
      break;
    default:
      break;
  }
}

int main(int argc, char ** argv)
{
  int  xkbEventType, xkbError, reason_rtrn, mjr, mnr;
  Bool fout_flag = False;
  Atom protocols[1];

  dpy = XkbOpenDisplay( "", &xkbEventType, &xkbError, NULL, NULL, &reason_rtrn);
  if (!dpy)
  {
    printf("Can't connect to X-server: %s\n", getenv("DISPLAY"));
    switch (reason_rtrn)
    {
      case XkbOD_BadLibraryVersion :
      case XkbOD_BadServerVersion :
	printf("xxkb was compiled with XKB version %d.%02d\n",
	       XkbMajorVersion,XkbMinorVersion);
	printf("But %s uses incompatibl eversion %d.%02d\n",
	       reason_rtrn == XkbOD_BadLibraryVersion ? "Xlib" : "Xserver",
	       mjr,mnr);
	break;
      case XkbOD_ConnectionRefused :
	printf("Connection refused\n");
	break;
      case XkbOD_NonXkbServer:
	printf("XKB extension not present\n");
	break;
      default:
	printf("Unknown error %d from XkbOpenDisplay\n", reason_rtrn);
    }
    exit(1);
  }

  scr = DefaultScreen(dpy);
  root = RootWindow(dpy, scr);
  wm_del_win = XInternAtom(dpy, "WM_DELETE_WINDOW", False);
  DefErrHandler = XSetErrorHandler((XErrorHandler) ErrHandler);

  load_image(dpy, argv[1], &pic);

  MainWin = XCreateSimpleWindow(dpy, root,
				0, 0,
				48, 48, 0,
				BlackPixel(dpy, scr),
				WhitePixel(dpy, scr));

  wm_hints = XAllocWMHints();
  wm_hints->window_group = MainWin;
  wm_hints->input = False;
  wm_hints->flags = InputHint | WindowGroupHint;
  XSetWMHints(dpy, MainWin, wm_hints);
  XStoreName(dpy, MainWin, name);

  class_hints.res_name  = name;
  class_hints.res_class = name;
  XSetClassHint(dpy, MainWin, &class_hints);
  XSetCommand(dpy, MainWin, argv, argc);

  size_hints = XAllocSizeHints();
  size_hints->x = 48;
  size_hints->y = 48;
  size_hints->flags = USPosition;
  XSetNormalHints(dpy, MainWin, size_hints);

  *protocols = wm_del_win;
  XSetWMProtocols(dpy, MainWin, protocols, 1);

  Atom r;
  int data = 1;
  r = XInternAtom(dpy, "KWM_DOCKWINDOW", False);
  XChangeProperty(dpy, MainWin, r, r, 32, 0, (unsigned char *)&data, 1);

  XMapWindow(dpy, MainWin);

  XkbSelectEventDetails(dpy, XkbUseCoreKbd, XkbStateNotify,
                        XkbAllStateComponentsMask, XkbGroupStateMask);

  XSelectInput(dpy, MainWin, ExposureMask | ButtonPressMask | KeyPressMask);

  getGC(MainWin, &gc);

  while (1)
  {
    XNextEvent(dpy, &ev);

    switch (ev.type)
    {
      case Expose:
	if (ev.xexpose.count != 0)
	{
	  break;
	}
	if (ev.xexpose.window == MainWin)
	{
	  update_window(MainWin, gc, 0);
	}
	break;
      case ButtonPress:
	switch (ev.xbutton.button)
	{
	  case Button1:
	    break;
	  case Button2:
	    break;
	  case Button3:
	    break;
	  default:
	    break;
	}
	break;
      default:
	printf("Unknown event %d\n", ev.type);
	break;
    }
  }
  return(0);
}

void Terminate()
{
  XFreeGC(dpy,gc);
  XDestroyWindow(dpy,MainWin);
  XCloseDisplay(dpy);
  exit(0);
}

void getGC(Window win, GC *gc)
{
  unsigned long valuemask=0;
  XGCValues values;
  *gc = XCreateGC(dpy, win, valuemask, &values);
}

void update_window(Window win, GC gc, int group)
{
  XPutImage(dpy, win, gc, pic, 0, 0, 0, 0, 48, 48);
}

void ErrHandler(Display *dpy, XErrorEvent* err)
{
  if((err->error_code == BadWindow) || (err->error_code == BadDrawable))
  {
    return;
  }
  (*DefErrHandler)(dpy, err);
}

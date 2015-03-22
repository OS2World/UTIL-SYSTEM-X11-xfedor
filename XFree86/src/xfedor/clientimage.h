/* Copyright 1989 GROUPE BULL -- See licence conditions in file COPYRIGHT */
#ifndef _XLIB_H_
#include <X11/Xlib.h>
#endif

#define VIDAND		1	/*	 S .  D	-> D */
#define VIDSTR		3	/*	 S	-> D */
#define VIDXOR		6	/*	 S ^  D -> D */
#define VIDOR		7       /*	 S +  D -> D */

typedef XImage ClientImage ;

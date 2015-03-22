/* Copyright 1989 GROUPE BULL -- See licence conditions in file COPYRIGHT */
/* parties communes a tr_font et tr_extrafont */
/* HMENU deja defined */

#include "bitmaps/left"
#include "bitmaps/right"

#define LEFT 1
#define RIGHT 2
     
#define WSCROLL 35
#define HSCROLL 25
#define HFONT  (HMENU + HSCROLL)
#define HFONTMAX (HFONT + 128)
#define WFONT 896		/* 56*16, 28*32, 14*64, 7*128 */

#define WLIFT  (WFONT - 2*WSCROLL)

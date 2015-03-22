/* Copyright 1989 GROUPE BULL -- See licence conditions in file COPYRIGHT */
#include <stdio.h>
#include "couche.h"	/* myEvent */
#include "edit.h" 	/* MODBACK .. NOCONSTRAINT ..*/
#include "clientimage.h"
#include "fedor.h"
extern char * strcpy() ;
extern char * strcat() ;

extern fedchar cartrav;
extern modcolor ;
extern modepoint ;
extern CurColor ;

static Apoint(x,y)
	int x,y ;
{	int color ;
	if ((color = Adjpoint(x,y))!= -1) Carpoint(x,y,color) ;
}

Autom_point (pev)
	myEvent * pev ; 
{   static short etat ;
    static int a,b,x,y,xi,yi ; 	/* coords courantes et prec en 0-128 */
    char s[10] ;
	
    switch (pev->type) {
	case EnterZone : strcpy(s,(modepoint==XCONSTRAINT)?"x":
				  (modepoint==YCONSTRAINT)?"y":"");
			 strcat(s,(modcolor==MODBACK)?"Erase":
				  (modcolor==MODINV)?"Inverse":"Color");
			 Afficher_boutons(s,"Select");
			 break ;
	case ButtonPressed :
			etat = 1 ;
			xi = x = convert(pev->x) ;
			yi = y = convert(pev->y) ;
			if (pev->click == 1) Apoint(x,y) ;
	                  else {
			    CurColor = Rast_Inq(cartrav.image,x,y) ;
			    Afficher_colors() ;  /* sur fenetre tr_edit */
			  }
			break ;
	case ButtonReleased : etat = 0 ;
			 Aff_all() ;
			      break ;
	case MoveMouse : if (etat) {
			    if (modepoint == NOCONSTRAINT) {
			    	a = convert(pev->x) ;
			    	b = convert(pev->y) ;
			    	if ( (x!=a) || (y!=b)) Apoint(a,b) ;
			    } else
			    if (modepoint == XCONSTRAINT) {
				b = convert(pev->y) ;
			    	if (y!=b) Apoint(xi,b) ;
			    } else
			    if (modepoint == YCONSTRAINT) {
				a = convert(pev->x) ;
			    	if (x!=a) Apoint(a,yi) ;
			    }
			    x = a ; y = b ;
			 }
			 break ;				
	case LeaveZone : 
		Afficher_boutons("","");
	        Afficher_numview(-1,-1) ; /* restaure le gris */
		break ;
	case CloseWindow : break ;				
    }
} 

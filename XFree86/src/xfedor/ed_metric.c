/* Copyright 1989 GROUPE BULL -- See licence conditions in file COPYRIGHT */
#include <stdio.h>
#include "couche.h"	/* myEvent */
#include "clientimage.h"	/* ClientImage pour fedor.h */
#include "fedor.h"	/* fedchar */

extern fedchar cartrav ;
extern Xleft,Baseliney ;

int MetrAff = 0 ;

Autom_metric(pev)
	myEvent * pev ; 
{  
	 int dhadj ;

    switch (pev->type) {
	case EnterZone : Afficher_boutons("HADJ","HINCR");
			 BiBox();
			 Invpoint(Xleft - cartrav.hadj, Baseliney);
			 Invpoint(Xleft - cartrav.hadj +cartrav.hincr , 
				  Baseliney);
			 MetrAff = 1 ;
			 break ;
	case ButtonPressed : 
			     if (pev->click == 1) {
				 Invpoint(Xleft - cartrav.hadj , Baseliney);
				 dhadj = cartrav.hadj ;
				 cartrav.hadj = Xleft -	convert(pev->x) ;
				 cartrav.hincr += (cartrav.hadj - dhadj) ;
				 Invpoint(Xleft - cartrav.hadj , Baseliney);
			     } else
			     if (pev->click == 2) {
			 	 Invpoint(Xleft -cartrav.hadj+cartrav.hincr , 
					  Baseliney);
				 cartrav.hincr = convert(pev->x) -
						Xleft + cartrav.hadj;
			 	 Invpoint(Xleft -cartrav.hadj+cartrav.hincr , 
					  Baseliney);
			     } 	
			     Aff_all() ;
			     break ;
	case ButtonReleased : break ;
	case MoveMouse : break ;				
	case LeaveZone : Afficher_boutons("","");
	          	 Afficher_numview(-1,-1) ; /* restaure le gris */
			 BiBox();
			 Invpoint(Xleft - cartrav.hadj, Baseliney);
			 Invpoint(Xleft - cartrav.hadj + cartrav.hincr , 
				  Baseliney);
			 MetrAff = 0 ;
			 break ;
    }
} 

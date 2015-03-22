/* Copyright 1989 GROUPE BULL -- See licence conditions in file COPYRIGHT */
#include <stdio.h>
#include "couche.h"	/* myEvent */
#include "style.h"	/* NORMAL .. */

static Carline(xorig,yorig,xfin,yfin)
	int xorig,yorig,xfin,yfin ;

{
     register short e,s1,s2,dx,dy;
     int x,y ;
     int inter,i;

     if ((xorig == xfin)&&(yorig == yfin))    
        { Adjpoint(xorig,yorig);
          return;
        }  
     x = xorig;
     y = yorig;
     if ((dx = (xfin - xorig)) <0)
         {s1 = -1;
          dx = -dx;
         }
         else
         {if ((xfin - xorig) >0)
             { s1 = 1;}
             else
             { s1 = 0;}
         }
     if ((dy = (yfin - yorig)) <0)
         {s2 = -1;
          dy = -dy;
         }
         else
         {if ((yfin -yorig) >0)
             { s2 = 1;}
             else
             { s2 = 0;}
         }
    /* on va echanger dx & dy suivant la pente */
    if ( dy > dx)
      { i = dx;
        dx = dy;
        dy = i;
        inter = 1 ;
      }
      else
      { inter = 0;}

    /* erreur initiale */
    e = 2*dy - dx;
    for (i = 1; i<=dx; i+=1)
       {
        Adjpoint(x,y);
        while (e >= 0)
           {
            if (inter)
               { x += s1;}
               else
               { y += s2;}
            e -= 2*dx;
           } /* end while */
        if (inter)
             { y += s2;}
             else
             { x += s1;}
        e += 2*dy;
       } /* end for */
          /* ON AFFICHE LE DERNIER POINT */  
     Adjpoint(x,y);
     Aff_all() ;
     CarFen();
}      


Autom_line (pev)
	myEvent * pev ; 
{   static int x,y ;
	
    switch (pev->type) {
	case EnterZone : Afficher_boutons("ORIGIN","ORIGIN");
			 x = -1 ;
			 break ;
	case ButtonPressed : if (x == -1 ) {
				Afficher_boutons("ENDLINE","ENDLINE");
			    	x = pev->x ; 
			    	y = pev->y ; 
			     	stylesouris(LIGNE,x,y);
			     }
			     break ;
	case ButtonReleased : stylesouris(NORMAL,0,0);
			      Afficher_boutons("ORIGIN","ORIGIN");
			      if (x != -1) {
				Dodo();
				Carline(convert(x),
					convert(y),
					convert(pev->x),
					convert(pev->y));
			      }
			      x = -1 ;
			      break ;
	case MoveMouse : break ;				
	case CloseWindow : break ;				
	case LeaveZone : stylesouris(NORMAL,0,0);
	        	 Afficher_numview(-1,-1) ; /* restaure le gris */
			 Afficher_boutons("","");
			 break ;
    }
} 

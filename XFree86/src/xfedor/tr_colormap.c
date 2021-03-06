/* Copyright 1989 GROUPE BULL -- See licence conditions in file COPYRIGHT */
#include <stdio.h>
#include "math.h"               /* ceil , sqrt */
#include "couche.h" 		/* myEvent */

extern root ;
extern BackColor, CurColor ;

void Init_colormap() ;
void Autom_colormap() ;

int nf_colormap ;   /* NUMERO DE LA FENETRE COLORMAP */      


	/*---------- reste du LOCAL */

#define XCMAP  200
#define YCMAP  200

int wcell ;         /* cote d'une cellule couleur */
int ncell ;         /* nombre de cellule par ligne si fenetre carre */
int xcell ;         /* nombre de cellule par ligne */
int ycell ;         /* nombre de cellule par colonne */
int Wcmap, Hcmap ;  /* dimension de la fenetre */
int ncolors ;       /* nombre de couleurs disponibles au max */

void Init_colormap()
{
     ncolors = w_ncolor() ;         /* entree max dans la colormap */

     /* on fait une fenetre carre si il y a moins de 16 couleurs, 
	sinon on la fait rectangulaire verticale */
     wcell = (ncolors>16)?16:32 ;
     ncell = (int)ceil(sqrt((double)ncolors)) ;
     xcell = (ncolors>16)?ncell/2:ncell ;
     ycell = (ncolors>16)?ncell*2:ncell ;
     Wcmap = xcell*(wcell+2); 
     Hcmap = ycell*(wcell+2); 
       
     nf_colormap = w_ouvrir(root,XCMAP,YCMAP,2+Wcmap,2+Hcmap,
		  	"XFEDOR : Colormap Window",
		  	CURS2,
		  	EnterWindow|LeaveWindow|ButtonPressed,WMON) ;
}


static Reaffich(fen)
int fen ;
{ 
  int i, j ;

  if (fen == nf_colormap)  {
    for (i=0 ; i < xcell ; i++)
      for (j=0 ; j < ycell ; j++)
	w_color(nf_colormap,2+i*(wcell+2),2+j*(wcell+2),
		2+i*(wcell+2)+wcell,2+j*(wcell+2)+wcell,
		((i*xcell+j)>ncolors)?BackColor:(i*xcell+j));
    /* cad que l'on met du BackColor la ou ya des trous */
  }
}


static int Traiter_clic(evt)
	myEvent * evt ;
{	    
  int i, j ;

  if (evt->click == 2) 
    {
      w_cacher(nf_colormap) ;
      return ;
    } else {
      i = (evt->x - 2)/(wcell + 2) ; if (i < 0) i = 0 ;
      j = (evt->y - 2)/(wcell + 2) ; if (j < 0) j = 0 ;
      CurColor = ((i*xcell+j)>ncolors)?BackColor:(i*xcell+j) ;
      Afficher_colors() ;  /* sur fenetre tr_edit */
    }
}


void Autom_colormap(pev)
	myEvent * pev ;
{

     switch (pev->type){
	case EnterWindow   : 
                Afficher_boutons("Select","Quit") ;
		break;   
	case Exposure :
		Reaffich(pev->window) ;
		break ;
 	case ButtonPressed :  
		Traiter_clic(pev);
           	break ;
        case LeaveWindow : 
		Afficher_boutons("","") ; 
		break ;
	case CloseWindow :
	        w_cacher(nf_colormap) ;
		break ;
      }		
}




/* Copyright 1989 GROUPE BULL -- See licence conditions in file COPYRIGHT */
#include <stdio.h>
#include "clientimage.h"  /* ClientImage pour fedor.h  */
#include "couche.h"	  /*  CURS2 */
#include "fedor.h"	  /* fedchar .. MAXFONTCARD */

extern root ;
extern fedchar cartrav ;	/* depuis tr_edit */
extern int Baseliney,Xleft ;		/* depuis tr_edit */
extern int BackColor, CurColor ;

void Init_extrafont();
void Extra_Resize() ;
void Autom_extrafont();

void ShowArrow();      
void ShowAChar() ;
void Scroll(); 
void ShowLift();		/* pour la fentre fonte principale */
int Shadow() ;
void ShowAFont() ;
void GetFromaFont() ;

int nf_extrafont ;   /* NUMERO DE LA FENETRE FONTE */      

fedchar extratab[MAXFONTCARD];	/* pour filer GetFont */

int PremExtra ; 

	/*------------ reste du LOCAL */
#define HMENU 0 
#include "font.h"

#define XFONT 0
#define YFONT (10+514+10+22+25+128)

static int zfont,zleft,zright,zlift ;

static int maxup ;

static int lifton = 0 ; 	/* flag ascenseur en cours */

static int Dern  ;		/* indice dans extratab */

static int Resol ; 		/* 16,32,64,ou 128 */
				   
static hfont ;			/* hauteur zone fonte */

static int barlift ;

void Init_extrafont () 
/***************/
{ 
    nf_extrafont = w_ouvrir(root,XFONT,YFONT,WFONT,HFONTMAX,
	     		"xfedor_extra",CURS2,
	     		EnterZone|ButtonPressed|ButtonReleased|
	     		MoveMouse|LeaveZone,WMON) ;

    zleft = w_crzon(nf_extrafont,0,0,WSCROLL,HFONT,CURS2);
    zlift = w_crzon(nf_extrafont,WSCROLL,0,WFONT-WSCROLL,HFONT,CURS2);
    zright = w_crzon(nf_extrafont,WFONT-WSCROLL,0,WFONT,HFONT,CURS2);

    zfont =  w_crzon(nf_extrafont,0,HFONT,WFONT,HFONTMAX,CURS1);
}

static Reaffich(fen) 
int fen ;
{
    if (fen == nf_extrafont) {
      w_clear(nf_extrafont);
      w_line(nf_extrafont,WSCROLL,0,WSCROLL,HFONT,ON);
      w_line(nf_extrafont,WFONT-WSCROLL,0,WFONT-WSCROLL,HFONT,ON);

      ShowArrow(nf_extrafont,HFONT) ;
      ShowAFont(nf_extrafont,extratab,PremExtra,
		Dern,Resol,HFONT,hfont,maxup) ; 
		/* valeurs calculees par Extra_Resize dans GetExtraFont */
    }
}    
   

void ShowArrow(nf,Hfen)
/*-------------*/
	int nf ;
	int Hfen ;	
{	int x ,y ;

	x = (WSCROLL - left_width)/2 ;
	y = Hfen - (HSCROLL +left_height)/2 ;
    	w_pixel(nf,left_bits,x,y,left_width,left_height);
    	w_pixel(nf,right_bits,WFONT-WSCROLL+x,y,right_width,right_height);
}

void ShowAChar(nf,tabchar,code,Prem,Dern,Resol,Hfen,hfont,maxup)
/*-------------*/
	int nf ;	/* fenetre */
	fedchar * tabchar ;	/* tableau ou sont les car */
	int code ;	/* indice dans tabchar du car a afficher */
	int Prem,Dern ; /* indice du premier et dernier visibles */
	int Resol ;	/* 16, 32, 64, 128 */
	int Hfen ;	/* ordonnee origine zone fonte */
	int hfont ;	/* taille reelle zone fonte */
	int maxup ;	/* max dans tabchar.up */

{	int xcode ;

	if ( (code < Prem) || (code > Dern)) return; /* invisible ? */
	
	xcode = (code - Prem) * Resol ;
	
	if (tabchar[code].hsize <= 0)	
	  	  w_gris(nf,xcode,Hfen+1,xcode+Resol,Hfen+hfont);
	else { 	  w_cleararea(nf,xcode,Hfen+1,xcode+Resol,Hfen+hfont);
		  w_bitblt(nf,tabchar[code].image,
			0,0,tabchar[code].hsize,
			tabchar[code].down-tabchar[code].up,
			xcode+(Resol-tabchar[code].hsize)/2,
			Hfen-maxup+tabchar[code].up+1);
        }	
	w_line(nf,xcode,Hfen,xcode+Resol,Hfen,ON);  
	w_line(nf,xcode,Hfen,xcode,Hfen+hfont,ON);
}


void Scroll(nf,tabchar,dir,Prem,Dern,Resol,Hfen,hfont,maxup)
/*-------------*/
	int nf ;	/* fenetre */
	fedchar * tabchar ;	/* tableau ou sont les car */
	int dir ;	/* LEFT ou RIGHT  */
	int *Prem,*Dern ; /* indice du premier et dernier visibles */
	int Resol ;	/* 16, 32, 64, 128 */
	int Hfen ;	/* ordonnee origine zone fonte */
	int hfont ;	/* taille reelle zone fonte */
	int maxup ;	/* max dans tabchar.up */
{
	int xsrc,ysrc,xdest,ydest ;
	int l,h;
		
	xsrc = xdest = -1; ysrc = ydest = Hfen ; 
	ShowLift(nf,OFF,*Prem,*Dern,Hfen) ;
	if (dir == RIGHT) {
		if (*Dern < MAXFONTCARD - 1) {
			(*Prem)++; (*Dern)++;
			xsrc +=  Resol  ;
		}
	} else 
 	if (*Prem > 0) {
		(*Prem)-- ; (*Dern)--;
		xdest += Resol  ;
	}
	/* decaler tous les caracteres  */
	w_rastop(nf,nf,xsrc, ysrc, (*Dern - *Prem)*Resol,hfont,xdest, ydest);
	/* Afficher le caractere */	
        ShowAChar(nf,tabchar,(dir == RIGHT) ? *Dern : *Prem,
			*Prem,*Dern,Resol,Hfen,hfont,maxup);
	ShowLift(nf,ON,*Prem,*Dern,Hfen) ;
}

void ShowLift(nf,onoff, Prem, Dern, Hfen)
/*-------------*/
	int nf ;	/* fenetre */
	int onoff;
	int Prem,Dern ; /* indice du premier et dernier visibles */
	int Hfen ;	/* ordonnee origine zone fonte */
{
	int xliftpos  ;	/* position ascenseur */
	int barlift ;

	barlift = (WLIFT*(Dern - Prem))/MAXFONTCARD ;	/* largeur barre ascenceur */
	xliftpos = WSCROLL + (Prem * WLIFT)/MAXFONTCARD + 1 ;
	if (onoff == ON)
	 	 w_gris(nf,xliftpos ,Hfen - HSCROLL + 2,xliftpos + barlift, 
				Hfen  - 2);
        else  	 w_cleararea(nf,xliftpos ,Hfen - HSCROLL+ 2,
				xliftpos + barlift, Hfen - 2);
	
}


int Shadow(nf,x,Prem, Dern, Hfen)
/*-------------*/
	int nf ;	/* fenetre */
	int x ;
	int Prem,Dern ; /* indice du premier et dernier visibles */
	int Hfen ;	/* ordonnee origine zone fonte */

/* soit x=-1 et on efface l'ascenseur a l'ex shadow
   sinon on trace un nouvel ascenseur en fonction de x */
/* rends l'abcisse de l'ascenceur corrige pour la limite WLIFT */
{
	static xshadow ;

 	if ( x > 0) {
		xshadow = x;
		barlift = (WLIFT*(Dern - Prem))/MAXFONTCARD ;
		if ((xshadow + barlift ) > WLIFT)
			xshadow = WLIFT - barlift - 1;
		xshadow += WSCROLL;
	}
	w_inv(nf,xshadow,Hfen-HSCROLL+2,xshadow + barlift,Hfen-2) ;
	return xshadow ;
}


		 
void Extra_Resize()
/* calcule maxup, hfont et Resol */
{
	int code ;
	int maxdown,maxhsize ;
	static int old_hfont ;

	maxup = maxdown = maxhsize = 0 ;
	for (code=0; code < MAXFONTCARD; code++) 
	   if (extratab[code].hsize > 0) {
		if (extratab[code].up < maxup ) 
			maxup = extratab[code].up ;
		if (extratab[code].down > maxdown ) 
			maxdown = extratab[code].down ;
		if (extratab[code].hsize > maxhsize ) 
			maxhsize = extratab[code].hsize;
	   }

	hfont = (((maxdown-maxup)>0)?(maxdown-maxup):32) + 1;
	Resol = New_Resol((maxhsize>0)?maxhsize:32);
	Dern = PremExtra + WFONT / Resol - 1 ;
	w_resize(nf_extrafont,WFONT,HFONT+hfont);
	if (hfont <= old_hfont) Reaffich(nf_extrafont); 
	old_hfont = hfont ;
}
 
void ShowAFont(nf,tabchar,Prem,Dern,Resol,Hfen,hfont,maxup)
/*-------------*/
	int nf ;	/* fenetre */
	fedchar * tabchar ;	/* tableau ou sont les car */
	int Prem,Dern ; /* indice du premier et dernier visibles */
	int Resol ;	/* 16, 32, 64, 128 */
	int Hfen ;	/* ordonnee origine zone fonte */
	int hfont ;	/* taille reelle zone fonte */
	int maxup ;	/* max dans tabchar.up */

{	int code;

	for (code = Prem ; code <= Dern ; code++) 
	    ShowAChar(nf,tabchar,code,Prem,Dern,Resol,
				Hfen,hfont,maxup);
	ShowLift(nf,ON,Prem,Dern,Hfen) ;

}

void GetFromaFont(tabchar,code)
/*-------------*/
	fedchar * tabchar ;
	int code ;			/* code dans tabchar */
{
	if ( (code >= 0) && (code < MAXFONTCARD) && 
	    (tabchar[code].hsize > 0 )) {
	        Dodo(); 
		CopyCar(&tabchar[code],&cartrav);
		Xleft = 0 ;
		Baseliney = - cartrav.up ;
		if (Baseliney < 0) Baseliney = 0 ;
		Rast_Off(&cartrav.image,BackColor) ;
		Rast_Op_1n(tabchar[code].image,&cartrav.image,
			0,0,
			Xleft,Baseliney+cartrav.up,
			cartrav.hsize,cartrav.down-cartrav.up,
			VIDSTR,CurColor);
		MontrerCarTrav();
	}
}


static int Traiter_zone(evt)
/* on a recu un click dans la zone */
	myEvent * evt ;
{
	int i ;

	if ((evt->zone == zleft) && (evt->click == 1))
		Scroll(nf_extrafont,extratab,LEFT,&PremExtra,&Dern,
				Resol,HFONT,hfont,maxup) ; else
	if ((evt->zone == zleft) && (evt->click == 2))
		Scroll(nf_extrafont,extratab,RIGHT,&PremExtra,&Dern,
				Resol,HFONT,hfont,maxup) ; else
	if ((evt->zone == zright) && (evt->click == 1))
		Scroll(nf_extrafont,extratab,RIGHT,&PremExtra,&Dern,
				Resol,HFONT,hfont,maxup) ; else
	if ((evt->zone == zright) && (evt->click == 2))
		Scroll(nf_extrafont,extratab,LEFT,&PremExtra,&Dern,
				Resol,HFONT,hfont,maxup) ; else
	if (evt->zone == zlift) {
	  lifton = 1 ;
	  Shadow(nf_extrafont,evt->x,PremExtra,Dern,HFONT);
	} else
	if (evt->zone == zfont)  
	  GetFromaFont(extratab,PremExtra + evt->x/Resol); 
}

/*********************** procedure AUTOM_EXTRAFONT ***********************/ 
/* Des que le controleur detecte l'entree de la souris dans la fenetre,
   il appelle cette procedure */

void Autom_extrafont(pev) 
  myEvent * pev ;  
{
  int code ;	/* pour reaffichage apres scroll ascenseur */
  int xshadow ;

  switch (pev->type){
	case Exposure :
		Reaffich(pev->window) ;
		if (lifton) Shadow(nf_extrafont,-1,PremExtra,Dern,HFONT);
		break ;
	case EnterZone   : 
	  if (pev->zone==zfont) Afficher_boutons("GET","GET") ; else
	  if (pev->zone == zlift) {
	    Afficher_boutons("Select","Select"); 
	    lifton = 1 ;
	    Shadow(nf_extrafont,pev->x,PremExtra,Dern,HFONT);
	  } else Afficher_boutons("Select","Select"); 
 	  break ;
	case ButtonPressed :  
	  if ((pev->zone != zlift) || (lifton == 0)){
	    Traiter_zone(pev);
	    break ;
	  }    /* no break : meme effet que le release ... SHAME on CODE */
 	case ButtonReleased :
	  if ((pev->zone==zlift)&&(lifton)) { 
	    xshadow = Shadow(nf_extrafont,-1,PremExtra,Dern,HFONT);
	    /* efface l'ascenseur courant et recupere xshadow */
	    lifton = 0 ;
	    ShowLift(nf_extrafont,OFF,PremExtra,Dern,HFONT) ;
	    xshadow -= WSCROLL ;
	    PremExtra = (xshadow * MAXFONTCARD) / WLIFT ;
	    Dern = PremExtra + WFONT/Resol - 1;
	    if (Dern >= MAXFONTCARD) Dern = MAXFONTCARD -1 ;
	    for (code=PremExtra ; code <= Dern ; code++) 
	      ShowAChar(nf_extrafont,extratab,code,
			PremExtra,Dern,Resol,
			HFONT,hfont,maxup);
	    ShowLift(nf_extrafont,ON,PremExtra,Dern,HFONT) ;
	  }
	  break ;		
   	case MoveMouse :
		if ((pev->zone==zlift)&&(lifton)) {
			Shadow(nf_extrafont,-1,PremExtra,Dern,HFONT);
			Shadow(nf_extrafont,pev->x,PremExtra,Dern,HFONT);
		}
		break ;
        case LeaveZone : 
		if ((pev->zone==zlift)&&(lifton)) {
			Shadow(nf_extrafont,-1,PremExtra,Dern,HFONT);
			lifton = 0 ;
		}
		Afficher_boutons("","");
		break ;
	case CloseWindow :
	    w_cacher(pev->window); //ET: not sure if this is enough...
	    break;
      }
}
            

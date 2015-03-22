/* Copyright 1989 GROUPE BULL -- See licence conditions in file COPYRIGHT */
#include <stdio.h>
#include "couche.h"	  /*  CURS2 */
#include "clientimage.h"  /* ClientImage pour fedor.h */
#include "fedor.h"	  /* BdfHeader */
#include "sos.h"	  /* SAVE */

extern nf_mono ;
extern char FileName[20] ;	/* depuis filer.c */
extern BdfHeader header ;	/* pour entete du ficher */


void Autom_save();
void Init_save();
void Aff_filename();
void ShowHeader() ;
static void Aff_item() ;
      
int nf_save ;   /* NUMERO DE LA FENETRE SAVE */      

	/*-------- le reste est du LOCAL */

static int     znamefile,zsave,zhelp,zfamilyname,zsize,zresol ;

static char * llib [] = {"FONTNAME","SIZE","RESOL"};

#define RangFontname 0
#define RangSize 1
#define RangResol 2

#define NbSaveItem 3
#define HSAVITEM 22
#define WSAVITEM 98
#define WVAL 142
#define WSAVE (WSAVITEM+WVAL)
#define HSAVE (NbSaveItem*HSAVITEM + HEAD)
#define HEAD 28

#define XSAVE (10 + 100+514+12 + 10 + 2)
#define YSAVE  (10 + 22 + 44 )  /* 2*HITEM de nf_tabbord */


void Init_save () 

{    int i,y ;

    nf_save = w_ouvrir(nf_mono, XSAVE, YSAVE, WSAVE, HSAVE,
		  	"XFEDOR : Save",CURS2,
		  	EnterZone|ButtonPressed|LeaveZone,WMON) ;

    znamefile = w_crzon(nf_save,WSAVITEM,0,WSAVE,HEAD,CURS2);
    zsave = w_crzon(nf_save,0,0,WSAVITEM,HEAD,CURS2);
    zhelp = w_crzon(nf_save,0,HEAD,WSAVITEM,HSAVE,CURS2);

    zfamilyname = w_crzon(nf_save,WSAVITEM,HEAD+RangFontname*HSAVITEM,
				WSAVE,HEAD+(RangFontname+1)*HSAVITEM,CURS2);
    zsize = w_crzon(nf_save,WSAVITEM,HEAD+RangSize*HSAVITEM,
				WSAVE,HEAD+(RangSize+1)*HSAVITEM,CURS2);
    zresol = w_crzon(nf_save,WSAVITEM,HEAD+RangResol*HSAVITEM,
				WSAVE,HEAD+(RangResol+1)*HSAVITEM,CURS2);
}

static Reaffich(fen)
int fen ;
{
    int y,i ;

    if (fen == nf_save) {
	 for (i=1,y=HEAD;i<=NbSaveItem;i++,y+=HSAVITEM) {
		 w_line(nf_save,0,y,WSAVE,y,ON);
		 w_centrer(nf_save,llib[i-1],0,y,WSAVITEM,
				y+HSAVITEM);
	 }
     w_line(nf_save,WSAVITEM,0,WSAVITEM,HSAVE,ON); /* barre vertic */
     w_centrer(nf_save,"S A V E",0,0,WSAVITEM,HEAD);
	
    ShowHeader() ;
    }
}

/******** sous modules de l'automate general Autom_garb *******/


static int Inverser_zone(zone)
	int zone;
{   int y ;

    y = HEAD + 2 ;
    if (zone==zfamilyname) y += RangFontname*HSAVITEM ; else
     if (zone==zsize) y += RangSize*HSAVITEM ; else
    if (zone==zresol) y += RangResol*HSAVITEM ; 

    if ((zone!=zhelp)&&(zone!=znamefile)&&(zone!=zsave)) 
		w_inv(nf_save,WSAVITEM+2,y,WSAVE-2,y+HSAVITEM-4);
    if (zone==zsave)  
		w_inv(nf_save,2,2,WSAVITEM-2,HEAD-2);
    if (zone==znamefile)  
		w_inv(nf_save,WSAVITEM+2,2,WSAVE-2,HEAD-2);                 
			 
}	

static int Traiter_clic(evt)
	myEvent * evt ;
{	int i;
	char s[20] ;

	if (evt->zone==zhelp) Helping(SAVE); else
        if (evt->zone==zsave) {
			   if (gd_confirm()) {
				if(SauveFont()) {
				   gd_envoyermsg("CANNOT SAVE FONT") ;
				}
			   }
			   w_cacher(nf_save) ;
			   } else
	if (evt->zone==znamefile) {
			   gd_lirechaine("Filename ?",FileName); 
			   Aff_filename();		   
			  } else
	if (evt->zone==zfamilyname) {
			   gd_lirechaine("FAMILY NAME ?",header.FamilyName);
			   Aff_item(RangFontname,header.FamilyName);
			  } else
	if (evt->zone==zsize) {
			   gd_lireentier("SIZE ?",&header.Size);
			   sprintf(s,"%d",header.Size);
			   Aff_item(RangSize,s);
			   } else
	if (evt->zone==zresol) {
			   gd_lireentier("RESOLUTION ?",&header.Resol);
			   sprintf(s,"%d",header.Resol);
			   Aff_item(RangResol,s);
			   } 
}

void Aff_filename()
/* affiche le nom du fichier devant contenir la fonte courante */
{
	w_centrer(nf_save,FileName,WSAVITEM,0,WSAVE,HEAD);
}


static void Aff_item(rang,s)
/* affiche la chaine s au niveau dependant de rang (rang 1,2..) */
	int rang ;
	char * s ;
{
	w_centrer(nf_save,s,WSAVITEM,HEAD+rang*HSAVITEM,WSAVE,
			HEAD+(rang+1)*HSAVITEM);
}


void ShowHeader() 
/* affiche les valeurs du BdfHeader  courant : header */
/* header est rempli par GetFont("initfont" | Name ) */
{	char s[20] ;
	
	
	Aff_filename(0);

	Aff_item(RangFontname,header.FamilyName);	
	sprintf(s,"%d",header.Size);
	Aff_item(RangSize,s);
	sprintf(s,"%d",header.Resol);
	Aff_item(RangResol,s);

}


void Autom_save(pev) 
  myEvent * pev ;  
{ 
	static zonecour = 0 ;
  switch(pev->type){
	case Exposure :
		Reaffich(pev->window) ;
		if (zonecour) Inverser_zone(zonecour);
		break ;
	case EnterZone   : 
		if(pev->zone==zhelp) Afficher_boutons("Help","Help") ; else
				     Afficher_boutons("Select","Select") ;
		Inverser_zone(pev->zone);
		zonecour = pev->zone ;
		break;
 	case ButtonPressed :  
		Traiter_clic(pev);
           	break ;
        case LeaveZone : 
		Inverser_zone(pev->zone);
		zonecour = 0 ;
	        Afficher_boutons("","") ;
        	break ;
	case CloseWindow : break ;				
  }
}

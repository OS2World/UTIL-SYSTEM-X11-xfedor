/* Copyright 1989 GROUPE BULL -- See licence conditions in file COPYRIGHT */
#define	MAXFONTCARD		256

typedef struct {
	int hincr,vincr;
	int hadj ,vadj;
	char name[50] ;
	int up,down,hsize ;

	ClientImage * image ;		
} fedchar ,*fedcharptr ;


typedef struct {
	char FamilyName[256] ;   
	int Size ;
	int Resol ;		/* on lit xres seul */
	char Properties[50][256] ;     /* on stocke au max 50 proprietes */
	int nprops ;
} BdfHeader ;

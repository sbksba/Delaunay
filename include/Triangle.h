#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__

#include "Define.h"
#include "Vecteur.h"

// triangle 3D
typedef struct sTriangle
{
	int type;	// Type de terrain du triangle
	int flip_param;	//param pour le flip
	int p1,p2,p3;		//indice sur le tableau de sommets dans la structure TabDel
	struct sTriangle *t1,*t2,*t3;	//pointeurs vers les voisins
	SPoint centre;	//centre defini comme la structure, mais en 2d donc le z ne sert a rien
	double rayon;	//rayon du cercle circonscrit au triangle
	double critere;	//critere pour permettre de choisir un triangle par rapport a un autre (ici l'aire)
}STriangle;

//liste chainee de triangle
typedef struct sLTriangle
{
	STriangle *t;
	struct sLTriangle *next;
}SLTriangle;

//premier élément de la liste chaînée
typedef struct sStartT
{
	SLTriangle *first;
}SStartT;

//la structure qui définit les triangles et les points du terrain
typedef struct sTabDel
{
	int nt; 		//nbr de triangles, pour info...
	SStartT *triangles;	//les triangles sont en liste chaînée

	int np; 		//nbr de points
	SPoint p[MAX_POINTS]; 	//le tableau de points
}STabDel;


STriangle* Triangle_Cree(int x, int y, int z); //alloue et cree le triangle
#endif


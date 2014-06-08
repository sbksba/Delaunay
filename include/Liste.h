#ifndef H_LISTE
#define H_LISTE

#include "Define.h"
#include "Triangle.h" 

//fonctions betes, a ne pas oublier qu'on a la structure TabDel en globale
void Liste_AddTriangle(STriangle *t1, SStartT *t); //ajout de t1 dans t
int Liste_EffTriangle(STriangle *t1, SStartT *t2); //elimination de t1 dans t2
int Liste_TriangleIsIn(STriangle *t1, SStartT *t2); //est-ce que t1 est dans la liste t2


void Global_AddTriangleListe(STriangle *t); 	//ajoute t dans la liste globale de triangle

void Global_DestructionListe(SStartT *t); 	//destruction d'une liste

//Rend si l'element a ete trouve
int Global_EffTriangleListe(STriangle *t);  	//on suppose qu'il n'y a plus de prob d'adjacence...

//fonctions pour gerer la structure TabDel GLOBALE
int Global_AddPoint(double x,double y,double z); //ajoute un point dans la structure, retourne l'indice de ce point
#endif


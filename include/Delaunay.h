#ifndef H_DELAUNAY
#define H_DELAUNAY

#include "Delaunay.h"
#include "Triangle.h"
#include "Liste.h"

int Delaunay_ChoisitPoint();  				//rend l'indice du point ajoute dans la structure TabDel
int Delaunay_PointInCercle(int p, STriangle *t); 	//est-ce que p est dans le cercle circonscrit de t

#endif

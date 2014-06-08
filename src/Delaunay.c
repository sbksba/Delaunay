
#include "Delaunay.h"

//les variables externes
extern STabDel tab;
extern SParam param;

//rend l'indice du point ajoute dans la structure TabDel
int Delaunay_ChoisitPoint()
{
	STriangle *t;
	double coef1,coef2,coef3;
	double sum;

	//On cherche le premier triangle de la liste, c'est par definition celui qui a la plus grande aire
	t = tab.triangles->first->t;
	
	//Est-ce qu'on veut juste le barycentre
	if(param.barycentre)
		{coef1=coef2=coef3=1;}
	else //veut-on une version ponderee
		{
		//on fait un modulo juste pour que sum soit bien calcule, lie aux limitations de l'informatique...
		coef1 = rand()%5+1;
		coef2 = rand()%5+1;
		coef3 = rand()%5+1;
		}
	
	sum = coef1 + coef2 + coef3;
	
	//on calcule la position ponderee du nouveau point
	//on ajoute le point et on retourne son indice
	return Global_AddPoint(	(tab.p[t->p1].x*coef1 + tab.p[t->p2].x*coef2 + tab.p[t->p3].x*coef3)/sum,
						(tab.p[t->p1].y*coef1 + tab.p[t->p2].y*coef2 + tab.p[t->p3].y*coef3)/sum,
						(tab.p[t->p1].z*coef1 + tab.p[t->p2].z*coef2 + tab.p[t->p3].z*coef3)/sum);
} 

//Est-ce que le point p est dans le cercle circonscrit de t?
int Delaunay_PointInCercle(int p, STriangle *t)
{
	double dist = (tab.p[p].x-t->centre.x)*(tab.p[p].x-t->centre.x)+(tab.p[p].y-t->centre.y)*(tab.p[p].y-t->centre.y);
	dist = sqrt(dist);
	return (dist<t->rayon);
}


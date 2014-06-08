#ifndef __TERRAINIMPL_H__
#define __TERRAINIMPL_H__

#include "Define.h"
#include "Graphique.h"

//nombre maximum de fonctions exponentielles
#define MAX_CENTER 200

//un point aura donc comme hauteur la somme des paraboles d'equation 
//z= (-1)^montagne*hauteur*exp(-etal_x*(x-centre_x)^2-etal_y*(y-centre_y)^2)
//on prend l'inverse si on veut une montagne

typedef struct sTerrainImpl		//on definit un terrain par un certain nbr de bosses
{
	int nb_centre;				//nbr de bosses
	double centre[MAX_CENTER][2];//les centres
	double etal[MAX_CENTER][2];		//l'etalement
	double hauteur[MAX_CENTER];	//la hauteur maximale
	int montagne[MAX_CENTER];	//est-ce une montagne
	//juste pour statistiques
	int nb_montagnes;			
}STerrainImpl;


//mont et valley: nbr de montagnes et vallees max voulus
//minx .. maxy: bornes du terrain
STerrainImpl* TerrainImpl_Init(int mont,int valley,double minx,double maxx, 
				 double miny,double maxy);
							

//dessine des points rouges pour donner une approx du terrain implicite
void TerrainImpl_Draw(STerrainImpl* t,double minx,double maxx, double miny,double maxy,double intervx,double intervy);

double TerrainImpl_CalcHeight(STerrainImpl* t,double x, double y);//calcule la hauteur d'un pt (x,y) utilise la version simple

#endif


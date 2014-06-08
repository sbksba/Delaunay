#ifndef H_DESTCONS
#define H_DESTCONS

#include "Define.h"
#include "Delaunay.h"
#include "TerrainImpl.h"

//fonctions d'initialisation
void DestCons_InitTab();		//Initiation de la structure de donnees
void DestCons_ResetTab(); 		//Appel DestCons_InitTab apres avoir fait un free de la memoire
void DestCons_InserePoint();		//point d'entree de l'algo Destructeur-Constructeur

#endif

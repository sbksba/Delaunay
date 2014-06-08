#ifndef H_VECTEUR
#define H_VECTEUR

#include "Define.h"
#include "Triangle.h" 

// vecteur 3D
typedef struct sVecteur
{
	double x,y,z;
}SVecteur;

void Vecteur_ProdVect(SVecteur* V1, SVecteur* V2, SVecteur* V3); //produit vectoriel de V1, V2 resultat mis dans V3
int Vecteur_Zup(SVecteur *V1, SVecteur *V2); //calcule la composante z du prod vect est dit si elle est pos

#endif

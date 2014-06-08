#include "Vecteur.h"

//les variables externes
extern STabDel tab;

// produit vectoriel de vecteurs 3D
void Vecteur_ProdVect(SVecteur *V1, SVecteur *V2, SVecteur *V3)
{
	V3->x=V1->y*V2->z-V1->z*V2->y;
	V3->y=V1->z*V2->x-V1->x*V2->z;
	V3->z=V1->x*V2->y-V1->y*V2->x;
}

int Vecteur_Zup(SVecteur *V1, SVecteur *V2) //calcule la composante z du prod vect est dit si elle est pos
{
return ((V1->x*V2->y-V1->y*V2->x)>0.0);
}

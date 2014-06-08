#ifndef H_TERRAN
#define H_TERRAN

#include <time.h>

#include "Define.h"
#include "Triangle.h" 

void Terran_JeuneMont(STriangle* origine,STriangle* t,double hauteur,int profondeur);
void Terran_VieilMont(STriangle* orig,STriangle* t,double h,int p,int max);
#endif


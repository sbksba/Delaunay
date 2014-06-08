#ifndef __GRAPHIQUE_H__
#define __GRAPHIQUE_H__

#include "Define.h"
#include "Liste.h"
#include "Triangle.h"

//fonction qui dessine les triangles
void Graphique_Dessin2D(); //filaire
void Graphique_Dessin3D(); //filaire
void Graphique_Dessin3DFull(); //full mais en ce moment, sans texture


void Graphique_RenderBitmapString(float x, float y, void *font,char *string);
#endif


#include "Graphique.h"

//les variables externes
extern STabDel tab;
extern SParam param;

//fonctions qui servent pour l'ecriture du texte
static void Graphique_SetOrthographicProjection();
static void Graphique_ResetPerspectiveProjection();

//fonction qui remplit le tableau v des valeurs r,g,b d'un point qui a la hauteur height
void Graphique_GetColor(double *v,double height);
//dessiner en 2d filaire
void Graphique_Dessin2D()
{
	SLTriangle *tmp;
	STriangle *t;

	//on a besoin de cet objet
	GLUquadricObj *g_Object=NULL;
	g_Object = gluNewQuadric();
	gluQuadricTexture(g_Object,GL_TRUE);	
	
	//on enleve le test de profondeur
	glDisable(GL_DEPTH_TEST);

	//on prend le premier
	tmp = tab.triangles->first;

	// contour vert
	glColor3f(0.0,1.0,0.0);						// la couleur courante est le vert
	
	//on parcourt les triangles					
	while(tmp!=NULL)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	// on tracera uniquement les contours des polygones
			t=tmp->t;
				
			glBegin(GL_TRIANGLES);// on va tracer des triangles
				glVertex3d(tab.p[t->p1].x,tab.p[t->p1].y,0.0);
				glVertex3d(tab.p[t->p2].x,tab.p[t->p2].y,0.0);
				glVertex3d(tab.p[t->p3].x,tab.p[t->p3].y,0.0);
			glEnd();

			//si on veut dessiner le cercle
			if(param.circle)
				{
				glTranslatef(t->centre.x,t->centre.y,0.0);
				gluDisk(g_Object,t->rayon-0.1,t->rayon,50,2);
				glTranslatef(-t->centre.x,-t->centre.y,0.0);
				}
			tmp = tmp->next;
		}
	//on remet le test de profondeur
	glEnable(GL_DEPTH_TEST);
}

//dessiner en 3D filaire
void Graphique_Dessin3D()
{
	SLTriangle *tmp;
	STriangle *t;

	//on prend le premier
	tmp = tab.triangles->first;

	// contour vert
	glColor3f(0.0,1.0,0.0);					// la couleur courante est le vert
	
	//on parcourt les triangles				
	while(tmp!=NULL)
		{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	// on tracera uniquement les contours des polygones
		t=tmp->t;

		//on ne dessine pas si on est sur un des points du bord
		if((t->p1>3)&&(t->p2>3)&&(t->p3>3))
			{
			glBegin(GL_TRIANGLES);// on va tracer des triangles
				glVertex3d(tab.p[t->p1].x,tab.p[t->p1].y,tab.p[t->p1].z);
				glVertex3d(tab.p[t->p2].x,tab.p[t->p2].y,tab.p[t->p2].z);
				glVertex3d(tab.p[t->p3].x,tab.p[t->p3].y,tab.p[t->p3].z);
			glEnd();
			}
		tmp = tmp->next;
		}
		
	//on recommence
	tmp = tab.triangles->first;

	// fond noir
	glColor3f(0.0,0.0,0.0);					// la couleur courante est le noir
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		// on tracera des polygones "pleins"
	glEnable(GL_POLYGON_OFFSET_FILL);			// on décalera les polygones vers l'arrière
	glPolygonOffset(1.0,1.0);				// coefficients du décalage
	glBegin(GL_TRIANGLES);					// on va tracer des triangles
		while(tmp!=NULL)
			{
			t=tmp->t;
			if((t->p1>3)&&(t->p2>3)&&(t->p3>3))
				{
				glVertex3d(tab.p[t->p1].x,tab.p[t->p1].y,tab.p[t->p1].z);
				glVertex3d(tab.p[t->p2].x,tab.p[t->p2].y,tab.p[t->p2].z);
				glVertex3d(tab.p[t->p3].x,tab.p[t->p3].y,tab.p[t->p3].z);
				}
			tmp = tmp->next;
			}
	glEnd();						// on a fini avec les triangles
	glDisable(GL_POLYGON_OFFSET_FILL);			// on ne décale plus
}

//dessin en 3D plein
void Graphique_Dessin3DFull()
{
	SLTriangle *tmp;
	STriangle *t;
	SVecteur V1,V2,V;
	GLdouble col[3];
	//on prend le premier
	tmp = tab.triangles->first;

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);			// on tracera des polygones "pleins"

	glEnable(GL_LIGHTING);					// on active l'éclairage
	glEnable(GL_LIGHT0);					// on allume la lumière n° 0
	glEnable(GL_NORMALIZE);					// on active la normalisation automatique des vecteurs normaux

	glBegin(GL_TRIANGLES);
	while(tmp!=NULL)
		{
		t=tmp->t;
		if((t->p1>3)&&(t->p2>3)&&(t->p3>3))
			{
			V1.x = tab.p[tmp->t->p2].x-tab.p[tmp->t->p1].x;			// vecteur P1P2
			V1.y = tab.p[tmp->t->p2].y-tab.p[tmp->t->p1].y;
			V1.z = tab.p[tmp->t->p2].z-tab.p[tmp->t->p1].z;

			V2.x = tab.p[tmp->t->p3].x-tab.p[tmp->t->p1].x;			// vecteur P1P2
			V2.y = tab.p[tmp->t->p3].y-tab.p[tmp->t->p1].y;
			V2.z = tab.p[tmp->t->p3].z-tab.p[tmp->t->p1].z;
			Vecteur_ProdVect(&V1,&V2,&V);						// vecteur normal au triangle
				
			glNormal3d(V.x, V.y, V.z) ;					// V est le vecteur normal courant

			Graphique_GetColor(col,tab.p[t->p1].z);
			glColor3dv(col);
			glVertex3d(tab.p[t->p1].x,tab.p[t->p1].y,tab.p[t->p1].z);

			Graphique_GetColor(col,tab.p[t->p2].z);
			glColor3dv(col);
			glVertex3d(tab.p[t->p2].x,tab.p[t->p2].y,tab.p[t->p2].z);

			Graphique_GetColor(col,tab.p[t->p3].z);
			glColor3dv(col);
			glVertex3d(tab.p[t->p3].x,tab.p[t->p3].y,tab.p[t->p3].z);
			}
		tmp = tmp->next;
		}
	glEnd();

	glDisable(GL_LIGHT0);					// on éteint la lumière n° 0
	glDisable(GL_LIGHTING);					// on désactive l'éclairage
	glDisable(GL_NORMALIZE);				// on désactive la normalisation automatique des vecteurs normaux
}

//remplit v avec les couleur dependant de la hauteur height
void Graphique_GetColor(GLdouble *v,double height)
{
	if(height>7.0)
		{
		v[0] = 0.7+((height-7.0)/10.0);
		v[1] = 0.7+((height-7.0)/10.0);
		v[2] = 0.7+((height-7.0)/10.0);
		}
	else
		{
		v[0]=0.0;
		v[2]=0.0;
		v[1]=0.3+height/10.0;
		}

	//ajoute le contraste
	v[0]+=param.light;
	v[1]+=param.light;
	v[2]+=param.light;
}

//fonctions ci-dessous servent pour afficher le texte
void Graphique_SetOrthographicProjection() 
{
	// switch to projection mode
	glMatrixMode(GL_PROJECTION);
	// save previous matrix which contains the 
	//settings for the perspective projection
	glPushMatrix();
	// reset matrix
	glLoadIdentity();
	// set a 2D orthographic projection
	gluOrtho2D(0,WIDTH, 0, HEIGHT);
	// invert the y axis, down is positive
	glScalef(1, -1, 1);
	// mover the origin from the bottom left corner
	// to the upper left corner
	glTranslatef(0, -HEIGHT, 0);
	glMatrixMode(GL_MODELVIEW);
}

void Graphique_ResetPerspectiveProjection() 
{
	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	
}

void Graphique_RenderBitmapString(float x, float y, void *font,char *string)
{
      
  char *c;
  if(param.texte)
	{
	glPushMatrix();
	glColor3f(0.0f,1.0f,1.0f);
	Graphique_SetOrthographicProjection();
	glLoadIdentity();
	glRasterPos2f(x, y);
	for (c=string; *c != '\0'; c++) 
		{
	    	glutBitmapCharacter(font, *c);
		}
	Graphique_ResetPerspectiveProjection();
	glPopMatrix();
	glPopMatrix();
	}
}

#include "Triangle.h"

//les variables externes
extern STabDel tab;

//Triangle_Cree aura besoin de:
static int Triangle_CalcParam(STriangle *t); 		//calcule le centre,le rayon,le critere de ce triangle
static int Triangle_CalcCercle(STriangle *t); 		//calcule juste le centre et le rayon
static int Triangle_CalcCritere(STriangle *t);		//calcule juste le critere

//Creation d'un triangle
STriangle* Triangle_Cree(int x, int y, int z) //alloue et cree le triangle
{
	STriangle *t = malloc(sizeof(*t));
	SVecteur v1, v2;
	if(t==NULL)
		//la on fait un printf et on rend null
		{
		printf("Malloc failed in cree_t\n");
		return NULL;
		}
	
	// indice des points du triangle dans la liste de points tab.p
	t->p1 = x;
	t->p2 = y;
	t->p3 = z;

	//on verifie qu'on est dans le sens trigo
	v1.x = 	tab.p[x].x - tab.p[y].x;
	v1.y = 	tab.p[x].y - tab.p[y].y;
	v1.z = 	tab.p[x].z - tab.p[y].z;
	v2.x = 	tab.p[x].x - tab.p[z].x;
	v2.y = 	tab.p[x].y - tab.p[z].y;
	v2.z = 	tab.p[x].z - tab.p[z].z;
	if(!Vecteur_Zup(&v1,&v2))
		{t->p1 = y; t->p2 = x;}

	Triangle_CalcParam(t);

	t->flip_param = 0;

	t->t1 = NULL;
	t->t2 = NULL;
	t->t3 = NULL;
	return t;
}

//Triangle_Cree aura besoin de:
int Triangle_CalcParam(STriangle *t) //calcule le centre,le rayon,le critere de ce triangle
{
	return Triangle_CalcCercle(t)&&Triangle_CalcCritere(t); 
}

int Triangle_CalcCercle(STriangle *t) //calcule juste le centre et le rayon
{
	int p1,p2,p3,p4;
	double a1,a2,b1,b2;
	//on doit passer par les mediatrices...
	//on choisit 2 points et vu qu'on ne veut pas avoir de mediat verticales,
	//on prend le premier point (on aurait pu prendre le 2eme)
	p1 = t->p1;
	p2 = t->p2;

	//on regarde si on a pas des points sur la meme verticale ou la meme horizontale
	if((fabs(tab.p[p1].x - tab.p[p2].x)<EPS)||(fabs(tab.p[p1].y - tab.p[p2].y)<EPS))
		{
		p3 = t->p1;
		p4 = t->p3;
		}
	else
		{
		p3 = p1;
		p4 = p2;

		p1 = t->p1;
		p2 = t->p3;
		}

	if(fabs(tab.p[p1].x - tab.p[p2].x)<EPS)
	{
		//on a l'ordonnee
		t->centre.y = (tab.p[p1].y + tab.p[p2].y)/2.0;

		if(fabs(tab.p[p3].y - tab.p[p4].y)<EPS)	
			t->centre.x = (tab.p[p3].x + tab.p[p4].x)/2.0;
		else
			{
			a2 = (tab.p[p3].y-tab.p[p4].y)/(tab.p[p3].x-tab.p[p4].x);
			a2 = -1.0/a2;
			b2 = (tab.p[p3].y+tab.p[p4].y)/2 - a2*(tab.p[p3].x+tab.p[p4].x)/2;
			t->centre.x = (t->centre.y - b2)/a2;
			}
	}
	else
		if(fabs(tab.p[p1].y - tab.p[p2].y)<EPS)
		{
		//on a l'ordonnee
		t->centre.x = (tab.p[p1].x + tab.p[p2].x)/2.0;

		if(fabs(tab.p[p3].x - tab.p[p4].x)<EPS)	
			t->centre.y = (tab.p[p3].y + tab.p[p4].y)/2.0;
		else
			{
			a2 = (tab.p[p3].y-tab.p[p4].y)/(tab.p[p3].x-tab.p[p4].x);
			a2 = -1.0/a2;
			b2 = (tab.p[p3].y+tab.p[p4].y)/2 - a2*(tab.p[p3].x+tab.p[p4].x)/2;
			t->centre.y = a2*(t->centre.x) + b2;
			}
		}
	else
	{
	a1 = (tab.p[p1].y-tab.p[p2].y)/(tab.p[p1].x-tab.p[p2].x);
	a2 = (tab.p[p3].y-tab.p[p4].y)/(tab.p[p3].x-tab.p[p4].x);
	
	a1 = -1/a1;
	a2 = -1/a2;

	b1 = (tab.p[p1].y+tab.p[p2].y)/2 - a1*(tab.p[p1].x+tab.p[p2].x)/2;
	b2 = (tab.p[p3].y+tab.p[p4].y)/2 - a2*(tab.p[p3].x+tab.p[p4].x)/2;

	//on cherche un point d'intersection
	//les droites ne sont pas paralleles donc c'est bon
	if(fabs(a1-a2)<EPS)
		{
		//if(DEBUG==1)
			printf("Erreur dans calc_cercle, a1==a2 : %f\n",a1);
		return 0;
		}

	t->centre.x = (b1-b2)/(a2-a1);
	t->centre.y = a1*t->centre.x+b1;
	}	

	//rayon suffit de prendre la dist entre un des points et le centre du cercle
	t->rayon = (t->centre.x-tab.p[p1].x)*(t->centre.x-tab.p[p1].x)+(t->centre.y-tab.p[p1].y)*(t->centre.y-tab.p[p1].y);
	t->rayon = sqrt(t->rayon);
	return 1;
}

int Triangle_CalcCritere(STriangle *t)//calcule juste le critere
{
	double h,l1,l2,l3;

	//on calcule AB
	l1 = sqrt((tab.p[t->p1].x - tab.p[t->p2].x)*(tab.p[t->p1].x - tab.p[t->p2].x)+(tab.p[t->p1].y - tab.p[t->p2].y)*(tab.p[t->p1].y - tab.p[t->p2].y));
	//on calcule AC
	l2 = sqrt((tab.p[t->p1].x - tab.p[t->p3].x)*(tab.p[t->p1].x - tab.p[t->p3].x)+(tab.p[t->p1].y - tab.p[t->p3].y)*(tab.p[t->p1].y - tab.p[t->p3].y));
	//on calcule BC
	l3 = sqrt((tab.p[t->p2].x - tab.p[t->p3].x)*(tab.p[t->p2].x - tab.p[t->p3].x)+(tab.p[t->p2].y - tab.p[t->p3].y)*(tab.p[t->p2].y - tab.p[t->p3].y));

	//on divise AB par 2*r pour obtenir le sinus de C
	h = l1/(2.0*t->rayon);
	//on multiplie maintenant par AC pour obtenir AH
	h *= l2;
	//l'aire est maintenant donnee par AH*CB/2
	t->critere = h*l3/2.0;

	return 1;
}


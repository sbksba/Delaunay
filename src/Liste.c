#include "Liste.h"

//les variables externes
extern STabDel tab;


/**** Fonction : Liste_AddTriangle
	* Entree :	-une liste de triangles tri�e dans l'ordre d�croissant selon le crit�re
			-un triangle � ajouter dans la liste
	*/

void Liste_AddTriangle(STriangle* t1, SStartT* t)
{
	SLTriangle *last = t->first;

	//si le triangle est NULL
	if(t1==NULL)
		return;

	// Si le triangle est d�j� existant, c'est fini
	if(!Liste_TriangleIsIn(t1,t))
	{
		if(DEBUG==1) 
			printf("Triangle t1 is not in list\n");

		// Structure allou�e servant de container
		SLTriangle *l= malloc(sizeof *l);
		
		if(l==NULL);
			//**** fonction d'erreur ??
		else
		// Si c'est le premier triangle
		if (t->first==NULL)
		{
			if(DEBUG==1)
				printf("Premier triangle\n");
			// on introduit la structure comme premier �l�ment
			t->first=l;
			// il n' a pas de structure suivante
			l->next=NULL;
			// on place le triangle dans la structure 
			l->t=t1;
		}
		// Si il existe d�j� des triangles dans la liste
		else 
		{
			if(DEBUG==1) 
				printf("Il existe deja un triangle\n");
			// Variable temporaire pour parcourir la liste
			// initialis�e au premier �l�ment
			SLTriangle *tmp=t->first;
	
			// Rangement dans un ordre d�croissant selon le crit�re
			// Notamment si c'est le premier
			if ( (tmp->t->critere) < (t1->critere) )
			{
				if(DEBUG==1) 
					printf("Nouveau premier\n");
				// Nouvel ordre 
				l->next=tmp;
				// Placement dans la structure
				l->t=t1;
				// Nouveau d�but de liste 
				t->first=l;
			}
			// Sinon on parcourt la liste pour trouver la bonne place
			else
			{
				while( (tmp!=NULL) && ((tmp->t->critere) >= (t1->critere)) )
				{
					if(DEBUG==1) 
						printf("On passe au prochain\n");
					last = tmp;
					tmp=tmp->next;
				}

			if(DEBUG==1) 
				printf("On l'ajoute a sa position\n");
			// M�me actions une fois au bon endroit
			l->next=tmp;
			l->t=t1;
			last->next=l;
			}
		}
		
	}
	else
		if(DEBUG==1) 
			printf("Triangle t1 is in list\n");

}


/**** Fonction : Liste_EffTriangle
	* Entree :	- Liste de triangle t2
			- Triangle t1 � �liminer dans la liste
	* Pr�condition : aucun autre triangle ne poss�de t1 comme voisin 
	* Sortie: est-ce que l'element a ete trouve
	*/
/*Remarque supplementaire, il y a un cas particulier dans cette fonction
par rapport a une fonction d'effacement de base: a cause du flip algorithme,
on peut avoir un lien vers un triangle qui existe aussi dans la liste globale si t2 n'est pas
la liste globale.
Ceci a comme consequence qu'on ne peut pas juste faire un free de cet element, on va d'abord l'effacer
dans la liste globale...
*/
int Liste_EffTriangle(STriangle *t1, SStartT *t2) 
{
	// Si le triangle est existe
	if (t1!=NULL)
	// Si la liste n'est pas vide
	if(t2->first!=NULL)
	{
		// Variable de fin de boucle
		int res=0;
		// Variable temporaire contenant la liste
		SLTriangle *tmp=t2->first, *tmp2=tmp;

		// Cas sp�cial si c'est le premier qui doit �tre supprim�
		if (tmp->t==t1)
		{
			// On indique son suivant comme premier �l�ment
			t2->first=tmp->next;
			//et on l'efface si flip_param est vrai et que t2 est different de la liste globale
			//on appelle eff_triangle pour qu'il efface le doublon qui existe
			if( (t1->flip_param)&&(t2!=tab.triangles))
					Global_EffTriangleListe(t1); //on l'efface a partir de la liste mere
				else
					free(tmp->t);
			free(tmp);
		}
		else
		// Tant que l'on n'a pas parcouru toute la liste 
		// ou trouv� le bon �l�ment
		while(!res) 
		{
			if (tmp==NULL)
				return 0;
			else
			// Si le bon �l�ment est trouv� 
			if(tmp->t==t1)
			{
				// on indique un lien entre son pr�c�dent et son suivant
				tmp2->next=tmp->next;
				// et on l'efface
				//ici on regarde si cet elemen a flip_param a vrai
				//si c'est le cas c'est qu'il est aussi dans la liste mere
				if( (t1->flip_param)&&(t2!=tab.triangles))
					Global_EffTriangleListe(t1); //on l'efface a partir de la liste mere
				else
					free(tmp->t);
				free(tmp);
				res=1;
			}
			else
			{
				tmp2=tmp;
				tmp=tmp->next;
			}
		}
	}
	return 1;

}


/**** Fonction : Liste_TriangleIsIn
	* Entree :	- liste de triangle
			- triangle a trouver dans la liste
	*/
int Liste_TriangleIsIn(STriangle *t1, SStartT *t2)
{
	// Valeur de retour
	int res=0;

	// Si la liste t2 n'est pas vide et si le triangle t1 a rechercher existe
	if((t2->first!=NULL)&&(t1!=NULL))
	{
		// Variable temporaire contenant la liste
		SLTriangle *tmp=t2->first;

		// Tant que l'on n'a pas parcouru toute la liste 
		// ou trouv� le bon �l�ment
		while( (!res)  &&  (tmp!=NULL) )
		{
			// Si le bon �l�ment est trouv� "res"=1 
			res=( (res) || (tmp->t==t1)) ;
			tmp=tmp->next;
		}
	}

	return res;

}

//fonctions pour gerer la structure TabDel GLOBALE
int Global_AddPoint(double x,double y,double z) //ajoute un point dans la structure, retourne l'indice de ce point
{
	tab.p[tab.np].x=x; 
	tab.p[tab.np].y=y;
	tab.p[tab.np].z=z;
	// rend ici l'indice de l'�l�ment qui vient d'�tre plac�
	return tab.np++;
}


void Global_AddTriangleListe(STriangle *t)
{
	if(DEBUG==1)
	{
		printf("Adding triangle in tab triangle list\n");
		printf("\t Points are: %d %d %d\n",t->p1,t->p2,t->p3);
		printf("\t(%f,%f) (%f,%f) (%f,%f)\n",tab.p[t->p1].x,tab.p[t->p1].y,tab.p[t->p2].x,tab.p[t->p2].y,tab.p[t->p3].x,tab.p[t->p3].y);
		printf("\tCenter is (%f %f) with rayon: %f\n",t->centre.x,t->centre.y,t->rayon);
		printf("Critere is %f\n",t->critere);
	}
	tab.nt++;

	t->flip_param = 1;
	Liste_AddTriangle(t,tab.triangles);
}


//on suppose qu'il n'y a plus de prob d'adjacence...
int Global_EffTriangleListe(STriangle *t)
{
	int res;
	tab.nt--;
	res = Liste_EffTriangle(t,tab.triangles);
	//si c'etait le dernier triangle
	if(tab.nt==0)
		tab.triangles->first = NULL;
	return res;
}


//destruction d'une liste
void Global_DestructionListe(SStartT *t)
{
	SLTriangle *tmp=t->first,*tmp2=t->first;

	while(tmp!=NULL)
	{
		if(tmp->t!=NULL)
			free(tmp->t);
		tmp=tmp->next;
		free(tmp2);
		tmp2=tmp;
	}
}


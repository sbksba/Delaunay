#include "DestCons.h"

//fonctions pour l'algo Destructeur-Constructeur
static void DestCons_DeterminerTAD(STriangle* t, SStartT* lt,int p); 	//determine les triangles a detruire
static void DestCons_DeterminerTAC(SStartT* Tad, SStartT* Tac, int p); 	//determine les triangles a construire
static void DestCons_Adjacence(SStartT* Tac); 	//lors de la creation de la liste Tac, il est difficile de gerer
					//l'adjacence des triangles en meme temps, ceci a donc ete separe

//les variables externes
extern STerrainImpl* terrainimpl;
extern STabDel tab;
extern SParam param;

//fonctions d'initialisation
//on met un carre de 4 points et on cree 2 triangles
void DestCons_InitTab()
{
	STriangle *tmp,*tmp2;

	//on alloue le start
	tab.triangles = malloc(sizeof(SStartT));

	if(tab.triangles==NULL)
		{
		printf("Erreur d'allocation de malloc dans DestCons_InitTab\n");
		exit(EXIT_FAILURE);
		}

	//on initialise le terrain pour avoir 2 triangles rectangles,
	//ce qui represente le terrain au depart
	tab.np=4;	//nbr de points
	tab.p[0].x = -10.0f;
	tab.p[0].y = 10.0f;
	tab.p[0].z = 0.0f;

	tab.p[1].x = 10.0f;
	tab.p[1].y = 10.0f;
	tab.p[1].z = 0.0f;

	tab.p[2].x = 10.0f;
	tab.p[2].y = -10.0f;
	tab.p[2].z = 0.0f;

	tab.p[3].x = -10.0f;
	tab.p[3].y = -10.0f;
	tab.p[3].z = 0.0f;

	//ajout des triangles dans les listes
	tab.triangles->first=NULL;
	//on commence par les construire
	tmp = Triangle_Cree(2,1,0);
	tmp2 = Triangle_Cree(2,0,3);

	//on met en place l'adjacence
	tmp->t1 = NULL;
	tmp->t2 = NULL;
	tmp->t3 = tmp2;

	tmp2->t1 = tmp;
	tmp2->t2 = NULL;
	tmp2->t3 = NULL;

	//ajout de tmp et tmp2 dans la liste globale
	tab.nt=0;	//nbr de triangles
	Global_AddTriangleListe(tmp2);
	Global_AddTriangleListe(tmp);
}

//on fait DestCons_InitTab mais on fait d'abord un free
void DestCons_ResetTab()
{
	//desalloction de la memoire
	Global_DestructionListe(tab.triangles);
	DestCons_InitTab();

	//param.stop_insert, il peut etre a vrai, donc on le remet a faux
	param.stop_insert = 0;

	//est-ce qu'on affiche le terrain implicite?
	//cela depend si on veut generer un terrain utilisant la methode implicite
	if(param.gentype&TERRAINIMPL)
		param.show_impl = 1;
	else
		param.show_impl = 0;
}

//fonctions pour l'algo
void DestCons_InserePoint()
{
	int p;
	// Listes des triangles à détruire et à creer 
	SStartT Tad,Tac;
	// Trianlge temporaire pour les parcours des listes de triangles Tad et Tac
	SLTriangle* tmp,*tmp2;
	//on initialise Tad et Tac
	Tac.first = NULL;
	Tad.first = NULL;
	
	//on regarde si on peut encore insere qq chose
	if(param.stop_insert)
		return;

	//on regarde si on peut encore ajouter des points, ou des triangles
	if((tab.np>=MAX_POINTS)||(tab.nt>=MAX_TRIANGLES))
	{
		param.stop_insert=1;
		return;
	}
	
	// Point choisi
	p = Delaunay_ChoisitPoint();

	//on commence dans le triangle qui est en tete de la liste de tab (variable globale)
	//cette liste n'est jamais vide
	//on determine les triangles a enlever
	DestCons_DeterminerTAD(tab.triangles->first->t,&Tad,p);
	//on determine les triangles a rajouter
	DestCons_DeterminerTAC(&Tad,&Tac,p);
	
	//on regarde si Tac->first est NULL, si c'est le cas alors on arrete l'insertion
	if(Tac.first==NULL)
	{
		param.stop_insert=1;
		return;
	}

	//suppression des triangles, suffit de parcourir la liste Tad.first
	tmp = Tad.first;
	while(tmp!=NULL)
		{
		#if DEBUG==1
			printf("Taking off a triangle\n");
			printf("\t Points are: %d %d %d\n",tmp->t->p1,tmp->t->p2,tmp->t->p3);
			printf("\t(%f,%f) (%f,%f) (%f,%f)\n",tab.p[tmp->t->p1].x,tab.p[tmp->t->p1].y,
				tab.p[tmp->t->p2].x,tab.p[tmp->t->p2].y,tab.p[tmp->t->p3].x,tab.p[tmp->t->p3].y);
			printf("Critere is %f\n",tmp->t->critere);
		#endif
		
		//on efface le triangle en question
		Global_EffTriangleListe(tmp->t);
		tmp2 = tmp;
		tmp = tmp -> next;	//on passe au prochain
		free(tmp2);
		}

	//ajout des triangles, on parcourt Tac.first
	tmp = Tac.first;

	while(tmp!=NULL)
		{
		
		#if DEBUG==1
			printf("Creating a triangle\n");
			printf("\t Points are: %d %d %d\n",tmp->t->p1,tmp->t->p2,tmp->t->p3);
			printf("\t(%f,%f) (%f,%f) (%f,%f)\n",tab.p[tmp->t->p1].x,tab.p[tmp->t->p1].y,
				tab.p[tmp->t->p2].x,tab.p[tmp->t->p2].y,tab.p[tmp->t->p3].x,tab.p[tmp->t->p3].y);
			printf("Critere is %f\n",tmp->t->critere);
		#endif
		
		//on ajoute le triangle dans la liste globale
		Global_AddTriangleListe(tmp->t);
		tmp2 = tmp;
		tmp = tmp -> next; //on passe au prochain
		free(tmp2);
		}
}

//determiner la liste Tad
void DestCons_DeterminerTAD(STriangle *t, SStartT *lt,int p)
{
	int i;
	STriangle* tmp;

	//on rajoute le triangle dans la liste lt
	Liste_AddTriangle(t,lt);
	//pour tout les voisins de t
	for(i=0;i<3;i++)
		{
		//on choisit le triangle voisin
		switch(i)
			{
			case 0:
				tmp = t->t1;
				break;
			case 1:
				tmp = t->t2;
				break;
			case 2:
				tmp = t->t3;
				break;
			default: break;
			}
		//si ce n'est pas un triangle vide
		if(tmp!=NULL)
			{
			//s'il n'est pas deja dans la liste lt
			if(!Liste_TriangleIsIn(tmp, lt))
				{
				//si p est dans le cercle circonscrit a tmp
				if(Delaunay_PointInCercle(p,tmp))
					{
					//On le supprime et on observe ses voisins
					DestCons_DeterminerTAD(tmp,lt,p);
					}
				}
			}
		}
}

//gere l'adjacence des triangles dans la liste Tac
void DestCons_Adjacence(SStartT* Tac)
{
	SLTriangle* lt,*lttmp;
	STriangle* t,*ttmp;
	// Deux points qu'on recherche qui constitue une arète du triangle
	// Le troisième est toujours le même
	int p,q;

	if(Tac!=NULL)
	{
		lt=Tac->first;	
		while(lt!=NULL)
		{
			// lt : pointeur vers le premier élément
			// lttmp : pointeur vers le deuxieme, parcourt la liste
			lttmp=lt->next;
			// t : premier triangle dont on cherche l'adjacence
			t=lt->t;
			p=t->p1;
			q=t->p2;

			// Tant qu'on a pas parcouru toute la liste 
			// (ou trouver les trois triangles) 
			while(lttmp!=NULL)
			{
				// ttmp : deuxieme triangle, premier qu'on examine
				//		  pointera successivement les différents triangles
				ttmp=lttmp->t;

				// Si le point1 de notre triangle recherché
				// est le point 2 du triangle courant (voir rapport)
				if(p==ttmp->p2)
				{
					// alors il est situé comme voisin t3 de notre triangle rech.
					t->t3=ttmp;
					ttmp->t2=t;
				}
				else

				// Sinon si le point2 de notre triangle recherché
				// est le point1 du triangle courant (voir rapport)
				if(q==ttmp->p1)
				{
					// alors il est situé comme voisin t2  
					t->t2=ttmp;
					ttmp->t3=t;
				}
				// On observe le triangle suivant pour voir s'il est ou 
				// non adjacant au triangle recherché			
				lttmp=lttmp->next;
			}
		// On passe à la mise à jour du triangle suivant
		lt=lt->next;
		}
	}
}


//determiner la liste Tac
void DestCons_DeterminerTAC(SStartT *Tad, SStartT *Tac, int p)
{
	SLTriangle *t;
	STriangle *tmp,*tmpv,*tmpc;
	int p1,p2;
	int i;

	if (Tad!=NULL)
	{
		t=Tad->first;
		//pour tous les triangles de Tad
		while(t!=NULL)
		{
			tmp=t->t;
			//pour tous les voisins de tmp
			for(i=0;i<3;i++)
			{
				//on choisit le triangle voisin
				switch(i)
				{
				case 0:
					tmpv = tmp->t1;
					p1=tmp->p1;
					p2=tmp->p2;
					break;
				case 1:
					tmpv = tmp->t2;
					p1=tmp->p2;
					p2=tmp->p3;
					break;
				case 2:
					tmpv = tmp->t3;
					p1=tmp->p3;
					p2=tmp->p1;
					break;
				default: break;
				}

				//Si le voisin est vide
				if (tmpv==NULL)
				{
					//Creer un nouveau triangle de sommet p et d'arete le cote interesse
					tmpc=Triangle_Cree(p1,p2,p);
					if(tmpc==NULL)
					{
						Global_DestructionListe(Tac);
						Tac->first=NULL;
						return;
					}
					//Rajouter ce triangle a la liste Tac
					Liste_AddTriangle(tmpc,Tac);
				}
				else
				//Si le voisin n'est pas dans Tad
				if(!Liste_TriangleIsIn(tmpv,Tad))
				{
					//Creer un triangle de sommet p et d'arete celle commune entre tmp et le voisin
					tmpc=Triangle_Cree(p1,p2,p);
					if(tmpc==NULL)
						{
						Global_DestructionListe(Tac);
						Tac->first=NULL;
						return;
						}
					//Etablir l'adjacence entre ce triangle construit tmpc et l'ancien voisin tmpv

					//Adjacence du triangle nouvellement construit
					tmpc->t1 = tmpv;

					// Adjacence du triangle voisin  
					if(tmpv->t1==tmp)
						tmpv->t1=tmpc;
					else
					if(tmpv->t2==tmp)
						tmpv->t2=tmpc;
					else
					if(tmpv->t3==tmp)
						tmpv->t3=tmpc;
					else
						{
							printf("Bizarre pas d'adjacence!!!!\n");
							exit(EXIT_FAILURE);
						}
						// code d'erreur
					//Rajouter ce triangle a la liste Tac
					Liste_AddTriangle(tmpc,Tac);

				}
			}

		t = t -> next;
		}

		//Etablir les adjacences entre les triangles de Tac
		DestCons_Adjacence(Tac);

	}
	else
		{
		printf("Bizarre liste de destruction nulle\n");
		exit(EXIT_FAILURE);
		}
}	

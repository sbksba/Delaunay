#include "Flipalgo.h"

//les variables externes
extern SParam param;
extern STabDel tab;

//calcule le cote illegal et met les triangle t,t1 dans res meme si pas change
static void FlipAlgo_CoteIllegal(int p3,int p4,SStartT* list);

//l'insertion d'un point utilisant le flip algorithme
void FlipAlgo_InserePoint()
{
	int i;
	int p;
	// Listes des triangles à détruire et à creer 
	SStartT list;
	SLTriangle *l,*l2;
	STriangle *t1,*t2,*t3,*t,*tmp,*tmp2;
	
	//on met la liste a NULL
	list.first=NULL;

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

	//creations des 3 nouveaux triangles et destruction du premier
	t = tab.triangles->first->t;
	t1 = Triangle_Cree(p,t->p1,t->p2); 

	//l'ordre est important pour conserver le zup... et donc pas avoir de prob avec l'adjacence
	//en effet, Triangle_Cree verifie que les triangles pointent vers le haut, si ce n'est pas le cas
	//il inverse deux des points, ce qui ne va pas nous arranger
	t2 = Triangle_Cree(p,t->p2,t->p3); 
	t3 = Triangle_Cree(p,t->p3,t->p1); 
	
	//on met en place l'adjacence
	t1->t1 = t3;
	t1->t2 = t->t1;
	t1->t3 = t2;
	t2->t1 = t1;
	t2->t2 = t->t2;
	t2->t3 = t3;
	t3->t1 = t2;
	t3->t2 = t->t3;
	t3->t3 = t1;

	//on doit trouver l'adjacence des triangles autour
	//en effet, ils pointent encore sur t, or t sera detruit et a la place
	//se trouve t1, t2 et t3...
	//par contre, on sait que les triangles t->ti doivent pointer sur ti avec i=1,2 et 3

	for(i=0;i<3;i++)
	{
		//on fait les uns apres les autres
		switch(i)
		{
		//gere l'adjacence entre t1 et t->t1
		case 0: tmp = t->t1;
			tmp2 = t1;
			break;
		//gere l'adjacence entre t2 et t->t2
		case 1: tmp = t->t2;
			tmp2 = t2;
			break;
		//gere l'adjacence entre t3 et t->t3
		case 2: tmp = t->t3;
			tmp2 = t3;
			break;
		}
		//si c'est NULL, il n'y a rien a faire
		if(tmp!=NULL)
		{
			//On trouve le pointeur a modifier
			if(tmp->t1==t)
				tmp->t1=tmp2; 
			if(tmp->t2==t)
				tmp->t2=tmp2;
			if(tmp->t3==t)
				tmp->t3=tmp2;
		}

	}

	//on enleve le triangle hote
	Global_EffTriangleListe(t);

	//on ajoute les triangles et leurs voisins dans la liste courante,
	//il est inutile d'ajouter les ti->t1 et les ti->t3
	//car c'est les 3 triangles t1,t2 et t3
	Liste_AddTriangle(t1,&list);
	Liste_AddTriangle(t2,&list);
	Liste_AddTriangle(t3,&list);
	
	Liste_AddTriangle(t1->t2,&list);
	Liste_AddTriangle(t2->t2,&list);
	Liste_AddTriangle(t3->t2,&list);

	//gerer les cotes illegaux
	int p1 = t1->p2, p2 = t1->p3, p3 = t2->p2, p4 = t2->p3,
		p5 = t3->p2, p6 = t3->p3;

	//trois appels a FlipAlgo_CoteIllegal
	FlipAlgo_CoteIllegal(p1,p2,&list);
	FlipAlgo_CoteIllegal(p3,p4,&list);
	FlipAlgo_CoteIllegal(p5,p6,&list);
	
	//on rajoute les triangles dans la liste de base
	l = list.first;

	//suffit de parcourir list.first
	while(l!=NULL)
	{
		Global_AddTriangleListe(l->t);
		l2 = l->next;
		free(l);
		l = l2;
	}
}	

//fonction qui teste la validite du cote p3-p4
//elle cherche les triangles qui ont ces sommets dans la liste list
void FlipAlgo_CoteIllegal(int p3,int p4,SStartT *list)
{
	SLTriangle *lt;

	STriangle *t,*t1;
	STriangle *newt,*newt1,*tmp;
	int p1,p2,p5,p6;		//p1 et p2 sont les points opposes, p3 et p4 les points en commun
	int i; 

	int t_c;
	STriangle* tmps[4]; //pour stocker les triangles tmp

	for(i=0;i<4;i++)
		tmps[i] = NULL;  //on met a NULL

	//on cherche les triangles interesses
	i=0;

	t1 = NULL;
	t = NULL;
	p1 = 0;
	p2 = 0;

	lt = list->first;
	while((i!=2)&&(lt!=NULL))
	{
		//si ce triangle a comme sommet p3 et p4
		if( ((lt->t->p1==p3)||(lt->t->p2==p3)||(lt->t->p3==p3)) &&
			((lt->t->p1==p4)||(lt->t->p2==p4)||(lt->t->p3==p4)) )
				{
				if(i++)
					t1 = lt->t;
				else
					t = lt->t;
				}
		lt=lt->next;
	}
	
	//si i!=2 on sort, car l'autre cote est null
	if(i!=2)
		return;

	//on cherche p1 et p2, il suffit d'utiliser l'adjacence entre t1 et t
	//Par exemple, si t1->t1 pointe sur t, alors on sait que p3 et p4 qui sont
	//les points en commun des deux triangles sont en t1->p1 et t1->p2 donc
	//le point p2 different de p3 et p4 est forcement t1->p3
	if(t1->t1==t)
		{p2 = t1->p3;}
	if(t1->t2==t)
		{p2 = t1->p1;}
	if(t1->t3==t)
		{p2 = t1->p2;}
	//on fait pareil pour obtenir p1
	if(t->t1==t1)
		{p1 = t->p3;}
	if(t->t2==t1)
		{p1 = t->p1;}
	if(t->t3==t1)
		{p1 = t->p2;}

	//on regarde si c'est un cote illegal
	if(Delaunay_PointInCercle(p2,t)||Delaunay_PointInCercle(p1,t1))
	{
		//on creer deux triangles
		newt = Triangle_Cree(p1,p3,p2);
		newt1 = Triangle_Cree(p2,p4,p1);

		//on inverse les cotes
		//de nos triangles pour faire comme newt et newt1,
		//c'est juste pour simplifier le code qui suit
		t->p1 = p1;
		t->p2 = p3;
		t->p3 = p2;

		t1->p1 = p2;
		t1->p2 = p4;
		t1->p3 = p1;

		//adjacence entre les triangles externes et ces 2 la
		for(i=0;i<6;i++)
			{
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
			case 3:
				tmp = t1->t1;
				break;
			case 4:
				tmp = t1->t2;
				break;
			case 5:
				tmp = t1->t3;
				break;
				}

			if(tmp==NULL) continue; //si tmp est NULL
			if((tmp==t)||(tmp==t1)) continue; //si c'est un de nos triangles
			
			//sinon c'est un triangle externe
			//on dissocie lequel faut changer, ce test suffit car on
			//sait qu'un seul des deux a maintenant p3 en sommet
			t_c = (tmp->p1==p3)||(tmp->p2==p3)||(tmp->p3==p3);

			//on s'occupe de l'adjacence externe vers interne
			if((tmp->p1==p1)||(tmp->p1==p2)||(tmp->p1==p3)||(tmp->p1==p4))
				{
				//le premier point est bon
				if((tmp->p2==p1)||(tmp->p2==p2)||(tmp->p2==p3)||(tmp->p2==p4))
					//c'est donc t1 qu'il faut changer
					//s'il a p3 en commun c'est avec t sinon c'est avec t1
					tmp->t1 = (t_c)?newt:newt1;
					
				//sinon c'est p3 qui est bon et donc c'est t3 qu'il faut changer
				else
					tmp->t3 = (t_c)?newt:newt1;
				}
			else
				//sinon c'est forcement t2 qu'il faut changer
				tmp->t2 = (t_c)?newt:newt1;

			//on s'occupe maintenant de l'adjacence interne vers externe
			if(t_c) //on change t_c
				if((t->p1==tmp->p1)||(t->p1==tmp->p2)||(t->p1==tmp->p3))
					tmps[0] = tmp;
				else	//sinon c'est t2
					tmps[1] = tmp;
			else
				if((t1->p1==tmp->p1)||(t1->p1==tmp->p2)||(t1->p1==tmp->p3))
					tmps[2] = tmp;
				else	//sinon c'est t2
					tmps[3] = tmp;
			}

		//adjacence entre nos 6 triangles
		newt->t1 = tmps[0];
		newt->t2 = (newt->p1==p1)?tmps[1]:newt1;
		newt->t3 = (newt->p1!=p1)?tmps[1]:newt1;

		newt1->t1 = tmps[2];
		newt1->t2 = (newt1->p1==p2)?tmps[3]:newt;
		newt1->t3 = (newt1->p1!=p2)?tmps[3]:newt;

		
		//on efface les anciens triangles
		Liste_EffTriangle(t,list);
		Liste_EffTriangle(t1,list);

		//on ajoute les nouveaux et leurs voisins
		Liste_AddTriangle(newt->t1,list);
		Liste_AddTriangle(newt->t2,list);
		Liste_AddTriangle(newt->t3,list);
		
		Liste_AddTriangle(newt1->t1,list);
		Liste_AddTriangle(newt1->t2,list);
		Liste_AddTriangle(newt1->t3,list);
		
		//on lance la recursivite sur les nouveaux cotes
		p1 = newt->p1;
		p2 = newt->p2;
		p3 = newt->p3;
		p4 = newt1->p1;
		p5 = newt1->p2;
		p6 = newt1->p3;

		//on lance la recursivite entre tous les nouveaux cotes
		FlipAlgo_CoteIllegal(p1,p2,list);
		FlipAlgo_CoteIllegal(p2,p3,list);
		FlipAlgo_CoteIllegal(p4,p5,list);
		FlipAlgo_CoteIllegal(p5,p6,list);
	}
}


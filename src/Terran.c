#include "Terran.h"

//les variables externes
extern STabDel tab;

static void Terran_AddHaut(int indice,double hauteur);
static void Terran_Set(int typ,STriangle* tr,int prof);
static void Terran_Rav(STriangle* orig,STriangle* t,double prof,int len);

void Terran_JeuneMont(STriangle* orig,STriangle* t,double h,int p)
{
	int point=-1;
	STriangle* t1=NULL,*t2=NULL,*t3=NULL;

	if(t!=NULL)
	{
		if(orig==NULL)
		{
		
			Terran_AddHaut(t->p1,h);
			Terran_AddHaut(t->p2,h);
			Terran_AddHaut(t->p3,h);
			t1=t->t1;
			t2=t->t2;
			t3=t->t3;
		}	
		else if(orig==t->t1) 
		{ 
		
			point =t->p3;
			t1=t->t2;
			t2=t->t1;
		}
		else if(orig==t->t2) 
		{
		
			point =t->p1;
			t1=t->t1;
			t2=t->t3;
		}
		else if(orig==t->t1)
		{
		
			point =t->p2;
			t1=t->t3;
			t2=t->t2;
		}

		if (point!=-1)
		{
			if (p<3)
			{
				h/=2;
				Terran_AddHaut(point,h);
			}
			else 
			{
				Terran_AddHaut(point,h);
			}
		}
		if (p>1)
		{
			if(t1!=NULL) Terran_JeuneMont(t,t1,h,p-1);
			if(t2!=NULL) Terran_JeuneMont(t,t2,h,p-1);
			if(t3!=NULL) Terran_JeuneMont(t,t3,h,p-1);
		}
	}
}

void Terran_AddHaut(int indice,double hauteur)
{
	tab.p[indice].z+=hauteur;
}


void Terran_VieilMont(STriangle* orig,STriangle* t,double h,int p,int max)
{
	int point=-1;
	STriangle* t1=NULL,*t2=NULL,*t3=NULL;

	if(t!=NULL)
	{
		if(orig==NULL)
		{
			Terran_Set(MONTV,t,p);
			Terran_AddHaut(t->p1,h/3);
			Terran_AddHaut(t->p2,h/3);
			Terran_AddHaut(t->p3,h/3);
			t1=t->t1;
			t2=t->t2;
			t3=t->t3;
			t->type=NONE;
		}	
	else
	if(t->type==MONTV)
	{
		t->type=NONE;
		if(orig==t->t1) 
		{ 
		
			point =t->p3;
			t1=t->t2;
			t2=t->t1;
		}
		else if(orig==t->t2) 
		{
		
			point =t->p1;
			t1=t->t1;
			t2=t->t3;
		}
		else if(orig==t->t1)
		{
		
			point =t->p2;
			t1=t->t3;
			t2=t->t2;
		}

		if (point!=-1)
		{
			if ((p>(max/10))&&(p<(3*max/10)))
			{
				h*=0.90;
				Terran_AddHaut(point,h);
			}
			else 
			{
				h*=0.95;
				Terran_AddHaut(point,h);
			}
		}
		if (p>1)
		{
			if(t1!=NULL) Terran_VieilMont(t,t1,h,p-1,max);
			if(t2!=NULL) Terran_VieilMont(t,t2,h,p-1,max);
			if(t3!=NULL) Terran_VieilMont(t,t3,h,p-1,max);
		}
	}
	}
}


void Terran_Set(int typ,STriangle* tr,int prof)
{
	if ((tr!=NULL)&&(tr->type==NONE))
	{
		tr->type=typ;
		if (prof>1) 
		{
			if (tr->t1!=NULL)
				if (tr->t1->type==NONE) 
					Terran_Set(typ,tr->t1,prof-1);
			if (tr->t2!=NULL)
				if (tr->t2->type==NONE) 
					Terran_Set(typ,tr->t2,prof-1);
			if (tr->t3!=NULL)
				if (tr->t3->type==NONE) 
					Terran_Set(typ,tr->t3,prof-1);
		}
	}
}


void Terran_Rav(STriangle* orig,STriangle* t,double prof,int len)
{
  srand((unsigned) time(NULL));

	if (t!=NULL)
	{
		STriangle* t1=NULL,*t2=NULL;

		
		prof-=((double)rand()/(double)RAND_MAX);
		prof+=((double)rand()/(double)RAND_MAX);
		Terran_AddHaut(t->p3,-prof);

		if((((double)rand()/(double)RAND_MAX)>0.8)&&(orig!=NULL))
		{
			if (orig==t->t1)
			{
				t1=t->t2;
				t2=t->t3;
			}
			else if (orig==t->t2)
			{
				t1=t->t1;
				t2=t->t3;
			}
			else if (orig==t->t3)
			{
				t1=t->t2;
				t2=t->t1;
			}
			
		}
		else 
		if (orig==NULL)
		{
			Terran_AddHaut(t->p1,-prof);
			prof+=((double)rand()/(double)RAND_MAX);
			Terran_AddHaut(t->p2,-prof);

			if (t->t1!=NULL) t1=t->t1;
			else 
				if (t->t2!=NULL) t1=t->t2;
			else
				if (t->t3!=NULL) t1=t->t3;
			

		}
		else

		{
			if (orig==t->t1)
			{
				if (((double)rand()/(double)RAND_MAX)>0.5)	t1=t->t2;
				else			t1=t->t3;
			}
			else if (orig==t->t2)
			{
				if (((double)rand()/(double)RAND_MAX)>0.5)	t1=t->t1;
				else			t1=t->t3;
			}
			else if (orig==t->t3)
			{
				if (((double)rand()/(double)RAND_MAX)>0.5)	t1=t->t2;
				else			t1=t->t1;
			}
		}

			if (t1!=NULL) Terran_Rav(t,t1,prof,len-1);
			if (t2!=NULL) Terran_Rav(t,t2,prof,len-1);
	}
}

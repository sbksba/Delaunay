#include "TerrainImpl.h"

//les variables externes
extern SParam param;

static double TerrainImpl_SimpleCalcHeight(STerrainImpl* t,double x, double y);	//calcule la hauteur d'un pt (x,y), version simple

	

//initialiser le terrain implicite
STerrainImpl* TerrainImpl_Init(int mont,int valley,double minx,double maxx, double miny,
			       double maxy)	//initialise les terrains
{
  STerrainImpl* res = malloc(sizeof *res);
  int i;
  double tmp,di,dj;
  double min=10000000.0,max=-1000000000.0;

  if(res==NULL)
    {
      printf("Malloc du terrain implicite a echoue\n");
      exit(1);
    }

  //nb de vallees peut etre retrouve en faisant nb_centre - nb_montagnes
  res->nb_montagnes = mont;
  res->nb_centre = mont+valley;

  for(i=0;i<mont+valley;i++)
    {
      //positions
      tmp = ((double) rand()) / ((double) rand()+1);

      tmp = (tmp > 0.0)? tmp:-tmp;
      tmp = (tmp>1.0)?1/tmp:tmp;
      res->centre[i][0] = minx + ( tmp * fabs(maxx-minx));

      tmp = ((double) rand()) / ((double) rand()+1);

      tmp = (tmp > 0.0)? tmp:-tmp;
      tmp = (tmp>1.0)?1/tmp:tmp;
      res->centre[i][1] = miny + ( tmp * fabs(maxy-miny));

      //on met les coef entre 10 et 35
      res->etal[i][0] = rand()%5 + 5;
      res->etal[i][1] = rand()%5 + 5;

      //montagne dira si c'est une montage ou une vallée
      //dans la version normalisée, on n'utilise pas de vallée donc montagne sera tjs vraie
      res->montagne[i]= (i<mont)||(param.norm_v2);

      //hauteur maximale 5
      res->hauteur[i] = ((double) rand())/((double) rand()+ 1.0);
      if(res->hauteur[i]>1.0)
	res->hauteur[i]=1/res->hauteur[i];
      res->hauteur[i]*=5.0;
    }

  //si c'est pas la version normalisée 2
  if(!param.norm_v2)
    {
      //on verifie maintenant que la hauteur maxi est 10. ceci par verification de la hauteur des centres
      for(i=0;i<mont+valley;i++)
	{
	  tmp = TerrainImpl_SimpleCalcHeight(res,res->centre[i][0],res->centre[i][1]);
		
	  //si haut dessus de la limite et une montagne
	  if((tmp>10.0)&&(res->montagne))
	    {
	      //alors on le baisse si on peut
	      if(tmp-10.0>res->hauteur[i]) //si c'est plus haut que la hauteur
		{
		  //on regarde si on peut le transformer en vallee
		  if(tmp-10-res->hauteur[i]<10)
		    res->montagne[i]=0;
		  //sinon on laisse c'est un cas extreme
		}
	      //sinon on peut baisser la montagne pour rendre cette pos valide
	      else
		res->hauteur[i] -= tmp-10.0;
	    }
	  else if(tmp>10.0) //c'est une vallee
	    //on met la hauteur a 10, c'est le mieux qu'on puisse faire
	    res->hauteur[i]= 10.0;
	  else
	    //sinon si c'est une vallee
	    if((!res->montagne)&&(tmp<-10.0))
	      {
		//alors on le baisse si on peut
		if(tmp+10.0<-res->hauteur[i]) //si c'est plus bas que la hauteur
		  {
		    //on regarde si on peut le transformer en montagne
		    if(tmp+10+res->hauteur[i]>-10.0)
		      res->montagne[i]=1;
		    //sinon on laisse c'est un cas extreme
		  }
		//sinon on peut baisser la montagne pour rendre cette pos valide
		else
		  res->hauteur[i] -= tmp+10.0;
	      }
	}
    }
  else //sinon
    {
      //on calcule le min et le max pour permettre de mettre les valeurs entre 0 et 1
      for(di=-10.0;di<10.0;di+=0.5)
	for(dj=-10.0;dj<10.0;dj+=0.5)
	  {
	    tmp = TerrainImpl_SimpleCalcHeight(res,di,dj);
	    if(tmp>max)
	      max = tmp;
	    if(tmp<min)
	      min = tmp;
	  }
      if(fabs(min-max)<EPS)
	{
	  min = 0.0;
	  max = 1.0;
	}
      param.min = min;
      param.max = max;
    }

  return res;
}

double TerrainImpl_SimpleCalcHeight(STerrainImpl* t,double x, double y)		//calcule la hauteur d'un pt (x,y)
{
  int i=0;
  double res = 0.0,tmp;
  for(i=0;i<t->nb_centre;i++)
    {
      tmp = t->hauteur[i]*exp( -(t->centre[i][0]-x)*(t->centre[i][0]-x)/t->etal[i][0]-(t->centre[i][1]-y)*(t->centre[i][1]-y)/ t->etal[i][1]);
      res += (t->montagne[i])?tmp:-tmp;
    }
  return res;
}

//difference entre ce TerrainImpl_CalcHeight et le simple?
//celui là prend en charge la normalisation
//par contre il utilise le simple...
double TerrainImpl_CalcHeight(STerrainImpl* t,double x, double y)		//calcule la hauteur d'un pt (x,y)
{
  double res = TerrainImpl_SimpleCalcHeight(t,x,y);
  if(param.norm_v2)
    {
      //on met la valeur entre 0 et 1 et on multiplie par 10
      res = (res-param.min)/(param.max-param.min);
      //on adoucit le terrain si on le doit
      if(param.flat)
	res = pow(res,param.power);
      res *= 10.0;
      return res;
    }
  return res;
}

//dessine des points rouges pour montrer a quoi ressemble le terrain
void TerrainImpl_Draw(STerrainImpl* t,double minx,double maxx, double miny,double maxy,double intervx,double intervy)	
{
  double i,j;
  double tmp;
  char s[50];
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	// on tracera uniquement les contours des polygones
  glDisable(GL_DEPTH_TEST);
	
  // contour rouge
  glColor3f(1.0,0.0,0.0);					
	
  glBegin(GL_POINTS);// on va tracer des points
  for(i=minx;i<maxx;i+=intervx)
    for(j=miny;j<maxy;j+=intervy)
      {
	tmp = TerrainImpl_CalcHeight(t,i,j);
	glVertex3d(i,j,tmp);
      }
  glEnd();

  glEnable(GL_DEPTH_TEST);

  //affichage texte
  sprintf(s,"Nombre de montagnes: %d, vallees: %d",t->nb_montagnes,t->nb_centre-t->nb_montagnes);
  Graphique_RenderBitmapString(10,40,(void *) ((int)GLUT_BITMAP_8_BY_13),s);
  if(param.norm_v2)
    {
      if(param.flat)
	sprintf(s,"Version normalisée optimisée avec aplatissement d'une puissance de: %d",param.power);
      else
	sprintf(s,"Version normalisée optimisée sans aplatissement");

      Graphique_RenderBitmapString(10,50,(void *) ((int)GLUT_BITMAP_8_BY_13),s);
    }
}

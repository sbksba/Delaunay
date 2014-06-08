#include "Main.h"

static void Main_inserePoint(); 					//choisi entre la version destruct et construct et flip
static void Main_redimFenetre(int largeur, int hauteur); 		//appeler lorsqu'on change la taille de la fenetre
static void Main_affichage(); 						//gere l'affichage
static void Main_clavier(unsigned char touche, int x, int y); 		//gere le clavier
static void Main_touchesSpeciales(int touche, int x, int y);		//gere le clavier mais les touches speciales
static void Main_timer(int value); 					//un timer qui gère l'insertion automatique

//Variables globales
//Le terrain 
STabDel tab;
//La structure necessaire pour le terrain implicite
STerrainImpl* terrainimpl = NULL;
//Structure pour les parametres et leur valeurs par defauts
SParam param = {1,1,1,0,1,2,-65.0,0.0,-6.0,200,0,0,1,
		TOUS,0,0.0,0.0,0,2,0,1.0,1.0,0.008,0.2,0.0};

//On passe a la prochaine generation
void Main_nextGeneration()
{
  param.gentype++;
  //si on doit cycler, on remet gentype a 1
  if(param.gentype>=END)
    param.gentype=1;
  //est-ce qu'on montre le terrain implicite
  param.show_impl = (param.gentype&TERRAINIMPL);
}

void Main_timer(int value)
{
  //on rappelle si on peut encore insere
  if((param.auto_insert)&&!param.stop_insert)
    Main_inserePoint();

  //on continue le timer pour faire varier le niveau de l'eau
  Main_affichage();
  glutTimerFunc(param.timer_wait,Main_timer, 0);

  //Pour enlever le warning
  (void) value;
}

//La fenêtre vient d'être redimensionnée à la taille largeur*hauteur
void Main_redimFenetre(int largeur, int hauteur)
{
  GLfloat rapport = ((float)(largeur))/((float)(hauteur));

  glViewport(0,0,largeur,hauteur);		// on utilise toute la fernêtre

  glMatrixMode(GL_PROJECTION);			// on va définir la matrice de projection
  glLoadIdentity();						// initialisation avec la matrice identité
  gluPerspective(40,rapport,1.0,25.0);	// on définit une vue en perspective
  glMatrixMode(GL_MODELVIEW);				// on repasse à la matrice de modélisation-visualisation

  glutPostRedisplay();					// on force l'affichage du contenu de la fenêtre
}

//Une demande d'affichage vient d'être générée
void Main_affichage()
{
  char s[50];

  glClear(GL_COLOR_BUFFER_BIT);			// on efface l'écran avec la couleur du fond
  glClear(GL_DEPTH_BUFFER_BIT);			// initialisation du z-buffer

  glLoadIdentity();				// initialisation de la matrice de modélisation-visualisation

  // on fixe la position de l'oeil, le point qu'il regarde et son orientation
  SPoint oeil = {0.0,0.0,5.0};
  SPoint point = {0.0,0.0,0.0};
  SVecteur V1 = {1.0,0.0,0.0}, V2 = {point.x-oeil.x,point.y-oeil.y,point.z-oeil.z};
  SVecteur orientation;
  Vecteur_ProdVect(&V1,&V2,&orientation);
  gluLookAt(oeil.x,oeil.y,oeil.z,point.x,point.y,point.z,orientation.x,orientation.y,orientation.z);

  glTranslated(0.0,0.0,param.trans);		// on translate la scène le long de l'axe des y de la valeur trans

  glRotated(param.angle,1.0,0.0,0.0);		// on tourne la scène d'un angle "angle" autour de l'axe Ox
  glRotated(param.angle2,0.0,0.0,1.0);
  glScalef(0.6f,0.6f,0.5f);


  //on selectionne le mode de dessin
  if(param.draw)
    {
      //on utilise le Stencil buffer pour que l'eau ne soit juste dessiner au dessus du terrain
      if(param.draw==1)
	Graphique_Dessin2D();
      else 
	if(param.filaire)
	  Graphique_Dessin3D();
	else
	  Graphique_Dessin3DFull(); 
    }
  else
    Graphique_RenderBitmapString(50,200,(void *) ((int)GLUT_BITMAP_8_BY_13),"Pas d'image");

  //est-ce qu'on affiche le terrain implicite
  if(param.show_impl)
    TerrainImpl_Draw(terrainimpl,-10,10,-10,10,0.5,0.5);
	
  glColor3f(1.0,1.0,1.0f);
  //l'eau
  if(param.draw>1) //on ne dessine pas l'eau en 2D
    if(param.water)
      {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	
	glColor4f(0.0,0.2,4.0f,1.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glBegin(GL_QUADS);	
	glVertex3d(-10.0,10.0,param.w_cur);
	glVertex3d(10.0,10.0,param.w_cur);
	glVertex3d(10.0,-10.0,param.w_cur);
	glVertex3d(-10.0,-10.0,param.w_cur);
	glEnd();
	//on change la valeur de w_cur
	param.w_cur += param.w_dif;
	if(fabs(param.w_cur-param.w_lvl)>param.w_max)
	  param.w_dif*=-1.0;
	glDisable(GL_BLEND);
	Graphique_RenderBitmapString(10,430,(void *) ((int)GLUT_BITMAP_8_BY_13),"L'eau est dessinée");
      }

  //tout le texte est dessiner ici
  if(param.gentype&TERRAINIMPL)
    Graphique_RenderBitmapString(10,10,(void *) ((int)GLUT_BITMAP_8_BY_13),"Utilisant le terrain implicite");	
  if(param.gentype&TERRANV)
    Graphique_RenderBitmapString(10,20,(void *) ((int)GLUT_BITMAP_8_BY_13),
				 "Utilisant la generation de vieilles montagnes");
  if(param.gentype&TERRANJ)
    Graphique_RenderBitmapString(10,30,(void *) ((int)GLUT_BITMAP_8_BY_13),
				 "Utilisant la generation de jeunes montagnes");

  if(param.flip_algo)
    Graphique_RenderBitmapString(10,440,(void *) ((int)GLUT_BITMAP_8_BY_13),
				 "On utilise le Flip algorithm");
  else
    Graphique_RenderBitmapString(10,440,(void *) ((int)GLUT_BITMAP_8_BY_13),
				 "On utilise l'algorithme Destructeur-Constructeur");
	
  if(param.barycentre)
    Graphique_RenderBitmapString(10,450,(void *) ((int)GLUT_BITMAP_8_BY_13),
				 "On utilise les barycentres pour calculer le prochain point");
  else
    Graphique_RenderBitmapString(10,450,(void *) ((int)GLUT_BITMAP_8_BY_13),
				 "On n'utilise pas les barycentres pour calculer le prochain point");
		

  sprintf(s,"Le nombre de triangles est: %d, de points: %d",tab.nt,tab.np);
  Graphique_RenderBitmapString(10,460,(void *) ((int)GLUT_BITMAP_8_BY_13),s);
  glutSwapBuffers();			// échange des buffers d'affichage
}

// la touche "touche" est enfoncée alors que le pointeur de la souris est en (x,y)
void Main_clavier(unsigned char touche, int x, int y)
{
  SLTriangle *tmp = NULL;
	
  switch(touche)
    {
    case 'a': //auto-insert
      param.auto_insert = !param.auto_insert;
      if(param.auto_insert)
	glutTimerFunc(param.timer_wait,Main_timer, 0);
      break;
    case 'b': //barycentre ou ponderation
      param.barycentre = !param.barycentre;
      break;
    case 'c': //dessin des cercles
      param.circle = !param.circle;
      break;
    case 'd':  //choix du type de dessin
      param.draw = (param.draw+1)%3;
      break;
    case 'f' : //filaire ou non
      param.filaire = !param.filaire;
      break;
    case 'i':  //insertion d'un point
      if(!param.auto_insert)
	{
	  Main_inserePoint();
	  if(DEBUG==1){
	    printf("Triangles are\n");
	    tmp =  tab.triangles->first;
	    while(tmp!=NULL)
	      {
			
		printf("\t Points are: %d %d %d\n",tmp->t->p1,tmp->t->p2,tmp->t->p3);
		printf("\t(%f,%f) (%f,%f) (%f,%f)\n",tab.p[tmp->t->p1].x,tab.p[tmp->t->p1].y,
		       tab.p[tmp->t->p2].x,tab.p[tmp->t->p2].y,tab.p[tmp->t->p3].x,tab.p[tmp->t->p3].y);
		printf("Critere is %f\n",tmp->t->critere);
		tmp = tmp -> next;
	      }
	  }
	}
      break;
    case 'j': //choix de l'algo entre dest-const et flip
      param.flip_algo = !param.flip_algo;
      break;
		
    case 'm': //calculer le carre dans la version normalise du terrain implicite
      if((terrainimpl!=NULL)&&(param.norm_v2))
	param.flat = !param.flat;
				
      break;
    case 'n': //normaliser ou pas dans le cas du terrain implicite
      if(terrainimpl!=NULL)
	{
	  param.norm_v2 = !param.norm_v2;
	  Main_touchesSpeciales(GLUT_KEY_F2,0,0); //on reinitialise le terrain
	  printf("Min: %f Max:%f\n",param.min,param.max);
	}
      break;
		
    case 'o': //decelerer l'interval des insertions
      param.timer_wait += 10;
      if(param.timer_wait>1000)
	param.timer_wait = 1000;
      break;
    case 'p': //accelerer l'interval des insertions
      param.timer_wait -= 10;
      if(param.timer_wait<10)
	param.timer_wait = 1;
      break;
		
    case 'q' : //quitter
      exit(0);
      break;
		
    case 't': //montrer le terrain implicite
      param.show_impl = !param.show_impl;
      break;
    case 'w': //montrer l'eau ou pas
      param.water = !param.water;
      param.w_cur = param.w_lvl;
      break;
    case 'x': //abaisser le niveau de l'eau
      if(param.water)
	{
	  param.w_lvl -= 0.1;
	  param.w_cur -= 0.1;
	}
      break;
    case 'z': //elever le niveau de l'eau
      if(param.water)
	{
	  param.w_lvl += 0.1;
	  param.w_cur += 0.1;
	}
      break;
    case '+': //eclaircir le terrain
      param.light+=0.05;
      if(param.light>1.0)
	param.light=1.0;
      break;
    case '-': //rendre plus fonce le terrain
      param.light-=0.05;
      if(param.light<-1.0)
	param.light=-1.0;
      break;

    }
  glutPostRedisplay();		// on reaffiche

  //Pour enlever le warning...
  (void) x;
  (void) y;
}


// la touche spéciale "touche" est enfoncée alors que le pointeur de la souris est en (x,y)
void Main_touchesSpeciales(int touche, int x, int y)
{
  int tmp1,tmp2,tmp3;

  switch(touche)
    {
	
    case GLUT_KEY_F3: //cycler les types de generations
      Main_nextGeneration();
      break;

      //touches pour gerer le terrain impl
    case GLUT_KEY_F2:  //nouveau terrain
    case GLUT_KEY_F5:  //baisser le nbr de montagnes
    case GLUT_KEY_F6:  //augmenter le nbr de montagnes
    case GLUT_KEY_F7:  //baisser le nbr de vallees
    case GLUT_KEY_F8:  //augmenter le nbr de vallees
      //s'il y a un terrain implicite
      if(terrainimpl!=NULL)
	{//on recupere le nbr de mont et vallees avant le free
	  tmp1=terrainimpl->nb_montagnes;
	  tmp2=terrainimpl->nb_centre-tmp1;
	  free(terrainimpl);
	}
      else
	{tmp1=4;tmp2=4;}
      //modification du nbr de mont et vallees
      if(touche==GLUT_KEY_F5)
	tmp1--;
      if(touche==GLUT_KEY_F6)
	tmp1++;
      if(touche==GLUT_KEY_F7)
	tmp2--;
      if(touche==GLUT_KEY_F8)
	tmp2++;
      //on gere min et max
      if(tmp1<0)
	tmp1=0;
      if(tmp2<0)
	tmp2=0;
      if(tmp1+tmp2>=MAX_CENTER)
	{
	  tmp3 = tmp1;
	  tmp1 = (tmp1>tmp2)?tmp1-(tmp1+tmp2-MAX_CENTER+1):tmp1;
	  tmp2 = (tmp3<tmp2)?tmp2-(tmp3+tmp2-MAX_CENTER+1):tmp2;
	}

      terrainimpl = TerrainImpl_Init(tmp1,tmp2,-10,10,-10,10);
      DestCons_ResetTab();
      break;
    case GLUT_KEY_F10: //affichage du texte ou non
      param.texte = ! param.texte;	
      break;

    case GLUT_KEY_UP :		//zoom avant
      param.trans+=0.2;				 
      break;
    case GLUT_KEY_DOWN :    //zoom arriere
      param.trans-=0.2;
      break;
    case GLUT_KEY_RIGHT : 	//rotation
      param.angle-=2.0;
      break;
    case GLUT_KEY_LEFT : 	//rotation		
      param.angle+=2.0;
      break;
    case GLUT_KEY_PAGE_UP:  //rotation
      param.angle2+= 2.0;
      break;
    case GLUT_KEY_PAGE_DOWN: //rotation 
      param.angle2-= 2.0;
      break;

    }
  glutPostRedisplay();		// on réaffiche

  //Pour enlever le warning...
  (void) x;
  (void) y;
}

//insertion d'un point
void Main_inserePoint()
{
  //choix de l'algo a utiliser
  if(param.flip_algo)
    FlipAlgo_InserePoint();
  else
    DestCons_InserePoint();
	
  //calcule de la hauteur du point
  if(param.gentype&TERRAINIMPL)
    tab.p[tab.np-1].z = TerrainImpl_CalcHeight(terrainimpl,tab.p[tab.np-1].x,tab.p[tab.np-1].y);
  //pour ces 2 generations, on le fait que de temps en temps
  if(param.gentype&TERRANV)
    if (tab.nt%30==0) Terran_VieilMont(NULL,(tab.triangles->first->next->next->t),1.0,20,20);
  if(param.gentype&TERRANJ)
    if(tab.nt%100==0) Terran_JeuneMont(NULL,(tab.triangles->first->t),0.5,7);
} 

//la fonction main
int main( int argc, char *argv[])
{
  glutInit(&argc,argv);				// doit être appelé avant toute autre fonction GLUT
  srand(time(NULL));
  DestCons_InitTab();				//on initialise la structure globale tab

  glutInitDisplayMode(GLUT_RGBA		// le mode de couleur sera Rouge-Vert-Bleu-Alpha
		      | GLUT_DOUBLE	// on utilisera le double buffer
		      | GLUT_DEPTH);// on utilisera le z-buffer
																 
  glutInitWindowSize(WIDTH,HEIGHT);
  glutCreateWindow("Delaunay");
  glClearColor(0.0,0.0,0.0,0.0);		// la couleur du fond sera noire
  glEnable(GL_DEPTH_TEST);			// activer le z-buffer

  /*Initiation OpenGL */
  GLfloat rapport = ((float)(WIDTH))/((float)(HEIGHT));
  glViewport(0,0,WIDTH,HEIGHT);		// on utilise toute la fernêtre
  glMatrixMode(GL_PROJECTION);			// on va définir la matrice de projection
  glLoadIdentity();						// initialisation avec la matrice identité
  gluPerspective(40,rapport,1.0,25.0);	// on définit une vue en perspective
  glMatrixMode(GL_MODELVIEW);				// on repasse à la matrice de modélisation-visualisation

  GLfloat positionLumiere[] = {1.0,1.0,0.0,0.0};		// direction dans laquelle se trouvera la lumière
  GLfloat couleurLumiere[] = {1.0,1.0,1.0,1.0};		// composante RGBA qu'aura la lumière
  glLightfv(GL_LIGHT0, GL_POSITION, positionLumiere);	// on positionne la lumière n° 0
  glLightfv(GL_LIGHT0, GL_DIFFUSE, couleurLumiere);	// on donne sa couleur à la lumière n° 0
  glLightfv(GL_LIGHT0, GL_SPECULAR, couleurLumiere);	// on donne sa couleur à la lumière n° 0

  //Lorsque la fenêtre sera redimensionnée redimFenetre() sera appelé
  glutReshapeFunc(Main_redimFenetre);	
	
  // Main_affichage() sera appelé dès qu'il faudra redessiner le contenu de la fenêtre
  glutDisplayFunc(Main_affichage);	
	
  //Main_clavier() sera appelé dès qu'une touche du clavier sera enfoncée
  glutKeyboardFunc(Main_clavier);		
	
  //Main_touchesSpeciales() sera appelé dès qu'une touche spéciale du clavier sera enfoncée
  glutSpecialFunc(Main_touchesSpeciales);	

  //Initialisation du terrain implicite
  terrainimpl = TerrainImpl_Init(4,4,-10,10,-10,10);

  //s'il y a un prob
  if(terrainimpl==NULL)
    return 1;

  glutMainLoop();						// la boucle événementielle commence
  return 0;
}

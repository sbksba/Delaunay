#ifndef H_DEFINE
#define H_DEFINE

#include <stdlib.h>
#include <GL/glut.h>
#include <time.h>
#include <math.h>
#include <stdio.h>

//taille de la fenêtre
#define WIDTH 640	
#define HEIGHT 480

// Type de terrain, sert pour la génération des jeunes et vieilles montagnes
#define NONE 0
#define MONTJ 1 
#define MONTV 2  

//Les différentes générations en masque
#define TERRANJ 1
#define TERRANV 2
#define TERRAINIMPL 4
#define END 8
#define TOUS (END-1)

// table de triangles 3D
#define MAX_TRIANGLES 9999/*100000*/
#define MAX_POINTS 10000/*15001*/

//erreur pour les flottants
#define EPS (0.0000001)

//pour les printf
#define DEBUG 0  

// point 3D
typedef struct sPoint
{
	double x,y,z;
}SPoint;

typedef struct sParam
{
	//quel algo utiliser
	int flip_algo;

	int texte; //affichage du texte

	//param pour l'affichage
	int filaire;	//filaire ou non
	int circle;	//pour la 2D, on affiche le cercle ou non
	int show_impl; //est_ce qu'on affiche le terrain implicite
	int draw; //0 no draw, 1 2d, 2 3d
		
	double angle;	//le premier angle
	double angle2;	//le 2eme angle
	double trans;	//le zoom

	//param pour les insertions
	int timer_wait;		//l'intervalle entre deux insertions
	int auto_insert;	//est-ce qu'on insere automatiquement
	int stop_insert;	//est-ce qu'on arrete l'insertion car trop de triangles ou de points
	int barycentre;	//est-ce qu'on pondere le calcul du barycentre du nouveau point

	//param pour le style de terrain
	int gentype;

	//quelques inteen pour avoir différentes version du programme Terrainimpl
	int norm_v2;		//version 2 qui permet d'être sûr d'avoir des valeurs entre 0.0 et 10.0
	double min,max;		//pour la normalisation
	int flat;		//est-ce qu'on aplatit le terrain pour créer des vallées
	int power;		//puissance de l'aplatissement

	//quelques inteens pour l'affichage
	int water;		//ajoute-t-on de l'eau
	double w_lvl;		//niveau de l'eau
	double w_cur;		//niveau courant
	double w_dif;		//difference a ajouter a chaque image
	double w_max;		//difference max entre w_lvl et w_cur
	double light;		//contraste...
}SParam;

#endif

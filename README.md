Implémentation de l'algorithme Delaunay
=======================================

> Triangulation de Delaunay utilisant un systeme d'insertion de points.     
> Il y a deux algorithmes implementes:    
> - Destruction/Construction    
> - L'Algorithme "Flip"    

Fichiers inclus
---------------

> Sources     
> - Delaunay.c     
> - DestCons.c     
> - Flipalgo.c     
> - Graphique.c     
> - Liste.c     
> - TerrainImpl.c     
> - Terran.c     
> - Triangle.c     
> - Vecteur.c     
> - Main.c    

> Headers
> - Define.h     
> - Delaunay.h     
> - DestCons.h     
> - Flipalgo.h     
> - Graphique.h     
> - Liste.h     
> - TerrainImpl.h     
> - Terran.h     
> - Triangle.h     
> - Vecteur.h     
> - Main.h    

Directive du Makefile
----------------------


> make    
> compil le programme    

> clean    
> efface tous les fichiers ".o" et l'executable      

> cleanall    
> efface les répertoires obj , bin et lib      

> proper     
> efface tous les fichiers temporaires du dossier     

> run     
> lance le programme     


Commandes
---------


> a: Insertion automatique de point    
> b: Mis en place de l'option Barycentre     
> c: Dessiner les cercles de Delaunay      
> f: Dessiner en fil de fer ou non     
> i: Insertion d'un point      
> j: Choix entre les deux algos: Destruction-Construction et Flip     
> m: Normaliser la hauteur des montagnes et des vallees utilisant un systeme mieux plus lisse que la version 'n'     
> n: Normaliser version 1     
> o: Decelerer l'interval des insertions     
> p: Accelerer l'interval des insertions     
> q: Quitter     
> t: Montrer le terrain implicite     
> w: Montrer l'eau ou pas     
> x: Abaisser le niveau de l'eau     
> z: Elever le niveau de l'eau     

> (-): Rendre plus fonce le terrain     
> (+): Rendre plus clair le terrain     

> F2: Un nouveau terrain       
> F3: Cycler dans les types de generations     
> F5: Baisser le nombre de montagnes     
> F6: Augmenter le nombre de montagnes     
> F7: Baisser le nombre de vallees     
> F8: Augmenter le nombre de vallees     
> F10: Afficher ou non le texte     

> Fleche haut: Zoom avant     
> Fleche bas: Zoom arriere     
> Droite: Rotation axe 1     
> Gauche: Anti-Rotation axe 1     
> Page Up: Rotation axe 2     
> Page Down: Rotation axe 2     

Bibliotheques
-------------

> Il est nescesaire d'installer la bibliotheque glut pour faire fonctionner le programme.      

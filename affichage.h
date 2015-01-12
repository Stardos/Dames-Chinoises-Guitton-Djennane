#ifndef __AFFICHAGE_H__
#define __AFFICHAGE_H__

	extern int Plateau[17][25];
	extern char Plateau_graph[34][50];
	extern int masque[17][25];

	void create_plateau ();
	void maj_plateau(pion pos1, pion pos2);
	void init_masque ();
	void initialisation_plateau ();
	void create_affich();
	void plateau_en_cours(Groupe_pions nom);
	void affiche(int n, HANDLE h, WORD wOldColorAttrs);
	
#endif

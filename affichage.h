#ifndef __AFFICHAGE_H__
#define __AFFICHAGE_H__
    typedef struct{

        int x;  //  On définit un pion par sa coordonnée en x.
        int y;  //  Et par sa coordonnée en y.

    } pion, PtSurpion;

    typedef struct{

        char couleur;           //  Un ensemble de pion est défini par sa couleur.
        pion pos_pions[10];     //  Pour chaque couleur un joueur a 10 pions.
        pion label[2][10];     //  Chaque couleur ne peut circuler que dans deux branches.
        pion coup_possible[10][6];  //  A chaque tour un joueur à un ensemble de coup possible pour une couleur (au plus 6*10).
        pion sommet;       //  Chaque couleur à un sommet à atteindre.


    } Groupe_pions, PtSurGroupe_pions;
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

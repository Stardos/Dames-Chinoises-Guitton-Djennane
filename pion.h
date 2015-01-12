#ifndef __PION_H__
#define __PION_H__
                            ////////////////////////////////////////
                            //         Structure de pions         //
                            ////////////////////////////////////////

    typedef struct{

        int x;  //  On définit un pion par sa coordonnée en x.
        int y;  //  Et par sa coordonnée en y.

    } pion, PtSurpion;




                            ////////////////////////////////////////
                            //  Structure d'un ensemble de pions  //
                            ////////////////////////////////////////
    typedef struct{

        char couleur;           //  Un ensemble de pion est défini par sa couleur.
        pion pos_pions[10];     //  Pour chaque couleur un joueur a 10 pions.
        pion label[2][10];     //  Chaque couleur ne peut circuler que dans deux branches.
        pion coup_possible[10][6];  //  A chaque tour un joueur à un ensemble de coup possible pour une couleur (au plus 6*10).
        pion sommet;       //  Chaque couleur à un sommet à atteindre.


    } Groupe_pions, PtSurGroupe_pions;

    extern Groupe_pions Groupe[6];
    extern  char couleurs[6];
    extern pion label[6][10];


	pion saut(pion dep, pion obstacle);
	pion Deplacement_test (pion pos_pion, pion dir);
	int test_branche(pion coord, int num_groupe);
	void coup_possible_pion(int i,  int k);
	void Tri_coups (int i, int k);
	void jouer (int i, int k, pion arri);
	int Deplacement (char coul, int num_pion);
	void init_pions_label(int num_couleur);
	void init_groupes (int n);
#endif

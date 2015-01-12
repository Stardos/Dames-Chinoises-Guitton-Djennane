#ifndef __PION_H__
#define __PION_H__

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

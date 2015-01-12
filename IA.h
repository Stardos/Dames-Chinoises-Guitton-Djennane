#ifndef __IA_H__
#define __IA_H__
	int abs (int a);
	int Distance(int nb_couleurs, int *indice);
	int Min( int a, int b);
	int Max( int a, int b);
	void IA_jouer ( int profondeur, int *indices_groupes, int nb_groupe);
	int alphabeta( int profondeur, int p, int alpha, int beta, int *indices_groupes, int nb_groupe);
	
#endif

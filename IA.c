#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>
#include "affichage.h"
#include "pion.h"
int okok =0;
int abs (int a);
int Min( int a, int b);
int Max( int a, int b);
void IA_jouer ( int profondeur, int *indices_groupes, int nb_groupe);
int alphabeta( int profondeur, int p, int alpha, int beta, int *indices_groupes, int nb_groupe);
int Distance(int nb_couleurs, int *indice);

int abs (int a){
	if (a <0){
		return -a;
	}
}     

int Distance(int nb_couleurs, int *indice){

	int i, j, pions_J1 = 0, pions_J2 =0;

	for (i=0; i<nb_couleurs; i++){
		for (j=0; j<=9; j++){
			pions_J1 = pions_J1 + sqrt(pow(Groupe[indice[i]].pos_pions[j].x-Groupe[indice[i]].sommet.x,2)+pow(Groupe[indice[i]].pos_pions[j].y-Groupe[indice[i]].sommet.y,2));
			pions_J2 = pions_J2 + sqrt(pow(Groupe[5-indice[i]].pos_pions[j].x-Groupe[5-indice[i]].sommet.x,2)+pow(Groupe[5-indice[i]].pos_pions[j].y-Groupe[5-indice[i]].sommet.y,2));
		}
	}

	return (pions_J1-pions_J2);
}

int Min( int a, int b){
	if (a > b){ 
		return b;
	}else{
		return a;
	}
}

int Max( int a,  int b){
	if (a > b){ 
		return a;
	}else{
		return b;
	}
}

void IA_jouer ( int profondeur, int *indices_groupes, int nb_groupe){
	int i, j, k, cpt=0;
	int alpha = -5000;
	int beta = 5000;
	int min_max;
	int indice_couleur;
	int indice_pion;
	pion meilleur_coup;
	pion coup[6];
	pion pos_avant;



	//	 Pour l'ensemble des couleurs d'un joueur.
	for (i=0; i<= nb_groupe-1; i++){

		//	Pour l'ensemble des pions d'un groupe.
		for(j = 0; j <= 9; j++){

			//	On met les coups associé au pion en cours, pour une couleur donnée.
			coup_possible_pion(indices_groupes[i],  j);

			//	On tri les coups pour n'avoir que ceux qui sont jouable au début.
			Tri_coups (indices_groupes[i],  j);

			cpt=0;

			while ( (cpt <= 5) && ((Groupe[indices_groupes[i]].coup_possible[j][cpt].x != 0) || (Groupe[indices_groupes[i]].coup_possible[j][cpt].y != 0))  ){

				coup[cpt].x = Groupe[indices_groupes[i]].coup_possible[j][cpt].x;
				coup[cpt].y = Groupe[indices_groupes[i]].coup_possible[j][cpt].y;
				cpt++;
			}
			
			
			//	On continue tant que l'on a pas atteint 5 (le nombre max de coup), et que ce coup ne soit pas {0,0}.
			for (k=0; k< cpt; k++){
				
				//	On commence par mémoriser la position du pion avant de le déplacer.
				pos_avant.x = Groupe[indices_groupes[i]].pos_pions[j].x;
				pos_avant.y = Groupe[indices_groupes[i]].pos_pions[j].y;
	
				//	On joue le coup proposé : en paramètre on donne : l'indice du groupe / le numéro du pion / le coup à jouer.
				jouer (indices_groupes[i], j, coup[k]);

				// On descend dans l'arbre en appelant la fonction de récursion alphabeta.
				min_max = alphabeta (profondeur, profondeur -1, alpha, beta, indices_groupes, nb_groupe);

				//	On compae la valeur remonté à toutes les autres remontées avec les coups d'avant, et on garde le maximum.
				if (min_max > alpha){
					//	On sauve la valeur max.
					alpha = min_max;

					//	On mémorise de quelle couleur il s'agit.
					indice_couleur = indices_groupes[i];

					//	On sauve quelle est ce pion.
					indice_pion = j;

					//	On sauve c'est coordonées.
					meilleur_coup.x =coup[k].x;
					meilleur_coup.y =coup[k].y;
				}

				// On déjoue le coup, pour ce faire on replace le pion à sa position initiale 
				jouer (indices_groupes[i], j,pos_avant);
				
			}
			
		}
	}
	printf ("Couleur joue : %i\n", indice_couleur);
	printf ( "Pion joue  : %i\n", indice_pion+1);
	printf( " Position joue : [%i ,%i ] \n", meilleur_coup.x, meilleur_coup.y );
	jouer (indice_couleur, indice_pion,meilleur_coup);
}


int alphabeta( int profondeur, int p, int alpha, int beta, int *indices_groupes, int nb_groupe){

	int i, j, k, cpt = 0;
	int min_max;
	int *indices_groupes2;
	pion pos_avant;
	pion coup[6];
	
	 HANDLE h = GetStdHandle ( STD_OUTPUT_HANDLE ); //->whats this for? cant it be changed?
     WORD wOldColorAttrs; //->whats this for? cant it be changed?
        CONSOLE_SCREEN_BUFFER_INFO csbiInfo; //->whats this for? cant it be changed?
        GetConsoleScreenBufferInfo(h, &csbiInfo);
        wOldColorAttrs = csbiInfo.wAttributes;
	if (p == 0){

		return Distance(nb_groupe, indices_groupes) ;

	}else{

		if ( (profondeur - p)%2 == 1 ){
			
			min_max = 5000;

			for (i=0; i<= nb_groupe -1; i++){

				for(j = 0; j <= 9; j++){

					

					//	On met les coups associé au pion en cours, pour une couleur donnée.
					coup_possible_pion(5-indices_groupes[i],  j);


					//	On tri les coups pour n'avoir que ceux qui sont jouable au début.
					Tri_coups (5-indices_groupes[i],  j);

					cpt=0;



					while ( (cpt <= 5) && ((Groupe[5-indices_groupes[i]].coup_possible[j][cpt].x != 0) || (Groupe[5-indices_groupes[i]].coup_possible[j][cpt].y != 0))  ){

						coup[cpt].x = Groupe[5-indices_groupes[i]].coup_possible[j][cpt].x;
						coup[cpt].y = Groupe[5-indices_groupes[i]].coup_possible[j][cpt].y;
						cpt++;

					}
					
			
					//	On continue tant que l'on a pas atteint 5 (le nombre max de coup), et que ce coup ne soit pas {0,0}.
			
					for (k=0; k< cpt; k++) {

						/* On stocke la position avant et après du pion*/

						pos_avant.x = Groupe[5-indices_groupes[i]].pos_pions[j].x;
						pos_avant.y = Groupe[5-indices_groupes[i]].pos_pions[j].y;

						/* On joue le coup proposé dans la liste */
						
						jouer (5-indices_groupes[i], j, coup[k]);

						min_max = Min( min_max, alphabeta (profondeur, p-1, alpha, beta, indices_groupes, nb_groupe) );

												/* On déjoue le coup, c'est comme si on jouait le coup inverse.*/
						if ((okok ==16)){   
							printf ("pion %i, couleur : %i \n", j, i);
						for (k=0; k < cpt; k++){
							printf (" coup[%i] : {%i, %i } \n", k, coup[k].x, coup[k].y);
						}   
 						affiche(nb_groupe, h, wOldColorAttrs);

						}
						okok++;
						jouer (5-indices_groupes[i], j,pos_avant);


						

						if (alpha >= min_max){

							return min_max;	

						}

						beta = Min (beta, min_max);

						
						
						
					}
				}
			}

		}else{

			min_max = -5000;

			for (i=0; i<= nb_groupe -1; i++){
				for(j = 0; j <= 9; j++){

					
					//	On met les coups associé au pion en cours, pour une couleur donnée.
					coup_possible_pion(indices_groupes[i],  j);

					//	On tri les coups pour n'avoir que ceux qui sont jouable au début.
					Tri_coups (indices_groupes[i],  j);

					cpt=0;

					while ( (cpt <= 5) && ((Groupe[indices_groupes[i]].coup_possible[j][cpt].x != 0) || (Groupe[indices_groupes[i]].coup_possible[j][cpt].y != 0))  ){

						coup[cpt].x = Groupe[indices_groupes[i]].coup_possible[j][cpt].x;
						coup[cpt].y = Groupe[indices_groupes[i]].coup_possible[j][cpt].y;
						cpt++;
					}
					
					
					//	On continue tant que l'on a pas atteint 5 (le nombre max de coup), et que ce coup ne soit pas {0,0}.
					for (k=0; k< cpt; k++) {
						/* On stocke la position avant et après du pion*/
						pos_avant.x = Groupe[indices_groupes[i]].pos_pions[j].x;
						pos_avant.y = Groupe[indices_groupes[i]].pos_pions[j].y;
						/* On joue le coup proposé dans la liste */
						jouer (indices_groupes[i], j, Groupe[indices_groupes[i]].coup_possible[j][k]);

						min_max = Max( min_max, alphabeta (profondeur, p-1, alpha, beta, indices_groupes, nb_groupe) );

						/* On déjoue le coup, c'est comme si on jouait le coup inverse.*/
						jouer (indices_groupes[i], j,pos_avant);
						if (beta <= min_max){
							return min_max;	
						}

						alpha = Max (alpha, min_max);

						
						
					}
				}
			}			
		}
		return min_max;
	}
}

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>
#include "affichage.h"

int dist (pion A, pion B);
int Min( int a, int b);
int Max( int a, int b);
void IA_jouer ( int profondeur, int *indices_groupes, int nb_groupe);
int alphabeta( int profondeur, int p, int alpha, int beta, int *indices_groupes, int nb_groupe);
int Distance(int nb_couleurs, int *indice);

int dist ( pion A, pion B){

	if (B.x > A.x + abs ( A.y - B.y))
		return abs ( A.y - B.y) + ( B.x - abs (A.y - B.y+ A.x)/2);
    else
        if ( B.x < A.x - abs (A.y -B.y))
            return abs ( A.y - B.y) + ( A.x - abs (A.y - B.y+ B.x)/2);
		else
            return abs (B.y - A.y);
}


int Distance(int nb_couleurs, int *indice){

	//int okok = 0 ;

	int i, j, pions_J1 = 0, pions_J2 =0;
	int dist_max;
	int dist_sommetJ1, dist_sommetJ2 ;


	for (i=0; i<nb_couleurs; i++){


		//	Distance entre deux sommets opposés.
		dist_max = dist(Groupe[indice[i]].label[0][0],Groupe[indice[i]].label[1][0]) ;

		for (j=0; j<=9; j++){

			dist_sommetJ1 =  dist( Groupe[indice[i]].sommet, Groupe[indice[i]].pos_pions[j] );
			pions_J1 = pions_J1 + ((100*dist_max)/( 1+ dist_max - dist_sommetJ1))  ;

			dist_sommetJ2 =  dist( Groupe[5-indice[i]].sommet, Groupe[5-indice[i]].pos_pions[j] );
			pions_J2 = pions_J2 + (100*(dist_max) /( 1+ dist_max - dist_sommetJ2))  ;

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

int choix_coup_a_jouer(int nb_coup_optimaux){
    return rand()%(nb_coup_optimaux);
}

void IA_jouer ( int profondeur, int *indices_groupes, int nb_groupe){
	int i, j, k, l, cpt=0;
	int alpha = -5000;
	int beta = 5000;
	int min_max;
	int nb_coups_optimaux;
	int random_coup;
	int* indice_couleur;
	int* indice_pion;
	pion* meilleur_coup;
	int* indice_couleur_tmp;
	int* indice_pion_tmp;
	pion* meilleur_coup_tmp;
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

					//	On mémorise de quelle couleur il s'agit dans un tableau.
                    indice_couleur = (int*)malloc(sizeof(int));
					indice_couleur[0] = indices_groupes[i];

					//	On sauve quelle est ce pion dans un tableau.
                    indice_pion = (int*)malloc(sizeof(int));
					indice_pion[0] = j;

					//	On sauve le meilleur coup dans un tableau
					meilleur_coup = (pion*)malloc(sizeof(pion));
					meilleur_coup[0] = coup[k];

					nb_coups_optimaux = 1;
				}else
                    if (alpha == min_max) {

                        // On initialise des tableaux temp pour récupérer les données des tableaux
                        indice_couleur_tmp = (int*)malloc(nb_coups_optimaux*sizeof(int));
                        indice_pion_tmp = (int*)malloc(nb_coups_optimaux*sizeof(int));
                        meilleur_coup_tmp = (pion*)malloc(nb_coups_optimaux*sizeof(pion));
			
			// On copie les données	
                        for (l=0; l<=nb_coups_optimaux-1;l++){
                            indice_couleur_tmp[l]=indice_couleur[l];
                            indice_pion_tmp[l]=indice_pion[l];
                            meilleur_coup_tmp[l]=meilleur_coup[l];
                        }
			
			// On libère la mémoire
                        free(indice_couleur);
                        free(indice_pion);
                        free(meilleur_coup);

                        nb_coups_optimaux++;

			//On recrée des talbeaux avec l'ancienne taille + 1
                        indice_couleur = (int*)malloc(nb_coups_optimaux*sizeof(int));
                        indice_pion = (int*)malloc(nb_coups_optimaux*sizeof(int));
                        meilleur_coup = (pion*)malloc(nb_coups_optimaux*sizeof(pion));

			// On recopie les données depuis les tableaux temp
                        for (l=0; l<=nb_coups_optimaux-2;l++){
                            indice_couleur[l]=indice_couleur_tmp[l];
                            indice_pion[l]=indice_pion_tmp[l];
                            meilleur_coup[l]=meilleur_coup_tmp[l];
                        }

			// On ajoute la caractérisation du nouveau coup à la fin des talbeaux
                        indice_couleur[l]=indices_groupes[i];
                        indice_pion[l]=j;
                        meilleur_coup[l]=coup[k];

			// On libère la mémoire
                        free(indice_couleur_tmp);
                        free(indice_pion_tmp);
                        free(meilleur_coup_tmp);
                    }

				// On déjoue le coup, pour ce faire on replace le pion à sa position initiale
				jouer (indices_groupes[i], j,pos_avant);

			}

		}
	}
	// On choisit un nombre aléatoire entre 0 et nb_coups_optimaux-1
	random_coup = choix_coup_a_jouer(nb_coups_optimaux);
	//printf ("Couleur joue : %i\n", indice_couleur);
	//printf ( "Pion joue  : %i\n", indice_pion+1);
	//printf( " Position joue : [%i ,%i ] \n", meilleur_coup.x, meilleur_coup.y );
	jouer (indice_couleur[random_coup], indice_pion[random_coup],meilleur_coup[random_coup]);
}


int alphabeta( int profondeur, int p, int alpha, int beta, int *indices_groupes, int nb_groupe){

	int i, j, k, cpt = 0;
	int min_max;
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
						//if ((okok ==16)){
						//	printf ("pion %i, couleur : %i \n", j, i);
						//for (k=0; k < cpt; k++){
						//	printf (" coup[%i] : {%i, %i } \n", k, coup[k].x, coup[k].y);
						//}
 						//affiche(nb_groupe, h, wOldColorAttrs);

						//}
						//okok++;
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

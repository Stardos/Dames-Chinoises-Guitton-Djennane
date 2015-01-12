#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>
#include "pion.h"

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

                            ////////////////////////////////////////
                            // Echéquié passé en variable globale //
                            ////////////////////////////////////////


    // On initialise seulement le premier quart de notre plateau de jeu (ligne 0 à 12; colonne 0 à 8)

    int Plateau[17][25] = { //0  1  2  3  4  5  6  7  8  9  10 11 12
                             {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, //0
                             {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, //1
                             {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1}, //2
                             {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0}, //3
                             {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1}, //4
                             {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0}, //5
                             {0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1}, //6
                             {0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0}, //7
                             {0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1}, //8
           };
    char Plateau_graph[34][50];

  
 

    int masque[17][25];

                                                    //      Création de l'échiquier        //
                                                    //                                     //
                                                      /////////////////////////////////////


// On initialise tout le plateau par double symétrie (d'abord par rapport à la colonne 12, puis par rapport à la lgine 8).

void create_plateau(){

    int C;     //  Déplacement dans les colonnes.
    int L;     //  Déplacement dans les lignes.

// symétrie par rapport à la colonne 12 .
    //  Parcours des lignes.
    for (L=0; L<=8; L++){

        //  Parcours des colonnes.
        for (C=0; C<=11; C++){

            //  On copie la valeur d'une case dans son symétrique par rapport à la colone 12.
            Plateau[L][24-C] = Plateau[L][C];
        }
    }

// symétrie par rapport à la colonne 12.
    //  Parcours des colonnes .
    for (C=0; C<=24; C++){

        //  Parcours des lignes
        for (L=0; L<=11; L++){

            //  On copie la valeur d'une dans son symétrique par rappor la ligne 8.
            Plateau[16-L][C] = Plateau[L][C];
        }
    }
}
                        ////////////////////////////////////////
                        //       Gestion des déplacements     //
                        ////////////////////////////////////////

//  Mise à jour du Plateau contenant cases occupées, cases libres et vide.
void maj_plateau(pion pos1, pion pos2){

        //  Le pion libère la case qu'il occupait.
        Plateau[pos1.y][pos1.x] = 1;
        //  Le pion occupe la case libre
        Plateau[pos2.y][pos2.x] = 2;

}





                        ////////////////////////////////////////
                        //  Masque passé en variable globale  //
                        ////////////////////////////////////////

// On initialise le masque, au début de la partie le masque et l'échéquié son identique.

void init_masque (){

    int L;
    int C;

    for (L=0; L <= 16; L++){

        for (C=0; C <= 24; C++ ){

            masque[L][C] = Plateau[L][C] ;
        }
    }
}





                        ////////////////////////////////////////
                        //  Tableau de l'affichage passé en   //
                        //          variable globale          //
                        ////////////////////////////////////////




//  On remplie le tableau qui nous premettra d'avoir un affichage graphique appréciable, par des espaces.

void initialisation_plateau (){

    int L;
    int C;

//  On aura besoin de deux fois plus de lignes et de colonnes pour que notre affichage soit bien aligné.
    for (L = 0; L< 34; L++){

        for (C=0; C< 50; C++){

             Plateau_graph[L][C] =' ';
        }
    }
}

//  On rempli notre tableau graphique en utlisant les informations contenus par le Masque.
//  L'affichage graphique fonctionne ainsi :
//              1.  Représentation des cases, pour toutes les lignes et colones paires :
//                  ->  Si on a un '1' alors on le remplace par un 'O' (on a une case).
//                  ->  Sinon on met un espace ' '.
//
//              2.  On relie les cases. Une fois que l'on a repéré les cases, on relie les cases avec des / et \ dans les lignes et colonnes impaires :
//                  -> Si on a un 'O' dans :
//                          ¤ la case au-dessus à droite, et en-dessous à gauche on met un '/' .
//                          ¤ la case au-dessus à gauche, et en-dessous à droite on met un '\' .
//                  -> Si on a un 'o' dans la case on met un espace ' '.

void create_affich(){

    int C;
    int L;

//  On associe à chaque 1 du masque 'O'.
//  On associe à chaque 0 du masque 'o'.

    //  On rempli les lignes du la grille graphique.
    //  Recopie on ne fait la ttraduction du masque que sur les lignes et colonnes pairs.
    for (L = 0; L< 33; L+=2){

        // On rempli les colonnes de la grille graphique.
        for(C=0; C< 49; C+=2) {

            //  Si on a une case (représenté par un 1 dans le maque).
            if( masque[L/2][C/2] == 1 ) {

                    Plateau_graph[L][C] ='O'; //  On affichera un 'O'
                    if (Plateau_graph[L][C-4] == 'O'){

                        Plateau_graph[L][C-2] ='-';
                    }

            }
            //  Sinon c'est que c'est un case vide (représenté par un 0 dans les masque).
            else{

                    Plateau_graph[L][C] =' '; //  On affichera un ' '.
                }


        }

    }

//  On créer les liaisons entre les cases de notre damier
/*  sur les lignes et colonnes impairs on lie les cases par des / et \  */
    for (L = 1; L< 33; L= L+2){

        for( C=1; C< 49; C+=2) {

            //  Si la case en au-dessus à droite est une case et que celle en-dessous à gauche aussi.
;            if ( ( Plateau_graph[L-1][C+1] == 'O' ) && ( Plateau_graph[L+1][C-1]=='O' ) ){

                Plateau_graph [L][C] = '/';
            }

            // si la case en au-dessus à gauche est une case et que celle en-dessous à droite aussi.
            if ( ( Plateau_graph[L-1][C-1] == 'O' ) && ( Plateau_graph[L+1][C+1] == 'O' ) ){

                Plateau_graph [L][C] ='\\' ;
            }



        }
    }
}








                        ////////////////////////////////////////
                        //  Place les pions selon leur couleur//
                        //          pour l'affichage          //
                        ////////////////////////////////////////

void plateau_en_cours(Groupe_pions nom){

    int C;
    //  On parcours tous les pions d'une couleur.
    for (C=0; C<=9; C++){

        //  On remplace le 'O' par la première la couleur du groupe nom.
        Plateau_graph[2*nom.pos_pions[C].y][2*(nom.pos_pions[C].x)] = nom.couleur;

        //  On met des 2 là où sont les pions.
        Plateau[nom.pos_pions[C].y][nom.pos_pions[C].x] = 2;

    }

}


                        ////////////////////////////////////////
                        //                                    //
                        //     Affichage du plateau de jeu    //
                        //                                    //
                        ////////////////////////////////////////


void affiche(int n, HANDLE h, WORD wOldColorAttrs){

    //  On stocke dans un tableau toutes les couleurs : Rouge, Bleu, Jaune, Blanc, Cyan, Vert.
    int affichage_couleur[6] = {FOREGROUND_RED, FOREGROUND_BLUE,FOREGROUND_GREEN+FOREGROUND_RED,FOREGROUND_GREEN+FOREGROUND_RED+FOREGROUND_BLUE ,FOREGROUND_GREEN + FOREGROUND_BLUE,FOREGROUND_GREEN };
    int i;
    int L;
    int C;
    int k;
    int ok;
    int groupe;     //  Variable pour reconnaitre dans quel groupe on est.
    char num_pion;
    int color;     //   Couleur à afficher.

//  On parcours l'ensemble de notre tableau d'affichage.
//  Pour chaque case on regarde si le caratère rencontré correspond à la couleur d'un des groupes :
//              ->  On regarde pour un groupe du joueur 1 et pour le groupe opposé du joueur 2 :
//                      ¤ Si la couleur correspond à un des groupes:
//                                -  On mémorise le numéreau du groupe
//                                -  On dit quelle est la couleur à afficher.
//                                -  On affiche le numéro du pion (1 à 9, et A si c'est le pion 10), dans la couleur sélectionnée plutôt.
//                      ¤ Sinon on affiche normalement le tableau.
    //  Parcours du tableau Plateau_graph.
    for (L = 0; L<34; L++){

        for (C=0; C<50; C++){

            groupe = 6; //  Si groupe reste à 6 c'est que l'on est pas sur une case avec un pion (de quelque couleur soit il).

            //  Pour l'ensemble des groupes de pions par joueur.
            for(i=0; i<=n-1; i++){

                // Si on recontre un pion du joueur 1
                if (Plateau_graph[L][C] == Groupe[i].couleur){

                    //  On précise la couleur qui sera affichée.
                    color = affichage_couleur[i];

                    //  On mémorise de quel groupe de pion il s'agit.
                    groupe = i;
                }

                //  Si on rencontre un pion du joueur 2
                if (Plateau_graph[L][C] == Groupe[5-i].couleur){

                    //  On précise la couleur qui sera affichée.
                    color = affichage_couleur[5-i];

                    //  On mémorise de quel groupe de pion il s'agit.
                    groupe = 5-i;
                }

            }

            //  Si on a rencontré un pion.
            if (groupe !=6){

                //  On change la couleur de l'affichage
                SetConsoleTextAttribute ( h,  color | FOREGROUND_INTENSITY );

                ok = 0;
                //  On interroge les 10 pions du groupe rencontré pour savoir du quel il s'agit (on s'arrête dès qu'on la trouvé).
                for(k=0; (k<=9)&&(!ok); k++){

                    //  Si les coordonnées correspondes au pion [k].
                    if((Groupe[groupe].pos_pions[k].x == C/2)&&(Groupe[groupe].pos_pions[k].y == L/2)){

                        //  On met le test à vrai pour dire qu'on l'a bien trouvé.
                        ok = 1;

                        //  On met en caractère le numéro de ce pion ( 0 -> '1'; 1 -> '2' etc...).
                        num_pion = k+1;

                        //  On est sur des caractère on ne peut donc pas afficher des chaînes de caractères, on remplace donc le 10 par un 'A'.
                        if((k+1 )==10){

                            printf("%c", 'A' );
                        }
                        else

                            printf("%d", num_pion );
                    }

                }
            //  Si on a pas rencontré de pion.
            }else{
                //  On affiche normalement le caractère.
                printf("%c", Plateau_graph[L][C]);
            }

            //   On remet la coleur d'écriture de la console à celle par défaut.
            SetConsoleTextAttribute ( h, wOldColorAttrs);

        }

        //  On passe à la ligne suivante du tableau.
        printf("\n");
    }
}





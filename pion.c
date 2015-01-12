#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>
#include "affichage.h"

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
    

                            ////////////////////////////////////////
                            //          Label de 0 à 5            //
                            ////////////////////////////////////////


    // On stocke dans un tableur appelé label, pour chaque label, les coordonnées des branhes.
    pion label[6][10] = {
        { {12,0}, {11,1}, {13,1}, {10,2}, {12,2}, {14,2}, {9,3}, {11,3}, {13,3}, {15,3} },           //  Label 0
        { {24,4}, {22,4}, {23,5}, {20,4}, {21,5}, {22,6}, {18,4}, {19,5}, {20,6}, {21,7}},          //  Label 1
        { {24,12}, {23,11}, {22,12}, {22,10}, {21,11}, {20,12}, {21,9}, {20,10}, {19,11}, {18,12} }, //  Label 2
        { {0,4}, {2,4}, {1,5}, {4,4}, {3,5}, {2,6}, {6,4}, {5,5}, {4,6}, {3,7}},                     //  Label 3
        { {0,12}, {1,11}, {2,12}, {2,10}, {3,11}, {4,12} , {3,9}, {4,10}, {5,11}, {6,12} },           //  Label 4
        { {12,16}, {11,15}, {13,15}, {10,14}, {12,14}, {14,14}, {9,13}, {11,13}, {13,13}, {15,13} }  //  Label 5
    };

    Groupe_pions Groupe[6];

    //  On met dans un tableau toutes les couleurs possible du jeu
    //  (les trois premières seront les couleurs du premier joueur, et les trois dernières celle du second joueur).
    char couleurs[6] = {'R', 'B', 'J', 'W', 'C', 'V'};



    //  Saut par dessus un pion.
pion saut(pion dep, pion obstacle){

    // coordonnées retournées par la fonction, représentent celles du pion après un saut.
    pion arrivee;
    arrivee.x = 2*(obstacle.x) - (dep.x);
    arrivee.y = 2*(obstacle.y) - (dep.y);
    return arrivee;

}

//  On regarde si le déplacement ce fait sur une case vide (on ne regardera pas dans cette fonction si on est dans une branche valide).
//      ->  Si la case est vide (Plateau [i][j] == 1) : On retourne les coordonnées de cette case.
//      ->  Si la case est occupée par un pion (Plateau [i][j] == 2) : On effectue un saut par dessus ce pion et on regarde si la case est vide :
//                                             ¤ Si elle est vide : on retourne les coordonnées de cette case.
//                                             ¤ Sinon on retourne le pion origine ({0,0}) qui siginifie que cette direction n'est pas possible pour ce pion.

pion Deplacement_test (pion pos_pion, pion dir){

    pion pos_pion2; //  Variable retournée si on doit faire un saut et qu'on y arrive.

    //  Si la direction testé est une case vide.
    if(Plateau[dir.y][dir.x] == 1){

        return dir;  // Alors c'est un coup possible, on retourne ces coordonnées.
    } else{
    //  Sinon c'est que la case est occupée (Plateau[dir.y][dir.x] == 2).


        //  On fait un saut par dessus la case occupée.
        pos_pion2 = saut(pos_pion, dir);

        if ( (pos_pion2.x >=0) && (pos_pion2.y >=0)&& (pos_pion2.x <=16) && (pos_pion2.y <= 24) ){
            //  On regarde si cette nouvelle case est occupée.
            if(Plateau[pos_pion2.y][pos_pion2.x] == 1){

                return pos_pion2 ;  // Alors c'est un coup possible, on retourne ces coordonnées.
            }
        }
    }

     //  Si les deux cases sont occupées on retourne les coordonnées de l'origine.
    pos_pion2.x = 0;
    pos_pion2.y = 0;
    return pos_pion2;
}


//  Pour qu'un coup soit valide, il faut que le pion ne se trouve pas dans une branche qui lui est interdite.
//          -> On va tester les 4 branches qui sont interdites pour un groupe.
//          -> On ne s'intéressera qu'au 7 dernières coordonnées des branches (les seuls où un pion peut se rendre).
//          -> On s'arrête dès qu'une coordonnées correspond.
//          -> On retourne:
//                  ¤ 0 si on n'est pas dans une branche interdite.
//                  ¤ 1 si on est pas dans une branche interdite.
int test_branche(pion coord, int num_groupe){
    int i,k;
    int ok; //  Booléen qui ce met à vrai dès que deux coordonnées correspondentes.

    //  On parcours les 6 branches tant qu'aucunes coordonnées ne correspond

        ok = 0;
        for(i=0 ; (i<=5) && (!ok) ; i++){

            //  On ne parcours que les 4 branches interdites.
            if  (( i!=num_groupe ) && ( i != 5-num_groupe  ) ) {

                //  On s'intéresse au 7 dernières coordonnées des branches.
                for ( k=3; (k<=9) && (!ok) ; k++ ){

                    //  Si le pion se trouve dans une branche interdite.
                    if( ( coord.x == label[i][k].x) && ( coord.y == label[i][k].y) ){

                        ok = 1; //  On renvoit qu'on est dans une situation interdite.
                    }
                }

            }

        }

    return ok;
}


void coup_possible_pion(int i,  int k){
    int j;
    pion dir;
    pion null;
    null.x = 0;
    null.y = 0;

    for ( j=0; j<= 5 ; j++){

        Groupe[i].coup_possible[k][j] = null;
    }

    for ( j=0; j<=3; j++ ){

        dir.x = Groupe[i].pos_pions[k].x +pow((-1),(j%2));
        dir.y = Groupe[i].pos_pions[k].y + pow((-1),(j/2));
        if ( (dir.x >=0) && (dir.y >= 0) && (dir.x <=16) && (dir.y <= 24)){
            if ( Plateau[dir.y][dir.x]!= 0){
                Groupe[i].coup_possible[k][j] = Deplacement_test( Groupe[i].pos_pions[k], dir);

                if(test_branche ( Groupe[i].coup_possible[k][j], i)  ){
                    Groupe[i].coup_possible[k][j] = null;
                }
            }
        }
        if (j >= 2){

            dir.x = Groupe[i].pos_pions[k].x +2*pow((-1),j);
            dir.y = Groupe[i].pos_pions[k].y;

            if ( (dir.x >=0) && (dir.y >= 0) && (dir.x <=16) && (dir.y <= 24)){
                if ( Plateau[dir.y][dir.x]!= 0){
                    Groupe[i].coup_possible[k][j+2] = Deplacement_test( Groupe[i].pos_pions[k], dir);


                    if(test_branche ( Groupe[i].coup_possible[k][j+2], i)  ){
                         Groupe[i].coup_possible[k][j+2] = null;
                    }
                }
            }
        }
    }
}


//  On va trier le tableau de coup possible pour un pion, pour mettre à la fin tous les coups à {0,0} (qui représent une case de tableau vide.

void Tri_coups (int i, int k){

    pion echange;
    pion null;
    null.x = 0;
    null.y = 0;
    int j,l;

    for (j = 0; j<= 3; j++){

        for(l=0 ; l<=4; l++){

            if ((Groupe[i].coup_possible[k][l].x == null.x)&&(Groupe[i].coup_possible[k][l].y == null.y)){

                echange = Groupe[i].coup_possible[k][l+1];
                Groupe[i].coup_possible[k][l+1] = null;
                Groupe[i].coup_possible[k][l] = echange;
            }
        }
    }
}

void jouer (int i, int k, pion arri){

    //  On met à jour le plateau contenant l'emplacement des pions.
    //  On met à 2 la nouvelle position.
    Plateau[arri.y][arri.x] = 2;

    //  On met à 1 l'ancienne position.
    Plateau[Groupe[i].pos_pions[k].y][Groupe[i].pos_pions[k].x] = 1;


    printf("{%i, %i}\n", Groupe[i].pos_pions[k].x, Groupe[i].pos_pions[k].y );
    
    Plateau_graph[2*Groupe[i].pos_pions[k].y][2*Groupe[i].pos_pions[k].x] = 'O';
    Plateau_graph[2*arri.y][2*arri.x] = Groupe[i].couleur;
    Groupe[i].pos_pions[k].y = arri.y;
    Groupe[i].pos_pions[k].x = arri.x;

}

int Deplacement (char coul, int num_pion){
    int i=0;
    int num_groupe;
    int ok = 0;
    int coup;
    pion null;
    null.x = 0;
    null.y = 0;
    for (i=0; i<=5; i++){

        if (coul == couleurs[i]){
            num_groupe = i;
            ok = 1;
        }
    }
    if (!ok){return 0;}

    coup_possible_pion(num_groupe, num_pion);
    Tri_coups(num_groupe, num_pion);
    i=0;
    while((i <=5)&& ((Groupe[num_groupe].coup_possible[num_pion][i].x!= null.x)||(Groupe[num_groupe].coup_possible[num_pion][i].y!= null.y))){

        printf ("Coup[%i] : {%i , %i} \n",i, Groupe[num_groupe].coup_possible[num_pion][i].x, Groupe[num_groupe].coup_possible[num_pion][i].y);
        i++;
    }
    coup = -1;
    if((Groupe[num_groupe].coup_possible[num_pion][0].x == null.x)&&(Groupe[num_groupe].coup_possible[num_pion][0].y== null.y)){
        return 0;
       }

    while ( ! ( (coup>=0)&&(coup<=i-1)) ){
        printf ("Coup à jouer : ");
        scanf ("%i", &coup);
       
    }


    jouer(num_groupe , num_pion, Groupe[num_groupe].coup_possible[num_pion][coup] );
    return ok;
}


//  Fonction qui nous permettra de placer les pions dans leur branche de départ : On initalise le plateau de départ.
void init_pions_label(int num_couleur){

    int j;
    //  On créer les 10 pions d'un groupe.
    for (j=0; j<=9; j++){

        //  Dont on initialise les coordonnées au label correspondant.
        Groupe[num_couleur].pos_pions[j] = label[num_couleur][j];

        //  On initialise aussi les 10 cases des 2 labels d'un groupe.

        //  Label[0] : correspond à la branche de départ.
        Groupe[num_couleur].label[0][j] = label[num_couleur][j];

        //  Lable[1] : correspond à la branche d'arrivée. (on a toujours labe[0] + label[1]).
        Groupe[num_couleur].label[1][j] = label[5-num_couleur][j];

        }
}


//  On créer un tableau qui contient l'ensemble des groupes :
//                         ->  On va lier tout les tableaux construit.
//                         ->  Le tableau de couleur permet d'initialiser la couleur (ainsi on peut dire que le groupe Groupe[0] sont les pions Rouges).
//                         ->  La tableau de point label initialie les pions dans leur branche de départ (Donc les pions rouges sont dans la brache de label 0).
//                         ->  Et permet aussi d'initialiser les deux branches autorisées pour une couleur ( pour les rouges : Label[0] = 0 et Label[1] = 5).

void init_groupes (int n){

    int i;

    //  n étant le nombre de groupe par joueur, on initialise le nombre de groupe nécessaire.
    for(i = 0; i < n; i++){

        // Pour le joueur 1 :
        //          ->  On lui met les couleurs Rouge / Bleu / Jaune (si n= 2 : Rouge et Bleu etc...).
        Groupe[i].couleur = couleurs[i];
        //          ->  On initialise les positions des pions dans leur branche, ainsi que les deux braches autorisées, pour le Groupe[i].
        init_pions_label(i);
        //          ->  La coordonnée du sommet à atteindre (on aura mis ces coordonnées en premier dans chaque label).
        Groupe[i].sommet= label[i][0];

        // Pour le joueur 2 :
        //          ->  On lui met les couleurs Gris / Orange / Vert (si n= 2 : Gris et Orange etc...).
        Groupe[5-i].couleur = couleurs[5-i];
        //          ->  On initialise les positions des pions dans leur branche, ainsi que les deux braches autorisées, pour le Groupe[i].
        init_pions_label(5-i);
        //          ->  La coordonnée du sommet à atteindre (on aura mis ces coordonnées en premier dans chaque label).
        Groupe[5-i].sommet= label[5-i][0];

    }

}

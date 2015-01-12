#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>
#include "affichage.h"
#include "pion.h"

typedef struct{
    int tour;
    int type ;
    int *mes_groupes;
}   joueur;


int main() {
    int tour =0;
    int mode=0 ;
    int profondeur = 0;
    int n=4;
    joueur J1;
    joueur J2;

    int continu = 1;
    int ok = 0;
    int suite;
    int i,L,C;
    char couleur_entre;
    int num_pion;


    /* Choix du mode de Jeux */
    while ((mode !=1) && (mode!=2)){
        while(getchar()!='\n');  //On vide le buffer
        printf (" Choississez le mode de jeux :\n");
        printf (" Mode 1 : Joueur vs Jouer\n");
        printf (" Mode 2 : Joueur vs IA\n");
        printf (" Mode : ");
        scanf ( "%i", &mode);
    }

    /* Choix du nombre de couleur */
    while ( (n<1) || (n>3 ) ){
        printf("Combien de couleur par joueur ? : ");
        scanf ("%i", &n);

    }


    /* Choix de la difficulté de l'IA */
    if (mode == 2){
        while ( (profondeur<1) || (profondeur>3 )){
            printf(" Choisir la difficulté (entre 1 et 3) : ");
            scanf ("%i", &profondeur );
        }
    }

    /*Initialisation des joueurs*/
    if (mode == 1){
        J1.type = 0;
    }else{
        J1.type = 1;
    }
    J1.tour = 0;
    J2.tour = 1;
    J2.type = 1;

    /*Initilisation des groupes*/
    J1.mes_groupes = (int*) malloc (n*sizeof(int));
    J2.mes_groupes = (int*) malloc (n*sizeof(int));
    for(i=0; i<=n-1; i++){
       J1.mes_groupes[i] = i;
       J2.mes_groupes[i] = 5-i;
    }


    /* Changement de couleur Application Console */

        HANDLE h = GetStdHandle ( STD_OUTPUT_HANDLE ); //->whats this for? cant it be changed?
        WORD wOldColorAttrs; //->whats this for? cant it be changed?
        CONSOLE_SCREEN_BUFFER_INFO csbiInfo; //->whats this for? cant it be changed?
        GetConsoleScreenBufferInfo(h, &csbiInfo);
        wOldColorAttrs = csbiInfo.wAttributes;


    create_plateau();
    init_masque();
    initialisation_plateau();
    create_affich();
    init_groupes(n);

    //  Initialisation des pions.
    for(i=0; i<= n-1; i++){
        plateau_en_cours(Groupe[i]);
        plateau_en_cours(Groupe[5-i]);
    }


    

    affiche(n, h, wOldColorAttrs);

    tour =0;
    while (continu == 1){
        if( ( J1.tour == tour%2) && (J1.type == 1 )){

            IA_jouer( profondeur, J1.mes_groupes, n);
            printf ("%i\n", tour);
        }else if( ( J2.tour == tour%2) && (J2.type == 1 )){
             IA_jouer( profondeur, J2.mes_groupes, n);
             printf ("%i\n", tour);
        
        }else{

            ok =0;
            suite = 0;
            while (!ok){

                while(getchar()!='\n');  //On vide le buffer
                printf ("couleur du pion a jouer :");
                scanf ("%c", &couleur_entre);

                /* On regarde si la couleur entrée est bien une des couleur du joueur*/
                for (i=0; i<= n-1; i++){

                    /* Si c'est le tour du joueur 1 */
                    if( J1.tour == tour%2){

                        //  Si on a bien une des couleurs du joueur.
                        if(couleurs[i] == couleur_entre){
                          suite = 1;  
                        }
                    //   Si c'est le tour du joueur 2.    
                    }else{
                        if(couleurs[5-i] == couleur_entre){
                          suite = 1;  
                        }
                    }
                }
                //  Si la couleur saisie est bien une des couleurs du joueur.
                if(suite){
                    printf ("numero du pion a jouer :");
                    scanf ("%i", &num_pion);
                    printf("couleur : %c, numero : %i \n", couleur_entre, num_pion);

                    ok = Deplacement(couleur_entre, num_pion-1);
                }

            }
        }
         //  Affichage du Plateau
        //for(L=0; L < 17; L++){

        //    for (C=0 ; C < 25; C++){
        //        printf("%i",Plateau[L][C]);
        //    }
        //    printf("\n");

        //}
        //system("cls");
         // Affichage du Plateau
   
        affiche(n, h, wOldColorAttrs);
        printf ("Continuer ? :");
        scanf ("%i", &continu);
        printf("\n");
        tour ++;

    }
    return 0;
}

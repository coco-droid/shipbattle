#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "../headers/ai.h"

void afficherShooting(int matrice[4][2]) {
    // Affiche les tirs réalisés
    for (int i = 0; i < 4; i++) {
        printf("Tir %d: (x:%2d, y:%2d)\n", i + 1, matrice[i][0], matrice[i][1]);
    }
}

int ajouter_coordonnees(int return_matrice[][2], int index, int x, int y) {
    if (x >= 0 && x < 10 && y >= 0 && y < 10) {
        return_matrice[index][0] = x;
        return_matrice[index][1] = y;
        return 1; // Indique qu'une coordonnée valide a été ajoutée
    }
    return 0; // Coordonnée invalide, non ajoutée
}

int** pnj_shoot(int matrice[10][10]) {
    printf("Début de l'IA de tir.\n");
    static int return_matrice[4][2];
    int cases_toucher[2];
    int i = 0;
    bool soti = false;

    // Initialiser return_matrice avec des valeurs invalides
    for(int k = 0; k < 4; k++) {
        return_matrice[k][0] = -1;
        return_matrice[k][1] = -1;
    }

    // Recherche de la cellule touchée dans la matrice
    while (!soti && i < 10) {
        int j = 0;
        while (j < 10) {
            if (matrice[i][j] == 2) { // Cellule touchée trouvée
                soti = true;
                cases_toucher[0] = i;
                cases_toucher[1] = j;
                break;
            }
            j++;
        }
        if (soti) break;
        i++;
    }

    // Tir
    if (soti) {
        printf("\nCellule TOUCHE: (%d, %d)", cases_toucher[0], cases_toucher[1]);
        int x = cases_toucher[0];
        int y = cases_toucher[1];
        int cases2_matrice[2];
        bool adjacent_found = false;

        // Vérifie les cellules adjacentes
        if (x > 0 && matrice[x-1][y] == 2) {
            cases2_matrice[0] = x-1;
            cases2_matrice[1] = y;
            adjacent_found = true;
        }
        if (x < 9 && matrice[x+1][y] == 2) {
            cases2_matrice[0] = x+1;
            cases2_matrice[1] = y;
            adjacent_found = true;
        }
        if (y > 0 && matrice[x][y-1] == 2) {
            cases2_matrice[0] = x;
            cases2_matrice[1] = y-1;
            adjacent_found = true;
        }
        if (y < 9 && matrice[x][y+1] == 2) {
            cases2_matrice[0] = x;
            cases2_matrice[1] = y+1;
            adjacent_found = true;
        }

        // Générer les tirs en fonction des cases adjacentes trouvées
        if (!adjacent_found) {
            printf("\nAucune case adjacente détectée, tir aléatoire.");
            int rand_x = cases_toucher[0];
            int rand_y = cases_toucher[1];
            int index = 0;

            // Ajouter les cellules adjacentes valides
            if (rand_x + 1 < 10 && index < 4) {
                return_matrice[index][0] = rand_x + 1;
                return_matrice[index][1] = rand_y;
                index++;
            }
            if (rand_x - 1 >= 0 && index < 4) {
                return_matrice[index][0] = rand_x - 1;
                return_matrice[index][1] = rand_y;
                index++;
            }
            if (rand_y + 1 < 10 && index < 4) {
                return_matrice[index][0] = rand_x;
                return_matrice[index][1] = rand_y + 1;
                index++;
            }
            if (rand_y - 1 >= 0 && index < 4) {
                return_matrice[index][0] = rand_x;
                return_matrice[index][1] = rand_y - 1;
                index++;
            }
        } else {
            printf("\nCellule ADJACENTE: (%d, %d)", cases2_matrice[0], cases2_matrice[1]);
            int index = 0;

            // Alignement horizontal
            if (cases2_matrice[0] == cases_toucher[0]) {
                if (cases2_matrice[1] > cases_toucher[1]) {
                    // Tir à droite
                    if (y + 1 < 10 && index < 4) {
                        return_matrice[index][0] = x;
                        return_matrice[index][1] = y + 1;
                        index++;
                    }
                    if (y + 2 < 10 && index < 4) {
                        return_matrice[index][0] = x;
                        return_matrice[index][1] = y + 2;
                        index++;
                    }
                    if (y - 1 >= 0 && index < 4) {
                        return_matrice[index][0] = x;
                        return_matrice[index][1] = y - 1;
                        index++;
                    }
                    if (y - 2 >= 0 && index < 4) {
                        return_matrice[index][0] = x;
                        return_matrice[index][1] = y - 2;
                        index++;
                    }
                } else {
                    // Tir à gauche
                    if (y - 1 >= 0 && index < 4) {
                        return_matrice[index][0] = x;
                        return_matrice[index][1] = y - 1;
                        index++;
                    }
                    if (y - 2 >= 0 && index < 4) {
                        return_matrice[index][0] = x;
                        return_matrice[index][1] = y - 2;
                        index++;
                    }
                    if (y + 1 < 10 && index < 4) {
                        return_matrice[index][0] = x;
                        return_matrice[index][1] = y + 1;
                        index++;
                    }
                    if (y + 2 < 10 && index < 4) {
                        return_matrice[index][0] = x;
                        return_matrice[index][1] = y + 2;
                        index++;
                    }
                }
            }
            // Alignement vertical
            else {
                if (cases2_matrice[0] > cases_toucher[0]) {
                    // Tir vers le bas
                    if (x + 1 < 10 && index < 4) {
                        return_matrice[index][0] = x + 1;
                        return_matrice[index][1] = y;
                        index++;
                    }
                    if (x + 2 < 10 && index < 4) {
                        return_matrice[index][0] = x + 2;
                        return_matrice[index][1] = y;
                        index++;
                    }
                    if (x - 1 >= 0 && index < 4) {
                        return_matrice[index][0] = x - 1;
                        return_matrice[index][1] = y;
                        index++;
                    }
                    if (x - 2 >= 0 && index < 4) {
                        return_matrice[index][0] = x - 2;
                        return_matrice[index][1] = y;
                        index++;
                    }
                } else {
                    // Tir vers le haut
                    if (x - 1 >= 0 && index < 4) {
                        return_matrice[index][0] = x - 1;
                        return_matrice[index][1] = y;
                        index++;
                    }
                    if (x - 2 >= 0 && index < 4) {
                        return_matrice[index][0] = x - 2;
                        return_matrice[index][1] = y;
                        index++;
                    }
                    if (x + 1 < 10 && index < 4) {
                        return_matrice[index][0] = x + 1;
                        return_matrice[index][1] = y;
                        index++;
                    }
                    if (x + 2 < 10 && index < 4) {
                        return_matrice[index][0] = x + 2;
                        return_matrice[index][1] = y;
                        index++;
                    }
                }
            }
        }
    } else {
        // Tir aléatoire
        printf("\nAucune cellule touchée détectée, tir aléatoire.");
        int index = 0;
        bool added[4] = {false, false, false, false}; // Pour éviter les doublons

        while(index < 4) {
            int x = rand() % 9;
            int y = rand() % 9;

            // Vérifier que la cellule n'a pas déjà été ajoutée
            bool duplicate = false;
            for(int m = 0; m < index; m++) {
                if(return_matrice[m][0] == x && return_matrice[m][1] == y) {
                    duplicate = true;
                    break;
                }
            }

            if(!duplicate) {
                return_matrice[index][0] = x;
                return_matrice[index][1] = y;
                index++;
            }
        }
    }

    /*Afficher les coordonnées retournées
    printf("\nCoordonnées de tir sélectionnées :\n");
    for(int k = 0; k < 4; k++) {
        if(return_matrice[k][0] != -1 && return_matrice[k][1] != -1) {
            printf("(%d, %d) ", return_matrice[k][0], return_matrice[k][1]);
        }
    }
    printf("\n");*/

    return (int**)return_matrice;
}
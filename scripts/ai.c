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

int** pnj_shoot(int matrice[10][10]) {
    static int return_matrice[4][2];
    int cases_toucher[2];
    int i = 0;
    bool soti = false;

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
        printf("\n Cellule TOUCHE: %d , %d", cases_toucher[0], cases_toucher[1]);
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
            printf("\n Aucune case adjacente détectée, tir aléatoire.");
            int rand_x = cases_toucher[0];
            int rand_y = cases_toucher[1];

            return_matrice[0][0] = (rand_x + 1 < 10) ? rand_x + 1 : rand_x;
            return_matrice[0][1] = rand_y;

            return_matrice[1][0] = (rand_x - 1 >= 0) ? rand_x - 1 : rand_x;
            return_matrice[1][1] = rand_y;

            return_matrice[2][0] = rand_x;
            return_matrice[2][1] = (rand_y + 1 < 10) ? rand_y + 1 : rand_y;

            return_matrice[3][0] = rand_x;
            return_matrice[3][1] = (rand_y - 1 >= 0) ? rand_y - 1 : rand_y;

        } else {
            printf("\n Cellule ADJACENTE: %d , %d", cases2_matrice[0], cases2_matrice[1]);
            
            if (cases2_matrice[0] == cases_toucher[0]) { 
                // Alignement horizontal
                if (cases2_matrice[1] > cases_toucher[1]) {
                    // Tir à droite
                    return_matrice[0][0] = x;
                    return_matrice[0][1] = (y - 1 >= 0) ? y - 1 : y;

                    return_matrice[1][0] = x;
                    return_matrice[1][1] = (y - 2 >= 0) ? y - 2 : y;

                    return_matrice[2][0] = x;
                    return_matrice[2][1] = (cases2_matrice[1] + 1 < 10) ? cases2_matrice[1] + 1 : cases2_matrice[1];

                    return_matrice[3][0] = x;
                    return_matrice[3][1] = (cases2_matrice[1] + 2 < 10) ? cases2_matrice[1] + 2 : cases2_matrice[1];
                } else {
                    // Tir à gauche
                    return_matrice[0][0] = x;
                    return_matrice[0][1] = (y + 1 < 10) ? y + 1 : y;

                    return_matrice[1][0] = x;
                    return_matrice[1][1] = (y + 2 < 10) ? y + 2 : y;

                    return_matrice[2][0] = x;
                    return_matrice[2][1] = (cases2_matrice[1] - 1 >= 0) ? cases2_matrice[1] - 1 : cases2_matrice[1];

                    return_matrice[3][0] = x;
                    return_matrice[3][1] = (cases2_matrice[1] - 2 >= 0) ? cases2_matrice[1] - 2 : cases2_matrice[1];
                }
            } else {
                // Alignement vertical
                if (cases2_matrice[0] > cases_toucher[0]) {
                    // Tir vers le bas
                    return_matrice[0][0] = (x - 1 >= 0) ? x - 1 : x;
                    return_matrice[0][1] = y;

                    return_matrice[1][0] = (x - 2 >= 0) ? x - 2 : x;
                    return_matrice[1][1] = y;

                    return_matrice[2][0] = (cases2_matrice[0] + 1 < 10) ? cases2_matrice[0] + 1 : cases2_matrice[0];
                    return_matrice[2][1] = y;

                    return_matrice[3][0] = (cases2_matrice[0] + 2 < 10) ? cases2_matrice[0] + 2 : cases2_matrice[0];
                    return_matrice[3][1] = y;
                } else {
                    // Tir vers le haut
                    return_matrice[0][0] = (x + 1 < 10) ? x + 1 : x;
                    return_matrice[0][1] = y;

                    return_matrice[1][0] = (x + 2 < 10) ? x + 2 : x;
                    return_matrice[1][1] = y;

                    return_matrice[2][0] = (cases2_matrice[0] - 1 >= 0) ? cases2_matrice[0] - 1 : cases2_matrice[0];
                    return_matrice[2][1] = y;

                    return_matrice[3][0] = (cases2_matrice[0] - 2 >= 0) ? cases2_matrice[0] - 2 : cases2_matrice[0];
                    return_matrice[3][1] = y;
                }
            }
        }
    } else {
        // Tir aléatoire
        int x = rand() % 10;
        int y = rand() % 10;
        return_matrice[0][0] = x;
        return_matrice[0][1] = y;
        return_matrice[1][0] = (x + 1 < 10) ? x + 1 : x;
        return_matrice[1][1] = y;
        return_matrice[2][0] = x;
        return_matrice[2][1] = (y + 1 < 10) ? y + 1 : y;
        return_matrice[3][0] = (x + 1 < 10) ? x + 1 : x;
        return_matrice[3][1] = (y + 1 < 10) ? y + 1 : y;
    }

    // Marquer les cellules où il y a eu des tirs avec une valeur de 5
    for (int i = 0; i < 4; i++) {
        matrice[return_matrice[i][0]][return_matrice[i][1]] = 5;
    }

    return (int**)return_matrice;
}


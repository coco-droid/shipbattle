#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "../headers/ai.h"
#include "../headers/player.h"
#include <time.h>


#define TAILLE 10  // Taille de la matrice (peut être ajustée)

// Prototypes de fonctions (assurez-vous que ces fonctions sont définies dans votre code)
void afficherShooting(int ai_shoot_m[][2], int size) {
    printf("\nAffichage des tirs:\n");
    for (int i = 0; i < size; i++) {
        printf("Tir %d: (%d, %d)\n", i, ai_shoot_m[i][0], ai_shoot_m[i][1]);
    }
}
// Fonction pour ajouter des coordonnées tout en vérifiant qu'elles ne sont pas répétées
int ajouter_coordonnees(int ai_shoot_m[][2], int index, int x, int y) {
    // Vérifie si les coordonnées sont valides
    if (index >= 0 && index < 4 && x >= 0 && x < TAILLE && y >= 0 && y < TAILLE) {
        // Vérifie si les coordonnées ont déjà été ajoutées
        for (int i = 0; i < index; i++) {
            if (ai_shoot_m[i][0] == x && ai_shoot_m[i][1] == y) {
                return 0; // Les coordonnées ont déjà été ajoutées
            }
        }

        // Ajoute les nouvelles coordonnées
        ai_shoot_m[index][0] = x;
        ai_shoot_m[index][1] = y;
        return 1; // Indique qu'une coordonnée valide a été ajoutée
    }
    return 0; // Coordonnée invalide, non ajoutée
}

// Fonction de tir de l'IA
void pnj_shoot(int matrice[TAILLE][TAILLE]) {
    printf("Début de l'IA de tir.\n");
    int cases_toucher[2] = {-1, -1}; // Initialisation avec des valeurs impossibles pour la matrice
    int index = 0;
    bool soti = false;

    // Recherche de la cellule touchée dans la matrice
    for (int i = 0; i < TAILLE && !soti; i++) {
        for (int j = 0; j < TAILLE; j++) {
            if (matrice[i][j] == 2) { // Cellule touchée trouvée
                soti = true;
                cases_toucher[0] = i;
                cases_toucher[1] = j;
                break;
            }
        }
    }

    if (soti) {
        printf("\nCellule TOUCHE: (%d, %d)", cases_toucher[0], cases_toucher[1]);
        int x = cases_toucher[0];
        int y = cases_toucher[1];
        int cases2_matrice[2];
        bool adjacent_found = false;

        // Vérifie les cellules adjacentes
        if (x > 0 && matrice[x - 1][y] == 2) {
            cases2_matrice[0] = x - 1;
            cases2_matrice[1] = y;
            adjacent_found = true;
        } else if (x < TAILLE - 1 && matrice[x + 1][y] == 2) {
            cases2_matrice[0] = x + 1;
            cases2_matrice[1] = y;
            adjacent_found = true;
        } else if (y > 0 && matrice[x][y - 1] == 2) {
            cases2_matrice[0] = x;
            cases2_matrice[1] = y - 1;
            adjacent_found = true;
        } else if (y < TAILLE - 1 && matrice[x][y + 1] == 2) {
            cases2_matrice[0] = x;
            cases2_matrice[1] = y + 1;
            adjacent_found = true;
        }

        // Générer les tirs en fonction des cases adjacentes trouvées
        if (!adjacent_found) {
            printf("\nAucune case adjacente détectée, tir aléatoire autour de la case touchée.\n");
            if (ajouter_coordonnees(ai_shoot_m, index++, x + 1, y)) {}
            if (ajouter_coordonnees(ai_shoot_m, index++, x - 1, y)) {}
            if (ajouter_coordonnees(ai_shoot_m, index++, x, y + 1)) {}
            if (ajouter_coordonnees(ai_shoot_m, index++, x, y - 1)) {}
        } else {
            printf("\nCellule ADJACENTE détectée à (%d, %d)", cases2_matrice[0], cases2_matrice[1]);
            if (cases2_matrice[0] == x) {  // Alignement horizontal
                if (ajouter_coordonnees(ai_shoot_m, index++, x, y + 1)) {}
                if (ajouter_coordonnees(ai_shoot_m, index++, x, y - 1)) {}
            } else {  // Alignement vertical
                if (ajouter_coordonnees(ai_shoot_m, index++, x + 1, y)) {}
                if (ajouter_coordonnees(ai_shoot_m, index++, x - 1, y)) {}
            }
        }
    } else {
        // Tir aléatoire si aucune cellule touchée n'est détectée
        printf("\nAucune cellule touchée détectée, tir aléatoire total.\n");
        while (index < 4) {
            int rand_x = rand() % TAILLE;
            int rand_y = rand() % TAILLE;

            // Vérifier que la cellule n'a pas déjà été ajoutée
            bool duplicate = false;
            for (int m = 0; m < index; m++) {
                if (ai_shoot_m[m][0] == rand_x && ai_shoot_m[m][1] == rand_y) {
                    duplicate = true;
                    break;
                }
            }

            if (!duplicate) {
                ai_shoot_m[index][0] = rand_x;
                ai_shoot_m[index][1] = rand_y;
                index++;
            }
        }
    }

    afficherShooting(ai_shoot_m, index);
}

#include "../headers/ai.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Fonction pour afficher les tirs
void afficherShooting(Coordonnees ai_shoot_m[], int size) {
    printf("\nAffichage des tirs :\n");
    for (int i = 0; i < size; i++) {
        printf("Tir %d : (%d, %d)\n", i + 1, ai_shoot_m[i].x, ai_shoot_m[i].y);
    }
}

// Fonction pour ajouter des coordonnées en évitant les répétitions
bool ajouter_coordonnees(Coordonnees ai_shoot_m[], int *index, int x, int y) {
    // Vérifie si les coordonnées sont valides
    if (x >= 0 && x < TAILLE && y >= 0 && y < TAILLE) {
        // Vérifie si les coordonnées ont déjà été ajoutées
        for (int i = 0; i < *index; i++) {
            if (ai_shoot_m[i].x == x && ai_shoot_m[i].y == y) {
                return false; // Les coordonnées ont déjà été ajoutées
            }
        }

        // Ajoute les nouvelles coordonnées
        ai_shoot_m[*index].x = x;
        ai_shoot_m[*index].y = y;
        (*index)++;
        return true; // Indique qu'une coordonnée valide a été ajoutée
    }
    return false; // Coordonnées invalides, non ajoutées
}

// Fonction de tir de l'IA
void pnj_shoot(int matrice[TAILLE][TAILLE], Coordonnees tirs_effectues[], int *nombre_tirs) {
    printf("Début de l'IA de tir.\n");
    int index = 0; // Compteur pour le nombre de tirs effectués
    srand(time(NULL)); // Initialiser le générateur de nombres aléatoires

    // Recherche de cellules touchées non coulées pour cibler les navires
    bool cible_trouvee = false;
    for (int i = 0; i < TAILLE && !cible_trouvee; i++) {
        for (int j = 0; j < TAILLE && !cible_trouvee; j++) {
            if (matrice[i][j] == 2) { // Cellule touchée non coulée trouvée
                printf("IA : J'ai trouvé une partie d'un navire touché en (%d, %d).\n", i, j);

                // Déterminer l'orientation du navire
                int dx[] = { -1, 1, 0, 0 };
                int dy[] = { 0, 0, -1, 1 };
                int orientation = -1; // 0: vertical, 1: horizontal

                // Vérifier si le navire est aligné verticalement ou horizontalement
                for (int dir = 0; dir < 4; dir++) {
                    int x = i + dx[dir];
                    int y = j + dy[dir];
                    if (x >= 0 && x < TAILLE && y >= 0 && y < TAILLE && matrice[x][y] == 2) {
                        if (dx[dir] != 0) {
                            orientation = 0; // Vertical
                            printf("IA : Le navire semble être orienté verticalement.\n");
                        } else {
                            orientation = 1; // Horizontal
                            printf("IA : Le navire semble être orienté horizontalement.\n");
                        }
                        break;
                    }
                }

                // Si aucune orientation n'est déterminée, essayer les deux
                if (orientation == -1) {
                    printf("IA : Orientation inconnue, je vais tester les deux directions.\n");
                    orientation = 2; // Inconnu
                }

                // Tirer en fonction de l'orientation
                if (orientation == 0 || orientation == 2) { // Vertical ou inconnu
                    // Vers le haut
                    int x = i - 1;
                    while (x >= 0 && matrice[x][j] == 2) {
                        x--;
                    }
                    if (x >= 0 && matrice[x][j] == 0 && index < 4) {
                        printf("IA : Je vais tirer en (%d, %d) vers le haut.\n", x, j);
                        ajouter_coordonnees(tirs_effectues, &index, x, j);
                    }

                    // Vers le bas
                    x = i + 1;
                    while (x < TAILLE && matrice[x][j] == 2) {
                        x++;
                    }
                    if (x < TAILLE && matrice[x][j] == 0 && index < 4) {
                        printf("IA : Je vais tirer en (%d, %d) vers le bas.\n", x, j);
                        ajouter_coordonnees(tirs_effectues, &index, x, j);
                    }
                }

                if (orientation == 1 || orientation == 2) { // Horizontal ou inconnu
                    // Vers la gauche
                    int y = j - 1;
                    while (y >= 0 && matrice[i][y] == 2) {
                        y--;
                    }
                    if (y >= 0 && matrice[i][y] == 0 && index < 4) {
                        printf("IA : Je vais tirer en (%d, %d) vers la gauche.\n", i, y);
                        ajouter_coordonnees(tirs_effectues, &index, i, y);
                    }

                    // Vers la droite
                    y = j + 1;
                    while (y < TAILLE && matrice[i][y] == 2) {
                        y++;
                    }
                    if (y < TAILLE && matrice[i][y] == 0 && index < 4) {
                        printf("IA : Je vais tirer en (%d, %d) vers la droite.\n", i, y);
                        ajouter_coordonnees(tirs_effectues, &index, i, y);
                    }
                }

                cible_trouvee = true;
            }
        }
    }

    // Si aucune cible spécifique n'est trouvée, mode chasse
    if (!cible_trouvee) {
        printf("IA : Aucune cible prioritaire trouvée, je passe en mode chasse.\n");

        // Stratégie de parité pour le mode chasse
        Coordonnees cellules_possibles[TAILLE * TAILLE];
        int nb_cellules = 0;

        for (int i = 0; i < TAILLE; i++) {
            for (int j = 0; j < TAILLE; j++) {
                if (matrice[i][j] == 0) {
                    // Stratégie de parité
                    if ((i + j) % 2 == 0) {
                        cellules_possibles[nb_cellules++] = (Coordonnees){ i, j };
                    }
                }
            }
        }

        // Mélanger les cellules possibles
        for (int i = nb_cellules - 1; i > 0; i--) {
            int j = rand() % (i + 1);
            Coordonnees temp = cellules_possibles[i];
            cellules_possibles[i] = cellules_possibles[j];
            cellules_possibles[j] = temp;
        }

        // Ajouter les tirs
        int idx = 0;
        while (index < 4 && idx < nb_cellules) {
            Coordonnees cible = cellules_possibles[idx++];
            printf("IA : Je vais tirer en (%d, %d) en mode chasse.\n", cible.x, cible.y);
            ajouter_coordonnees(tirs_effectues, &index, cible.x, cible.y);
        }
    }

    // Compléter les tirs restants si nécessaire
    if (index < 4) {
        for (int i = 0; i < TAILLE && index < 4; i++) {
            for (int j = 0; j < TAILLE && index < 4; j++) {
                if (matrice[i][j] == 0) {
                    printf("IA : Je vais tirer en (%d, %d) pour compléter mes tirs.\n", i, j);
                    ajouter_coordonnees(tirs_effectues, &index, i, j);
                }
            }
        }
    }

    *nombre_tirs = index; // Mettre à jour le nombre de tirs effectués

    // Afficher les tirs effectués
    afficherShooting(tirs_effectues, *nombre_tirs);
}

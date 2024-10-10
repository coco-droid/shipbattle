#include "../headers/fleet.h"
#include "../headers/player.h"
Player player_one;
Player player_two;
int* player_one_grid[10][10]={0};
int* player_two_grid[10][10]={0};
int* hint_player_one_grid[10][10]={0};
int* hint_player_two_grid[10][10]={0};
int* ai_shoot_m[4][2];
int* who_Played=1;
void initialize_player(struct player* p, const char* name, struct fleet f, const char* type, int id, char ip) {
    strncpy(p->name, name, sizeof(p->name) - 1);
    p->name[sizeof(p->name) - 1] = '\0'; // Ensure null-termination
    p->fleet = f;
    strncpy(p->type, type, sizeof(p->type) - 1);
    p->type[sizeof(p->type) - 1] = '\0'; // Ensure null-termination
    p->id = id;
    p->ip = ip;
    p->health=17;
}

#define GRID_SIZEV 10
#define SHOOT_SIZE 4

void shoot_on_them(int boat_positions[GRID_SIZEV][GRID_SIZEV], int shots_taken[GRID_SIZEV][GRID_SIZEV], int salvos[SHOOT_SIZE][2]) {
    for (int i = 0; i < SHOOT_SIZE; i++) {
        // Les coordonnées de tir sont directement récupérées depuis la matrice salvos
        int x = salvos[i][0];  // Coordonnée x du tir
        int y = salvos[i][1];  // Coordonnée y du tir

        // Vérifie si le tir est dans les limites de la grille
        if (x >= 0 && x < GRID_SIZEV && y >= 0 && y < GRID_SIZEV) {
            if (boat_positions[y][x] > 0) {  // Si un bateau est présent à cet emplacement
                shots_taken[y][x] = 2;  // Marque comme "bateau touché"

                // Vérifie si le bateau est coulé
                int boat_id = boat_positions[y][x];  // Identifiant du bateau
                int is_sunk = 1;  // Par défaut, considère que le bateau est coulé

                // Parcourt la grille pour vérifier si toutes les parties du bateau sont touchées
                for (int r = 0; r < GRID_SIZEV; r++) {
                    for (int c = 0; c < GRID_SIZEV; c++) {
                        if (boat_positions[r][c] == boat_id && shots_taken[r][c] != 2) {
                            is_sunk = 0;  // Le bateau n'est pas encore entièrement coulé
                            break;
                        }
                    }
                    if (!is_sunk) break;
                }

                // Si le bateau est coulé, marque toutes ses cellules comme "3" (bateau coulé)
                if (is_sunk) {
                    for (int r = 0; r < GRID_SIZEV; r++) {
                        for (int c = 0; c < GRID_SIZEV; c++) {
                            if (boat_positions[r][c] == boat_id) {
                                shots_taken[r][c] = 3;  // Bateau coulé
                            }
                        }
                    }
                    printf("Bateau coulé avec l'ID %d\n", boat_id);
                } else {
                    printf("Bateau touché mais pas encore coulé en (%d, %d)\n", x, y);
                }
            } else {
                shots_taken[y][x] = 1;  // Tir manqué (pas de bateau à cet endroit)
                printf("Tir manqué en (%d, %d)\n", x, y);
            }
        } else {
            printf("Les coordonnées (%d, %d) sont hors de la grille\n", x, y);
        }
    }
}

void afficherMatrice(int matrice[10][10]) {
    //system('cls');
    // Afficher la ligne supérieure
    printf("  +");
    for (int i = 0; i < 10; i++) {
        printf("----");
    }
    printf("+\n");

    // Afficher les lignes de la matrice avec des bordures
    for (int i = 0; i < 10; i++) {
        printf("%2d|", i); // Afficher l'index de la ligne
        for (int j = 0; j < 10; j++) {
            printf(" %2d ", matrice[i][j]);
        }
        printf("|\n");
    }

    // Afficher la ligne inférieure
    printf("  +");
    for (int i = 0; i < 10; i++) {
        printf("----");
    }
    printf("+\n");
}
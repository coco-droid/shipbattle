#include <SDL2/SDL.h>
#include "../headers/player.h"
#include "../headers/fleet.h"
#include "../headers/window/choose_fleet.h"
#include "../headers/window/dialog_1.h"
#include "../headers/window/playing_interface.h"
#include "../headers/graphics.h"
#include "../headers/logics.h"
#include "../headers/ai.h"
// Fonction qui vérifie si un bateau peut être placé dans la grille sans dépasser ou entrer en collision avec un autre bateau
int no_problem(int grid[10][10], int x, int y, int size, int vertical) {
    if (vertical) {
        if (x + size > 10) return 0;  // Dépasse la grille
        for (int i = -1; i <= size; i++) {
            for (int j = -1; j <= 1; j++) {
                int xi = x + i;
                int yj = y + j;
                if (xi >= 0 && xi < 10 && yj >= 0 && yj < 10) {
                    if (grid[xi][yj] != 0) return 0;  // Collision ou proximité avec un autre bateau
                }
            }
        }
    } else {
        if (y + size > 10) return 0;  // Dépasse la grille
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= size; j++) {
                int xi = x + i;
                int yj = y + j;
                if (xi >= 0 && xi < 10 && yj >= 0 && yj < 10) {
                    if (grid[xi][yj] != 0) return 0;  // Collision ou proximité avec un autre bateau
                }
            }
        }
    }
    return 1;
}

void place_ship(int grid[10][10], int x, int y, int size, int vertical, int ship_id) {
    if (vertical) {
        for (int i = 0; i < size; i++) {
            grid[x + i][y] = ship_id;
        }
    } else {
        for (int i = 0; i < size; i++) {
            grid[x][y + i] = ship_id;
        }
    }
}

void player_two_computer_def() {
    printf("Initialisation de l'état du joueur deux\n");
    Fleet player_two_fleet;

    // Initialiser la flotte en fonction de la flotte du joueur un
    if (strcmp(player_one.fleet.name, "American Fleet") == 0) {
        printf("Initialisation de la flotte russe\n");
        initialize_russian_fleet(&player_two_fleet);
    } else {
        printf("Initialisation de la flotte américaine\n");
        initialize_american_fleet(&player_two_fleet);
    }

    // Initialiser le joueur deux (ordinateur)
    initialize_player(&player_two, "Computer", player_two_fleet, "offline", 0, "");

    // Seed pour la génération aléatoire
    srand(time(NULL));

    // Tailles et IDs des bateaux
    int ship_sizes[] = {6, 5, 4, 3, 2};  // Tailles standards des bateaux
    int ship_ids[] = {13, 12, 10, 9, 8}; // IDs des bateaux

    // Stratégie : placer les plus grands bateaux en premier
    for (int i = 0; i < 5; i++) {
        int placed = 0;
        int attempts = 0;
        while (!placed && attempts < 100) {  // Limite le nombre de tentatives pour éviter une boucle infinie
            int x, y, vertical;

            // Stratégie de placement
            if (i % 2 == 0) {
                // Placer les bateaux le long des bords pour les plus grands
                vertical = rand() % 2;
                if (vertical) {
                    x = rand() % (10 - ship_sizes[i]);
                    y = (rand() % 2) * 9;  // Soit 0 soit 9
                } else {
                    x = (rand() % 2) * 9;
                    y = rand() % (10 - ship_sizes[i]);
                }
            } else {
                // Placer les bateaux restants aléatoirement au centre
                vertical = rand() % 2;
                x = rand() % (10 - ship_sizes[i]);
                y = rand() % (10 - ship_sizes[i]);
            }

            // Vérifier si le bateau peut être placé
            if (no_problem(player_two_grid, x, y, ship_sizes[i], vertical)) {
                place_ship(player_two_grid, x, y, ship_sizes[i], vertical, ship_ids[i]);
                placed = 1;  // Bateau placé avec succès
            }
            attempts++;
        }
        if (!placed) {
            printf("Impossible de placer le bateau de taille %d après %d tentatives.\n", ship_sizes[i], attempts);
        }
    }
}

void player_2_acting() {
    // Afficher un message indiquant que le joueur 2 agit
    printf("Player 2 acting!!!\n");
    // Déclarer un tableau pour recevoir les tirs effectués par l'IA
    Coordonnees tirs_effectues[4];
    int nombre_tirs = 0;

    // Appeler la fonction pnj_shoot en passant la matrice de l'IA et en récupérant les tirs
    pnj_shoot(hint_player_two_grid, tirs_effectues, &nombre_tirs);

    // Boucler sur les tirs effectués pour les traiter
    for (int i = 0; i < nombre_tirs; i++) {
        int x = tirs_effectues[i].x;
        int y = tirs_effectues[i].y;

        // Afficher les coordonnées du tir
        printf("AI shoots at (%d, %d)\n", x, y);

        // Appeler la fonction FireAtCell pour mettre à jour le plateau du joueur 1
        FireAtCell(&player_one,player_one_grid, hint_player_two_grid, y, x);
        // Afficher les grilles après le tir
        printf("Player 1 grid\n");
        afficherMatrice(player_one_grid);
        printf("Player 2 hint grid\n");
        afficherMatrice(hint_player_two_grid);
    }
}
void Computer_mode(SDL_Window* window, SDL_Renderer* renderer){
    printf("Hints AI matrice \n");
    afficherMatrice(hint_player_two_grid);
     player_two_init_state=&player_two_computer_def;
     player_two_acting=&player_2_acting;
     Choose_fleet(first_window,first_renderer);
    //Dialog_1(first_window,first_renderer);
}
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
void validate_ships(int grid[10][10], Fleet *fleet) {
    Ships *ships[] = {
        &fleet->aircraft_carrier,
        &fleet->destroyer,
        &fleet->submarine,
        &fleet->cruiser,
        &fleet->torpedo_boat
    };
    int num_ships = sizeof(ships) / sizeof(Ships*);
    
    for (int i = 0; i < num_ships; i++) {
        Ships *ship = ships[i];
        int ship_id = ship->ship_id;
        int size = ship->n_cases;
        int positions_x[10];
        int positions_y[10];
        int count = 0;

        // Collecter les positions du bateau dans la grille
        for (int x = 0; x < 10; x++) {
            for (int y = 0; y < 10; y++) {
                if (grid[x][y] == ship_id) {
                    positions_x[count] = x;
                    positions_y[count] = y;
                    count++;
                }
            }
        }

        // Vérifier le nombre de segments
        if (count != size) {
            printf("Bateau ID %d mal placé (segments: %d au lieu de %d). Repositionnement...\n", ship_id, count, size);
        } else {
            // Vérifier l'alignement
            int aligned_horizontally = 1;
            int aligned_vertically = 1;
            for (int j = 1; j < count; j++) {
                if (positions_x[j] != positions_x[0]) {
                    aligned_horizontally = 0;
                }
                if (positions_y[j] != positions_y[0]) {
                    aligned_vertically = 0;
                }
            }

            if (!(aligned_horizontally || aligned_vertically)) {
                printf("Bateau ID %d non aligné. Repositionnement...\n", ship_id);
            } else {
                // Vérifier la contiguïté
                if (aligned_horizontally) {
                    // Trier les y pour vérifier la séquence
                    for (int j = 0; j < count - 1; j++) {
                        for (int k = j + 1; k < count; k++) {
                            if (positions_y[j] > positions_y[k]) {
                                int temp = positions_y[j];
                                positions_y[j] = positions_y[k];
                                positions_y[k] = temp;
                            }
                        }
                    }
                    for (int j = 0; j < count - 1; j++) {
                        if (positions_y[j] + 1 != positions_y[j + 1]) {
                            printf("Bateau ID %d non contigu. Repositionnement...\n", ship_id);
                            aligned_horizontally = 0; // Marquer comme non valide
                            break;
                        }
                    }
                } else if (aligned_vertically) {
                    // Trier les x pour vérifier la séquence
                    for (int j = 0; j < count - 1; j++) {
                        for (int k = j + 1; k < count; k++) {
                            if (positions_x[j] > positions_x[k]) {
                                int temp = positions_x[j];
                                positions_x[j] = positions_x[k];
                                positions_x[k] = temp;
                            }
                        }
                    }
                    for (int j = 0; j < count - 1; j++) {
                        if (positions_x[j] + 1 != positions_x[j + 1]) {
                            printf("Bateau ID %d non contigu. Repositionnement...\n", ship_id);
                            aligned_vertically = 0; // Marquer comme non valide
                            break;
                        }
                    }
                }

                if ((aligned_horizontally && aligned_vertically) || (!aligned_horizontally && !aligned_vertically)) {
                    // Si les deux sont vrais, cela signifie que le bateau est de taille 1, ce qui peut être valide
                    // Sinon, il est mal aligné ou non contigu
                    if (!(aligned_horizontally || aligned_vertically)) {
                        printf("Bateau ID %d mal aligné ou non contigu. Repositionnement...\n", ship_id);
                    }
                }
            }

            // Si le bateau est mal placé, le repositionner
            if (count != size ||
                !(aligned_horizontally || aligned_vertically) ||
                (aligned_horizontally && (positions_y[count-1] - positions_y[0] + 1 != size)) ||
                (aligned_vertically && (positions_x[count-1] - positions_x[0] + 1 != size))) {
                
                // Retirer le bateau de la grille
                for (int j = 0; j < count; j++) {
                    grid[positions_x[j]][positions_y[j]] = 0;
                }

                // Réinitialiser les informations du bateau
                ship->ShipX = -1;
                ship->ShipY = -1;
                ship->horizontal = 0;
                ship->health = size;
                memset(ship->cases, 0, sizeof(ship->cases));

                // Tenter de re-positionner le bateau
                int placed = 0;
                int attempts = 0;
                while (!placed && attempts < 100) {
                    int x, y, vertical;
                    vertical = rand() % 2; // Orientation aléatoire
                    if (vertical) {
                        x = rand() % (10 - size + 1);
                        y = rand() % 10;
                    } else {
                        x = rand() % 10;
                        y = rand() % (10 - size + 1);
                    }

                    // Vérifier si le bateau peut être placé
                    if (no_problem(grid, x, y, size, vertical)) {
                        place_ship(grid, x, y, size, vertical, ship_id);
                        placed = 1;  // Bateau placé avec succès
                        // Enregistrer la position et l'orientation du bateau
                        ship->ShipX = x;
                        ship->ShipY = y;
                        ship->horizontal = !vertical;
                        ship->n_cases = size;
                        ship->health = size;
                        // Initialiser le tableau 'cases' du navire si nécessaire
                        memset(ship->cases, 0, sizeof(ship->cases));
                        for (int j = 0; j < size; j++) {
                            if (vertical) {
                                ship->cases[x + j][y] = 1;
                            } else {
                                ship->cases[x][y + j] = 1;
                            }
                        }
                    }
                    attempts++;
                }
                if (!placed) {
                    printf("Impossible de repositionner le bateau ID %d de taille %d après %d tentatives.\n", ship_id, size, attempts);
                } else {
                    printf("Bateau ID %d repositionné avec succès.\n", ship_id);
                }
            }
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
    int ship_sizes[] = {6, 5, 4, 3, 2};  // Tailles des bateaux
    int ship_ids[] = {13, 12, 10, 9, 8}; // IDs des bateaux

    // Pointeurs vers les navires de la flotte
    Ships* ships[] = {
        &player_two_fleet.aircraft_carrier,  // Taille 6
        &player_two_fleet.destroyer,         // Taille 5
        &player_two_fleet.submarine,         // Taille 4
        &player_two_fleet.cruiser,           // Taille 3
        &player_two_fleet.torpedo_boat       // Taille 2
    };

    // Mélanger l'ordre des navires pour rendre le placement moins prévisible
    for (int i = 0; i < 5; i++) {
        int rand_index = rand() % 5;
        // Échanger les tailles
        int temp_size = ship_sizes[i];
        ship_sizes[i] = ship_sizes[rand_index];
        ship_sizes[rand_index] = temp_size;
        // Échanger les IDs
        int temp_id = ship_ids[i];
        ship_ids[i] = ship_ids[rand_index];
        ship_ids[rand_index] = temp_id;
        // Échanger les pointeurs de navire
        Ships* temp_ship = ships[i];
        ships[i] = ships[rand_index];
        ships[rand_index] = temp_ship;
    }

    // Placement aléatoire des bateaux
    for (int i = 0; i < 5; i++) {
        int placed = 0;
        int attempts = 0;
        while (!placed && attempts < 100) {  // Limite le nombre de tentatives pour éviter une boucle infinie
            int x, y, vertical;
            vertical = rand() % 2; // Orientation aléatoire
            if (vertical) {
                x = rand() % (10 - ship_sizes[i] + 1);
                y = rand() % 10;
            } else {
                x = rand() % 10;
                y = rand() % (10 - ship_sizes[i] + 1);
            }

            // Vérifier si le bateau peut être placé
            if (no_problem(player_two_grid, x, y, ship_sizes[i], vertical)) {
                place_ship(player_two_grid, x, y, ship_sizes[i], vertical, ship_ids[i]);
                placed = 1;  // Bateau placé avec succès
                // Enregistrer la position et l'orientation du bateau
                ships[i]->ShipX = x;
                ships[i]->ShipY = y;
                ships[i]->horizontal = !vertical;
                ships[i]->ship_id = ship_ids[i];
                ships[i]->n_cases = ship_sizes[i];
                ships[i]->health = ship_sizes[i];
                // Initialiser le tableau 'cases' du navire si nécessaire
                memset(ships[i]->cases, 0, sizeof(ships[i]->cases));
                for (int j = 0; j < ship_sizes[i]; j++) {
                    if (vertical) {
                        ships[i]->cases[x + j][y] = 1;
                    } else {
                        ships[i]->cases[x][y + j] = 1;
                    }
                }
            }
            attempts++;
        }
        if (!placed) {
            printf("Impossible de placer le bateau de taille %d après %d tentatives.\n", ship_sizes[i], attempts);
        }
    }

    // Valider et repositionner les bateaux si nécessaire
    validate_ships(player_two_grid, &player_two_fleet);

    // Mettre à jour la flotte du joueur deux avec les positions des navires
    player_two.fleet = player_two_fleet;
}

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
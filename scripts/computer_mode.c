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
int no_problem (int grid[10][10], int x, int y, int size, int vertical) {
    if (vertical) {
        if (x + size > 10) return 0;  // Dépasse la grille
        for (int i = 0; i < size; i++) {
            if (grid[x + i][y] != 0) return 0;  // Collision avec un autre bateau
        }
    } else {
        if (y + size > 10) return 0;  // Dépasse la grille
        for (int i = 0; i < size; i++) {
            if (grid[x][y + i] != 0) return 0;  // Collision avec un autre bateau
        }
    }
    return 1;
}
// Fonction qui place un bateau dans la grille
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
    printf("init player two state");
    Fleet player_two_fleet;

    // Initialiser la flotte en fonction de la flotte du joueur un
    if (strcmp(&player_one.fleet, "American Fleet") == 0) {
        printf("Will initialize the Russian fleet!\n");
        initialize_russian_fleet(&player_two_fleet);
    } else {
        printf("Will initialize the American fleet!\n");
        initialize_american_fleet(&player_two_fleet);
    }

    // Initialiser le joueur deux (ordinateur)
    initialize_player(&player_two, "Computer", player_two_fleet, "offline", 0, "");

    // Initialiser aléatoirement les positions des bateaux dans la grille du joueur deux
    srand(time(NULL));  // Seed pour la génération aléatoire

    // Correction des tailles des bateaux :
    // Destroyer (2 cases), Torpedo Boat (2 cases), Submarine (3 cases), Cruiser (4 cases), Aircraft Carrier (5 cases)
    int ship_sizes[] = {2, 3, 4, 5, 6};  // Tailles corrigées des bateaux
    int ship_ids[] = {8, 9, 10, 12, 13};  // ID des bateaux : destroyer, torpedo_boat, submarine, cruiser, aircraft_carrier

    for (int i = 0; i < 5; i++) {
        int placed = 0;
        while (!placed) {
            // Générer une position aléatoire
            int x = rand() % 9;
            int y = rand() % 9;
            int vertical = rand() % 2;  // 0 = horizontal, 1 = vertical

            // Vérifier si le bateau peut être placé
            if (no_problem(player_two_grid, x, y, ship_sizes[i], vertical)) {
                place_ship(player_two_grid, x, y, ship_sizes[i], vertical, ship_ids[i]);
                placed = 1;  // Bateau placé avec succès
            }
        }
    }
}

void player_2_acting() {
    // Print action message for player 2
    printf("Player 2 acting!!!\n");

    // Call the pnj_shoot function and store the result (assumed to return a 2D array of shots)
    int **shoot_On = pnj_shoot(hint_player_two_grid);

    // Check if the result from pnj_shoot is valid (not NULL)
    if (shoot_On == NULL) {
        printf("Error: Failed to get shots from pnj_shoot.\n");
        return;
    }

    // Print confirmation of first shot, assuming shoot_On[1][0] exists
    printf("Shot data received! First value: %d\n", &shoot_On[1][0]);

    // Display the shots (assuming afficherShooting works correctly with int**)
    //afficherShooting(shoot_On);

    // Loop through the shoot_On array for 4 shots (assuming 4 shots are planned)
    for (int i = 0; i < 4; i++) {
        printf("AI shoot me on %d , %d",&shoot_On[i][0],&shoot_On[i][1]);
        FireAtCell(player_one_grid, hint_player_two_grid,&shoot_On[i][0],&shoot_On[i][1]);
        //shoot_On[i][0] = 0;
        //shoot_On[i][1] = 0;
    }

    // Print end message
    printf("All actions completed!\n");
    }

void Computer_mode(SDL_Window* window, SDL_Renderer* renderer){
     player_two_init_state=&player_two_computer_def;
     player_two_acting=&player_2_acting;
     Choose_fleet(first_window,first_renderer);
    //Dialog_1(first_window,first_renderer);
}
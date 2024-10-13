#include "../../headers/graphics.h"
#include "../../headers/events.h"
#include "../../headers/log.h"
#include "../../headers/damier.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include "../../headers/window/playing_interface.h"
#include <time.h>
#include "../../headers/player.h"
#include "../../headers/ships.h"
#include "../../headers/fleet.h"
#include "../../headers/boat_on_grid.h"
#include <stdio.h>

SDL_Texture *shipTexture = NULL;
static Grid main_grid; // Grille principale accessible globalement
bool shuffled=true;
// Fonction pour charger une texture depuis un fichier
SDL_Texture* loadTexture(const char *file, SDL_Renderer *renderer) {
    SDL_Texture *texture = IMG_LoadTexture(renderer, file);
    if (!texture) {
        SDL_Log("Failed to load texture %s: %s", file, IMG_GetError());
    }
    return texture;
}

void PlayGame() {
    printf("Le jeu est prêt à être joué\n");
    Fleet myfleet = player_one.fleet;
    // Afficher des informations pour le débogage
    printf("ShipX: %d\n", myfleet.destroyer.ShipX);
    printf("Name: %s\n", player_one.name);
    printf("Health: %d\n", player_one.health);
    OpenPlayWindow(first_window,first_renderer, &second_window, &second_renderer);
    PlayingInterface(second_window, second_renderer);
}

// Fonction pour afficher la matrice (à implémenter selon votre logique)
extern void afficherMatrice(int grid[10][10]);

// Fonction pour mettre à jour la grille en fonction des positions actuelles des bateaux
void UpdateGrid(Fleet *fleet, int grid[10][10]) {
    // Réinitialiser la grille
    memset(grid, 0, sizeof(int) * 10 * 10);

    // Tableau de tous les bateaux
    Ships *ships[] = {
        &fleet->destroyer,
        &fleet->torpedo_boat,
        &fleet->submarine,
        &fleet->aircraft_carrier,
        &fleet->cruiser
    };

    // Placer chaque bateau dans la grille
    for (int i = 0; i < 5; i++) {
        Ships *ship = ships[i];
        int length = ship->n_cases;
        int x = ship->ShipX;
        int y = ship->ShipY;

        if (ship->horizontal) {
            for (int j = 0; j < length; j++) {
                if (x + j < 10 && y < 10) {
                    grid[y][x + j] = ship->ship_id;
                }
            }
        } else {
            for (int j = 0; j < length; j++) {
                if (x < 10 && y + j < 10) {
                    grid[y + j][x] = ship->ship_id;
                }
            }
        }
    }
    afficherMatrice(grid);
}

// Fonction pour dessiner un bateau
void DrawShips(Grid *grid, Ships *ship) {
    SDL_Texture *shipTexture = ship->horizontal ? ship->texture_h : ship->texture_v;

    SDL_Rect boatRect = {
        grid->xPos + grid->cellSize * ship->ShipX,
        grid->yPos + grid->cellSize * ship->ShipY,
        ship->horizontal ? grid->cellSize * ship->n_cases : grid->cellSize,
        ship->horizontal ? grid->cellSize : grid->cellSize * ship->n_cases
    };

    // Dessiner le bateau (avec image)
    SDL_RenderCopy(grid->renderer, shipTexture, NULL, &boatRect);
}

// Fonction pour ajuster la position du bateau à la grille
void SnapToGrid(Grid *grid, int *x, int *y, int shipLength, bool horizontal) {
    int cellX = (*x - grid->xPos) / grid->cellSize;
    int cellY = (*y - grid->yPos) / grid->cellSize;

    if (horizontal) {
        cellX = (cellX < 0) ? 0 : (cellX > grid->gridWidth - shipLength ? grid->gridWidth - shipLength : cellX);
        cellY = (cellY < 0) ? 0 : (cellY >= grid->gridHeight ? grid->gridHeight - 1 : cellY);
    } else {
        cellX = (cellX < 0) ? 0 : (cellX >= grid->gridWidth ? grid->gridWidth - 1 : cellX);
        cellY = (cellY < 0) ? 0 : (cellY > grid->gridHeight - shipLength ? grid->gridHeight - shipLength : cellY);
    }

    *x = grid->xPos + cellX * grid->cellSize;
    *y = grid->yPos + cellY * grid->cellSize;
}

// Fonction pour vérifier les collisions entre deux bateaux
bool CheckCollision(Ships *ship1, Ships *ship2) {
    int x1 = ship1->ShipX;
    int y1 = ship1->ShipY;
    int w1 = ship1->horizontal ? ship1->n_cases : 1;
    int h1 = ship1->horizontal ? 1 : ship1->n_cases;

    int x2 = ship2->ShipX;
    int y2 = ship2->ShipY;
    int w2 = ship2->horizontal ? ship2->n_cases : 1;
    int h2 = ship2->horizontal ? 1 : ship2->n_cases;

    SDL_Rect rect1 = {x1, y1, w1, h1};
    SDL_Rect rect2 = {x2, y2, w2, h2};

    return SDL_HasIntersection(&rect1, &rect2);
}

// Fonction pour mettre à jour la position du bateau avec vérification des collisions et des limites
void UpdateShipPosition(Grid *grid, Ships *ship, int mouseX, int mouseY, Fleet *fleet) {
    int newShipX = (mouseX - grid->xPos) / grid->cellSize;
    int newShipY = (mouseY - grid->yPos) / grid->cellSize;

    // Vérifier les contraintes de la grille pour les placements horizontaux et verticaux
    if (ship->horizontal) {
        if (newShipX < 0) {
            newShipX = 0;
        } else if (newShipX > grid->gridWidth - ship->n_cases) {
            newShipX = grid->gridWidth - ship->n_cases;
        }

        if (newShipY < 0) {
            newShipY = 0;
        } else if (newShipY >= grid->gridHeight) {
            newShipY = grid->gridHeight - 1;
        }
    } else {
        if (newShipY < 0) {
            newShipY = 0;
        } else if (newShipY > grid->gridHeight - ship->n_cases) {
            newShipY = grid->gridHeight - ship->n_cases;
        }

        if (newShipX < 0) {
            newShipX = 0;
        } else if (newShipX >= grid->gridWidth) {
            newShipX = grid->gridWidth - 1;
        }
    }

    // Stocker l'ancienne position
    int oldShipX = ship->ShipX;
    int oldShipY = ship->ShipY;

    // Mettre à jour la nouvelle position
    ship->ShipX = newShipX;
    ship->ShipY = newShipY;

    // Vérifier les collisions avec les autres bateaux
    bool collision = false;
    Ships *allShips[] = {
        &fleet->destroyer,
        &fleet->torpedo_boat,
        &fleet->submarine,
        &fleet->aircraft_carrier,
        &fleet->cruiser
    };

    for (int i = 0; i < 5; i++) {
        Ships *otherShip = allShips[i];
        if (otherShip != ship) {
            if (CheckCollision(ship, otherShip)) {
                collision = true;
                break;
            }
        }
    }

    if (collision) {
        // Réinitialiser à l'ancienne position si collision détectée
        ship->ShipX = oldShipX;
        ship->ShipY = oldShipY;
    }

    // Mettre à jour la grille
    UpdateGrid(fleet, player_one_grid);
}

// Fonction pour faire glisser un bateau (drag and drop)
void DragShip(Grid *grid, Ships *ship, int mouseX, int mouseY, Fleet *fleet) {
    SnapToGrid(grid, &mouseX, &mouseY, ship->n_cases, ship->horizontal);
    UpdateShipPosition(grid, ship, mouseX, mouseY, fleet);
}

// Fonction pour faire pivoter un bateau et mettre à jour la grille
void RotateShip(Grid *grid, Ships *ship, Fleet *fleet) {
    // Stocker l'ancienne orientation et position
    bool oldHorizontal = ship->horizontal;
    int oldX = ship->ShipX;
    int oldY = ship->ShipY;

    // Basculer l'orientation
    ship->horizontal = !ship->horizontal;

    // Ajuster la position si nécessaire pour rester dans la grille
    if (ship->horizontal) {
        if (ship->ShipX + ship->n_cases > grid->gridWidth) {
            ship->ShipX = grid->gridWidth - ship->n_cases;
        }
    } else {
        if (ship->ShipY + ship->n_cases > grid->gridHeight) {
            ship->ShipY = grid->gridHeight - ship->n_cases;
        }
    }

    // Vérifier les collisions
    bool collision = false;
    Ships *allShips[] = {
        &fleet->destroyer,
        &fleet->torpedo_boat,
        &fleet->submarine,
        &fleet->aircraft_carrier,
        &fleet->cruiser
    };
    for (int i = 0; i < 5; i++) {
        Ships *otherShip = allShips[i];
        if (otherShip != ship) {
            if (CheckCollision(ship, otherShip)) {
                collision = true;
                break;
            }
        }
    }

    if (collision) {
        // Réinitialiser l'orientation et la position si collision
        ship->horizontal = oldHorizontal;
        ship->ShipX = oldX;
        ship->ShipY = oldY;
    }

    // Mettre à jour la grille
    UpdateGrid(fleet, player_one_grid);
}

// Fonction pour randomiser la position des bateaux (bouton Shuffle)
void ShuffleShips() {
    shuffled=false;
    Fleet *fleet = &player_one.fleet;

    // Réinitialiser la grille
    memset(player_one_grid, 0, sizeof(player_one_grid));

    // Tableau de tous les bateaux
    Ships *ships[] = {
        &fleet->destroyer,
        &fleet->torpedo_boat,
        &fleet->submarine,
        &fleet->aircraft_carrier,
        &fleet->cruiser
    };

    // Randomiser les placements pour chaque bateau
    for (int i = 0; i < 5; i++) {
        Ships *ship = ships[i];
        bool placed = false;
        while (!placed) {
            // Générer une orientation et une position aléatoires
            ship->horizontal = rand() % 2;
            int maxX = ship->horizontal ? 10 - ship->n_cases : 9;
            int maxY = ship->horizontal ? 9 : 10 - ship->n_cases;
            ship->ShipX = rand() % (maxX + 1);
            ship->ShipY = rand() % (maxY + 1);

            // Vérifier les collisions
            bool collision = false;
            for (int j = 0; j < i; j++) {
                if (CheckCollision(ship, ships[j])) {
                    collision = true;
                    break;
                }
            }

            if (!collision) {
                placed = true;
            }
        }
    }

    // Mettre à jour la grille
    UpdateGrid(fleet, player_one_grid);

    printf("Les bateaux ont été mélangés.\n");
}

// Fonction pour dessiner la flotte sur la grille
void DrawFleet(Grid *grid, Fleet *fleet) {
    Ships *ships[] = {
        &fleet->destroyer,
        &fleet->torpedo_boat,
        &fleet->submarine,
        &fleet->aircraft_carrier,
        &fleet->cruiser
    };

    for (int i = 0; i < 5; i++) {
        Ships *ship = ships[i];
        if (ship->name[0] != '\0') {
            DrawShips(grid, ship);
        }
    }
}

void ShowPlaceBoat(SDL_Window* window, SDL_Renderer* renderer) {
    // Charger et créer la texture de fond
    SDL_Surface *backgroundSurface = IMG_Load("medias/images/metalic_panel.png");
    if (!backgroundSurface) {
        LogMessage("Erreur de chargement de l'image : %s\n", IMG_GetError());
        return;
    }
    SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface);
    if (!backgroundTexture) {
        LogMessage("Erreur de création de texture : %s\n", SDL_GetError());
        return;
    }
    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
    SDL_DestroyTexture(backgroundTexture);

    // Initialiser la grille
    main_grid.renderer = renderer;
    main_grid.gridWidth = 10;
    main_grid.gridHeight = 10;
    main_grid.cellSize = 41;
    main_grid.xPos = 90;
    main_grid.yPos = 40;

    // Charger l'image de fond de la grille
    SDL_Surface* bgSurface = IMG_Load("medias/images/oceangrid.png");
    if (!bgSurface) {
        printf("Échec du chargement de l'image de la grille : %s\n", IMG_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }
    main_grid.backgroundImage = SDL_CreateTextureFromSurface(renderer, bgSurface);
    SDL_FreeSurface(bgSurface);

    if (!main_grid.backgroundImage) {
        printf("Échec de la création de la texture de la grille : %s\n", IMG_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return;
    }

    // Initialiser la flotte
    Fleet *fleet = &player_one.fleet;
    srand((unsigned int)time(NULL));

    // Réinitialiser la grille
    memset(player_one_grid, 0, sizeof(player_one_grid));

    // Placer aléatoirement les bateaux
    if(shuffled){
    ShuffleShips();
    }
    else
    {
        printf("glich detected");
    }
    
    // Boucle principale des événements
    bool running = true;
    SDL_Event event;
    bool dragging = false;
    Ships *currentShip = NULL;
    int offsetX = 0, offsetY = 0;
    int width = 0;
    int height = 0;
    SDL_Color textColor = {255, 255, 255, 255};

    // Création des boutons Play et Shuffle
    CreateClickableElement(renderer, 214, 520, &width, &height, "Play", textColor, "medias/images/btn-play.png", PlayGame, 12);
    CreateClickableElement(renderer, 414, 520, &width, &height, "Shuffle", textColor, "medias/images/btn-play.png", ShuffleShips, 12);

    while (running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        int mouseX = event.button.x;
                        int mouseY = event.button.y;

                        // Vérifier si des éléments cliquables ont été cliqués
                        TriggerClickCallbacks(mouseX, mouseY);

                        // Tableau de tous les bateaux pour itération facile
                        Ships *ships[] = {
                            &fleet->destroyer,
                            &fleet->torpedo_boat,
                            &fleet->submarine,
                            &fleet->aircraft_carrier,
                            &fleet->cruiser
                        };

                        // Vérifier si un bateau est cliqué
                        for (int i = 0; i < 5; i++) {
                            Ships *ship = ships[i];
                            SDL_Rect shipRect = {
                                main_grid.xPos + main_grid.cellSize * ship->ShipX,
                                main_grid.yPos + main_grid.cellSize * ship->ShipY,
                                ship->horizontal ? main_grid.cellSize * ship->n_cases : main_grid.cellSize,
                                ship->horizontal ? main_grid.cellSize : main_grid.cellSize * ship->n_cases
                            };

                            if (SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &shipRect)) {
                                dragging = true;
                                currentShip = ship;
                                offsetX = mouseX - shipRect.x;
                                offsetY = mouseY - shipRect.y;
                                break;
                            }
                        }
                    }
                    break;

                case SDL_MOUSEBUTTONUP:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        dragging = false;
                        currentShip = NULL;
                    }
                    break;

                case SDL_MOUSEMOTION:
                    if (dragging && currentShip) {
                        int mouseX = event.motion.x - offsetX;
                        int mouseY = event.motion.y - offsetY;
                        DragShip(&main_grid, currentShip, mouseX, mouseY, fleet);
                    }
                    break;

                case SDL_KEYDOWN:
                    // Gérer la touche Entrée pour basculer l'orientation du bateau
                    if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER) {
                        if (currentShip) {
                            RotateShip(&main_grid, currentShip, fleet);
                        }
                    }
                    break;
            }
        }

        // Effacer l'écran
        //SDL_RenderClear(renderer);

        // Rendre la grille et la flotte
        SDL_RenderCopy(renderer, main_grid.backgroundImage, NULL, &(SDL_Rect){main_grid.xPos, main_grid.yPos, main_grid.gridWidth * main_grid.cellSize, main_grid.gridHeight * main_grid.cellSize});
        DrawFleet(&main_grid, fleet);

        // Optionnel : surligner le bateau sélectionné
        if (currentShip) {
            SDL_Rect highlightRect = {
                main_grid.xPos + main_grid.cellSize * currentShip->ShipX,
                main_grid.yPos + main_grid.cellSize * currentShip->ShipY,
                currentShip->horizontal ? main_grid.cellSize * currentShip->n_cases : main_grid.cellSize,
                currentShip->horizontal ? main_grid.cellSize : main_grid.cellSize * currentShip->n_cases
            };
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Surlignage jaune
            SDL_RenderDrawRect(renderer, &highlightRect);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Réinitialiser la couleur
        }

        // Présenter le rendu final
        SDL_RenderPresent(renderer);
    }

    // Nettoyer les ressources
    SDL_DestroyTexture(main_grid.backgroundImage);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

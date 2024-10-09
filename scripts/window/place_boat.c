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

// Fonction pour charger une texture
SDL_Texture* loadTexture(const char *file, SDL_Renderer *renderer) {
    SDL_Texture *texture = IMG_LoadTexture(renderer, file);
    if (!texture) {
        SDL_Log("Failed to load texture %s: %s", file, IMG_GetError());
    }
    return texture;
}

void PlayGame() {
    printf("The game is ready to play\n");
    Fleet myfleet = player_one.fleet;
    // Afficher ShipX
    printf("ShipX: %d\n", myfleet.destroyer.ShipX);
    printf("Name: %s\n", player_one.name);
    printf("Health: %d\n", player_one.health);
    OpenPlayWindow(first_window, first_renderer, &second_window, &second_renderer);
    PlayingInterface(second_window, second_renderer);
}

// Fonction pour dessiner un rectangle coloré (indication de sélection)
void DrawSelectionRect(SDL_Renderer *renderer, int x, int y, int width, int height, bool isDragging) {
    SDL_Rect rect = {x, y, width, height};
    if (isDragging) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 128); // Vert semi-transparent
        SDL_RenderFillRect(renderer, &rect);
    }
}

// Fonction pour afficher la matrice (à implémenter selon votre logique)
extern void afficherMatrice(int grid[10][10]);

// Fonction pour placer un bateau dans la grille
void PlaceShipInGrid(Ships *ship, int grid[10][10], int oldX, int oldY, bool oldHorizontal) {
    int length = ship->n_cases;

    // Remplacer l'ancienne position par des zéros
    if (oldHorizontal) {
        for (int i = 0; i < length; i++) {
            if (oldX + i < 10 && oldY < 10) {
                grid[oldY][oldX + i] = 0;  // Remplace les anciennes cases horizontalement
            }
        }
    } else {
        for (int i = 0; i < length; i++) {
            if (oldY + i < 10 && oldX < 10) {
                grid[oldY + i][oldX] = 0;  // Remplace les anciennes cases verticalement
            }
        }
    }

    // Placer le navire à la nouvelle position
    int x = ship->ShipX;
    int y = ship->ShipY;

    if (ship->horizontal) {
        for (int i = 0; i < length; i++) {
            if (x + i < 10 && y < 10) {
                grid[y][x + i] = ship->ship_id;  // Remplit les cases horizontalement
            }
        }
    } else {
        for (int i = 0; i < length; i++) {
            if (y + i < 10 && x < 10) {
                grid[y + i][x] = ship->ship_id;  // Remplit les cases verticalement
            }
        }
    }

    // Afficher la matrice après modification
    afficherMatrice(grid);
}

// Fonction pour dessiner un bateau
void DrawShips(Grid *grid, Ships *ship, bool isDragging) {
    SDL_Texture *shipTexture = ship->horizontal ? ship->texture_h : ship->texture_v;

    SDL_Rect boatRect = {
        grid->xPos + grid->cellSize * ship->ShipX,
        grid->yPos + grid->cellSize * ship->ShipY,
        ship->horizontal ? grid->cellSize * ship->n_cases : grid->cellSize,
        ship->horizontal ? grid->cellSize : grid->cellSize * ship->n_cases
    };

    // Dessiner le rectangle coloré si en train de glisser
    DrawSelectionRect(grid->renderer, boatRect.x, boatRect.y, boatRect.w, boatRect.h, isDragging);

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
bool CheckCollision(Ships *ship1, Ships *ship2, Grid *grid) {
    SDL_Rect rect1 = {
        grid->xPos + grid->cellSize * ship1->ShipX,
        grid->yPos + grid->cellSize * ship1->ShipY,
        ship1->horizontal ? grid->cellSize * ship1->n_cases : grid->cellSize,
        ship1->horizontal ? grid->cellSize : grid->cellSize * ship1->n_cases
    };

    SDL_Rect rect2 = {
        grid->xPos + grid->cellSize * ship2->ShipX,
        grid->yPos + grid->cellSize * ship2->ShipY,
        ship2->horizontal ? grid->cellSize * ship2->n_cases : grid->cellSize,
        ship2->horizontal ? grid->cellSize : grid->cellSize * ship2->n_cases
    };

    // Vérifier l'intersection
    return !(rect1.x + rect1.w <= rect2.x ||
             rect1.x >= rect2.x + rect2.w ||
             rect1.y + rect1.h <= rect2.y ||
             rect1.y >= rect2.y + rect2.h);
}

// Fonction pour mettre à jour la position du bateau avec vérification des collisions et des limites
void UpdateShipPosition(Grid *grid, Ships *ship, int mouseX, int mouseY, Fleet *fleet) {
    int newShipX = (mouseX - grid->xPos) / grid->cellSize;
    int newShipY = (mouseY - grid->yPos) / grid->cellSize;

    // Vérifier les contraintes de la grille pour les placements horizontaux et verticaux
    if (ship->horizontal) {
        // Si horizontal, s'assurer que le bateau ne dépasse pas la limite droite
        if (newShipX < 0) {
            newShipX = 0;
        } else if (newShipX > grid->gridWidth - ship->n_cases) {
            newShipX = grid->gridWidth - ship->n_cases;
        }

        // S'assurer que le bateau ne dépasse pas les limites haut et bas
        if (newShipY < 0) {
            newShipY = 0;
        } else if (newShipY >= grid->gridHeight) {
            newShipY = grid->gridHeight - 1;
        }
    } else {
        // Si vertical, s'assurer que le bateau ne dépasse pas la limite inférieure
        if (newShipY < 0) {
            newShipY = 0;
        } else if (newShipY > grid->gridHeight - ship->n_cases) {
            newShipY = grid->gridHeight - ship->n_cases;
        }

        // S'assurer que le bateau ne dépasse pas les limites gauche et droite
        if (newShipX < 0) {
            newShipX = 0;
        } else if (newShipX >= grid->gridWidth) {
            newShipX = grid->gridWidth - 1;
        }
    }

    // Stocker l'ancienne position
    int oldShipX = ship->ShipX;
    int oldShipY = ship->ShipY;
    bool oldHorizontal = ship->horizontal;

    // Mettre à jour temporairement la nouvelle position
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
    printf("Stack trace: %s\n", ship->name);
    for (int i = 0; i < 5; i++) {
        Ships *otherShip = allShips[i];
        if (otherShip != ship && otherShip->name[0] != '\0') {
            if (CheckCollision(ship, otherShip, grid)) {
                collision = true;
                break;
            }
        }
    }

    if (collision || newShipX < 0 || newShipY < 0 ||
        (ship->horizontal && (newShipX + ship->n_cases > grid->gridWidth)) ||
        (!ship->horizontal && (newShipY + ship->n_cases > grid->gridHeight))) {
        // Réinitialiser à l'ancienne position si collision ou hors limites détectée
        ship->ShipX = oldShipX;
        ship->ShipY = oldShipY;
    } else {
        // Mettre à jour la position dans la grille
        PlaceShipInGrid(ship, player_one_grid, oldShipX, oldShipY, oldHorizontal);
    }
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
    SnapToGrid(grid, &ship->ShipX, &ship->ShipY, ship->n_cases, ship->horizontal);

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
        if (otherShip != ship && otherShip->name[0] != '\0') {
            if (CheckCollision(ship, otherShip, grid)) {
                collision = true;
                break;
            }
        }
    }

    // Vérifier les limites de la grille
    if (ship->horizontal) {
        if (ship->ShipX + ship->n_cases > grid->gridWidth) {
            collision = true;
        }
    } else {
        if (ship->ShipY + ship->n_cases > grid->gridHeight) {
            collision = true;
        }
    }

    if (collision) {
        // Réinitialiser l'orientation et la position si collision ou hors limites
        ship->horizontal = oldHorizontal;
        ship->ShipX = oldX;
        ship->ShipY = oldY;
    } else {
        // Mettre à jour la grille avec la nouvelle orientation
        PlaceShipInGrid(ship, player_one_grid, oldX, oldY, oldHorizontal);
    }
}

// Fonction pour placer aléatoirement un bateau dans la grille
void RandomizeShipPlacement(Grid *grid, Ships *ship, Fleet *fleet) {
    bool placed = false;
    while (!placed) {
        bool horizontal = rand() % 2;
        int maxX = horizontal ? grid->gridWidth - ship->n_cases : grid->gridWidth - 1;
        int maxY = horizontal ? grid->gridHeight - 1 : grid->gridHeight - ship->n_cases;

        int startX = rand() % (horizontal ? grid->gridWidth - ship->n_cases + 1 : grid->gridWidth);
        int startY = rand() % (horizontal ? grid->gridHeight : grid->gridHeight - ship->n_cases + 1);

        ship->ShipX = startX;
        ship->ShipY = startY;
        ship->horizontal = horizontal;

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
            if (otherShip != ship && otherShip->name[0] != '\0') {
                if (CheckCollision(ship, otherShip, grid)) {
                    collision = true;
                    break;
                }
            }
        }

        if (!collision) {
            placed = true;
            PlaceShipInGrid(ship, player_one_grid, 0, 0, !ship->horizontal); // Assurez-vous que l'ancien horizontal est inverse
        }
    }

    // Mettre à jour la flotte principale
    switch (ship->ship_id) {
        case 8:
            player_one.fleet.destroyer = *ship;
            break;
        case 9:
            player_one.fleet.torpedo_boat = *ship;
            break;
        case 10:
            player_one.fleet.submarine = *ship;
            break;
        case 12:
            player_one.fleet.aircraft_carrier = *ship;
            break;
        case 13:
            player_one.fleet.cruiser = *ship;
            break;
        default:
            break;
    }
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
        if (ship->name[0] != '\0') {  // Vérifier si le bateau est défini
            DrawShips(grid, ship, ship->isDragging);
        }
    }
}

void ShowPlaceBoat(SDL_Window* window, SDL_Renderer* renderer) {
    // Charger et créer la texture de fond
    SDL_Surface *backgroundSurface = IMG_Load("medias/images/metalic_panel.png");
    if (!backgroundSurface) {
        LogMessage("Image Load Error: %s\n", IMG_GetError());
        return;
    }
    SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface);
    if (!backgroundTexture) {
        LogMessage("Texture Creation Error: %s\n", SDL_GetError());
        return;
    }
    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
    SDL_DestroyTexture(backgroundTexture);

    // Initialiser la grille
    Grid grid;
    grid.renderer = renderer;
    grid.gridWidth = 10;
    grid.gridHeight = 10;
    grid.cellSize = 41;
    grid.xPos = 90;
    grid.yPos = 40;

    // Charger l'image de fond de la grille
    SDL_Surface* bgSurface = IMG_Load("medias/images/oceangrid.png");
    if (!bgSurface) {
        printf("Failed to load grid image: %s\n", IMG_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }
    grid.backgroundImage = SDL_CreateTextureFromSurface(renderer, bgSurface);
    SDL_FreeSurface(bgSurface);

    if (!grid.backgroundImage) {
        printf("Failed to create grid texture: %s\n", IMG_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return;
    }

    // Initialiser la flotte
    Fleet fleet = player_one.fleet;
    srand((unsigned int)time(NULL));

    // Placer aléatoirement les bateaux
    RandomizeShipPlacement(&grid, &fleet.destroyer, &fleet);
    RandomizeShipPlacement(&grid, &fleet.torpedo_boat, &fleet);
    RandomizeShipPlacement(&grid, &fleet.submarine, &fleet);
    RandomizeShipPlacement(&grid, &fleet.aircraft_carrier, &fleet);
    RandomizeShipPlacement(&grid, &fleet.cruiser, &fleet);

    // Mettre à jour la grille principale avec les positions initiales
    PlaceShipInGrid(&fleet.torpedo_boat, player_one_grid, 0, 0, !fleet.torpedo_boat.horizontal);
    PlaceShipInGrid(&fleet.destroyer, player_one_grid, 0, 0, !fleet.destroyer.horizontal);
    PlaceShipInGrid(&fleet.submarine, player_one_grid, 0, 0, !fleet.submarine.horizontal);
    PlaceShipInGrid(&fleet.aircraft_carrier, player_one_grid, 0, 0, !fleet.aircraft_carrier.horizontal);
    PlaceShipInGrid(&fleet.cruiser, player_one_grid, 0, 0, !fleet.cruiser.horizontal);

    // Copier les positions initiales dans player_one.fleet
    player_one.fleet = fleet;

    // Boucle principale des événements
    bool running = true;
    SDL_Event event;
    bool dragging = false;
    Ships *currentShip = NULL;
    int offsetX = 0, offsetY = 0;
    int width = 0; 
    int height = 0; 
    SDL_Color textColor = {255, 255, 255, 255};

    CreateClickableElement(renderer, 214, 520, &width, &height, "Play", textColor, "medias/images/btn-play.png", PlayGame, 12);

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

                        TriggerClickCallbacks(mouseX, mouseY);

                        // Tableau de tous les bateaux pour itération facile
                        Ships *ships[] = {
                            &fleet.destroyer,
                            &fleet.torpedo_boat,
                            &fleet.submarine,
                            &fleet.aircraft_carrier,
                            &fleet.cruiser
                        };

                        // Vérifier si un bateau est cliqué
                        for (int i = 0; i < 5; i++) {
                            Ships *ship = ships[i];
                            SDL_Rect shipRect = {
                                grid.xPos + grid.cellSize * ship->ShipX,
                                grid.yPos + grid.cellSize * ship->ShipY,
                                ship->horizontal ? grid.cellSize * ship->n_cases : grid.cellSize,
                                ship->horizontal ? grid.cellSize : grid.cellSize * ship->n_cases
                            };

                            if (SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &shipRect)) {
                                dragging = true;
                                printf("Ship selected: %s\n", ship->name); // Assurez-vous que Ships struct a un nom
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
                        DragShip(&grid, currentShip, mouseX, mouseY, &fleet);
                    }
                    break;

                case SDL_KEYDOWN:
                    // Gérer la touche Entrée pour basculer l'orientation du bateau
                    if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER) {
                        if (currentShip) {
                            printf("Toggling orientation for ship: %s\n", currentShip->name); // Assurez-vous que Ships struct a un nom
                            RotateShip(&grid, currentShip, &fleet);
                        }
                    }
                    break;
            }
        }

        // Rendre la grille et la flotte
        SDL_RenderCopy(renderer, grid.backgroundImage, NULL, &(SDL_Rect){grid.xPos, grid.yPos, grid.gridWidth * grid.cellSize, grid.gridHeight * grid.cellSize});
        DrawFleet(&grid, &fleet);

        // Optionnel : surligner le bateau sélectionné
        if (currentShip) {
            SDL_Rect highlightRect = {
                grid.xPos + grid.cellSize * currentShip->ShipX,
                grid.yPos + grid.cellSize * currentShip->ShipY,
                currentShip->horizontal ? grid.cellSize * currentShip->n_cases : grid.cellSize,
                currentShip->horizontal ? grid.cellSize : grid.cellSize * currentShip->n_cases
            };
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Surlignage jaune
            SDL_RenderDrawRect(renderer, &highlightRect);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Réinitialiser la couleur
        }

        SDL_RenderPresent(renderer);
    }

    // Nettoyer les ressources
    SDL_DestroyTexture(grid.backgroundImage);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

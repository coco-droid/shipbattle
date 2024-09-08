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

// Function to load a texture
SDL_Texture* loadTexture(const char *file, SDL_Renderer *renderer) {
    SDL_Texture *texture = IMG_LoadTexture(renderer, file);
    if (!texture) {
        SDL_Log("Failed to load texture %s: %s", file, IMG_GetError());
    }
    return texture;
}

void PlayGame() {
    printf("The game is ready to play");
    Fleet myflee=player_one.fleet;
    //afficher ShipX
    printf("ShipX: %d\n",myflee.destroyer.ShipX);
    printf("Name: %s\n",player_one.name);
    OpenPlayWindow(first_window,first_renderer,&second_window,&second_renderer);
    PlayingInterface(second_window,second_renderer);
}
// Function to draw a colored rectangle (for selection indication)
void DrawSelectionRect(SDL_Renderer *renderer, int x, int y, int width, int height, bool isDragging) {
    SDL_Rect rect = {x, y, width, height};
    if (isDragging) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 128); // Semi-transparent green
        SDL_RenderFillRect(renderer, &rect);
    }
}

void PlaceShipInGrid(Ships *ship, int grid[10][10], int oldX, int oldY, bool oldHorizontal) {
    int length = ship->n_cases;

    // Remplacer l'ancienne position par des zéros
    if (oldHorizontal) {
        for (int i = 0; i < length; i++) {
            grid[oldY][oldX + i] = 0;  // Remplace les anciennes cases horizontalement
        }
    } else {
        for (int i = 0; i < length; i++) {
            grid[oldY + i][oldX] = 0;  // Remplace les anciennes cases verticalement
        }
    }

    // Placer le navire à la nouvelle position
    int x = ship->ShipX;
    int y = ship->ShipY;

    if (ship->horizontal) {
        for (int i = 0; i < length; i++) {
            grid[y][x + i] = ship->ship_id;  // Remplit les cases horizontalement
        }
    } else {
        for (int i = 0; i < length; i++) {
            grid[y + i][x] = ship->ship_id;  // Remplit les cases verticalement
        }
    }

    // Afficher la matrice après modification
    afficherMatrice(grid);
}

// Function to draw a ship
void DrawShips(Grid *grid, Ships *ship, bool isDragging) {
    SDL_Texture *shipTexture = ship->horizontal ? ship->texture_h : ship->texture_v;

    SDL_Rect boatRect = {
        grid->xPos + grid->cellSize * ship->ShipX,
        grid->yPos + grid->cellSize * ship->ShipY,
        ship->horizontal ? grid->cellSize * ship->n_cases : grid->cellSize,
        ship->horizontal ? grid->cellSize : grid->cellSize * ship->n_cases
    };

    // Draw the colored rectangle if dragging
    DrawSelectionRect(grid->renderer, boatRect.x, boatRect.y, boatRect.w, boatRect.h, isDragging);

    // Draw the ship (with image)
    SDL_RenderCopy(grid->renderer, shipTexture, NULL, &boatRect);
}

// Function to calculate the "snap" position of the ship on the grid
void SnapToGrid(Grid *grid, int *x, int *y, int shipLength, bool horizontal) {
    int cellX = (*x - grid->xPos) / grid->cellSize;
    int cellY = (*y - grid->yPos) / grid->cellSize;

    if (horizontal) {
        cellX = (cellX < 0) ? 0 : (cellX >= grid->gridWidth - shipLength + 1 ? grid->gridWidth - shipLength : cellX);
        cellY = (cellY < 0) ? 0 : (cellY >= grid->gridHeight ? grid->gridHeight - 1 : cellY);
    } else {
        cellX = (cellX < 0) ? 0 : (cellX >= grid->gridWidth ? grid->gridWidth - 1 : cellX);
        cellY = (cellY < 0) ? 0 : (cellY >= grid->gridHeight - shipLength + 1 ? grid->gridHeight - shipLength : cellY);
    }

    *x = grid->xPos + cellX * grid->cellSize;
    *y = grid->yPos + cellY * grid->cellSize;
}

// Function to check if two ships overlap
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

    // Check for intersection
    return !(rect1.x + rect1.w <= rect2.x ||
             rect1.x >= rect2.x + rect2.w ||
             rect1.y + rect1.h <= rect2.y ||
             rect1.y >= rect2.y + rect2.h);
}

// Function to update ship position with collision and boundary checks
void UpdateShipPosition(Grid *grid, Ships *ship, int mouseX, int mouseY, Fleet *fleet) {
    int newShipX = (mouseX - grid->xPos) / grid->cellSize;
    int newShipY = (mouseY - grid->yPos) / grid->cellSize;

    // Check boundary constraints for horizontal and vertical placements
    if (ship->horizontal) {
        // If horizontal, ensure ship doesn't exceed right boundary
        if (newShipX < 0) {
            newShipX = 0;
        } else if (newShipX > grid->gridWidth - ship->n_cases) {
            newShipX = grid->gridWidth - ship->n_cases;
        }

        // Ensure ship doesn't exceed top and bottom boundaries
        if (newShipY < 0) {
            newShipY = 0;
        } else if (newShipY >= grid->gridHeight) {
            newShipY = grid->gridHeight - 1;
        }
    } else {
        // If vertical, ensure ship doesn't exceed bottom boundary
        if (newShipY < 0) {
            newShipY = 0;
        } else if (newShipY > grid->gridHeight - ship->n_cases) {
            newShipY = grid->gridHeight - ship->n_cases;
        }

        // Ensure ship doesn't exceed left and right boundaries
        if (newShipX < 0) {
            newShipX = 0;
        } else if (newShipX >= grid->gridWidth) {
            newShipX = grid->gridWidth - 1;
        }
    }

    // Temporarily update ship position
    int oldShipX = ship->ShipX;
    int oldShipY = ship->ShipY;
    ship->ShipX = newShipX;
    ship->ShipY = newShipY;
    // Check for collisions with other ships
    bool collision = false;
    Ships *allShips[] = {
        &fleet->destroyer,
        &fleet->torpedo_boat,
        &fleet->submarine,
        &fleet->aircraft_carrier,
        &fleet->cruiser
    };
    printf("stacke trace: %s",ship->name);
    for (int i = 0; i < 5; i++) {
        Ships *otherShip = allShips[i];
        if (otherShip != ship && otherShip->name[0] != '\0') {
            if (CheckCollision(ship, otherShip, grid)) {
                collision = true;
                break;
            }
        }
    }
    switch (ship->ship_id)
    {
    case 8:
        player_one.fleet.destroyer.ShipX=newShipX;
        player_one.fleet.destroyer.ShipY=newShipY;
    break;
    case 9:
          player_one.fleet.torpedo_boat.ShipX=newShipX;
          player_one.fleet.torpedo_boat.ShipY=newShipY;
    break;
    case 10:
          player_one.fleet.submarine.ShipX=newShipX;
          player_one.fleet.submarine.ShipY=newShipY;
    break;
    case 12:
          player_one.fleet.aircraft_carrier.ShipX=newShipX;
          player_one.fleet.aircraft_carrier.ShipY=newShipY;
    break;
    case 13:
          player_one.fleet.cruiser.ShipX=newShipX;
          player_one.fleet.cruiser.ShipY=newShipY;
    break;
    }
    
    if (collision) {
        // Revert to old position if collision detected
        ship->ShipX = oldShipX;
        ship->ShipY = oldShipY;
    }
    PlaceShipInGrid(ship,player_one_grid,oldShipX,oldShipY,ship->horizontal);
}

// Function to randomly place a ship on the grid
void RandomizeShipPlacement(Grid *grid, Ships *ship) {
    bool horizontal = rand() % 2;
    int maxX = horizontal ? grid->gridWidth - ship->n_cases + 1 : grid->gridWidth;
    int maxY = horizontal ? grid->gridHeight : grid->gridHeight - ship->n_cases + 1;

    int startX = rand() % maxX;
    int startY = rand() % maxY;

    ship->ShipX = startX;
    ship->ShipY = startY;
    ship->horizontal = horizontal;
    int newShipX=startX;
    int newShipY=startY;
    switch (ship->ship_id)
    {
    case 8:
        player_one.fleet.destroyer.ShipX=newShipX;
        player_one.fleet.destroyer.ShipY=newShipY;
        player_one.fleet.destroyer.horizontal=horizontal;
    break;
    case 9:
          player_one.fleet.torpedo_boat.ShipX=newShipX;
          player_one.fleet.torpedo_boat.ShipY=newShipY;
            player_one.fleet.torpedo_boat.horizontal=horizontal;
    break;
    case 10:
          player_one.fleet.submarine.ShipX=newShipX;
          player_one.fleet.submarine.ShipY=newShipY;
            player_one.fleet.submarine.horizontal=horizontal;
    break;
    case 12:
          player_one.fleet.aircraft_carrier.ShipX=newShipX;
          player_one.fleet.aircraft_carrier.ShipY=newShipY;
            player_one.fleet.aircraft_carrier.horizontal=horizontal;
    break;
    case 13:
          player_one.fleet.cruiser.ShipX=newShipX;
          player_one.fleet.cruiser.ShipY=newShipY;
            player_one.fleet.cruiser.horizontal=horizontal;
    break;
    }
    
}

// Function to draw a fleet on the grid
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
        if (ship->name[0] != '\0') {  // Check if the ship is defined
            DrawShips(grid, ship, ship->isDragging);
        }
    }
}


void ShowPlaceBoat(SDL_Window* window, SDL_Renderer* renderer) {
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

    Grid grid;
    grid.renderer = renderer;
    grid.gridWidth = 10;
    grid.gridHeight = 10;
    grid.cellSize = 41;
    grid.xPos = 90;
    grid.yPos = 40;

    // Load the grid background image
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
        printf("Failed to create grid texture: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return;
    }
    // Initialize the fleet
    Fleet fleet;
    fleet=player_one.fleet;
    // Initialize ships here
    srand(time(NULL));
    // Randomly place 
    RandomizeShipPlacement(&grid, &fleet.destroyer);
    RandomizeShipPlacement(&grid, &fleet.torpedo_boat);
    RandomizeShipPlacement(&grid, &fleet.submarine);
    RandomizeShipPlacement(&grid, &fleet.aircraft_carrier);
    RandomizeShipPlacement(&grid, &fleet.cruiser);
    PlaceShipInGrid(&fleet.torpedo_boat,player_one_grid,0,0,&fleet.torpedo_boat.horizontal);
    PlaceShipInGrid(&fleet.destroyer,player_one_grid,0,0,&fleet.destroyer.horizontal);
    PlaceShipInGrid(&fleet.submarine,player_one_grid,0,0,&fleet.submarine.horizontal);
    PlaceShipInGrid(&fleet.aircraft_carrier,player_one_grid,0,0,&fleet.aircraft_carrier.horizontal);
    PlaceShipInGrid(&fleet.cruiser,player_one_grid,0,0,&fleet.cruiser.horizontal);
    bool running = true;
    SDL_Event event;
    bool dragging = false;
    Ships *currentShip = NULL;
    int offsetX = 0, offsetY = 0;
    int width = 0; 
    int height = 0; 
    SDL_Color textColor = {255, 255, 255, 255};
    CreateClickableElement(renderer,214,520,&width,&height, "Play", textColor,"medias/images/btn-play.png",PlayGame,12);
    while (running) {
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;

            // Clic gauche pour sélectionner et déplacer le navire
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    int mouseX = event.button.x;
                    int mouseY = event.button.y;
                
                    TriggerClickCallbacks(mouseX, mouseY);

                    Ships *ships[] = {
                        &fleet.destroyer,
                        &fleet.torpedo_boat,
                        &fleet.submarine,
                        &fleet.aircraft_carrier,
                        &fleet.cruiser
                    };

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
                            printf("ship selected");
                            currentShip = ship;
                            offsetX = mouseX - shipRect.x;
                            offsetY = mouseY - shipRect.y;
                            break;
                        }
                    }
                }
                // Clic droit pour changer l'orientation du navire
                if (event.button.button == SDL_BUTTON_RIGHT && currentShip) {
                    printf("inverse l orientation");
                    currentShip->horizontal = !currentShip->horizontal;  // Inverser l'orientation
                }
                break;

            // Relâcher le bouton gauche pour arrêter de déplacer le navire
            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    dragging = false;
                    currentShip = NULL;
                }
                break;

            // Mouvement de la souris pour déplacer le navire sélectionné
            case SDL_MOUSEMOTION:
                if (dragging && currentShip) {
                    int mouseX = event.motion.x - offsetX;
                    int mouseY = event.motion.y - offsetY;
                    UpdateShipPosition(&grid, currentShip, mouseX, mouseY, &fleet);
                }
                break;
        }
    }
    DrawGrid(&grid);
    DrawFleet(&grid, &fleet);
    SDL_RenderPresent(renderer);
}

    SDL_DestroyTexture(grid.backgroundImage);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

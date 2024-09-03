#include <SDL2/SDL_image.h>
#include "../headers/damier.h"
#include "../headers/graphics.h"
#include "../headers/ships.h"
#include "../headers/fleet.h"
#include <stdbool.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include "../headers/boat_on_grid.h"

#define MAX_SHIPS 10

ClickableArea ships[MAX_SHIPS];
Ships* the_ships[MAX_SHIPS];
SDL_Texture* shipTextures[MAX_SHIPS]; // Stocke les textures des bateaux
int shipCount = 0;
ClickableArea* currentDragArea = NULL;
int dragOffsetX = 0;
int dragOffsetY = 0;
Grid* the_grid;
// Fonction pour redessiner la grille entière
void RedrawGrid() {
    // Parcourez toutes les cellules de la grille et redessinez-les
    DrawGrid(&the_grid);
    // Mettez à jour l'écran
    SDL_RenderPresent(first_renderer);
}

// Fonction pour dessiner un bateau sur le damier
void onClick(SDL_Event *event) {
    printf("Bateau cliqué à la position (%d, %d)\n", event->button.x, event->button.y);
}

void onHover(SDL_Event *event) {
    printf("Survol à la position (%d, %d)\n", event->motion.x, event->motion.y);
}

void onDragStart(SDL_Event *event) {
    printf("Début du drag à la position (%d, %d)\n", event->button.x, event->button.y);
    for (int i = 0; i < shipCount; i++) {
        if (event->button.x >= ships[i].x && event->button.x <= ships[i].x + ships[i].width &&
            event->button.y >= ships[i].y && event->button.y <= ships[i].y + ships[i].height) {
            currentDragArea = &ships[i];
            dragOffsetX = event->button.x - ships[i].x;
            dragOffsetY = event->button.y - ships[i].y;
            break;
        }
    }
}

void onDragMove(SDL_Event *event, Grid* grid) {
    if (currentDragArea != NULL) {
        // Calculer la position de la souris sur la grille
        int gridX = (event->motion.x - dragOffsetX) / grid->cellSize;
        int gridY = (event->motion.y - dragOffsetY) / grid->cellSize;

        // Calculer la nouvelle position du bateau en pixels
        int newX = grid->xPos + gridX * grid->cellSize;
        int newY = grid->yPos + gridY * grid->cellSize;

        // Vérifier si la nouvelle position est différente de l'ancienne
        if (newX != currentDragArea->x || newY != currentDragArea->y) {
            // Mettre à jour la position du bateau
            currentDragArea->x = newX;
            currentDragArea->y = newY;

            // Redessiner la grille et les bateaux
            RedrawGrid();
        }
    }
}

void onDragEnd(SDL_Event *event) {
    printf("Fin du drag à la position (%d, %d)\n", event->button.x, event->button.y);
    currentDragArea = NULL;
}

void LoadShipTexture(Ships* ship, SDL_Renderer* renderer) {
    if (shipCount >= MAX_SHIPS) {
        printf("Nombre maximum de bateaux atteint\n");
        return;
    }

    // Charger l'image du bateau à partir du fichier
    SDL_Surface* surface = IMG_Load("medias/images/ships/cruiser-180.png");
    if (!surface) {
        printf("Erreur lors du chargement de l'image: %s\n", IMG_GetError());
        return;
    }

    // Créer une texture à partir de la surface
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) {
        printf("Erreur lors de la création de la texture: %s\n", SDL_GetError());
        return;
    }

    // Stocker la texture dans le tableau
    shipTextures[shipCount] = texture;
}

void DrawShip(Ships* ship, int startX, int startY, bool horizontal, SDL_Renderer* renderer, int cellSize) {
    if (shipCount >= MAX_SHIPS) {
        printf("Nombre maximum de bateaux atteint\n");
        return;
    }

    // Charger la texture du bateau si ce n'est pas déjà fait
    LoadShipTexture(ship, renderer);

    // Obtenir les dimensions de l'image
    int imgWidth, imgHeight;
    SDL_QueryTexture(shipTextures[shipCount], NULL, NULL, &imgWidth, &imgHeight);

    // Calculer les dimensions du rectangle où dessiner le bateau
    SDL_Rect destRect;
    destRect.x = startX;
    destRect.y = startY;

    if (horizontal) {
        destRect.w = ship->n_cases * cellSize;
        destRect.h = cellSize;
    } else {
        destRect.w = cellSize;
        destRect.h = ship->n_cases * cellSize;
    }

    // Initialiser la zone cliquable pour le bateau
    ClickableArea shipArea = {
        .x = destRect.x,
        .y = destRect.y,
        .width = destRect.w,
        .height = destRect.h,
        .onClick = onClick,
        .onHover = onHover,
        .onDragStart = onDragStart,
        .onDragMove = onDragMove,
        .onDragEnd = onDragEnd
    };

    // Ajouter la zone cliquable à la liste des bateaux
    ships[shipCount] = shipArea;
    the_ships[shipCount++] = ship;
    RegisterClickableArea(shipArea);

    // Dessiner la texture du bateau à l'intérieur du rectangle
    SDL_RenderCopy(renderer, shipTextures[shipCount - 1], NULL, &destRect);

    // Mettre à jour l'écran
    SDL_RenderPresent(renderer);
}

void DrawOnGrid(Grid* grid, Ships* ship, int col, int row, bool horizontal) {
    int startX = grid->xPos + col * grid->cellSize;
    int startY = grid->yPos + row * grid->cellSize;
    int cellSize = grid->cellSize;
    the_grid=&grid;
    // Vérifier si le bateau peut être dessiné à cette position
    int shipWidth = horizontal ? ship->n_cases * cellSize : cellSize;
    int shipHeight = horizontal ? cellSize : ship->n_cases * cellSize;

    // Assurez-vous que le bateau ne dépasse pas les limites de la grille
    if (col + (horizontal ? ship->n_cases - 1 : 0) >= grid->gridWidth || 
        row + (horizontal ? 0 : ship->n_cases - 1) >= grid->gridHeight) {
        printf("Le bateau dépasse les limites de la grille\n");
        return;
    }

    // Dessiner le bateau à la position calculée
    DrawShip(ship, startX, startY, horizontal, grid->renderer, cellSize);
}

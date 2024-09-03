#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include "headers/damier.h"
#include <SDL2/SDL_ttf.h>
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

// Fonction pour initialiser SDL
bool initSDL(SDL_Window **window, SDL_Renderer **renderer) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    *window = SDL_CreateWindow("Drag and Drop Example",
                               SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED,
                               WINDOW_WIDTH, WINDOW_HEIGHT,
                               SDL_WINDOW_SHOWN);
    if (!*window) {
        SDL_Log("Could not create window: %s", SDL_GetError());
        SDL_Quit();
        return false;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!*renderer) {
        SDL_Log("Could not create renderer: %s", SDL_GetError());
        SDL_DestroyWindow(*window);
        SDL_Quit();
        return false;
    }

    // Initialiser SDL_image
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        SDL_Log("Could not initialize SDL_image: %s", IMG_GetError());
        SDL_DestroyRenderer(*renderer);
        SDL_DestroyWindow(*window);
        SDL_Quit();
        return false;
    }

    return true;
}

// Fonction pour charger une image
SDL_Texture* loadTexture(const char *file, SDL_Renderer *renderer) {
    SDL_Texture *texture = IMG_LoadTexture(renderer, file);
    if (!texture) {
        SDL_Log("Failed to load texture %s: %s", file, IMG_GetError());
    }
    return texture;
}

// Fonction pour dessiner un rectangle coloré (pour indication de sélection)
void DrawSelectionRect(SDL_Renderer *renderer, int x, int y, int width, int height, bool isDragging) {
    SDL_Rect rect = {x, y, width, height};
    if (isDragging) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 128); // Vert semi-transparent
        SDL_RenderFillRect(renderer, &rect);
    } else {
        //SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); // Transparent
    }
}

// Fonction pour dessiner le bateau
void DrawShips(Grid *grid, SDL_Texture *shipTexture, int shipLength, int shipX, int shipY, bool horizontal, bool isDragging) {
    SDL_Rect boatRect = {
        grid->xPos + grid->cellSize * shipX,
        grid->yPos + grid->cellSize * shipY,
        horizontal ? grid->cellSize * shipLength : grid->cellSize,
        horizontal ? grid->cellSize : grid->cellSize * shipLength
    };

    // Dessiner le rectangle coloré si en cours de drag
    DrawSelectionRect(grid->renderer, boatRect.x, boatRect.y, boatRect.w, boatRect.h, isDragging);

    // Dessiner le bateau (avec image)
    SDL_RenderCopy(grid->renderer, shipTexture, NULL, &boatRect);
}

// Fonction pour calculer la position "snap" du bateau sur la grille
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

// Fonction pour dessiner la grille

int main(int argc, char *argv[]) {
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *shipTexture = NULL;

    if (!initSDL(&window, &renderer)) {
        return 1;
    }
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

    // Charger l'image de fond de la grille
    SDL_Surface* bgSurface = IMG_Load("medias/images/oceangrid.png");
    if (!bgSurface) {
        printf("Failed to load grid image: %s\n", IMG_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    grid.backgroundImage = SDL_CreateTextureFromSurface(renderer, bgSurface);
    SDL_FreeSurface(bgSurface);

    if (!grid.backgroundImage) {
        printf("Failed to create grid texture: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // Charger l'image du bateau
    shipTexture = loadTexture("medias/images/ships/cruiser-180.png", renderer);
    if (!shipTexture) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    bool running = true;
    SDL_Event event;
    bool dragging = false;
    bool horizontal = true; // Orientation initiale du bateau
    int shipX = 3, shipY = 2;
    int shipLength = 5;
    int offsetX = 0, offsetY = 0;
    int boatRectX = grid.xPos + grid.cellSize * shipX;
    int boatRectY = grid.yPos + grid.cellSize * shipY;

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
                        boatRectX = grid.xPos + grid.cellSize * shipX;
                        boatRectY = grid.yPos + grid.cellSize * shipY;
                        int boatRectWidth = horizontal ? grid.cellSize * shipLength : grid.cellSize;
                        int boatRectHeight = horizontal ? grid.cellSize : grid.cellSize * shipLength;
                        SDL_Rect boatRect = {boatRectX, boatRectY, boatRectWidth, boatRectHeight};
                        if (SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &boatRect)) {
                            dragging = true;
                            offsetX = mouseX - boatRectX;
                            offsetY = mouseY - boatRectY;
                        }
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        dragging = false;
                        // Snap à la grille après relâchement
                        SnapToGrid(&grid, &boatRectX, &boatRectY, shipLength, horizontal);
                        shipX = (boatRectX - grid.xPos) / grid.cellSize;
                        shipY = (boatRectY - grid.yPos) / grid.cellSize;
                    }
                    break;
                case SDL_MOUSEMOTION:
                    if (dragging) {
                        int mouseX = event.motion.x - offsetX;
                        int mouseY = event.motion.y - offsetY;
                        // Snap pendant le glisser
                        SnapToGrid(&grid, &mouseX, &mouseY, shipLength, horizontal);
                        boatRectX = mouseX; // Mise à jour de la position du bateau pendant le drag
                        boatRectY = mouseY;
                    }
                    break;
            }
        }

        // Effacer l'écran
        //SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        //SDL_RenderClear(renderer);

        // Dessiner la grille avec l'image de fond
        DrawGrid(&grid);

        // Dessiner le bateau sur la grille
        DrawShips(&grid, shipTexture, shipLength, shipX, shipY, horizontal, dragging);

        // Mettre à jour l'affichage
        SDL_RenderPresent(renderer);
    }

    // Nettoyer et quitter
    SDL_DestroyTexture(shipTexture);
    SDL_DestroyTexture(grid.backgroundImage);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}

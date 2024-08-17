#include "../../headers/graphics.h"
#include "../../headers/events.h"
#include "../../headers/log.h"
#include "../../headers/damier.h" // Include the new grid module
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

// Callback function for handling cell clicks
void OnCellClick(Cell cell, int x, int y) {
    // Log which cell was clicked and the event coordinates
    printf("Cell clicked: Row %d, Col %d at coordinates (%d, %d)\n", cell.row, cell.col, x, y);
}

void ShowPlaceBoat(SDL_Window* window, SDL_Renderer* renderer) {
    // Initialize log
    InitLogFile("logs.txt");

    // Set background color (black)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Load and render the background texture
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

    // Set up the grid
    Grid grid;
    grid.renderer = renderer;
    grid.gridWidth = 10;  // 10 columns
    grid.gridHeight = 10; // 10 rows
    grid.cellSize = 41;   // Each cell is 41x41 pixels
    grid.xPos = 90;       // X position of the grid
    grid.yPos = 40;       // Y position of the grid

    // Load the grid background image (e.g., "oceangrid.png") as a texture
    SDL_Surface* bgSurface = IMG_Load("medias/images/oceangrid.png");
    if (!bgSurface) {
        printf("Failed to load grid image: %s\n", IMG_GetError());
        return;
    }
    grid.backgroundImage = SDL_CreateTextureFromSurface(renderer, bgSurface);
    SDL_FreeSurface(bgSurface);

    if (!grid.backgroundImage) {
        printf("Failed to create grid texture: %s\n", SDL_GetError());
        return;
    }

    // Assign the callback function for handling cell clicks
    grid.onCellClick = OnCellClick;

    // Draw the grid with the background
    DrawGrid(&grid);

    // Free the grid background image texture
    SDL_DestroyTexture(grid.backgroundImage);

    // Present everything on the screen
    SDL_RenderPresent(renderer);
}

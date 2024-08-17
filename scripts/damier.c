#include "../headers/events.h"
#include "../headers/damier.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>



void DrawGrid(Grid* grid) {
    int cellSize = grid->cellSize;
    int gridWidth = grid->gridWidth * cellSize;
    int gridHeight = grid->gridHeight * cellSize;
    int xPos = grid->xPos;
    int yPos = grid->yPos;

    // Draw the background image if provided
    if (grid->backgroundImage) {
        SDL_Rect bgRect = {xPos, yPos, gridWidth, gridHeight};
        SDL_RenderCopy(grid->renderer, grid->backgroundImage, NULL, &bgRect);
    }

    // Set grid color (white lines)
    SDL_SetRenderDrawColor(grid->renderer, 255, 255, 255, 255);

    // Draw vertical lines
    for (int i = 0; i <= grid->gridWidth; i++) {
        int x = xPos + i * cellSize;
        SDL_RenderDrawLine(grid->renderer, x, yPos, x, yPos + gridHeight);
    }

    // Draw horizontal lines
    for (int i = 0; i <= grid->gridHeight; i++) {
        int y = yPos + i * cellSize;
        SDL_RenderDrawLine(grid->renderer, xPos, y, xPos + gridWidth, y);
    }

    // Load font for labeling
    TTF_Font* font = TTF_OpenFont("medias/font/Sora.ttf", 20);
    if (!font) {
        LogMessage("Failed to load font: %s\n", TTF_GetError());
        return;
    }

    SDL_Color textColor = {255, 255, 255};  // White text

    // Labels for numbers (1 to gridWidth)
    for (int i = 1; i <= grid->gridWidth; i++) {
        char number[3];
        snprintf(number, sizeof(number), "%d", i);

        SDL_Surface* textSurface = TTF_RenderText_Solid(font, number, textColor);
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(grid->renderer, textSurface);

        SDL_Rect textRect = {
            xPos + (i - 1) * cellSize + (cellSize / 2) - (textSurface->w / 2),
            yPos + gridHeight + 5,
            textSurface->w, textSurface->h
        };
        SDL_RenderCopy(grid->renderer, textTexture, NULL, &textRect);

        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
    }

    // Labels for letters (A to corresponding gridHeight)
    for (int i = 0; i < grid->gridHeight; i++) {
        char letter[2];
        letter[0] = 'A' + i;
        letter[1] = '\0';

        SDL_Surface* textSurface = TTF_RenderText_Solid(font, letter, textColor);
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(grid->renderer, textSurface);

        SDL_Rect textRect = {
            xPos - textSurface->w - 5,
            yPos + i * cellSize + (cellSize / 2) - (textSurface->h / 2),
            textSurface->w, textSurface->h
        };
        SDL_RenderCopy(grid->renderer, textTexture, NULL, &textRect);

        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
    }

    TTF_CloseFont(font);
}

Cell GetCellFromCoordinates(Grid* grid, int x, int y) {
    int cellX = (x - grid->xPos) / grid->cellSize;
    int cellY = (y - grid->yPos) / grid->cellSize;

    if (cellX >= 0 && cellX < grid->gridWidth && cellY >= 0 && cellY < grid->gridHeight) {
        Cell cell = {cellY + 1, cellX + 1};
        return cell;
    } else {
        Cell cell = {-1, -1};  // Invalid cell
        return cell;
    }
}

void HandleGridClick(Grid* grid, int x, int y) {
    Cell cell = GetCellFromCoordinates(grid, x, y);
    if (cell.row != -1 && cell.col != -1 && grid->onCellClick) {
        grid->onCellClick(cell, x, y);
    }
}

void ColorCell(Grid* grid, Cell cell, SDL_Color color) {
    int cellSize = grid->cellSize;
    int xPos = grid->xPos + (cell.col - 1) * cellSize;
    int yPos = grid->yPos + (cell.row - 1) * cellSize;

    SDL_SetRenderDrawColor(grid->renderer, color.r, color.g, color.b, color.a);
    SDL_Rect cellRect = {xPos, yPos, cellSize, cellSize};
    SDL_RenderFillRect(grid->renderer, &cellRect);
}


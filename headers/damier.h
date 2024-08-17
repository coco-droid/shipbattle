#ifndef GRID_H
#define GRID_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

// Struct to represent a cell in the grid
typedef struct {
    int row;
    int col;
} Cell;

// Callback type for handling cell click events
typedef void (*CellClickCallback)(Cell cell, int x, int y);

// Struct to represent the grid and its properties
typedef struct {
    SDL_Renderer* renderer;       // Renderer for drawing the grid
    int gridWidth;                // Number of columns in the grid
    int gridHeight;               // Number of rows in the grid
    int cellSize;                 // Size of each cell (cells are squares)
    int xPos;                     // X position of the top-left corner of the grid
    int yPos;                     // Y position of the top-left corner of the grid
    SDL_Texture* backgroundImage; // Optional background image for the grid
    CellClickCallback onCellClick;// Callback for cell click events
} Grid;

// Function to draw the grid
void DrawGrid(Grid* grid);

// Function to get the grid cell from x, y coordinates
Cell GetCellFromCoordinates(Grid* grid, int x, int y);

// Function to handle a click event on the grid
void HandleGridClick(Grid* grid, int x, int y);

// Function to color a specific cell in the grid
void ColorCell(Grid* grid, Cell cell, SDL_Color color);

#endif // GRID_H

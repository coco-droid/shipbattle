#ifndef BOAT_ON_GRID_H
#define BOAT_ON_GRID_H
#include "./fleet.h"
#include "./damier.h"
#include "./ships.h"
#include <stdbool.h>
#include <stdio.h>
#include <SDL2/SDL.h>
void DrawOnGrid(Grid* grid, Ships* ship, int col, int row, bool horizontal);
/*void DrawFleet(Grid* grid, Fleet* fleet);
void place_ships_randomly(Grid* grid, Fleet* fleet);
void Computer_mode(SDL_Window* window, SDL_Renderer* renderer);*/
#endif 
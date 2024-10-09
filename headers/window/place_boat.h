#ifndef PLACE_BOAT.h
#define PLACE_BOAT.h 

#include <SDL2/SDL.h> 
#include "../damier.h"
#include "../player.h"
#include "../fleet.h"
void ShowPlaceBoat (SDL_Window* Window,SDL_Renderer* Renderer); 
void DrawFleet(Grid *grid, Fleet *fleet);
#endif 

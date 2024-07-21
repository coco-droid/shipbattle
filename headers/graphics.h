#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <SDL2/SDL.h>

void SetRoundedCorners(SDL_Window* window);
SDL_Window* CreateRoundedWindow(const char* title, int x, int y, int w, int h, Uint32 flags);

#endif // GRAPHIC_H

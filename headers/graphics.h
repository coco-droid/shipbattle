#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL2/SDL.h>

// Function prototypes
SDL_Window* CreateRoundedWindow(const char *title, int x, int y, int w, int h, Uint32 flags);
void CreateClickableElement(SDL_Renderer* renderer, int x, int y, int w, int h, const char* text, SDL_Color textColor, const char* imagePath, void (*callback)(void));

#endif // GRAPHICS_H

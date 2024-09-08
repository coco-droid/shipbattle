#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL2/SDL.h>
#include "./events.h"
extern SDL_Window *first_window;
extern SDL_Renderer *first_renderer;
extern SDL_Window *second_window;
extern SDL_Renderer *second_renderer;
// Function prototypes
SDL_Window* CreateRoundedWindow(const char *title, int x, int y, int w, int h, Uint32 flags);
void CreateClickableElement(SDL_Renderer* renderer, int x, int y, int* w, int* h, const char* text, SDL_Color textColor, const char* imagePath, EventCallback callback, int fontSize);
void RenderText(SDL_Renderer* renderer, const char* text, int x, int y, SDL_Color color, int fontSize, int maxWidth);
void OpenPlayWindow(SDL_Window* window1, SDL_Renderer* renderer1, SDL_Window** window2, SDL_Renderer** renderer2);
#endif // GRAPHICS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include "headers/graphics.h"
#include "headers/window.h"
#include "headers/events.h"
#include "headers/log.h"

// Define window dimensions
const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 600;
int main(int argc, char *argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL Initialization Error: %s\n", SDL_GetError());
        return -1;
    }

    // Initialize SDL_image
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        SDL_Log("SDL_image Initialization Error: %s\n", IMG_GetError());
        SDL_Quit();
        return -1;
    }

    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        SDL_Log("TTF Initialization Error: %s\n", TTF_GetError());
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    // Create a window with rounded corners
    SDL_Window *window = CreateRoundedWindow("Rounded Corner Window",
                                             SDL_WINDOWPOS_UNDEFINED,
                                             SDL_WINDOWPOS_UNDEFINED,
                                             WINDOW_WIDTH,
                                             WINDOW_HEIGHT,
                                             SDL_WINDOW_BORDERLESS | SDL_WINDOW_ALWAYS_ON_TOP);
    if (!window) {
        SDL_Log("Window Creation Error: %s\n", SDL_GetError());
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    // Create renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        SDL_Log("Renderer Creation Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    // Set the blend mode to allow transparency
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // Initialize events
    InitEvents();

    // Declare and initialize the alive flag
    int aliveFlag = 1;
    alive = &aliveFlag;

    // Show the startup menu
    ShowStartupMenu(window, renderer);

    // Main loop
    while (*alive) {
            ProcessEvents(window, renderer);
    }

    // Cleanup and quit
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}

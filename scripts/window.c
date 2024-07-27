#include "../headers/window.h"
#include "../headers/graphics.h"
#include "../headers/events.h"
#include <SDL2/SDL_image.h>
#include <stdio.h>

FILE *logFile;

// Définition du pointeur 'alive'

void Button1Callback(SDL_Event *event) {
    if (event->type == SDL_MOUSEBUTTONDOWN) {
        SDL_Log("Button 1 clicked!\n");
        if (alive) {
            *alive = 0; // Change la valeur du pointeur 'alive' au clic
        }
    }
}

void Button2Callback(SDL_Event *event) {
    if (event->type == SDL_MOUSEBUTTONDOWN) {
        SDL_Log("Button 2 clicked!\n");
        if (alive) {
            *alive = 0; // Change la valeur du pointeur 'alive' au clic
        }
    }
}

void ShowStartupMenu(SDL_Window* window, SDL_Renderer* renderer) {
    // Initialize log
    InitLogFile("logs.txt");

    // Load the background image
    SDL_Surface *backgroundSurface = IMG_Load("medias/images/bg-first.png");
    if (!backgroundSurface) {
        LogMessage("Image Load Error: %s\n", IMG_GetError());
        return;
    }
    LogMessage("Background image loaded.\n");

    SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface);
    if (!backgroundTexture) {
        LogMessage("Texture Creation Error: %s\n", SDL_GetError());
        return;
    }
    LogMessage("Texture created.\n");
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Render the background texture
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

        // Present the renderer
        SDL_RenderPresent(renderer);
    // Register event callbacks
    RegisterEventCallback(SDL_MOUSEBUTTONDOWN, Button1Callback);
    RegisterEventCallback(SDL_MOUSEBUTTONDOWN, Button2Callback);

    // Initialize the alive flag
    int aliveFlag = 1;
    alive = &aliveFlag;

    // Main loop to handle events and render
    while (*alive) {
        // Handle events
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                *alive = 0; // Exit the loop if the window is closed
            }
            ProcessEvents(window, renderer);
        }
        // Clear the screen with a black color
    }

    // Cleanup
    SDL_DestroyTexture(backgroundTexture);
    CloseLogFile();
}

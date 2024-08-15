#include "../../headers/window/startup.h"
#include "../../headers/graphics.h"
#include "../../headers/events.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "../../headers/window/dialog_1.h"
#include <stdio.h>



// Définition du pointeur 'alive'

void Button1Callback(SDL_Event *event) {
    InitLogFile("logs.txt");
    if (event->type == SDL_MOUSEBUTTONDOWN) {
        LogMessage("Button 1 clicked!\n");
        Dialog_1(first_window,first_renderer);
        ClearEvents();
    }
}

void Button2Callback(SDL_Event *event) {
     InitLogFile("logs.txt");
    if (event->type == SDL_MOUSEBUTTONDOWN) {
        //LogMessage("Button 2 clicked!\n");
    }
}
void Button3Callback(SDL_Event *event) {
    InitLogFile("logs.txt");
    LogMessage('button click');
    if (event->type == SDL_MOUSEBUTTONDOWN) {
         if (alive) {
                    *alive = 0;
                }
    }
}
void ShowStartupMenu(SDL_Window* window, SDL_Renderer* renderer) {
    // Initialize log
    InitLogFile("logs.txt");
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Set to black color
    SDL_RenderClear(renderer);
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
        
    // Register event callbacks
   /* RegisterEventCallback(SDL_MOUSEBUTTONDOWN, Button1Callback);
    RegisterEventCallback(SDL_MOUSEBUTTONDOWN, Button2Callback);
    RegisterEventCallback(SDL_MOUSEBUTTONDOWN, Button3Callback);*/
    // Initialize the alive flag
    int aliveFlag = 1;
    alive = &aliveFlag;
    int width = 0; 
    int height = 0; 
    int width2 = 35; 
    int height2 = 37; 
    int width3 = 30; 
    int height3 = 30; 
    SDL_Color textColor = {255, 255, 255, 255};
    CreateClickableElement(renderer,214,520,&width,&height, "Play", textColor,"medias/images/btn-play.png",Button1Callback,12);
    CreateClickableElement(renderer,16,555,&width2,&height2,NULL, textColor,"medias/images/btn-settings.png",Button2Callback,12);
    CreateClickableElement(renderer,558,11,&width3,&height3,NULL, textColor,"medias/images/btn-close.png",Button3Callback,12);
    SDL_RenderPresent(renderer);
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

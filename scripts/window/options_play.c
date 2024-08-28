#include "../../headers/load_text.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>  // Include SDL_ttf for text rendering
#include <stdio.h>
#include <string.h>
#include"../../headers/window/options_play.h" 
#include "../../headers/events.h"
#include "../../headers/log.h"
#include "../../headers/window/place_boat.h"
#include "../../headers/graphics.h"
#include "../../headers/computer_mode.h"
void BeforeCallback2(SDL_Event *event) {
    InitLogFile("logs.txt");
    if (event->type == SDL_MOUSEBUTTONDOWN) {
        LogMessage("Button 1 clicked!\n");
        //ShowStartupMenu(first_window,first_renderer);
        ClearEvents();
    }
}
void ComputerCallback(SDL_Event *event)
{
    InitLogFile("logs.txt");
    if (event->type == SDL_MOUSEBUTTONDOWN) {
        LogMessage("Computer clicked!\n");
        ClearEvents();
        Computer_mode(first_window,first_renderer);
    }
}
void AfterCallback2(SDL_Event *event) {
    InitLogFile("logs.txt");
    if (event->type == SDL_MOUSEBUTTONDOWN) {
        LogMessage("Button 2 clicked!\n");
    }
}
void CloseCallback2(SDL_Event *event) {
    InitLogFile("logs.txt");
    if (event->type == SDL_MOUSEBUTTONDOWN) {
       *alive=0;
    }
}
void ShowOption(SDL_Window* Window,SDL_Renderer* Renderer) 
{
    InitLogFile("logs.txt");
    LogMessage("Image Load Error:");
    SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
    SDL_RenderClear(Renderer);
      SDL_Surface *backgroundSurface = IMG_Load("medias/images/bg-first.png");
    if (!backgroundSurface) {
        LogMessage("Image Load Error: %s\n", IMG_GetError());
        return;
    }
    LogMessage("Background image loaded.\n");

    SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(Renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface);
    if (!backgroundTexture) {
        LogMessage("Texture Creation Error: %s\n", SDL_GetError());
        return;
    }
    LogMessage("Texture created.\n");
    SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
    SDL_RenderClear(Renderer);
    // Render the background texture
    SDL_RenderCopy(Renderer, backgroundTexture, NULL, NULL);
    int width3 = 30;
    int height3 = 30;
    int width2 = 0;
    int height2 = 0;
    SDL_Color textColor = {255, 255, 255, 255}; // White color
    CreateClickableElement(Renderer, 558, 11, &width3, &height3, NULL, textColor, "medias/images/btn-close.png", CloseCallback2, 12);
    CreateClickableElement(Renderer, 12, 11, &width3, &height3, NULL, textColor, "medias/images/left-arrow.png", BeforeCallback2, 12);
    CreateClickableElement(Renderer,220, 158, &width2, &height2,"Computer", textColor, "medias/images/square-btn.png", ComputerCallback, 12);
    CreateClickableElement(Renderer,220, 220, &width2, &height2,"Local user", textColor, "medias/images/square-btn.png", BeforeCallback2, 12);
    CreateClickableElement(Renderer,220, 285, &width2, &height2,"Online user", textColor, "medias/images/square-btn.png", BeforeCallback2, 12);
    SDL_RenderPresent(Renderer);
        int quit = 0;
    while (!quit) {
            ProcessEvents(Window,Renderer);     
    }
    
}
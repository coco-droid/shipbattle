#include "../../headers/load_text.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>  // Include SDL_ttf for text rendering
#include <stdio.h>
#include <string.h>
#include"../../headers/window/options_play.h" 

void ShowOption(SDL_Window* Window,SDL_Renderer* Renderer) 
{
    InitLogFile("logs.txt");
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
}
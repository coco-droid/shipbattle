#include "../../headers/events.h"
#include "../../headers/log.h"
#include "../../headers/graphics.h"
#include "../../headers/window/launch_party.h"
#include "../../headers/window/startup.h"
#include "../../headers/window/options_play.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string.h>
#include "../../headers/sockets/server.h"

void Launch_party(SDL_Window* Window, SDL_Renderer* Renderer) {
    InitLogFile("logs.txt");
    LogMessage("Loading Player Name Input Screen\n");

    SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
    SDL_RenderClear(Renderer);

    SDL_Surface *backgroundSurface = IMG_Load("medias/images/bg-first.png");
    if (!backgroundSurface) {
        LogMessage("Image Load Error: %s\n", IMG_GetError());
        return;
    }

    SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(Renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface);
    if (!backgroundTexture) {
        LogMessage("Texture Creation Error: %s\n", SDL_GetError());
        return;
    }

    SDL_RenderCopy(Renderer, backgroundTexture, NULL, NULL);

    SDL_Surface *inputSurface = IMG_Load("medias/images/input-txt.png");
    if (!inputSurface) {
        LogMessage("Input Background Image Load Error: %s\n", IMG_GetError());
        return;
    }
    SDL_Texture *inputBackgroundTexture = SDL_CreateTextureFromSurface(Renderer, inputSurface);
    SDL_FreeSurface(inputSurface);
    if (!inputBackgroundTexture) {
        LogMessage("Input Background Texture Creation Error: %s\n", SDL_GetError());
        return;
    }

    SDL_Rect inputRect = {150, 150, 300, 50};
    TTF_Font *font = TTF_OpenFont("medias/font/Sora.ttf", 24);
    if (!font) {
        LogMessage("Font Load Error: %s\n", TTF_GetError());
        return;
    }

    SDL_Color textColor = {255, 255, 255, 255};  

}

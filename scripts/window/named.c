#include "../../headers/events.h"
#include "../../headers/log.h"
#include "../../headers/graphics.h"
#include "../../headers/window/named.h"
#include "../../headers/window/startup.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string.h>

#define MAX_NAME_LENGTH 20

static char playerName[MAX_NAME_LENGTH] = "Your Name"; 

void SubmitNameCallback(const char *name) {
    InitLogFile("logs.txt");
    LogMessage("name valued");
}

void BeforeLeft(SDL_Event *event) {
    InitLogFile("logs.txt");
    if (event->type == SDL_MOUSEBUTTONDOWN) {
        LogMessage("Button 1 clicked!\n");
        ClearEvents();
        ShowStartupMenu(first_window, first_renderer);
    }
}

void ShowPlayerNameInput(SDL_Window* Window, SDL_Renderer* Renderer) {
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

    // Define the TextInput structure
    TextInput textInputArea = {
        .x = inputRect.x,
        .y = inputRect.y,
        .width = inputRect.w,
        .height = inputRect.h,
        .font = font,
        .textColor = textColor,
        .backgroundTexture = inputBackgroundTexture,
        .text = "Your Name",
        .isDefaultText = true,
        .onSubmit = SubmitNameCallback
    };

    // Register the text input area
    RegisterTextInputArea(&textInputArea);

    int width = 0;
    int height = 0;
    int width3 = 30;
    int height3 = 30;

    CreateClickableElement(Renderer, 220, 220, &width, &height, "Submit", textColor, "medias/images/square-btn.png", SubmitNameCallback, 12);
    CreateClickableElement(Renderer, 12, 11, &width3, &height3, NULL, textColor, "medias/images/left-arrow.png", BeforeLeft, 12);
SDL_RenderPresent(Renderer);
    // Main event loop
    SDL_Event e;
    int quit = 0;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }

            // Handle text input events
            ProcessEvents(Window,Renderer);
        }

        // Render the input area with the background and text
        RenderTextInputWithBackground(Renderer, inputBackgroundTexture);
        
    }
    
    SDL_DestroyTexture(inputBackgroundTexture);
    SDL_DestroyTexture(backgroundTexture);
    TTF_CloseFont(font);
}

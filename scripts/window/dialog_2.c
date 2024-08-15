#include "../../headers/window/dialog_2.h"
#include "../../headers/window/dialog_1.h"
#include "../../headers/window/startup.h"
#include "../../headers/graphics.h"
#include "../../headers/events.h"
#include "../../headers/load_text.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>  // Include SDL_ttf for text rendering
#include <stdio.h>
#include "../../headers/log.h"
#include "../../headers/window/options_play.h"
#include <string.h>
void BeforeCallbackD2(SDL_Event *event) {
    InitLogFile("logs.txt");
    if (event->type == SDL_MOUSEBUTTONDOWN) {
        LogMessage("Button 1 clicked!\n");
        ShowStartupMenu(first_window,first_renderer);
        ClearEvents();
    }
}

void AfterCallbackD2(SDL_Event *event) {
    InitLogFile("logs.txt");
    if (event->type == SDL_MOUSEBUTTONDOWN) {
        LogMessage("Button 2 clicked!\n");
        Dialog_1(first_window,first_renderer);
    }
}

void CloseCallbackD2(SDL_Event *event) {
    InitLogFile("logs.txt");
    if (event->type == SDL_MOUSEBUTTONDOWN) {
       *alive=0;
    }
}
void SkipCallbackD2 (SDL_Event *event){
    InitLogFile("logs.txt");
    if (event->type == SDL_MOUSEBUTTONDOWN) {
      ShowOption(first_window,first_renderer);
    }
}
void Dialog_2(SDL_Window* window, SDL_Renderer* renderer) {
    // Initialize log
    InitLogFile("logs.txt");
    // Main loop to handle events and render
    SDL_Event e;
    LogMessage("dialog 1 launch");
        // Clear the screen before rendering
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Set to black color
        SDL_RenderClear(renderer);

        // Render the background texture
        //SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
        while(*alive){
        // Load and render the first image (448px height)
        SDL_Surface *image1Surface = IMG_Load("medias/images/dialog_2.jpeg");
        if (!image1Surface) {
            LogMessage("Image Load Error: %s\n", IMG_GetError());
            return;
        }
        SDL_Texture *image1Texture = SDL_CreateTextureFromSurface(renderer, image1Surface);
        SDL_FreeSurface(image1Surface);
        SDL_Rect image1Rect = {0,0, 650, 500};  // Example position (x: 100, y: 100)
        SDL_RenderCopy(renderer, image1Texture, NULL, &image1Rect);
        SDL_DestroyTexture(image1Texture);

        // Load and render the second image (112px height)
        SDL_Surface *image2Surface = IMG_Load("medias/images/bottom-board.png");
        if (!image2Surface) {
            LogMessage("Image Load Error: %s\n", IMG_GetError());
            return;
        }
        SDL_Texture *image2Texture = SDL_CreateTextureFromSurface(renderer, image2Surface);
        SDL_FreeSurface(image2Surface);
        SDL_Rect image2Rect = {0, 490, 600, 122};  // Example position (x: 100, y: 600)
        SDL_RenderCopy(renderer, image2Texture, NULL, &image2Rect);
        const char* filename = "dialog_1"; // base filename
        const char* language = "en"; // language choice (e.g., "en" for English, "fr" for French)
        char* text = load_text(filename, language);
        // Render Lorem Ipsum text on the second image
        SDL_Color textColor = {255, 255, 255, 255};  // White text color
        RenderText(renderer,"Commander, battle is inevitable. Lead our fleet with precision and strength. The eyes of the world are upon us. Engage the enemy and secure victory.", image2Rect.x + 10, image2Rect.y + 10, textColor, 16, image2Rect.w - 20);

        SDL_DestroyTexture(image2Texture);

        // Render the buttons
        int width3 = 30;
        int height3 = 30;
        CreateClickableElement(renderer, 558, 11, &width3, &height3, NULL, textColor, "medias/images/btn-close.png", CloseCallbackD2, 12);
        CreateClickableElement(renderer, 12, 11, &width3, &height3, NULL, textColor, "medias/images/left-arrow.png", BeforeCallbackD2, 12);
        CreateClickableElement(renderer, 558, 558, &width3, &height3, NULL, textColor, "medias/images/right-arrow.png", AfterCallbackD2, 12);
        CreateClickableElement(renderer, 519, 558, &width3, &height3, NULL, textColor, "medias/images/left2-arrow.png", AfterCallbackD2, 12);
        CreateClickableElement(renderer, 13, 562, &width3, &height3, NULL, textColor, "medias/images/skip.png",SkipCallbackD2, 12);
        // Present the rendered content on the screen
        SDL_RenderPresent(renderer);

        // Handle events
       while (SDL_PollEvent(&e) != 0 && alive) {
            if (e.type == SDL_QUIT) {
                *alive = 0; // Exit the loop if the window is closed
            }
            ProcessEvents(window, renderer);
        }
    }
    // Cleanup
   
   CloseLogFile();
}

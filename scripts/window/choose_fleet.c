#include "../../headers/load_text.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>  // Include SDL_ttf for text rendering
#include <stdio.h>
#include <string.h>
#include"../../headers/window/options_play.h" 
#include "../../headers/window/choose_fleet.h"
#include "../../headers/player.h"
#include "../../headers/fleet.h"
#include"../../headers/window/named.h"
#include "../../headers/events.h"
#include "../../headers/log.h"
#include "../../headers/window/place_boat.h"
#include "../../headers/logics.h"
#include "../../headers/window/dialog_1.h"
#include "../../headers/graphics.h"
#include <stdio.h>
void BeforeChooseFleet(SDL_Event *event) {
    InitLogFile("logs.txt");
    if (event->type == SDL_MOUSEBUTTONDOWN) {
        LogMessage("Button 1 clicked!\n");
        ShowStartupMenu(first_window,first_renderer);
        ClearEvents();
    }
}
void USA_fleet(SDL_Event *event)
{
    InitLogFile("logs.txt");
    if (event->type == SDL_MOUSEBUTTONDOWN) {
        LogMessage("Computer clicked!\n");
        Fleet player_one_fleet;
        printf("will initialize the fleet!");
        initialize_american_fleet(&player_one_fleet);
        initialize_player(&player_one,playerName,player_one_fleet,"offline",0,"");
        printf("fleet initialized!!");
        if(player_two_init_state==NULL)
        {
            printf("player two not initialized");
        }
        player_two_init_state();
        printf("AI matrices:\n");
        afficherMatrice(player_two_grid);
        ClearEvents();
        Dialog_1(first_window,first_renderer);
    }
}
void Russian_fleet(SDL_Event *event) {
    InitLogFile("logs.txt");
    if (event->type == SDL_MOUSEBUTTONDOWN) {
        LogMessage("Computer clicked!\n");
        Fleet player_one_fleet;
        printf("will initialize the fleet!");
        initialize_russian_fleet(&player_one_fleet);
        initialize_player(&player_one,playerName,player_one_fleet,"offline",0,"");
        printf("fleet initialized!!");
        if(player_two_init_state==NULL)
        {
            printf("player two not initialized");
        }
        player_two_init_state();
        printf("AI matrices:\n");
        afficherMatrice(player_two_grid);
        ClearEvents();
        Dialog_1(first_window,first_renderer);
    }
}
void CloseChooseFleet(SDL_Event *event) {
    InitLogFile("logs.txt");
    if (event->type == SDL_MOUSEBUTTONDOWN) {
       *alive=0;
    }
}
void Choose_fleet(SDL_Window* Window,SDL_Renderer* Renderer) 
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
    CreateClickableElement(Renderer, 558, 11, &width3, &height3, NULL, textColor, "medias/images/btn-close.png", CloseChooseFleet, 12);
    CreateClickableElement(Renderer, 12, 11, &width3, &height3, NULL, textColor, "medias/images/left-arrow.png", BeforeChooseFleet, 12);
    CreateClickableElement(Renderer,220, 158, &width2, &height2,"USA", textColor, "medias/images/square-btn.png",USA_fleet, 12);
    CreateClickableElement(Renderer,220, 220, &width2, &height2,"Russia", textColor, "medias/images/square-btn.png",Russian_fleet, 12);
    SDL_RenderPresent(Renderer);
    int quit=0;
    while (!quit) {
            ProcessEvents(Window,Renderer);     
    }
}
#include "../../headers/window/startup.h"
#include "../../headers/graphics.h"
#include "../../headers/events.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "../../headers/window/choose_fleet.h"
#include <stdio.h>
#include "../../headers/log.h"
#include "../../headers/player.h"

void  GoChooseFleet(SDL_Event *event) {
    InitLogFile("logs.txt");
    if (event->type == SDL_MOUSEBUTTONDOWN) {
         Choose_fleet(first_window,first_renderer);
    }
}
void Connect_to(SDL_Window* window, SDL_Renderer* renderer) {
    printf("kol");
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
       TTF_Font *font = TTF_OpenFont("medias/font/Sora.ttf", 24);
    if (!font) {
        printf("Erreur de chargement de la police : %s\n", TTF_GetError());
        SDL_DestroyTexture(backgroundTexture);
        TTF_Quit();
        return;
    }

    // Couleur du texte (blanc)
    SDL_Color textColor = {255, 255, 255, 255};
    // Créer la texture pour l'adresse IP
    const char* vs = " vs ";

    // Calculez la longueur totale nécessaire
    size_t length = strlen(player_one.name) + strlen(vs) + strlen(player_two.name) + 1; // +1 pour le caractère nul '\0'

    // Allouez la mémoire pour 'por'
    char* por = malloc(length);
    if (por == NULL) {
        // Gérer l'erreur d'allocation de mémoire
        fprintf(stderr, "Erreur d'allocation de mémoire\n");
        exit(EXIT_FAILURE);
    }

    // Copiez le nom du premier joueur dans 'por'
    strcpy(por, player_one.name);

    // Concaténez " vs " à 'por'
    strcat(por, vs);

    // Concaténez le nom du second joueur à 'por'
    strcat(por, player_two.name);
    SDL_Surface* ipSurface = TTF_RenderText_Blended(font,por, textColor);
    if (!ipSurface) {
        printf("Erreur de rendu du texte (IP) : %s\n", TTF_GetError());
        SDL_DestroyTexture(backgroundTexture);
        TTF_CloseFont(font);
        TTF_Quit();
        return;
    }

    SDL_Texture* ipTexture = SDL_CreateTextureFromSurface(renderer, ipSurface);
    int ip_text_width = ipSurface->w;
    int ip_text_height = ipSurface->h;
    SDL_FreeSurface(ipSurface);
    if (!ipTexture) {
        printf("Erreur de création de la texture du texte (IP) : %s\n", SDL_GetError());
        SDL_DestroyTexture(backgroundTexture);
        TTF_CloseFont(font);
        TTF_Quit();
        return;
    }

    // Initialiser les variables pour le statut du serveur
    SDL_Texture* statusTexture = NULL;
    int status_text_width = 0;
    int status_text_height = 0;
    SDL_Rect statusRect;

    // Obtenir la taille de la fenêtre pour centrer le texte
    int window_width, window_height;
    SDL_GetWindowSize(window, &window_width, &window_height);

    // Positionner le texte de l'adresse IP au centre horizontalement, et un peu au-dessus du centre vertical
    SDL_Rect ipRect = {
        (window_width - ip_text_width) / 2,
        (window_height / 2) - ip_text_height - 10,
        ip_text_width,
        ip_text_height
    };

    // Afficher la texture de fond
    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

    // Afficher la texture du texte (adresse IP)
    SDL_RenderCopy(renderer, ipTexture, NULL, &ipRect);
    // Initialize the alive flag
    int aliveFlag = 1;
    alive = &aliveFlag;
    int width = 0; 
    int height = 0; 
    int width2 = 35; 
    int height2 = 37; 
    int width3 = 30; 
    int height3 = 30; 
    CreateClickableElement(renderer,214,520,&width,&height, "Play", textColor,"medias/images/btn-play.png",GoChooseFleet,12);

SDL_RenderPresent(renderer);

   
}
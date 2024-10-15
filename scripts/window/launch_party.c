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
#include "../../headers/window/choose_fleet.h"
void Launch_party(SDL_Window* Window, SDL_Renderer* Renderer) {
    // Vérifier que la fenêtre et le renderer sont valides
    if (!Window || !Renderer) {
        printf("Fenêtre ou Renderer invalide.\n");
        return;
    }

    // Initialisation de SDL_ttf
    if (TTF_Init() == -1) {
        printf("Erreur d'initialisation de SDL_ttf : %s\n", TTF_GetError());
        return;
    }

    // Charger et créer la texture de fond
    SDL_Surface *backgroundSurface = IMG_Load("medias/images/bg-first.png");
    if (!backgroundSurface) {
        printf("Erreur de chargement de l'image : %s\n", IMG_GetError());
        TTF_Quit();
        return;
    }

    SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(Renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface);
    if (!backgroundTexture) {
        printf("Erreur de création de la texture : %s\n", SDL_GetError());
        TTF_Quit();
        return;
    }

    // Charger la police
    TTF_Font *font = TTF_OpenFont("medias/font/Sora.ttf", 24);
    if (!font) {
        printf("Erreur de chargement de la police : %s\n", TTF_GetError());
        SDL_DestroyTexture(backgroundTexture);
        TTF_Quit();
        return;
    }

    // Couleur du texte (blanc)
    SDL_Color textColor = {255, 255, 255, 255};

    // Récupérer l'adresse IP locale
    char ip_address[INET_ADDRSTRLEN];
    if (get_local_ip(ip_address, sizeof(ip_address)) != 0) {
        strncpy(ip_address, "IP non disponible", sizeof(ip_address));
    }

    // Créer la texture pour l'adresse IP
    SDL_Surface* ipSurface = TTF_RenderText_Blended(font, ip_address, textColor);
    if (!ipSurface) {
        printf("Erreur de rendu du texte (IP) : %s\n", TTF_GetError());
        SDL_DestroyTexture(backgroundTexture);
        TTF_CloseFont(font);
        TTF_Quit();
        return;
    }

    SDL_Texture* ipTexture = SDL_CreateTextureFromSurface(Renderer, ipSurface);
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
    SDL_GetWindowSize(Window, &window_width, &window_height);

    // Positionner le texte de l'adresse IP au centre horizontalement, et un peu au-dessus du centre vertical
    SDL_Rect ipRect = {
        (window_width - ip_text_width) / 2,
        (window_height / 2) - ip_text_height - 10,
        ip_text_width,
        ip_text_height
    };

    // Afficher la texture de fond
    SDL_RenderCopy(Renderer, backgroundTexture, NULL, NULL);

    // Afficher la texture du texte (adresse IP)
    SDL_RenderCopy(Renderer, ipTexture, NULL, &ipRect);

    SDL_RenderPresent(Renderer);
 
}
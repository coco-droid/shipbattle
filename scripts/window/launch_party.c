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
#include <pthread.h> // Nécessaire pour utiliser les threads

// Variable pour indiquer le statut du serveur
volatile int server_status_flag = 0; // 0: en démarrage, 1: actif, -1: erreur

// Fonction pour le thread du serveur
void* server_thread(void* arg) {
    int port = *(int*)arg;

    // Ouvrir le port dans le pare-feu
    if (open_firewall_port(port) != 0) {
        printf("Échec de l'ouverture du port dans le pare-feu.\n");
        server_status_flag = -1; // Indique une erreur
        return NULL;
    }

    server_status_flag = 1; // Indique que le serveur est actif

    // Lancer le serveur (appel bloquant)
    if (run_server(port) != 0) {
        printf("Erreur lors de l'exécution du serveur.\n");
        server_status_flag = -1; // Indique une erreur
    }

    return NULL;
}

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

    // Maintenant, lancer le serveur dans un thread séparé
    int port = 9000; // Vous pouvez demander à l'utilisateur de spécifier le port
    pthread_t serverThread;
    if (pthread_create(&serverThread, NULL, server_thread, &port) != 0) {
        printf("Erreur lors de la création du thread du serveur.\n");
        // Mettre à jour le statut du serveur
        server_status_flag = -1;
    }

    // Boucle principale pour garder la fenêtre ouverte
    int running = 1;
    SDL_Event event;
    int previous_status = 0; // Pour suivre les changements de statut du serveur
    while (running) {
        // Gestion des événements
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }
/*
        // Mise à jour du statut du serveur si nécessaire
        if (server_status_flag != previous_status) {
            previous_status = server_status_flag;

            // Mettre à jour le texte et la couleur du statut du serveur
            if (server_status_flag == 1) {
                const char* server_status = "Actif";
                SDL_Color statusColor = {0, 255, 0, 255}; // Vert

                // Créer la texture pour le statut du serveur
                SDL_Surface* statusSurface = TTF_RenderText_Blended(font, server_status, statusColor);
                if (!statusSurface) {
                    printf("Erreur de rendu du texte (statut) : %s\n", TTF_GetError());
                    break;
                }

                statusTexture = SDL_CreateTextureFromSurface(Renderer, statusSurface);
                status_text_width = statusSurface->w;
                status_text_height = statusSurface->h;
                SDL_FreeSurface(statusSurface);

                if (!statusTexture) {
                    printf("Erreur de création de la texture du texte (statut) : %s\n", SDL_GetError());
                    break;
                }

                // Positionner le texte du statut du serveur en dessous de l'adresse IP
                statusRect = (SDL_Rect){
                    (window_width - status_text_width) / 2,
                    (window_height / 2) + 10,
                    status_text_width,
                    status_text_height
                };
            } else {
                // Si le serveur n'est pas actif, détruire la texture du statut
                if (statusTexture) {
                    SDL_DestroyTexture(statusTexture);
                    statusTexture = NULL;
                }
            }

            // Effacer le renderer avec une couleur noire
            SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
            SDL_RenderClear(Renderer);

            // Afficher la texture de fond
            SDL_RenderCopy(Renderer, backgroundTexture, NULL, NULL);

            // Afficher la texture du texte (adresse IP)
            SDL_RenderCopy(Renderer, ipTexture, NULL, &ipRect);

            // Afficher la texture du texte (statut du serveur) si disponible
            if (statusTexture) {
                SDL_RenderCopy(Renderer, statusTexture, NULL, &statusRect);
            }

            SDL_RenderPresent(Renderer);
        }
*/
if(client_connected)
{
    printf("client connected");
}
        SDL_Delay(16); // Contrôle du taux de rafraîchissement (~60 FPS)
    }

    // Nettoyage
    SDL_DestroyTexture(ipTexture);
    if (statusTexture) {
        SDL_DestroyTexture(statusTexture);
    }
    SDL_DestroyTexture(backgroundTexture);
    TTF_CloseFont(font);
    TTF_Quit();

    // Optionnel : attendre la fin du thread serveur (dépend de votre implémentation)
    pthread_cancel(serverThread); // Non recommandé ; mieux vaut implémenter un arrêt propre
    pthread_join(serverThread, NULL);
}

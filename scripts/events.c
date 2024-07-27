#include "../headers/events.h"
#include "../headers/graphics.h"
#include <stdio.h>

#define MAX_CALLBACKS 50

// Tableaux pour stocker les callbacks et les types d'événements
static EventCallback eventCallbacks[MAX_CALLBACKS];
static Uint32 eventTypes[MAX_CALLBACKS];

// Déclaration externe du pointeur 'alive'
int *alive;

// Initialise le module d'événements
void InitEvents(void) {
    for (int i = 0; i < MAX_CALLBACKS; i++) {
        eventCallbacks[i] = NULL;
        eventTypes[i] = 0;
    }
}

// Enregistre un callback pour un type d'événement spécifique
void RegisterEventCallback(Uint32 eventType, EventCallback callback) {
    for (int i = 0; i < MAX_CALLBACKS; i++) {
        if (eventCallbacks[i] == NULL) {
            eventCallbacks[i] = callback;
            eventTypes[i] = eventType;
            return;
        }
    }
    fprintf(stderr, "No space left to register event callback\n");
}

// Gère les événements SDL, incluant la boucle d'événements
void ProcessEvents(SDL_Window* window, SDL_Renderer* renderer) {
    SDL_Event e;
    int quit = 0;
    int showOptions = 0;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                if (alive) {
                    *alive = 0;  // Met à jour la valeur de 'alive' pour signaler la fin de la session
                }
                quit = 1;  // Quitte la boucle d'événements
            }

            // Appelle les callbacks enregistrés
            for (int i = 0; i < MAX_CALLBACKS; i++) {
                if (eventCallbacks[i] != NULL && e.type == eventTypes[i]) {
                    eventCallbacks[i](&e);
                }
            }

            // Gestion des clics de la souris pour afficher les options
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX = e.button.x;
                int mouseY = e.button.y;
                if (!showOptions && mouseX >= 100 && mouseY >= 100 && mouseX <= 300 && mouseY <= 150) {
                    showOptions = 1;  // Passe à l'écran d'options
                }
            }
        }
    }
}


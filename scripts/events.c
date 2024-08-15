#include "../headers/events.h"
#include "../headers/log.h"
#include <stdio.h>
#include <stdbool.h>
#define MAX_CALLBACKS 50

// Tableau pour stocker les zones cliquables
static ClickableArea clickableAreas[MAX_CALLBACKS];
static int numAreas = 0;

// Variables pour suivre l'état de la souris
static bool isDragging = false;
static ClickableArea* currentDragArea = NULL;
int aliveValue = 1; // Valeur initiale
int *alive = &aliveValue;
// Initialise le module d'événements
void InitEvents(void) {
    for (int i = 0; i < MAX_CALLBACKS; i++) {
        clickableAreas[i].onClick = NULL;
        clickableAreas[i].onHover = NULL;
        clickableAreas[i].onDragStart = NULL;
        clickableAreas[i].onDragMove = NULL;
        clickableAreas[i].onDragEnd = NULL;
    }
    numAreas = 0;
}

// Enregistre une zone cliquable avec ses callbacks associés
void RegisterClickableArea(ClickableArea area) {
    if (numAreas < MAX_CALLBACKS) {
        clickableAreas[numAreas] = area;
        numAreas++;
    } else {
        fprintf(stderr, "Pas d'espace pour enregistrer une nouvelle zone cliquable\n");
    }
}

// Fonction pour vérifier si une position (x, y) est dans une zone cliquable
bool IsInArea(ClickableArea* area, int x, int y) {
    return (x >= area->x && x <= area->x + area->width &&
            y >= area->y && y <= area->y + area->height);
}

// Gère les événements SDL, incluant la détection de clics, survols et drag-and-drop
void ProcessEvents(SDL_Window* window, SDL_Renderer* renderer) {
    InitLogFile("logs.txt");
    SDL_Event e;
    int quit = 0;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                if (alive) {
                    *alive = 0;
                }
                quit = 1;
            }

            // Gestion des événements de la souris
            if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP || e.type == SDL_MOUSEMOTION) {
                for (int i = 0; i < numAreas; i++) {
                    ClickableArea* area = &clickableAreas[i];

                    // Clic dans la zone
                    if (e.type == SDL_MOUSEBUTTONDOWN && IsInArea(area, e.button.x, e.button.y)) {
                        if (area->onClick) {
                            LogMessage("Clic détecté !");
                            area->onClick(&e);
                        }
                        // Commence le drag
                        if (area->onDragStart) {
                            isDragging = true;
                            currentDragArea = area;
                            area->onDragStart(&e);
                        }
                    }

                    // Relâchement du bouton de la souris
                    if (e.type == SDL_MOUSEBUTTONUP && isDragging) {
                        if (currentDragArea && currentDragArea->onDragEnd) {
                            currentDragArea->onDragEnd(&e);
                        }
                        isDragging = false;
                        currentDragArea = NULL;
                    }

                    // Mouvement de la souris dans la zone (survol ou glisser)
                    if (e.type == SDL_MOUSEMOTION) {
                        if (IsInArea(area, e.motion.x, e.motion.y)) {
                            if (area->onHover) {
                                area->onHover(&e);
                            }
                            if (isDragging && currentDragArea && currentDragArea->onDragMove) {
                                currentDragArea->onDragMove(&e);
                            }
                        }
                    }
                }
            }
        }
    }
}
void ClearEvents(void) {
    for (int i = 0; i < MAX_CALLBACKS; i++) {
        clickableAreas[i].onClick = NULL;
        clickableAreas[i].onHover = NULL;
        clickableAreas[i].onDragStart = NULL;
        clickableAreas[i].onDragMove = NULL;
        clickableAreas[i].onDragEnd = NULL;
    }
    numAreas = 0;
}

#ifndef EVENTS_H
#define EVENTS_H

#include <SDL2/SDL.h>

// Définition du type de callback pour les événements
typedef void (*EventCallback)(SDL_Event*);
extern int *alive;
// Structure pour gérer les zones cliquables
typedef struct {
    int x, y, width, height;
    EventCallback onClick;
    EventCallback onHover;
    EventCallback onDragStart;
    EventCallback onDragEnd;
    EventCallback onDragMove;
} ClickableArea;

// Fonction d'initialisation des événements
void InitEvents(void);

// Fonction pour enregistrer un callback pour une zone
void RegisterClickableArea(ClickableArea area);
void ClearEvents(void);
// Fonction pour gérer les événements SDL
void ProcessEvents(SDL_Window* window, SDL_Renderer* renderer);

#endif // EVENTS_H


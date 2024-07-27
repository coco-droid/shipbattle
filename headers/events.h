#ifndef EVENTS_H
#define EVENTS_H

#include <SDL2/SDL.h>

typedef void (*EventCallback)(SDL_Event *event);

// Déclaration externe du pointeur 'alive'
extern int *alive;

// Initialise le module d'événements
void InitEvents(void);

// Enregistre un callback pour un type d'événement spécifique
void RegisterEventCallback(Uint32 eventType, EventCallback callback);

// Gère les événements SDL, incluant la boucle d'événements
void ProcessEvents(SDL_Window* window, SDL_Renderer* renderer);

#endif // EVENTS_H

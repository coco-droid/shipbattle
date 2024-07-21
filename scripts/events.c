#include "../headers/events.h"
#include "../headers/graphics.h"
void close_game_events(int* game_is_running) {
    // Gestion des événements liés au jeu
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                *game_is_running =0;
                break;
            // Autres événements liés au jeu
        }
    }
}
#include "../headers/ships.h"
#include <string.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
void initialize_ship(Ships* s, const char* name, const char* image_h_path, const char* image_v_path, int ammunitions, int n_cases, int health, int damage, SDL_Renderer* renderer) {
    // Copier le nom du navire
    strncpy(s->name, name, MAX_NAME_LENGTH - 1);
    s->name[MAX_NAME_LENGTH - 1] = '\0';  // Assurer la terminaison de chaîne

    // Charger les textures à partir des chemins d'accès
    s->texture_h = IMG_LoadTexture(renderer, image_h_path);
    if (!s->texture_h) {
        // Gérer l'erreur de chargement de la texture horizontale
        fprintf(stderr, "Erreur lors du chargement de la texture horizontale: %s\n", SDL_GetError());
        return;
    }

    s->texture_v = IMG_LoadTexture(renderer, image_v_path);
    if (!s->texture_v) {
        // Gérer l'erreur de chargement de la texture verticale
        fprintf(stderr, "Erreur lors du chargement de la texture verticale: %s\n", SDL_GetError());
        SDL_DestroyTexture(s->texture_h);  // Libérer la texture horizontale si la verticale échoue
        return;
    }

    // Initialiser les autres attributs
    s->ammunitions = ammunitions;
    s->n_cases = n_cases;
    s->health = health;
    s->damage = damage;
    
    // Initialiser les cases si nécessaire
    memset(s->cases, 0, sizeof(s->cases));
}

// Fonction pour libérer les ressources des navires
void cleanup_ship(Ships* s) {
    if (s->texture_h) {
        SDL_DestroyTexture(s->texture_h);
    }
    if (s->texture_v) {
        SDL_DestroyTexture(s->texture_v);
    }
}


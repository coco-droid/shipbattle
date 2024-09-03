#ifndef SHIPS_H
#define SHIPS_H

#define MAX_NAME_LENGTH 200  // Définir une taille maximale pour le nom
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct ships {
    char name[MAX_NAME_LENGTH];  // Nom du navire
    int ammunitions;
    int n_cases;
    int health;
    int ShipX;
    int ShipY;
    int ship_id;
    bool horizontal;
    bool isDragging;
    int damage;
    SDL_Texture* texture_h;  // Texture pour l'image horizontale
    SDL_Texture* texture_v;  // Texture pour l'image verticale
    // Le damier pour représenter les cellules du navire
    char cases[8][8];
} Ships;

// Fonctions pour manipuler les navires
void initialize_ship(Ships* s, const char* name, const char* image_h_path, const char* image_v_path, int ammunitions, int n_cases, int health, int damage, SDL_Renderer* renderer ,int ship_id);
void cleanup_ship(Ships* s);  // Fonction pour libérer les ressources
#endif

#ifndef SHIPS_H
#define SHIPS_H

#define MAX_NAME_LENGTH 200  // Définir une taille maximale pour le nom
#include <stdbool.h>
typedef struct ships {
    char name[MAX_NAME_LENGTH];  // Nom du navire
    int ammunitions;
    int n_cases;
    int health;
    int ShipX;
    int ShipY;
    bool horizontal;
    bool isDragging;
    int damage;
    char image[200];
    // Le damier pour représenter les cellules du navire
    char cases[8][8];
} Ships;

// Fonctions pour manipuler les navires
void initialize_ship(Ships* s, const char* name,char* image, int ammunitions, int n_cases, int health, int damage);

#endif

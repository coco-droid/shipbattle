#ifndef SHIPS_H
#define SHIPS_H

#define MAX_NAME_LENGTH 20  // Définir une taille maximale pour le nom

typedef struct ships {
    char name[MAX_NAME_LENGTH];  // Nom du navire
    int ammunitions;
    int n_cases;
    int health;
    int damage;
    // Le damier pour représenter les cellules du navire
    char cases[8][8];
} Ships;

// Fonctions pour manipuler les navires
void initialize_ship(Ships* s, const char* name, int ammunitions, int n_cases, int health, int damage);

#endif

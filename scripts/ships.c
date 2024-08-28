#include "../headers/ships.h"
#include <string.h>

void initialize_ship(Ships* s, const char* name,char* image, int ammunitions, int n_cases, int health, int damage) {
    strncpy(s->name, name, MAX_NAME_LENGTH - 1);
    strncpy(s->image,image, MAX_NAME_LENGTH - 1);
    s->image[MAX_NAME_LENGTH - 1] = '\0';
    s->name[MAX_NAME_LENGTH - 1] = '\0';  // Assurer la terminaison de chaîne
    s->ammunitions = ammunitions;
    s->n_cases = n_cases;
    s->health = health;
    s->damage = damage;
    // Initialiser les cases si nécessaire
    memset(s->cases, 0, sizeof(s->cases));
}

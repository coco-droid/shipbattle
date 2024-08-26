#ifndef FLEET_H
#define FLEET_H

#include "./ships.h"

typedef struct fleet {
    char name[20];  // Nom de la flotte
    Ships destroyer;
    Ships torpedo_boat;
    Ships submarine;
    Ships aircraft_carrier;
    Ships cruiser;
    int id;  // Identifiant de la flotte
} Fleet;

// Fonctions pour initialiser une flotte
void initialize_american_fleet(Fleet* f);
void initialize_russian_fleet(Fleet* f);

#endif

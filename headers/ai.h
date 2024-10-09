#ifndef AI_H
#define AI_H

#include <stdbool.h>

#define TAILLE 10  // Taille de la matrice (peut être ajustée)

// Structure pour représenter des coordonnées
typedef struct {
    int x;
    int y;
} Coordonnees;

// Prototypes des fonctions
void afficherShooting(Coordonnees ai_shoot_m[], int size);
bool ajouter_coordonnees(Coordonnees ai_shoot_m[], int *index, int x, int y);
void pnj_shoot(int matrice[TAILLE][TAILLE], Coordonnees tirs_effectues[], int *nombre_tirs);

#endif // AI_H

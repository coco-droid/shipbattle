#ifndef PLAYER_H
#define PLAYER_H
#include "./fleet.h"
#include <stdbool.h>
typedef struct player {
  //name of player ,type of fleet with a default fleet type and the type of player "computer ,local player, online  player and a ids "
    char name[20];
    struct fleet fleet;
    char type[20];
    int health;
    bool is_winner;
    int id;
    char ip;
} Player;
void initialize_player(struct player* p, const char* name, struct fleet f, const char* type, int id, char ip);
void afficherMatrice(int matrice[10][10]);
extern Player player_one;
extern int* player_one_grid[10][10];
extern int* player_two_grid[10][10];
extern int* hint_player_one_grid[10][10];
extern int* hint_player_two_grid[10][10];
extern int* ai_shoot_m[4][2];
extern int* who_Played;
extern Player player_two;
#endif
#ifndef PLAYER_H
#define PLAYER_H
#include "./fleet.h"
struct player {
  //name of player ,type of fleet with a default fleet type and the type of player "computer ,local player, online  player and a ids "
    char name[20];
    struct fleet fleet;
    char type[20];
    int id;
};
//use the struct to define destroyer ,torpedo_boat,submarine,aircraft carrier,cruiser 
struct fleet american = { };
struct fleet russian = { };
#endif
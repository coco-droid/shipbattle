#include "../headers/fleet.h"
#include "../headers/player.h"
Player player_one;
Player player_two;
void initialize_player(struct player* p, const char* name, struct fleet f, const char* type, int id, char ip) {
    strncpy(p->name, name, sizeof(p->name) - 1);
    p->name[sizeof(p->name) - 1] = '\0'; // Ensure null-termination
    p->fleet = f;
    strncpy(p->type, type, sizeof(p->type) - 1);
    p->type[sizeof(p->type) - 1] = '\0'; // Ensure null-termination
    p->id = id;
    p->ip = ip;
}
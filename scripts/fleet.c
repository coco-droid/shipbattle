#include "../headers/fleet.h"
#include "../headers/ships.h"
#include <string.h>  // For strncpy
#include "../headers/graphics.h"
void initialize_american_fleet(Fleet* f) {
    strncpy(f->name, "American Fleet", sizeof(f->name) - 1);
    f->name[sizeof(f->name) - 1] = '\0';  // Assurer la terminaison par null
    f->id = 1;

    initialize_ship(&f->destroyer, "USS Destroyer", "medias/images/ships/cruiser-180.png", "medias/images/ships/cruiser-90.png", 10, 2, 2, 1, first_renderer,8);
    initialize_ship(&f->torpedo_boat, "USS Torpedo Boat", "medias/images/ships/cruiser-180.png", "medias/images/ships/cruiser-90.png", 20, 3, 3, 2, first_renderer,9);
    initialize_ship(&f->submarine, "USS Nautilus", "medias/images/ships/cruiser-180.png", "medias/images/ships/cruiser-90.png", 20, 4, 4, 3, first_renderer,10);
    initialize_ship(&f->aircraft_carrier, "USS Enterprise", "medias/images/ships/cruiser-180.png", "medias/images/ships/cruiser-90.png", 35, 5, 5, 4, first_renderer,12);
    initialize_ship(&f->cruiser, "USS Missouri", "medias/images/ships/cruiser-180.png", "medias/images/ships/cruiser-90.png", 0, 6, 6, 5, first_renderer,13);
}
void initialize_russian_fleet(Fleet* f) {
    strncpy(f->name, "Russian Fleet", sizeof(f->name) - 1);
    f->name[sizeof(f->name) - 1] = '\0';  // Assurer la terminaison par null
    f->id = 2;

    initialize_ship(&f->destroyer, "Kirov", "medias/images/ships/cruiser-180.png", "medias/images/ships/cruiser-90.png", 10, 2, 2, 1, first_renderer,8);
    initialize_ship(&f->torpedo_boat, "Slava", "medias/images/ships/cruiser-180.png", "medias/images/ships/cruiser-90.png", 20, 3, 3, 2, first_renderer,9);
    initialize_ship(&f->submarine, "K-19", "medias/images/ships/cruiser-180.png", "medias/images/ships/cruiser-90.png", 20, 4, 4, 3, first_renderer,10);
    initialize_ship(&f->aircraft_carrier, "Admiral Kuznetsov", "medias/images/ships/cruiser-180.png", "medias/images/ships/cruiser-90.png", 35, 5, 5, 4, first_renderer,12);
    initialize_ship(&f->cruiser, "Sverdlov", "medias/images/ships/cruiser-180.png", "medias/images/ships/cruiser-90.png", 0, 6, 6, 5, first_renderer,13);
}

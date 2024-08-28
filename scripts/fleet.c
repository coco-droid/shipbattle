#include "../headers/fleet.h"
void initialize_american_fleet(Fleet* f) {
    strncpy(f->name, "American Fleet", sizeof(f->name) - 1);
    f->name[sizeof(f->name) - 1] = '\0';  // Assurer la terminaison de chaîne
    f->id = 1;

    initialize_ship(&f->destroyer,'cruiser-180.png', "USS Destroyer", 10, 2, 2, 1);
    initialize_ship(&f->torpedo_boat,'cruiser-180.png', "USS Torpedo Boat", 20, 3, 3, 2);
    initialize_ship(&f->submarine,'cruiser-180.png', "USS Nautilus", 20, 4, 4, 3);
    initialize_ship(&f->aircraft_carrier,'cruiser-180.png',"USS Enterprise", 35, 5, 5, 4);
    initialize_ship(&f->cruiser,'cruiser-180.png',"USS Missouri", 0, 6, 6, 5);
}

void initialize_russian_fleet(Fleet* f) {
    strncpy(f->name, "Russian Fleet", sizeof(f->name) - 1);
    f->name[sizeof(f->name) - 1] = '\0';  // Assurer la terminaison de chaîne
    f->id = 2;

    initialize_ship(&f->destroyer,'cruiser-180.png',"Kirov", 10, 2, 2, 1);
    initialize_ship(&f->torpedo_boat,'cruiser-180.png',"Slava", 20, 3, 3, 2);
    initialize_ship(&f->submarine,'cruiser-180.png',"K-19", 20, 4, 4, 3);
    initialize_ship(&f->aircraft_carrier,'cruiser-180.png',"Admiral Kuznetsov", 35, 5, 5, 4);
    initialize_ship(&f->cruiser,'cruiser-180.png',"Sverdlov", 0, 6, 6, 5);
}

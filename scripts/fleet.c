#include "../headers/fleet.h"
void initialize_american_fleet(Fleet* f) {
    strncpy(f->name, "American Fleet", sizeof(f->name) - 1);
    f->name[sizeof(f->name) - 1] = '\0';  // Assurer la terminaison de chaîne
    f->id = 1;

    initialize_ship(&f->destroyer,'cruiser-180', "USS Destroyer", 10, 2, 2, 1);
    initialize_ship(&f->torpedo_boat,'cruiser-180', "USS Torpedo Boat", 20, 3, 3, 2);
    initialize_ship(&f->submarine,'cruiser-180', "USS Nautilus", 20, 4, 4, 3);
    initialize_ship(&f->aircraft_carrier,'cruiser-180',"USS Enterprise", 35, 5, 5, 4);
    initialize_ship(&f->cruiser,'cruiser-180',"USS Missouri", 0, 6, 6, 5);
}

void initialize_russian_fleet(Fleet* f) {
    strncpy(f->name, "Russian Fleet", sizeof(f->name) - 1);
    f->name[sizeof(f->name) - 1] = '\0';  // Assurer la terminaison de chaîne
    f->id = 2;

    initialize_ship(&f->destroyer,'cruiser-180',"Kirov", 10, 2, 2, 1);
    initialize_ship(&f->torpedo_boat,'cruiser-180',"Slava", 20, 3, 3, 2);
    initialize_ship(&f->submarine,'cruiser-180',"K-19", 20, 4, 4, 3);
    initialize_ship(&f->aircraft_carrier,'cruiser-180',"Admiral Kuznetsov", 35, 5, 5, 4);
    initialize_ship(&f->cruiser,'cruiser-180',"Sverdlov", 0, 6, 6, 5);
}

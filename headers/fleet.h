#ifndef FLEET_H
#define FLEET_H
#include "./ships.h"
struct fleet {
  //name of fleet ,the 5 ships on the fleet ,the id of the player have the fleet 
    char name[20];
    struct ships destroyer;
    struct ships torpedo_boat;
    struct ships submarine;
    struct ships aircraft_carrier;
    struct ships cruiser;
    int id;
};
//use the struct to define destroyer ,torpedo_boat,submarine,aircraft carrier,cruiser 
struct fleet american = { };
struct fleet russian = { };
#endif
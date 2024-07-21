#ifndef SHIPS_H
#define SHIPS_H

struct ships {
   int ammunitions;
   int n_cases;
   int health;
   int damage;
};
//use the struct to define destroyer ,torpedo_boat,submarine,aircraft carrier,cruiser 
struct ships destroyer = { 10, 2, 2, 1 };
struct ships torpedo_boat = { 20, 3, 3, 2 };
struct ships submarine = { 20, 4, 4, 3 };   
struct ships aircraft_carrier = { 35, 5, 5, 4 };
struct ships cruiser = { 0, 6, 6, 5 };

#endif
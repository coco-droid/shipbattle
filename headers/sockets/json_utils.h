#ifndef JSON_UTILS_H
#define JSON_UTILS_H
#include <cjson/cJSON.h> 
#include "../ships.h"
#include "../fleet.h"

char* serialize_matrix(int matrix[10][10]);
int deserialize_matrix(const char *json_str, int matrix[10][10]);

// Sérialisation et désérialisation de la structure Ships
char* serialize_ships(const Ships *ship);
int deserialize_ships(const char *json_str, Ships *ship);

// Sérialisation et désérialisation de la structure Fleet
char* serialize_fleet(const Fleet *fleet);
int deserialize_fleet(const char *json_str, Fleet *fleet);
#endif // JSON_UTILS_H

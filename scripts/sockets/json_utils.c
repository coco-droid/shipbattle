// json_utils.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cjson/cJSON.h> 
#include "../../headers/ships.h"
#include "../../headers/fleet.h"
// Fonction pour sérialiser une matrice en JSON
char* serialize_matrix(int matrix[10][10]) {
    cJSON *json_matrix = cJSON_CreateArray();
    for (int i = 0; i < 10; i++) {
        cJSON *row = cJSON_CreateIntArray(matrix[i], 10);
        cJSON_AddItemToArray(json_matrix, row);
    }
    char *json_str = cJSON_PrintUnformatted(json_matrix);
    cJSON_Delete(json_matrix);
    return json_str;
}

// Fonction pour désérialiser une matrice depuis JSON
int deserialize_matrix(const char *json_str, int matrix[10][10]) {
    cJSON *json_matrix = cJSON_Parse(json_str);
    if (!json_matrix) {
        fprintf(stderr, "Erreur lors de l'analyse du JSON\n");
        return -1;
    }

    if (!cJSON_IsArray(json_matrix)) {
        fprintf(stderr, "Le JSON n'est pas un tableau\n");
        cJSON_Delete(json_matrix);
        return -1;
    }

    for (int i = 0; i < 10; i++) {
        cJSON *row = cJSON_GetArrayItem(json_matrix, i);
        if (!cJSON_IsArray(row)) {
            fprintf(stderr, "Ligne %d n'est pas un tableau\n", i);
            cJSON_Delete(json_matrix);
            return -1;
        }

        for (int j = 0; j < 10; j++) {
            cJSON *item = cJSON_GetArrayItem(row, j);
            if (!cJSON_IsNumber(item)) {
                fprintf(stderr, "Élément [%d][%d] n'est pas un nombre\n", i, j);
                cJSON_Delete(json_matrix);
                return -1;
            }
            matrix[i][j] = item->valueint;
        }
    }

    cJSON_Delete(json_matrix);
    return 0;
}

// Fonction pour désérialiser une chaîne JSON en structure Ships
int deserialize_ships(const char *json_str, Ships *ship) {
    cJSON *json_ship = cJSON_Parse(json_str);
    if (!json_ship) {
        fprintf(stderr, "Erreur lors de l'analyse du JSON des Ships\n");
        return -1;
    }

    if (!cJSON_IsObject(json_ship)) {
        fprintf(stderr, "Le JSON des Ships n'est pas un objet\n");
        cJSON_Delete(json_ship);
        return -1;
    }

    // Extraction des champs
    cJSON *json_name = cJSON_GetObjectItemCaseSensitive(json_ship, "name");
    if (cJSON_IsString(json_name) && (json_name->valuestring != NULL)) {
        strncpy(ship->name, json_name->valuestring, MAX_NAME_LENGTH);
        ship->name[MAX_NAME_LENGTH - 1] = '\0';
    } else {
        fprintf(stderr, "Le champ 'name' est manquant ou invalide\n");
        cJSON_Delete(json_ship);
        return -1;
    }

    // Répétez ce processus pour les autres champs
    cJSON *json_ammunitions = cJSON_GetObjectItemCaseSensitive(json_ship, "ammunitions");
    if (cJSON_IsNumber(json_ammunitions)) {
        ship->ammunitions = json_ammunitions->valueint;
    } else {
        fprintf(stderr, "Le champ 'ammunitions' est manquant ou invalide\n");
        cJSON_Delete(json_ship);
        return -1;
    }

    cJSON *json_n_cases = cJSON_GetObjectItemCaseSensitive(json_ship, "n_cases");
    if (cJSON_IsNumber(json_n_cases)) {
        ship->n_cases = json_n_cases->valueint;
    } else {
        fprintf(stderr, "Le champ 'n_cases' est manquant ou invalide\n");
        cJSON_Delete(json_ship);
        return -1;
    }

    cJSON *json_health = cJSON_GetObjectItemCaseSensitive(json_ship, "health");
    if (cJSON_IsNumber(json_health)) {
        ship->health = json_health->valueint;
    } else {
        fprintf(stderr, "Le champ 'health' est manquant ou invalide\n");
        cJSON_Delete(json_ship);
        return -1;
    }

    cJSON *json_ShipX = cJSON_GetObjectItemCaseSensitive(json_ship, "ShipX");
    if (cJSON_IsNumber(json_ShipX)) {
        ship->ShipX = json_ShipX->valueint;
    } else {
        fprintf(stderr, "Le champ 'ShipX' est manquant ou invalide\n");
        cJSON_Delete(json_ship);
        return -1;
    }

    cJSON *json_ShipY = cJSON_GetObjectItemCaseSensitive(json_ship, "ShipY");
    if (cJSON_IsNumber(json_ShipY)) {
        ship->ShipY = json_ShipY->valueint;
    } else {
        fprintf(stderr, "Le champ 'ShipY' est manquant ou invalide\n");
        cJSON_Delete(json_ship);
        return -1;
    }

    cJSON *json_ship_id = cJSON_GetObjectItemCaseSensitive(json_ship, "ship_id");
    if (cJSON_IsNumber(json_ship_id)) {
        ship->ship_id = json_ship_id->valueint;
    } else {
        fprintf(stderr, "Le champ 'ship_id' est manquant ou invalide\n");
        cJSON_Delete(json_ship);
        return -1;
    }

    cJSON *json_horizontal = cJSON_GetObjectItemCaseSensitive(json_ship, "horizontal");
    if (cJSON_IsBool(json_horizontal)) {
        ship->horizontal = cJSON_IsTrue(json_horizontal);
    } else {
        fprintf(stderr, "Le champ 'horizontal' est manquant ou invalide\n");
        cJSON_Delete(json_ship);
        return -1;
    }

    cJSON *json_isDragging = cJSON_GetObjectItemCaseSensitive(json_ship, "isDragging");
    if (cJSON_IsBool(json_isDragging)) {
        ship->isDragging = cJSON_IsTrue(json_isDragging);
    } else {
        fprintf(stderr, "Le champ 'isDragging' est manquant ou invalide\n");
        cJSON_Delete(json_ship);
        return -1;
    }

    cJSON *json_damage = cJSON_GetObjectItemCaseSensitive(json_ship, "damage");
    if (cJSON_IsNumber(json_damage)) {
        ship->damage = json_damage->valueint;
    } else {
        fprintf(stderr, "Le champ 'damage' est manquant ou invalide\n");
        cJSON_Delete(json_ship);
        return -1;
    }

    // Désérialisation du tableau cases[8][8]
    cJSON *json_cases = cJSON_GetObjectItemCaseSensitive(json_ship, "cases");
    if (!cJSON_IsArray(json_cases)) {
        fprintf(stderr, "Le champ 'cases' n'est pas un tableau\n");
        cJSON_Delete(json_ship);
        return -1;
    }

    int cases_count = cJSON_GetArraySize(json_cases);
    if (cases_count != 8) {
        fprintf(stderr, "Le champ 'cases' doit avoir 8 lignes\n");
        cJSON_Delete(json_ship);
        return -1;
    }

    for(int i = 0; i < 8; i++) {
        cJSON *json_row = cJSON_GetArrayItem(json_cases, i);
        if (!cJSON_IsString(json_row) || (json_row->valuestring == NULL)) {
            fprintf(stderr, "La ligne %d de 'cases' est invalide\n", i);
            cJSON_Delete(json_ship);
            return -1;
        }
        size_t len = strlen(json_row->valuestring);
        if (len != 8) {
            fprintf(stderr, "La ligne %d de 'cases' doit avoir 8 caractères\n", i);
            cJSON_Delete(json_ship);
            return -1;
        }
        memcpy(ship->cases[i], json_row->valuestring, 8);
    }

    // Note : Les textures SDL ne sont pas désérialisées

    cJSON_Delete(json_ship);
    return 0;
}

// Fonction pour sérialiser une structure Fleet en chaîne JSON
char* serialize_fleet(const Fleet *fleet) {
    cJSON *json_fleet = cJSON_CreateObject();
    if (json_fleet == NULL) {
        return NULL;
    }

    cJSON_AddStringToObject(json_fleet, "name", fleet->name);
    cJSON_AddNumberToObject(json_fleet, "id", fleet->id);

    // Sérialisation de chaque navire
    cJSON *json_destroyer = cJSON_Parse(serialize_ships(&fleet->destroyer));
    cJSON *json_torpedo_boat = cJSON_Parse(serialize_ships(&fleet->torpedo_boat));
    cJSON *json_submarine = cJSON_Parse(serialize_ships(&fleet->submarine));
    cJSON *json_aircraft_carrier = cJSON_Parse(serialize_ships(&fleet->aircraft_carrier));
    cJSON *json_cruiser = cJSON_Parse(serialize_ships(&fleet->cruiser));

    if (!json_destroyer || !json_torpedo_boat || !json_submarine ||
        !json_aircraft_carrier || !json_cruiser) {
        fprintf(stderr, "Erreur lors de la sérialisation des Ships\n");
        cJSON_Delete(json_fleet);
        if(json_destroyer) cJSON_Delete(json_destroyer);
        if(json_torpedo_boat) cJSON_Delete(json_torpedo_boat);
        if(json_submarine) cJSON_Delete(json_submarine);
        if(json_aircraft_carrier) cJSON_Delete(json_aircraft_carrier);
        if(json_cruiser) cJSON_Delete(json_cruiser);
        return NULL;
    }

    cJSON_AddItemToObject(json_fleet, "destroyer", json_destroyer);
    cJSON_AddItemToObject(json_fleet, "torpedo_boat", json_torpedo_boat);
    cJSON_AddItemToObject(json_fleet, "submarine", json_submarine);
    cJSON_AddItemToObject(json_fleet, "aircraft_carrier", json_aircraft_carrier);
    cJSON_AddItemToObject(json_fleet, "cruiser", json_cruiser);

    char *json_str = cJSON_PrintUnformatted(json_fleet);
    cJSON_Delete(json_fleet);
    return json_str;
}
// Fonction pour désérialiser une chaîne JSON en structure Fleet
int deserialize_fleet(const char *json_str, Fleet *fleet) {
    cJSON *json_fleet = cJSON_Parse(json_str);
    if (!json_fleet) {
        fprintf(stderr, "Erreur lors de l'analyse du JSON de la flotte\n");
        return -1;
    }

    if (!cJSON_IsObject(json_fleet)) {
        fprintf(stderr, "Le JSON de la flotte n'est pas un objet\n");
        cJSON_Delete(json_fleet);
        return -1;
    }

    // Extraction des champs de Fleet
    cJSON *json_name = cJSON_GetObjectItemCaseSensitive(json_fleet, "name");
    if (cJSON_IsString(json_name) && (json_name->valuestring != NULL)) {
        strncpy(fleet->name, json_name->valuestring, sizeof(fleet->name));
        fleet->name[sizeof(fleet->name) - 1] = '\0';
    } else {
        fprintf(stderr, "Le champ 'name' de la flotte est manquant ou invalide\n");
        cJSON_Delete(json_fleet);
        return -1;
    }

    cJSON *json_id = cJSON_GetObjectItemCaseSensitive(json_fleet, "id");
    if (cJSON_IsNumber(json_id)) {
        fleet->id = json_id->valueint;
    } else {
        fprintf(stderr, "Le champ 'id' de la flotte est manquant ou invalide\n");
        cJSON_Delete(json_fleet);
        return -1;
    }

    // Extraction de chaque navire
    cJSON *json_destroyer = cJSON_GetObjectItemCaseSensitive(json_fleet, "destroyer");
    cJSON *json_torpedo_boat = cJSON_GetObjectItemCaseSensitive(json_fleet, "torpedo_boat");
    cJSON *json_submarine = cJSON_GetObjectItemCaseSensitive(json_fleet, "submarine");
    cJSON *json_aircraft_carrier = cJSON_GetObjectItemCaseSensitive(json_fleet, "aircraft_carrier");
    cJSON *json_cruiser = cJSON_GetObjectItemCaseSensitive(json_fleet, "cruiser");

    if (!json_destroyer || !json_torpedo_boat || !json_submarine ||
        !json_aircraft_carrier || !json_cruiser) {
        fprintf(stderr, "Un des navires de la flotte est manquant\n");
        cJSON_Delete(json_fleet);
        return -1;
    }

    // Désérialisation de chaque navire
    char *str_destroyer = cJSON_PrintUnformatted(json_destroyer);
    char *str_torpedo_boat = cJSON_PrintUnformatted(json_torpedo_boat);
    char *str_submarine = cJSON_PrintUnformatted(json_submarine);
    char *str_aircraft_carrier = cJSON_PrintUnformatted(json_aircraft_carrier);
    char *str_cruiser = cJSON_PrintUnformatted(json_cruiser);

    if (!str_destroyer || !str_torpedo_boat || !str_submarine ||
        !str_aircraft_carrier || !str_cruiser) {
        fprintf(stderr, "Erreur lors de la conversion des navires en chaîne\n");
        cJSON_Delete(json_fleet);
        if(str_destroyer) free(str_destroyer);
        if(str_torpedo_boat) free(str_torpedo_boat);
        if(str_submarine) free(str_submarine);
        if(str_aircraft_carrier) free(str_aircraft_carrier);
        if(str_cruiser) free(str_cruiser);
        return -1;
    }

    if (deserialize_ships(str_destroyer, &fleet->destroyer) != 0 ||
        deserialize_ships(str_torpedo_boat, &fleet->torpedo_boat) != 0 ||
        deserialize_ships(str_submarine, &fleet->submarine) != 0 ||
        deserialize_ships(str_aircraft_carrier, &fleet->aircraft_carrier) != 0 ||
        deserialize_ships(str_cruiser, &fleet->cruiser) != 0) {
        fprintf(stderr, "Erreur lors de la désérialisation des navires\n");
        free(str_destroyer);
        free(str_torpedo_boat);
        free(str_submarine);
        free(str_aircraft_carrier);
        free(str_cruiser);
        cJSON_Delete(json_fleet);
        return -1;
    }

    // Libération de la mémoire allouée pour les chaînes intermédiaires
    free(str_destroyer);
    free(str_torpedo_boat);
    free(str_submarine);
    free(str_aircraft_carrier);
    free(str_cruiser);

    cJSON_Delete(json_fleet);
    return 0;
}

char* serialize_ships(const Ships *ship) {
    cJSON *json_ship = cJSON_CreateObject();
    if (json_ship == NULL) {
        return NULL;
    }

    cJSON_AddStringToObject(json_ship, "name", ship->name);
    cJSON_AddNumberToObject(json_ship, "ammunitions", ship->ammunitions);
    cJSON_AddNumberToObject(json_ship, "n_cases", ship->n_cases);
    cJSON_AddNumberToObject(json_ship, "health", ship->health);
    cJSON_AddNumberToObject(json_ship, "ShipX", ship->ShipX);
    cJSON_AddNumberToObject(json_ship, "ShipY", ship->ShipY);
    cJSON_AddNumberToObject(json_ship, "ship_id", ship->ship_id);
    cJSON_AddBoolToObject(json_ship, "horizontal", ship->horizontal);
    cJSON_AddBoolToObject(json_ship, "isDragging", ship->isDragging);
    cJSON_AddNumberToObject(json_ship, "damage", ship->damage);

    // Sérialisation du tableau cases[8][8] en tableau de chaînes
    cJSON *json_cases = cJSON_CreateArray();
    if (json_cases == NULL) {
        cJSON_Delete(json_ship);
        return NULL;
    }

    for(int i = 0; i < 8; i++) {
        char row[9];
        memcpy(row, ship->cases[i], 8);
        row[8] = '\0';  // Assure la terminaison nulle
        cJSON_AddItemToArray(json_cases, cJSON_CreateString(row));
    }
    cJSON_AddItemToObject(json_ship, "cases", json_cases);

    // Note : Les textures SDL ne sont pas sérialisées

    char *json_str = cJSON_PrintUnformatted(json_ship);
    cJSON_Delete(json_ship);
    return json_str;
}
// json_utils.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cjson/cJSON.h> 

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

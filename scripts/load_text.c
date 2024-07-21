#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/load_text.h"

char* load_text(const char* filename, const char* language) {
    char filepath[256];
    char* text = malloc(MAX_TEXT_LENGTH * sizeof(char));

    // Construire le chemin du fichier en fonction de la langue
    snprintf(filepath, sizeof(filepath), "medias/text/%s/%s.txt",language,filename);

    // Ouvrir le fichier et lire son contenu
    FILE* file = fopen(filepath, "r");
    if (file == NULL) {
        fprintf(stderr, "Erreur d'ouverture du fichier %s\n", filepath);
        free(text);
        return NULL;
    }

    fread(text, 1, MAX_TEXT_LENGTH, file);
    fclose(file);

    return text;
}
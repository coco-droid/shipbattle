#include "../headers/log.h"
#include <stdarg.h>
#include <stdlib.h>

// Pointeur vers le fichier de log
static FILE *logFile = NULL;

// Fonction pour initialiser le fichier de log
void InitLogFile(const char *filename) {
    logFile = fopen(filename, "w");
    if (logFile == NULL) {
        fprintf(stderr, "Could not open log file '%s'.\n", filename);
        exit(EXIT_FAILURE);
    }
}

// Fonction pour enregistrer un message de log
void LogMessage(const char *format, ...) {
    if (logFile == NULL) {
        fprintf(stderr, "Log file is not initialized.\n");
        return;
    }
    
    va_list args;
    va_start(args, format);
    vfprintf(logFile, format, args);
    va_end(args);
    fflush(logFile); // Assure que les messages sont écrits immédiatement
}

// Fonction pour fermer le fichier de log
void CloseLogFile(void) {
    if (logFile != NULL) {
        fclose(logFile);
        logFile = NULL;
    }
}

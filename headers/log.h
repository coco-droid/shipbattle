#ifndef LOG_H
#define LOG_H

#include <stdio.h>

// Initialiser le fichier de log
void InitLogFile(const char *filename);

// Enregistrer un message de log
void LogMessage(const char *format, ...);

// Fermer le fichier de log
void CloseLogFile(void);

#endif // LOG_H

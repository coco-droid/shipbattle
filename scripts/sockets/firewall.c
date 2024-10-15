#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

// Structure pour passer les paramètres au thread
typedef struct {
    int port;
} OpenFirewallPortParams;

// Fonction du thread pour ouvrir le port dans le pare-feu
DWORD WINAPI OpenFirewallPortThread(LPVOID lpParam) {
    if (lpParam == NULL) {
        printf("Paramètre du thread invalide.\n");
        return 1;
    }

    OpenFirewallPortParams* params = (OpenFirewallPortParams*)lpParam;
    int port = params->port;

    // Libérer la mémoire allouée pour les paramètres
    free(params);

    // Construire les paramètres de la commande netsh
    char parameters[256];
    snprintf(parameters, sizeof(parameters),
             "advfirewall firewall add rule name=\"BatailleNavale\" "
             "dir=in action=allow protocol=TCP localport=%d", port);

    SHELLEXECUTEINFOA sei = { sizeof(sei) };
    sei.lpVerb = "runas";           // Demande l'exécution en tant qu'administrateur
    sei.lpFile = "netsh";           // Programme à exécuter
    sei.lpParameters = parameters;  // Paramètres de la commande
    sei.nShow = SW_HIDE;            // Masquer la fenêtre de la console
    sei.fMask = SEE_MASK_NOCLOSEPROCESS;

    if (!ShellExecuteExA(&sei)) {
        DWORD dwError = GetLastError();
        if (dwError == ERROR_CANCELLED) {
            printf("L'utilisateur a annulé l'élévation des privilèges.\n");
        } else {
            printf("Erreur lors de l'ouverture du port %d dans le pare-feu. Code erreur: %lu\n", port, dwError);
        }
        return -1;
    }

    // Attendre que la commande netsh se termine
    WaitForSingleObject(sei.hProcess, INFINITE);

    // Vérifier le code de sortie du processus netsh
    DWORD exitCode;
    if (!GetExitCodeProcess(sei.hProcess, &exitCode)) {
        printf("Impossible d'obtenir le code de sortie du processus netsh.\n");
        CloseHandle(sei.hProcess);
        return -1;
    }

    CloseHandle(sei.hProcess);

    if (exitCode != 0) {
        printf("Erreur lors de l'ouverture du port %d dans le pare-feu. netsh a retourné le code %lu\n", port, exitCode);
        return -1;
    }

    printf("Port %d ouvert dans le pare-feu.\n", port);
    return 0;
}

// Fonction pour ouvrir le port dans le pare-feu de manière asynchrone
int open_firewall_port(int port) {
    // Allouer et définir les paramètres pour le thread
    OpenFirewallPortParams* params = (OpenFirewallPortParams*)malloc(sizeof(OpenFirewallPortParams));
    if (params == NULL) {
        printf("Échec de l'allocation de la mémoire pour les paramètres du thread.\n");
        return -1;
    }
    params->port = port;

    // Créer le thread
    HANDLE hThread = CreateThread(
        NULL,                   // Sécurité par défaut
        0,                      // Taille de pile par défaut
        OpenFirewallPortThread, // Fonction du thread
        (LPVOID)params,         // Paramètres de la fonction
        0,                      // Démarrer immédiatement
        NULL                    // Pas besoin de l'ID du thread
    );

    if (hThread == NULL) {
        printf("Échec de la création du thread. Code erreur: %lu\n", GetLastError());
        free(params);
        return -1;
    }

    // Fermer le handle du thread car il n'est pas nécessaire de le garder ouvert
    CloseHandle(hThread);

    // Retourner immédiatement, l'opération est en cours
    return 0;
}

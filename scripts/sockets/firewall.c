// firewall.c
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int open_firewall_port(int port) {
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

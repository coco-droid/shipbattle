// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include <cjson/cJSON.h> 
#include "libwebsockets.h"
#include "./headers/sockets/server.h"
int main(void) {
    // Récupérer l'adresse IP locale
    char ip_address[INET_ADDRSTRLEN];
    if (get_local_ip(ip_address, sizeof(ip_address)) == 0) {
        printf("Adresse IP locale du serveur : %s\n", ip_address);
    } else {
        fprintf(stderr, "Impossible de récupérer l'adresse IP locale\n");
        return -1;
    }

    // Définir le port
    int port = 9000; // Vous pouvez demander à l'utilisateur de spécifier le port

    // Ouvrir le port dans le pare-feu
    if (open_firewall_port(port) != 0) {
        fprintf(stderr, "Échec de l'ouverture du port dans le pare-feu.\n");
        // Vous pouvez choisir de continuer ou de quitter
        // return -1;
    }

    // Lancer le serveur
    if (run_server(port) != 0) {
        fprintf(stderr, "Erreur lors de l'exécution du serveur.\n");
        return -1;
    }

    return 0;
}

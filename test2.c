#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include <cjson/cJSON.h> 
#include "libwebsockets.h"
#include "./headers/sockets/client.h"

int main(int argc, char **argv) {

    char ip_hote[16];
    int port;

    printf("Enter IP address: ");
    if (scanf("%15s", ip_hote) != 1) {
        fprintf(stderr, "Invalid IP address input.\n");
        return -1;
    }

    printf("Enter Port: ");
    if (scanf("%d", &port) != 1) {
        fprintf(stderr, "Invalid port input.\n");
        return -1;
    }

    if (run_client(ip_hote, port) != 0) {
        fprintf(stderr, "Erreur lors de l'exécution du client.\n");
        return -1;
    }

    return 0;
}
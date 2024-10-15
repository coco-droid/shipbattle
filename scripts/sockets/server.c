// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include <cjson/cJSON.h> 
#include "libwebsockets.h"
#include "../../headers/window/named.h"
#include "../../headers/sockets/p2p.h"
#include "../../headers/sockets/server.h"

int client_connected=0;
// Structure pour stocker les données du jeu
typedef struct {
    int player_matrix[10][10];
    // ... autres données
} game_data_t;

// Callback pour le serveur
static int callback_game(struct lws *wsi, enum lws_callback_reasons reason,
                         void *user, void *in, size_t len) {
    switch (reason) {
        case LWS_CALLBACK_ESTABLISHED:
            printf("Client connecté\n");
            client_connected = 1;

            // Send server's player name to the client
            {
                cJSON *data = cJSON_CreateObject();
                cJSON_AddStringToObject(data, "name", playerName); // Replace with actual name
                p2p_emit(wsi, "player_name", data);
                // 'data' is freed by p2p_emit
            }
            break;

        case LWS_CALLBACK_RECEIVE:
            printf("Données reçues : %.*s\n", (int)len, (char *)in);
            // Use p2p to handle the message
            p2p_handle_received_message(wsi, (char *)in, len);
            break;

        case LWS_CALLBACK_CLOSED:
            printf("Client déconnecté\n");
            break;

        default:
            break;
    }
    return 0;
}

// Protocoles supportés
static struct lws_protocols protocols[] = {
    {
        "game-protocol",
        callback_game,
        0,
        1024,
    },
    { NULL, NULL, 0, 0 } /* terminator */
};

// Fonction pour exécuter le serveur
int run_server(int port) {
    struct lws_context_creation_info info;
    memset(&info, 0, sizeof(info));

    info.port = port;
    info.protocols = protocols;
    info.gid = -1;
    info.uid = -1;
    info.options = 0;

    // Register handlers
    register_server_handlers();

    struct lws_context *context = lws_create_context(&info);
    if (context == NULL) {
        fprintf(stderr, "Erreur lors de la création du contexte WebSocket\n");
        return -1;
    }

    printf("Serveur WebSocket en écoute sur le port %d\n", port);

    while (1) {
        lws_service(context, 1000);
    }

    lws_context_destroy(context);
    return 0;
}

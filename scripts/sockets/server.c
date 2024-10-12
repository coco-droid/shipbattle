// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include <cjson/cJSON.h> 
#include "libwebsockets.h"
#include "../../headers/sockets/server.h"

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
            break;

        case LWS_CALLBACK_RECEIVE:
            printf("Données reçues : %.*s\n", (int)len, (char *)in);
            // Désérialiser la matrice reçue
            {
                int received_matrix[10][10];
                if (deserialize_matrix((char *)in, received_matrix) == 0) {
                    // Traitez la matrice reçue
                    printf("Matrice reçue :\n");
                    for (int i = 0; i < 10; i++) {
                        for (int j = 0; j < 10; j++) {
                            printf("%d ", received_matrix[i][j]);
                        }
                        printf("\n");
                    }

                    // Exemple : Echo de la matrice au client
                    // Vous pouvez intégrer la logique de jeu ici
                    // send_matrix(wsi, received_matrix);
                }
            }
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


// client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include <cjson/cJSON.h> 
#include "libwebsockets.h"

// Prototypes
char* serialize_matrix(int matrix[10][10]);
int deserialize_matrix(const char *json_str, int matrix[10][10]);

// Structure pour stocker les données du jeu
typedef struct {
    int opponent_matrix[10][10];
    // ... autres données
} game_data_t;

// Callback pour le client
static int callback_client(struct lws *wsi, enum lws_callback_reasons reason,
                          void *user, void *in, size_t len) {
    switch (reason) {
        case LWS_CALLBACK_CLIENT_ESTABLISHED:
            printf("Connecté au serveur\n");
            // Envoyer une matrice initiale
            {
                int initial_matrix[10][10] = {0}; // Exemple de matrice initiale
                initial_matrix[0][0] = 1; // Exemple de modification
                char *json_str = serialize_matrix(initial_matrix);
                if (json_str) {
                    // Préparation du buffer
                    int msg_len = strlen(json_str);
                    unsigned char *buf = malloc(LWS_SEND_BUFFER_PRE_PADDING + msg_len + LWS_SEND_BUFFER_POST_PADDING);
                    if (!buf) {
                        fprintf(stderr, "Erreur d'allocation de mémoire pour le buffer\n");
                        free(json_str);
                        break;
                    }

                    memcpy(&buf[LWS_SEND_BUFFER_PRE_PADDING], json_str, msg_len);
                    int ret = lws_write(wsi, &buf[LWS_SEND_BUFFER_PRE_PADDING], msg_len, LWS_WRITE_TEXT);
                    if (ret < msg_len) {
                        fprintf(stderr, "Erreur lors de l'envoi du message\n");
                    }

                    free(buf);
                    free(json_str);
                }
            }
            break;

        case LWS_CALLBACK_CLIENT_RECEIVE:
            printf("Données reçues du serveur : %.*s\n", (int)len, (char *)in);
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
                }
            }
            break;

        case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
            fprintf(stderr, "Erreur de connexion au serveur\n");
            break;

        case LWS_CALLBACK_CLOSED:
            printf("Déconnecté du serveur\n");
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
        callback_client,
        0,
        1024,
    },
    { NULL, NULL, 0, 0 } /* terminator */
};

// Fonction pour envoyer une matrice au serveur
int send_matrix_client(struct lws *wsi, int matrix[10][10]) {
    char *json_str = serialize_matrix(matrix);
    if (!json_str) {
        fprintf(stderr, "Erreur lors de la sérialisation de la matrice\n");
        return -1;
    }

    int msg_len = strlen(json_str);
    unsigned char *buf = malloc(LWS_SEND_BUFFER_PRE_PADDING + msg_len + LWS_SEND_BUFFER_POST_PADDING);
    if (!buf) {
        fprintf(stderr, "Erreur d'allocation de mémoire pour le buffer\n");
        free(json_str);
        return -1;
    }

    memcpy(&buf[LWS_SEND_BUFFER_PRE_PADDING], json_str, msg_len);
    int ret = lws_write(wsi, &buf[LWS_SEND_BUFFER_PRE_PADDING], msg_len, LWS_WRITE_TEXT);
    if (ret < msg_len) {
        fprintf(stderr, "Erreur lors de l'envoi du message\n");
    }

    free(buf);
    free(json_str);
    return ret;
}

// Fonction pour exécuter le client
int run_client(const char *ip_hote, int port) {
    struct lws_context_creation_info info;
    memset(&info, 0, sizeof(info));

    info.port = CONTEXT_PORT_NO_LISTEN;
    info.protocols = protocols;
    info.gid = -1;
    info.uid = -1;
    info.options = 0;

    struct lws_context *context = lws_create_context(&info);
    if (context == NULL) {
        fprintf(stderr, "Erreur lors de la création du contexte WebSocket\n");
        return -1;
    }

    struct lws_client_connect_info ccinfo = {0};
    ccinfo.context = context;
    ccinfo.address = ip_hote;
    ccinfo.port = port;
    ccinfo.path = "/";
    ccinfo.host = ip_hote;
    ccinfo.origin = "origin";
    ccinfo.protocol = protocols[0].name;
    ccinfo.ssl_connection = 0; // Non sécurisé, utilisez 1 si SSL est configuré

    struct lws *wsi = lws_client_connect_via_info(&ccinfo);
    if (wsi == NULL) {
        fprintf(stderr, "Erreur lors de la connexion au serveur WebSocket\n");
        lws_context_destroy(context);
        return -1;
    }

    printf("Tentative de connexion à %s:%d\n", ip_hote, port);

    while (1) {
        lws_service(context, 1000);
    }

    lws_context_destroy(context);
    return 0;
}

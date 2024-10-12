// include/server.h
#ifndef SERVER_H
#define SERVER_H

#include <libwebsockets.h>
#include "json_utils.h"
#include "get_ip_windows.h"
#include "firewall.h"
#include <stddef.h>
// Fonction principale pour exécuter le serveur
int run_server(int port);

// Prototype du callback pour le serveur WebSocket
static int callback_game(struct lws *wsi, enum lws_callback_reasons reason,
                         void *user, void *in, size_t len);
// Prototypes
int get_local_ip(char *buffer, size_t size);
int open_firewall_port(int port);
char* serialize_matrix(int matrix[10][10]);
int deserialize_matrix(const char *json_str, int matrix[10][10]);
#endif // SERVER_H

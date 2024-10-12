// include/client.h
#ifndef CLIENT_H
#define CLIENT_H

#include <libwebsockets.h>
#include "json_utils.h"

// Fonction principale pour exécuter le client
int run_client(const char *ip_host, int port);

// Prototype du callback pour le client WebSocket
int callback_client(struct lws *wsi, enum lws_callback_reasons reason,
                   void *user, void *in, size_t len);

#endif // CLIENT_H

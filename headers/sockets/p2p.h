#ifndef P2P_H
#define P2P_H

#include <cjson/cJSON.h>
#include "libwebsockets.h"

typedef void (*p2p_handler_t)(struct lws *wsi, cJSON *data);

void p2p_register_handler(const char* message_type, p2p_handler_t handler);
void p2p_emit(struct lws *wsi, const char* message_type, cJSON *data);

void p2p_handle_received_message(struct lws *wsi, const char *message, size_t len);

void handle_player_move(struct lws *wsi, cJSON *data);
void handle_player_name(struct lws *wsi, cJSON *data);
void register_server_handlers();
void register_client_handlers();
#endif

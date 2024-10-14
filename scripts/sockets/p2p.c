#include "../../headers/sockets/p2p.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cjson/cJSON.h>

typedef struct p2p_handler_entry {
    char *message_type;
    p2p_handler_t handler;
    struct p2p_handler_entry *next;
} p2p_handler_entry_t;

static p2p_handler_entry_t *handlers = NULL;

void p2p_register_handler(const char* message_type, p2p_handler_t handler) {
    p2p_handler_entry_t *entry = malloc(sizeof(p2p_handler_entry_t));
    entry->message_type = strdup(message_type);
    entry->handler = handler;
    entry->next = handlers;
    handlers = entry;
}

void p2p_emit(struct lws *wsi, const char* message_type, cJSON *data) {
    cJSON *json_message = cJSON_CreateObject();
    cJSON_AddStringToObject(json_message, "type", message_type);
    cJSON_AddItemToObject(json_message, "data", data); // 'data' is now owned by 'json_message'
    char *message_str = cJSON_PrintUnformatted(json_message);
    cJSON_Delete(json_message);

    size_t message_len = strlen(message_str);

    unsigned char *buf = malloc(LWS_PRE + message_len);
    if (buf == NULL) {
        fprintf(stderr, "Memory allocation error in p2p_emit\n");
        free(message_str);
        return;
    }

    memcpy(&buf[LWS_PRE], message_str, message_len);
    lws_write(wsi, &buf[LWS_PRE], message_len, LWS_WRITE_TEXT);

    free(buf);
    free(message_str);
}

void p2p_handle_received_message(struct lws *wsi, const char *message, size_t len) {
    char *message_copy = malloc(len + 1);
    if (!message_copy) {
        fprintf(stderr, "Memory allocation error in p2p_handle_received_message\n");
        return;
    }
    memcpy(message_copy, message, len);
    message_copy[len] = '\0';

    cJSON *json_message = cJSON_Parse(message_copy);
    free(message_copy);

    if (!json_message) {
        fprintf(stderr, "Error parsing JSON message\n");
        return;
    }

    cJSON *json_type = cJSON_GetObjectItemCaseSensitive(json_message, "type");
    cJSON *json_data = cJSON_GetObjectItemCaseSensitive(json_message, "data");

    if (!cJSON_IsString(json_type) || !json_data) {
        fprintf(stderr, "Invalid message format\n");
        cJSON_Delete(json_message);
        return;
    }

    const char *message_type = json_type->valuestring;

    p2p_handler_entry_t *entry = handlers;
    while (entry) {
        if (strcmp(entry->message_type, message_type) == 0) {
            entry->handler(wsi, json_data);
            break;
        }
        entry = entry->next;
    }

    if (!entry) {
        fprintf(stderr, "No handler registered for message type '%s'\n", message_type);
    }

    cJSON_Delete(json_message);
}

//handlers
void handle_player_name(struct lws *wsi, cJSON *data) {
    cJSON *json_name = cJSON_GetObjectItemCaseSensitive(data, "name");
    if (cJSON_IsString(json_name)) {
        const char *player_name = json_name->valuestring;
        printf("Received player name from server: %s\n", player_name);
        // Store the server's player name
    }
}

void handle_player_move(struct lws *wsi, cJSON *data) {
    cJSON *json_x = cJSON_GetObjectItemCaseSensitive(data, "x");
    cJSON *json_y = cJSON_GetObjectItemCaseSensitive(data, "y");
    if (cJSON_IsNumber(json_x) && cJSON_IsNumber(json_y)) {
        int x = json_x->valueint;
        int y = json_y->valueint;
        printf("Received player move: (%d, %d)\n", x, y);
        // Process the move
    }
}

void register_server_handlers() {
    p2p_register_handler("player_name", handle_player_name);
    p2p_register_handler("player_move", handle_player_move);
    // Register other handlers as needed
}

void register_client_handlers() {
    p2p_register_handler("player_name", handle_player_name);
    p2p_register_handler("player_move", handle_player_move);
    // Register other handlers as needed
}

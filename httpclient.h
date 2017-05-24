#include "json.h"
#include "iomessage.h"

#ifndef C_SDK_HTTP_H
#define C_SDK_HTTP_H

struct _ioFogHttpClient {
    char *url_base_rest;
    char *url_get_config;
    char *url_get_next_messages;
    char *url_get_publishers_messages;
    char *url_post_message;
    char *request_body_id;
};

typedef struct _ioFogHttpClient iofog_http_client;

iofog_http_client *new_iofog_http_client(const char *id, int ssl, const char *host, int port);

void free_iofog_http_client(iofog_http_client *client);

int _get_config(iofog_http_client *client, json_object **config);

int _post_message(iofog_http_client *client, io_message *message, json_object **post_response);

#endif //C_SDK_HTTP_H

#include "json.h"

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

iofog_http_client *new_iofog_http_client(char *id, int ssl, char *host, int port);

void free_iofog_http_client(iofog_http_client *client);

struct json_object* getConfig();


#endif //C_SDK_HTTP_H

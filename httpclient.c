#include <stdio.h>
#include "httpclient.h"
#include "global.h"

iofog_http_client *new_iofog_http_client(char *id, int ssl, char *host, int port){
    iofog_http_client *client = malloc(sizeof(iofog_http_client));
    const char *protocol_rest = HTTP;
    if (ssl) {
        protocol_rest = HTTPS;
    }

    size_t url_size = 64;
    client->url_base_rest = malloc(url_size);
    client->url_get_config = malloc(url_size);
    client->url_get_next_messages  = malloc(url_size);
    client->url_get_publishers_messages  = malloc(url_size);
    client->url_post_message  = malloc(url_size);
    client->request_body_id  = malloc(url_size);


    sprintf(client->url_base_rest, "%s://%s:%d", protocol_rest, host, port);
    sprintf(client->url_get_config, "%s%s", client->url_base_rest, URL_GET_CONFIG);
    sprintf(client->url_get_next_messages, "%s%s", client->url_base_rest, URL_GET_NEXT_MESSAGES);
    sprintf(client->url_get_publishers_messages, "%s%s", client->url_base_rest, URL_GET_PUBLISHERS_MESSAGES);
    sprintf(client->url_post_message, "%s%s", client->url_base_rest, URL_POST_MESSAGE);
    sprintf(client->request_body_id, "{\"%s\": %s}", ID, id);

    return client;
}

void free_iofog_http_client(iofog_http_client *client) {
    free(client->url_base_rest);
    free(client->url_get_config);
    free(client->url_get_next_messages);
    free(client->url_get_publishers_messages);
    free(client->url_post_message);
    free(client->request_body_id);
    free(client);
}

struct json_object *getConfig() {
    return NULL;
}

#include <stdio.h>
#include "httpclient.h"
#include "global.h"
#include "util.h"

iofog_http_client *new_iofog_http_client(const char *id, int ssl, const char *host, int port) {
    iofog_http_client *client = malloc(sizeof(iofog_http_client));
    const char *protocol_rest = HTTP;
    if (ssl) {
        protocol_rest = HTTPS;
    }

    size_t url_size = 64;
    client->url_base_rest = malloc(url_size);
    client->url_get_config = malloc(url_size);
    client->url_get_next_messages = malloc(url_size);
    client->url_get_publishers_messages = malloc(url_size);
    client->url_post_message = malloc(url_size);
    client->request_body_id = malloc(url_size);


    sprintf(client->url_base_rest, "%s://%s:%d", protocol_rest, host, port);
    sprintf(client->url_get_config, "%s%s", client->url_base_rest, URL_GET_CONFIG);
    sprintf(client->url_get_next_messages, "%s%s", client->url_base_rest, URL_GET_NEXT_MESSAGES);
    sprintf(client->url_get_publishers_messages, "%s%s", client->url_base_rest, URL_GET_PUBLISHERS_MESSAGES);
    sprintf(client->url_post_message, "%s%s", client->url_base_rest, URL_POST_MESSAGE);
    sprintf(client->request_body_id, "{\"%s\": \"%s\"}", ID, id);

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

int _get_config(iofog_http_client *client, json_object **config) {
    json_object *response;
    int res_code = make_post_request(client->url_get_config, APPLICATION_JSON, client->request_body_id, &response);
    if (res_code != 0) {
        return res_code;
    }
    *config = json_tokener_parse(json_object_get_string(json_object_object_get(response, CONFIG)));
    json_object_put(response);
    if (*config == NULL) {
        fprintf(stderr, "Error while parsing config json\n");
        return 1;
    }

    return 0;
}

int _post_message(iofog_http_client *client, io_message *message, json_object **post_response) {
    char *message_str;
    io_message_to_json_string(message, &message_str);
    int res_code = make_post_request(client->url_post_message, APPLICATION_JSON, message_str, post_response);
    free_json_string(message_str);
    return res_code;
}

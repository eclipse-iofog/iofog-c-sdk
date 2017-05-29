#include <stdio.h>
#include <string.h>
#include "httpclient.h"
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

// user is responsible for calling json_object_put on config
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

int _post_message(iofog_http_client *client, io_message *message, post_message_response *post_response) {
    json_object *resp;
    char *message_str;
    io_message_to_json_string(message, &message_str);

    int rc = make_post_request(client->url_post_message, APPLICATION_JSON, message_str, &resp);
    if (rc != 0) {
        post_response->id = NULL;
        post_response->timestamp = 0;
    } else {
        const char *id = json_object_get_string(json_object_object_get(resp, ID));
        size_t id_len = get_len(id);
        post_response->id = calloc(id_len + 1, 1);
        memcpy(post_response->id, id, id_len);
        post_response->timestamp = json_object_get_int64(json_object_object_get(resp, TIMESTAMP));
    }

    json_object_put(resp);
    free_json_string(message_str);
    return rc;
}

int _get_next_messages(iofog_http_client *client, get_next_messages_response *get_response) {
    json_object *resp;
    int rc = make_post_request(client->url_get_next_messages, APPLICATION_JSON, client->request_body_id, &resp);
    if (rc != 0) {
        get_response->messages = NULL;
        get_response->count = 0;
    } else {
        get_response->count = json_object_get_int(json_object_object_get(resp, COUNT));
        get_response->messages = malloc(get_response->count * sizeof(io_message *));
        json_object *messages = json_object_object_get(resp, MESSAGES);
        for (int i = 0; i < get_response->count; ++i) {
            const char *json_str = json_object_to_json_string_ext(json_object_array_get_idx(messages, (size_t) i),
                                                                  JSON_C_TO_STRING_NOSLASHESCAPE |
                                                                  JSON_C_TO_STRING_PRETTY);
            get_response->messages[i] = io_message_from_json_string(json_str);
        }
    }
    json_object_put(resp);
    return rc;
}

int _get_next_messages_with_query(iofog_http_client *client, json_object *query,
                                  get_messages_with_query_response *get_response) {
    json_object *resp;
    const char *q = json_object_to_json_string_ext(query, JSON_C_TO_STRING_NOSLASHESCAPE | JSON_C_TO_STRING_PRETTY);
    int rc = make_post_request(client->url_get_publishers_messages, APPLICATION_JSON, q, &resp);
    if (rc != 0) {
        get_response->messages = NULL;
        get_response->count = 0;
        get_response->time_frame_start = 0;
        get_response->time_frame_end = 0;
    } else {
        get_response->count = json_object_get_int(json_object_object_get(resp, COUNT));
        get_response->time_frame_start = json_object_get_int64(json_object_object_get(resp, TIME_FRAME_START));
        get_response->time_frame_end = json_object_get_int64(json_object_object_get(resp, TIME_FRAME_END));
        get_response->messages = malloc(get_response->count * sizeof(io_message *));
        json_object *messages = json_object_object_get(resp, MESSAGES);
        for (int i = 0; i < get_response->count; ++i) {
            const char *json_str = json_object_to_json_string_ext(json_object_array_get_idx(messages, (size_t) i),
                                                                  JSON_C_TO_STRING_NOSLASHESCAPE |
                                                                  JSON_C_TO_STRING_PRETTY);
            get_response->messages[i] = io_message_from_json_string(json_str);
        }
    }
    json_object_put(resp);
    return rc;

}

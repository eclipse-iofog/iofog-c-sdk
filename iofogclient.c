/*
 *******************************************************************************
 * Copyright (c) 2018 Edgeworx, Inc.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License v. 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0
 *
 * SPDX-License-Identifier: EPL-2.0
 *******************************************************************************
*/

#include <stdlib.h>
#include "iofogclient.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>


iofog_client *new_iofog_client() {
    char *selfname = getenv(SELFNAME);
    if (selfname == NULL) {
        fprintf(stderr, "Cannot create client with empty id: %s environment variable is not set\n", SELFNAME);
        return NULL;
    }

    char *ssl_var = getenv(SSL);
    int ssl;
    if (ssl_var == NULL) {
        fprintf(stderr, "Empty %s environment variable. Using default value of %d\n", SSL, SSL_DEFAULT);
        ssl = SSL_DEFAULT;
    } else {
        ssl = atoi(ssl_var);
    }

    const char *host = IOFOG;
    int ping_res;
    if (fork() == 0) {
        int fd = open("/dev/null", O_WRONLY);
        dup2(fd, 2);  // redirect stderr
        close(fd);
        exit(execl("/bin/ping", "-c", "3", host, NULL));
    } else {
        wait(&ping_res);
        ping_res = WEXITSTATUS(ping_res);
        if (ping_res != 0) {
            fprintf(stderr, "Host %s is unreachable. Switching to %s\n", host, HOST_DEFAULT);
            host = HOST_DEFAULT;
        }
    }

    iofog_client *client = malloc(sizeof(iofog_client));
    client->host = host;
    client->id = selfname;
    client->ssl = ssl;
    client->port = PORT_IOFOG;
    client->http_client = new_iofog_http_client(selfname, ssl, host, PORT_IOFOG);

    return client;
}

void free_iofog_client(iofog_client *client) {
    free_iofog_http_client(client->http_client);
    free(client);
}

int get_config(iofog_client *client, json_object **config) {
    return _get_config(client->http_client, config);
}

// do not forget to free post response
int post_message(iofog_client *client, io_message *message, post_message_response *post_response) {
    return _post_message(client->http_client, message, post_response);
}

// do not forget to free get response
int get_next_messages(iofog_client *client, get_next_messages_response *get_response) {
    return _get_next_messages(client->http_client, get_response);
}

// do not forget to free get response
int get_next_messages_with_query(iofog_client *client, get_messages_query *query,
                                 get_messages_with_query_response *get_response) {
    json_object *q = json_object_new_object();
    json_object *pub = json_object_new_array();
    for (int i = 0; i < query->publishers_count; ++i) {
        json_object_array_add(pub, json_object_new_string(query->publishers[i]));
    }
    json_object_object_add(q, ID, json_object_new_string(client->id));
    json_object_object_add(q, TIME_FRAME_START, json_object_new_int64((int64_t) query->time_frame_start));
    json_object_object_add(q, TIME_FRAME_END, json_object_new_int64((int64_t) query->time_frame_end));
    json_object_object_add(q, PUBLISHERS, pub);
    int rc = _get_next_messages_with_query(client->http_client, q, get_response);
    json_object_put(q);
    json_object_put(pub);
    return rc;
}

void free_post_message_response(post_message_response *post_response) {
    free(post_response->id);
}

void free_get_next_messages_response(get_next_messages_response *get_response) {
    for (int i = 0; i < get_response->count; ++i) {
        free_io_message(get_response->messages[i]);
    }
    free(get_response->messages);
}

void free_get_next_messages_with_query_response(get_messages_with_query_response *get_response) {
    for (int i = 0; i < get_response->count; ++i) {
        free_io_message(get_response->messages[i]);
    }
    free(get_response->messages);
}

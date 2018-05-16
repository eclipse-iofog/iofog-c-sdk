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

#include "json.h"
#include "iomessage.h"
#include "global.h"

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

int _post_message(iofog_http_client *client, io_message *message, post_message_response *post_response);

int _get_next_messages(iofog_http_client *client, get_next_messages_response *get_response);

int _get_next_messages_with_query(iofog_http_client *client, json_object *query,
                                  get_messages_with_query_response *get_response);


#endif //C_SDK_HTTP_H

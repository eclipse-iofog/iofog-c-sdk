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

#ifndef C_SDK_IOFOGCLIENT_H
#define C_SDK_IOFOGCLIENT_H

#include "httpclient.h"
#include "iomessage.h"
#include "global.h"

struct _IoFogClient {
    const char *id;
    const char *host;
    int port;
    int ssl;
    iofog_http_client *http_client;
};

typedef struct _IoFogClient iofog_client;

iofog_client *new_iofog_client();

void free_iofog_client(iofog_client *client);

void free_post_message_response(post_message_response *post_response);

void free_get_next_messages_response(get_next_messages_response *get_response);

void free_get_next_messages_with_query_response(get_messages_with_query_response *get_response);

int get_config(iofog_client *client, json_object **config);

int post_message(iofog_client *client, io_message *message, post_message_response *post_response);

int get_next_messages(iofog_client *client, get_next_messages_response *get_response);

int get_next_messages_with_query(iofog_client *client, get_messages_query *query,
                                 get_messages_with_query_response *get_response);

#endif //C_SDK_IOFOGCLIENT_H

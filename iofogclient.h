#ifndef C_SDK_IOFOGCLIENT_H
#define C_SDK_IOFOGCLIENT_H

#include "httpclient.h"
#include "iomessage.h"

struct _IoFogClient {
    const char* id;
    const char* host;
    int port;
    int ssl;
    iofog_http_client *http_client;
};

typedef struct _IoFogClient iofog_client;

iofog_client* new_iofog_client();
void free_iofog_client(iofog_client *client);
int get_config(iofog_client *client, json_object **config);
int post_message(iofog_client *client, io_message *message, json_object **post_response);

#endif //C_SDK_IOFOGCLIENT_H

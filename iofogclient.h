#ifndef C_SDK_IOFOGCLIENT_H
#define C_SDK_IOFOGCLIENT_H

struct _IoFogClient {
    const char* id;
    const char* host;
    int port;
    int ssl;
};

typedef struct _IoFogClient iofog_client;

iofog_client* new_iofog_client();
void free_iofog_client(iofog_client *client);


#endif //C_SDK_IOFOGCLIENT_H

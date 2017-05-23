#include <stdlib.h>
#include "iofogclient.h"
#include "global.h"
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
        exit(execl("/bin/ping", "-c", "3", host));
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

    return client;
}

void free_iofog_client(iofog_client *client) {
    free(client);
}


#ifndef C_SDK_GLOBAL_H
#define C_SDK_GLOBAL_H

#include "iomessage.h"

extern const char *IOFOG;
extern const char *ID;
extern const char *CONFIG;

extern const int PORT_IOFOG;
extern const char *SELFNAME;
extern const char *SSL;
extern const short SSL_DEFAULT;
extern const char *HOST_DEFAULT;

extern const char *URL_GET_CONFIG;
extern const char *URL_GET_NEXT_MESSAGES;
extern const char *URL_GET_PUBLISHERS_MESSAGES;
extern const char *URL_POST_MESSAGE;
extern const char *URL_GET_CONTROL_WS;
extern const char *URL_GET_MESSAGE_WS;

extern const char *APPLICATION_JSON;
extern const char *HTTP;
extern const char *HTTPS;
extern const char *WS;
extern const char *WSS;

extern const char *TIME_FRAME_START;
extern const char *TIME_FRAME_END;
extern const char *COUNT;
extern const char *MESSAGES;
extern const char *PUBLISHERS;

extern const unsigned char CODE_ACK;
extern const unsigned char CODE_CONTROL_SIGNAL;
extern const unsigned char CODE_MSG;
extern const unsigned char CODE_RECEIPT;

extern const int WS_ATTEMPT_LIMIT;
extern const int WS_CONNECT_TIMEOUT;

struct _postMessageResponse {
    char *id;
    long long timestamp;
};


struct _getNextMessagesResponse {
    io_message **messages;
    int count;
};

struct _getMessagesWithQueryResponse {
    io_message **messages;
    int count;
    long long time_frame_start;
    long long time_frame_end;
};

struct _getMessagesQuery {
    long long time_frame_start;
    long long time_frame_end;
    char **publishers;
    int publishers_count;
};

typedef struct _postMessageResponse post_message_response;
typedef struct _getNextMessagesResponse get_next_messages_response;
typedef struct _getMessagesWithQueryResponse get_messages_with_query_response;
typedef struct _getMessagesQuery get_messages_query;


#endif

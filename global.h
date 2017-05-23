#ifndef C_SDK_GLOBAL_H
#define C_SDK_GLOBAL_H

extern const char *IOFOG;
extern const char *ID;

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

extern const unsigned char CODE_ACK;
extern const unsigned char CODE_CONTROL_SIGNAL;
extern const unsigned char CODE_MSG;
extern const unsigned char CODE_RECEIPT;

extern const int WS_ATTEMPT_LIMIT;
extern const int WS_CONNECT_TIMEOUT;

#endif

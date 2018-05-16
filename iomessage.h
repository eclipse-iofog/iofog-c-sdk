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

#ifndef C_SDK_IOMESSAGE_H
#define C_SDK_IOMESSAGE_H

extern const int IO_MESSAGE_VERSION;
extern const int IO_MESSAGE_HEADER_SIZE;
extern const unsigned char CODE_MSG;

extern const char *ID;
extern const char *TAG;
extern const char *GROUP_ID;
extern const char *SEQUENCE_NUMBER;
extern const char *SEQUENCE_TOTAL;
extern const char *PRIORITY;
extern const char *TIMESTAMP;
extern const char *PUBLISHER;
extern const char *AUTH_ID;
extern const char *AUTH_GROUP;
extern const char *VERSION;
extern const char *CHAIN_POSITION;
extern const char *HASH;
extern const char *PREVIOUS_HASH;
extern const char *NONCE;
extern const char *DIFFICULTY_TARGET;
extern const char *INFO_TYPE;
extern const char *INFO_FORMAT;
extern const char *CONTEXT_DATA;
extern const char *CONTENT_DATA;

struct _IoMessage {
    char *id;
    char *tag;
    char *group_id;
    int sequence_number;
    int sequence_total;
    int priority;
    long long timestamp;
    char *publisher;
    char *auth_id;
    char *auth_group;
    int version;
    long long chain_position;
    char *hash;
    char *previous_hash;
    char *nonce;
    int difficulty_target;
    char *info_type;
    char *info_format;
    char *context_data;
    char *content_data;
};

typedef struct _IoMessage io_message;

/// Method for converting io_message structure to bytes according to specification
/// \param msg io_message that should be converted
/// \param buf pointer to byte buffer for result storing
/// \return number of bytes written in buf
int io_message_to_bytes(io_message *msg, unsigned char **buf);

/// Method for decoding io_message from bytes according to specification
/// \param bytes byte buffer where io_message is stored
/// \return pointer to decoded io_message structure
io_message *io_message_from_bytes(unsigned char *bytes);

/// Method for encoding io_message @param msg into json string
/// \param msg io_message that needed to be encoded
/// \param str pointer to string where result should be stored
/// \return length of json string
int io_message_to_json_string(io_message *msg, char **str);

/// Method for decoding json string into io_message structure
/// \param str json string with io_message
/// \return pointer to decoded io_message structure
io_message *io_message_from_json_string(const char *str);

/// Method for preparing message package that can be sent via socket
/// \param msg io_message to send
/// \param buf pointer to byte buffer where result should be stored
/// \return number of bytes in @param buf
int prepare_io_message_for_sending_via_socket(io_message *msg, unsigned char **buf);

void encode_data_base64(io_message *msg);

/// Method for unpacking message received via socket
/// \param msg pointer to io_message pointer where result should be stored
/// \param buf received bytes via socket
void get_message_received_via_socket(io_message **msg, unsigned char *buf);

/// Cleaning method
/// \param buf buffer to free
void free_byte_buffer(unsigned char *buf);

/// Cleaning method
/// \param str string to free
void free_json_string(char *str);

/// Cleaning method
/// \param msg io_message to free
void free_io_message(io_message *msg);

#endif //C_SDK_IOMESSAGE_H
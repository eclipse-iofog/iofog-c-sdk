#include <stdlib.h>
#include <memory.h>
#include "iomessage.h"
#include "util.h"
#include <json.h>

const int IO_MESSAGE_VERSION = 4;
const int IO_MESSAGE_HEADER_SIZE = 33;

const char *TAG = "tag";
const char *GROUP_ID = "groupid";
const char *SEQUENCE_NUMBER = "sequencenumber";
const char *SEQUENCE_TOTAL = "sequencetotal";
const char *PRIORITY = "priority";
const char *TIMESTAMP = "timestamp";
const char *PUBLISHER = "publisher";
const char *AUTH_ID = "authid";
const char *AUTH_GROUP = "authgroup";
const char *VERSION = "version";
const char *CHAIN_POSITION = "chainposition";
const char *HASH = "hash";
const char *PREVIOUS_HASH = "previoushash";
const char *NONCE = "nonce";
const char *DIFFICULTY_TARGET = "difficultytarget";
const char *INFO_TYPE = "infotype";
const char *INFO_FORMAT = "infoformat";
const char *CONTEXT_DATA = "contextdata";
const char *CONTENT_DATA = "contentdata";


int io_message_to_bytes(io_message *msg, unsigned char **buffer) {

    if (msg->version != IO_MESSAGE_VERSION) {
        *buffer = NULL;
        return 0;
    }

    size_t msg_len = get_io_message_size_in_bytes(msg);
    *buffer = malloc(msg_len);

    unsigned char *header = *buffer;
    unsigned char *body = *buffer + IO_MESSAGE_HEADER_SIZE;

    put_intBE(IO_MESSAGE_VERSION, header, 2);
    header += 2;

    pack_bytes(&header, &body, msg->id, 1);
    pack_bytes(&header, &body, msg->tag, 2);
    pack_bytes(&header, &body, msg->group_id, 1);
    pack_int(&header, &body, msg->sequence_number, 1);
    pack_int(&header, &body, msg->sequence_total, 1);
    pack_int(&header, &body, msg->priority, 1);
    pack_long(&header, &body, msg->timestamp, 1);
    pack_bytes(&header, &body, msg->publisher, 1);
    pack_bytes(&header, &body, msg->auth_id, 2);
    pack_bytes(&header, &body, msg->auth_group, 2);
    pack_long(&header, &body, msg->chain_position, 1);
    pack_bytes(&header, &body, msg->hash, 2);
    pack_bytes(&header, &body, msg->previous_hash, 2);
    pack_bytes(&header, &body, msg->nonce, 2);
    pack_int(&header, &body, msg->difficulty_target, 1);
    pack_bytes(&header, &body, msg->info_type, 1);
    pack_bytes(&header, &body, msg->info_format, 1);
    pack_bytes(&header, &body, msg->context_data, 4);
    pack_bytes(&header, &body, msg->content_data, 4);

    return (int) msg_len;
}

io_message *io_message_from_bytes(unsigned char *bytes) {

    int version = get_intBE(bytes, 2);
    if (version != IO_MESSAGE_VERSION) {
        return NULL;
    }

    io_message *msg = calloc(1, sizeof(io_message));
    msg->version = version;

    unsigned char *header = bytes + 2;
    unsigned char *body = bytes + IO_MESSAGE_HEADER_SIZE;

    unpack_bytes(&header, &body, &msg->id, 1);
    unpack_bytes(&header, &body, &msg->tag, 2);
    unpack_bytes(&header, &body, &msg->group_id, 1);
    unpack_int(&header, &body, &msg->sequence_number, 1);
    unpack_int(&header, &body, &msg->sequence_total, 1);
    unpack_int(&header, &body, &msg->priority, 1);
    unpack_long(&header, &body, &msg->timestamp, 1);
    unpack_bytes(&header, &body, &msg->publisher, 1);
    unpack_bytes(&header, &body, &msg->auth_id, 2);
    unpack_bytes(&header, &body, &msg->auth_group, 2);
    unpack_long(&header, &body, &msg->chain_position, 1);
    unpack_bytes(&header, &body, &msg->hash, 2);
    unpack_bytes(&header, &body, &msg->previous_hash, 2);
    unpack_bytes(&header, &body, &msg->nonce, 2);
    unpack_int(&header, &body, &msg->difficulty_target, 1);
    unpack_bytes(&header, &body, &msg->info_type, 1);
    unpack_bytes(&header, &body, &msg->info_format, 1);
    unpack_bytes(&header, &body, &msg->context_data, 4);
    unpack_bytes(&header, &body, &msg->content_data, 4);

    return msg;
}

void free_io_message(io_message *msg) {
    free(msg->id);
    free(msg->tag);
    free(msg->group_id);
    free(msg->publisher);
    free(msg->auth_id);
    free(msg->auth_group);
    free(msg->hash);
    free(msg->previous_hash);
    free(msg->nonce);
    free(msg->info_type);
    free(msg->info_format);
    free(msg->context_data);
    free(msg->content_data);
    free(msg);
}

void free_byte_buffer(unsigned char *buf) {
    free(buf);
}

void free_json_string(char *str) {
    free(str);
}

int prepare_io_message_for_sending_via_socket(io_message *msg, unsigned char **buf) {
    unsigned char *msg_bytes;
    int msg_len = io_message_to_bytes(msg, &msg_bytes);
    if (msg_len == 0) {
        *buf = NULL;
        return 0;
    }

    int pack_len = msg_len + 5;
    *buf = malloc((size_t) pack_len);
    unsigned char *pack = *buf;

    *pack = CODE_MSG;
    pack++;
    put_intBE(msg_len, pack, 4);
    pack += 4;
    memcpy(pack, msg_bytes, (size_t) msg_len);
    free_byte_buffer(msg_bytes);

    return pack_len;
}

void get_message_received_via_socket(io_message **msg, unsigned char *buf) {
    *msg = io_message_from_bytes(buf + 5);
}

int io_message_to_json_string(io_message *msg, char **str) {

    struct json_object *jobj = json_object_new_object();
    json_object_object_add(jobj, ID, json_object_new_string(msg->id == NULL ? "" : msg->id));
    json_object_object_add(jobj, TAG, json_object_new_string(msg->tag == NULL ? "" : msg->tag));
    json_object_object_add(jobj, GROUP_ID, json_object_new_string(msg->group_id == NULL ? "" : msg->group_id));
    json_object_object_add(jobj, SEQUENCE_NUMBER, json_object_new_int(msg->sequence_number));
    json_object_object_add(jobj, SEQUENCE_TOTAL, json_object_new_int(msg->sequence_total));
    json_object_object_add(jobj, PRIORITY, json_object_new_int(msg->priority));
    json_object_object_add(jobj, TIMESTAMP, json_object_new_int64((int64_t) msg->timestamp));
    json_object_object_add(jobj, PUBLISHER, json_object_new_string(msg->publisher == NULL ? "" : msg->publisher));
    json_object_object_add(jobj, AUTH_ID, json_object_new_string(msg->auth_id == NULL ? "" : msg->auth_id));
    json_object_object_add(jobj, AUTH_GROUP, json_object_new_string(msg->auth_group == NULL ? "" : msg->auth_group));
    json_object_object_add(jobj, VERSION, json_object_new_int(msg->version));
    json_object_object_add(jobj, CHAIN_POSITION, json_object_new_int64((int64_t) msg->chain_position));
    json_object_object_add(jobj, HASH, json_object_new_string(msg->hash == NULL ? "" : msg->hash));
    json_object_object_add(jobj, PREVIOUS_HASH,
                           json_object_new_string(msg->previous_hash == NULL ? "" : msg->previous_hash));
    json_object_object_add(jobj, NONCE, json_object_new_string(msg->nonce == NULL ? "" : msg->nonce));
    json_object_object_add(jobj, DIFFICULTY_TARGET, json_object_new_int(msg->difficulty_target));
    json_object_object_add(jobj, INFO_TYPE, json_object_new_string(msg->info_type == NULL ? "" : msg->info_type));
    json_object_object_add(jobj, INFO_FORMAT, json_object_new_string(msg->info_format == NULL ? "" : msg->info_format));
    char *context_data_encoded = "";
    char *content_data_encoded = "";
    if (msg->context_data != NULL) {
        context_data_encoded = base64_encode(msg->context_data, get_len(msg->context_data));
    }
    json_object_object_add(jobj, CONTEXT_DATA, json_object_new_string(context_data_encoded));
    if (msg->content_data != NULL) {
        content_data_encoded = base64_encode(msg->content_data, get_len(msg->content_data));
    }
    json_object_object_add(jobj, CONTENT_DATA, json_object_new_string(content_data_encoded));


    if (msg->context_data != NULL) {
        free(context_data_encoded);
    }
    if (msg->content_data != NULL) {
        free(content_data_encoded);
    }
    const char *jsonStr = json_object_to_json_string_ext(jobj, JSON_C_TO_STRING_PLAIN | JSON_C_TO_STRING_NOSLASHESCAPE);
    size_t len = strlen(jsonStr);
    *str = malloc(len + 1);
    memcpy(*str, jsonStr, (size_t) len);
    (*str)[len] = 0;

    json_object_put(jobj);

    return (int) len;
}

io_message *io_message_from_json_string(const char *str) {

    struct json_object *jobj = json_tokener_parse(str);
    io_message *msg = calloc(1, sizeof(io_message));

    unpack_bytes_json(jobj, ID, &msg->id);
    unpack_bytes_json(jobj, TAG, &msg->tag);
    unpack_bytes_json(jobj, GROUP_ID, &msg->group_id);
    msg->sequence_number = json_object_get_int(json_object_object_get(jobj, SEQUENCE_NUMBER));
    msg->sequence_total = json_object_get_int(json_object_object_get(jobj, SEQUENCE_TOTAL));
    msg->priority = json_object_get_int(json_object_object_get(jobj, PRIORITY));
    msg->timestamp = json_object_get_int64(json_object_object_get(jobj, TIMESTAMP));
    unpack_bytes_json(jobj, PUBLISHER, &msg->publisher);
    unpack_bytes_json(jobj, AUTH_ID, &msg->auth_id);
    unpack_bytes_json(jobj, AUTH_GROUP, &msg->auth_group);
    msg->version = json_object_get_int(json_object_object_get(jobj, VERSION));
    msg->chain_position = json_object_get_int64(json_object_object_get(jobj, CHAIN_POSITION));
    unpack_bytes_json(jobj, HASH, &msg->hash);
    unpack_bytes_json(jobj, PREVIOUS_HASH, &msg->previous_hash);
    unpack_bytes_json(jobj, NONCE, &msg->nonce);
    msg->difficulty_target = json_object_get_int(json_object_object_get(jobj, DIFFICULTY_TARGET));
    unpack_bytes_json(jobj, INFO_TYPE, &msg->info_type);
    unpack_bytes_json(jobj, INFO_FORMAT, &msg->info_format);
    unpack_bytes_json(jobj, CONTEXT_DATA, &msg->context_data);
    unpack_bytes_json(jobj, CONTENT_DATA, &msg->content_data);
    json_object_put(jobj);


    if (msg->context_data != NULL) {
        char *context_data_decoded = NULL;
        context_data_decoded = base64_decode(msg->context_data, get_len(msg->context_data));
        free(msg->context_data);
        msg->context_data = context_data_decoded;
    }
    if (msg->content_data != NULL) {
        char *content_data_decoded = NULL;
        content_data_decoded = base64_decode(msg->content_data, get_len(msg->content_data));
        free(msg->content_data);
        msg->content_data = content_data_decoded;
    }

    return msg;
}

#include "util.h"
#include <memory.h>
#include <math.h>
#include <curl.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>
#include <assert.h>

int get_intBE(unsigned char *buf, int len) {
    int res = 0;
    int i;
    int shift = 0;
    for (i = len - 1; i >= 0; i--) {
        res += (int) buf[i] << shift;
        shift += 8;
    }
    return res;
}

long long get_longBE(unsigned char *buf, int len) {
    long long res = 0;
    int i;
    int shift = 0;
    for (i = len - 1; i >= 0; i--) {
        res += (int) buf[i] << shift;
        shift += 8;
    }
    return res;
}

int get_num_of_bytes_for_int(int num) {
    if (num == 0) {
        return 1;
    }

    int num_of_bits = (int) log2(num) + 1;
    int num_of_bytes = (int) ceil(num_of_bits / 8.0);
    return num_of_bytes;
}

int get_num_of_bytes_for_long(long long num) {
    if (num == 0) {
        return 1;
    }

    int num_of_bits = (int) log2(num) + 1;
    int num_of_bytes = (int) ceil(num_of_bits / 8.0);
    return num_of_bytes;
}

void put_intBE(int num, unsigned char *dest, int len) {
    int i;
    int shift;
    for (i = len - 1, shift = 0; i >= 0; i--, shift += 8) {
        dest[i] = (unsigned char) (num >> shift);
    }
}

void put_longBE(long long num, unsigned char *dest, int len) {
    int i;
    int shift;
    for (i = len - 1, shift = 0; i >= 0; i--, shift += 8) {
        dest[i] = (unsigned char) (num >> shift);
    }
}

size_t get_sizeBE(unsigned char *buf, int len) {
    size_t res = 0;
    int shift;
    int i;
    for (i = len - 1, shift = 0; i >= 0; i--, shift += 8) {
        res += buf[i] >> 2 * shift;
    }
    return res;
}

size_t get_len(const char *p) {
    return p != NULL ? strlen(p) : 0;
}

size_t get_io_message_size_in_bytes(io_message *msg) {
    size_t msg_size = get_len(msg->id) +
                      get_len(msg->tag) +
                      get_len(msg->group_id) +
                      get_num_of_bytes_for_int(msg->sequence_number) +
                      get_num_of_bytes_for_int(msg->sequence_total) +
                      get_num_of_bytes_for_int(msg->priority) +
                      get_num_of_bytes_for_long(msg->timestamp) +
                      get_len(msg->publisher) +
                      get_len(msg->auth_id) +
                      get_len(msg->auth_group) +
                      get_num_of_bytes_for_long(msg->chain_position) +
                      get_len(msg->hash) +
                      get_len(msg->previous_hash) +
                      get_len(msg->nonce) +
                      get_num_of_bytes_for_int(msg->difficulty_target) +
                      get_len(msg->info_type) +
                      get_len(msg->info_format) +
                      get_len(msg->context_data) +
                      get_len(msg->content_data);

    return msg_size + IO_MESSAGE_HEADER_SIZE;
}

void unpack_bytes(unsigned char **header, unsigned char **body, char **dest, int size_len) {
    size_t field_len = (size_t) get_sizeBE(*header, size_len);
    *header += size_len;
    if (field_len != 0) {
        *dest = malloc(field_len + 1);
        memcpy(*dest, *body, field_len);
        (*dest)[field_len] = 0;
    }
    *body += field_len;
}

void unpack_int(unsigned char **header, unsigned char **body, int *dest, int size_len) {
    size_t field_len = get_sizeBE(*header, size_len);
    *header += size_len;
    if (field_len != 0) {
        *dest = get_intBE(*body, (int) field_len);
    }
    *body += field_len;
}

void unpack_long(unsigned char **header, unsigned char **body, long long *dest, int size_len) {
    size_t field_len = get_sizeBE(*header, size_len);
    *header += size_len;
    if (field_len != 0) {
        *dest = get_longBE(*body, (int) field_len);
    }
    *body += field_len;
}

void pack_bytes(unsigned char **header, unsigned char **body, char *src, int size_len) {
    int field_len = (int) get_len(src);
    put_intBE(field_len, *header, size_len);
    *header += size_len;
    memcpy(*body, src, (size_t) field_len);
    *body += field_len;
}

void pack_int(unsigned char **header, unsigned char **body, int src, int size_len) {
    int field_len = get_num_of_bytes_for_int(src);
    put_intBE(field_len, *header, size_len);
    *header += size_len;
    put_intBE(src, *body, field_len);
    *body += field_len;
}

void pack_long(unsigned char **header, unsigned char **body, long long src, int size_len) {
    int field_len = get_num_of_bytes_for_long(src);
    put_intBE(field_len, *header, size_len);
    *header += size_len;
    put_longBE(src, *body, field_len);
    *body += field_len;
}

void unpack_bytes_json(json_object *jobj, const char *key, char **dest) {
    struct json_object *field;
    int field_len;
    field = json_object_object_get(jobj, key);
    field_len = json_object_get_string_len(field);
    if (field_len != 0) {
        *dest = malloc((size_t) (field_len + 1));
        memcpy(*dest, json_object_get_string(field), (size_t) field_len);
        (*dest)[field_len] = 0;
    }
}

size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    if (userdata != NULL) {
        *((struct json_object **) userdata) = json_tokener_parse(ptr);
    }
    return nmemb * size;
}

int make_post_request(const char *url, const char *body_type, const char *body, json_object **result) {
    struct curl_slist *headers = NULL;
    CURL *curl = curl_easy_init();
    CURLcode res = CURLE_FAILED_INIT;
    if (curl) {
        char cnt_type_header[64];
        sprintf(cnt_type_header, "Content-Type: %s", body_type);
        headers = curl_slist_append(headers, cnt_type_header);

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, get_len(body));
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, result);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    } else {
        fprintf(stderr, "curl_easy_init() failed\n");
    }

    return res;
}


char *base64_encode(const char *data, size_t input_length) {

    BIO *bio, *b64;
    BUF_MEM *bufferPtr;

    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new(BIO_s_mem());
    bio = BIO_push(b64, bio);

    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); //Ignore newlines - write everything in one line
    BIO_write(bio, data, (int) input_length);
    BIO_flush(bio);
    BIO_get_mem_ptr(bio, &bufferPtr);
    BIO_set_close(bio, BIO_NOCLOSE);

    char *encoded_data = malloc(bufferPtr->length + 1);
    memcpy(encoded_data, bufferPtr->data, bufferPtr->length);
    encoded_data[bufferPtr->length] = '\0';

    BIO_free_all(bio);
    return encoded_data;

}


char *base64_decode(const char *data, size_t input_length) {

    BIO *bio, *b64;
    size_t len = input_length;
    size_t padding = 0;

    if (data[len - 1] == '=') {
        padding++;
    }
    if (data[len - 2] == '=') {
        padding++;
    }

    size_t decoded_len = ((len * 3) / 4 - padding);
    char *decoded_data = malloc(decoded_len + 1);
    decoded_data[decoded_len] = '\0';

    bio = BIO_new_mem_buf(data, -1);
    b64 = BIO_new(BIO_f_base64());
    bio = BIO_push(b64, bio);

    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); //Do not use newlines to flush buffer
    int length = BIO_read(bio, decoded_data, (int) get_len(data));
    assert(length == decoded_len); //length should equal decoded_len, else something went horribly wrong
    BIO_free_all(bio);


    return decoded_data;
}

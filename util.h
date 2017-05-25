#include <stddef.h>
#include "iomessage.h"
#include "json.h"

#ifndef C_SDK_UTIL_H
#define C_SDK_UTIL_H

/// \param msg io_message
/// \return number of bytes needed to store @param msg
size_t get_io_message_size_in_bytes(io_message *msg);

/// \param header current position in header part of byte representation of
/// io_message taken by reference so it can be altered after portion of bytes is read
/// \param body current position in body part of byte representation of
/// io_message taken by reference so it can be altered after portion of bytes is read
/// \param dest pointer where read bytes should be stored taken by reference so memory can be allocated
/// \param size_len number of bytes in which length of fetched field is stored
void unpack_bytes(unsigned char **header, unsigned char **body, char **dest, int size_len);

/// \param header current position in header part of byte representation of
/// io_message taken by reference so it can be altered after portion of bytes is read
/// \param body current position in body part of byte representation of
/// io_message taken by reference so it can be altered after portion of bytes is read
/// \param dest pointer where read integer should be stored
/// \param size_len number of bytes in which length of fetched field is stored
void unpack_int(unsigned char **header, unsigned char **body, int *dest, int size_len);

/// \param header current position in header part of byte representation of
/// io_message taken by reference so it can be altered after portion of bytes is read
/// \param body current position in body part of byte representation of
/// io_message taken by reference so it can be altered after portion of bytes is read
/// \param dest pointer where read long long should be stored
/// \param size_len number of bytes in which length of fetched field is stored
void unpack_long(unsigned char **header, unsigned char **body, long long *dest, int size_len);

/// \param header current position in header part of byte representation of
/// io_message taken by reference so it can be altered after next portion of bytes is written
/// \param body current position in body part of byte representation of
/// io_message taken by reference so it can be altered after next portion of bytes is written
/// \param src bytes to pack
/// \param size_len number of bytes in which length of packed field is stored
void pack_bytes(unsigned char **header, unsigned char **body, char *src, int size_len);

/// \param header current position in header part of byte representation of
/// io_message taken by reference so it can be altered after next portion of bytes is written
/// \param body current position in body part of byte representation of
/// io_message taken by reference so it can be altered after next portion of bytes is written
/// \param src integer to pack
/// \param size_len number of bytes in which length of packed field is stored
void pack_int(unsigned char **header, unsigned char **body, int src, int size_len);

/// \param header current position in header part of byte representation of
/// io_message taken by reference so it can be altered after next portion of bytes is written
/// \param body current position in body part of byte representation of
/// io_message taken by reference so it can be altered after next portion of bytes is written
/// \param src long long to pack
/// \param size_len number of bytes in which length of packed field is stored
void pack_long(unsigned char **header, unsigned char **body, long long src, int size_len);

/// \param num integer to encode in bytes
/// \param dest pointer to buffer where to store @param num
/// \param len number of bytes in which @param num should be placed
void put_intBE(int num, unsigned char *dest, int len);

/// \param num long long to encode in bytes
/// \param dest pointer to buffer where to store @param num
/// \param len number of bytes in which @param num should be placed
void put_longBE(long long num, unsigned char *dest, int len);

/// \param buf pointer to buffer from where integer number should be fetched
/// \param len number of bytes in which integer is stored
/// \return fetched number of type integer
int get_intBE(unsigned char *buf, int len);

/// \param buf pointer to buffer from where size_t number should be fetched
/// \param len number of bytes in which size_t is stored
/// \return fetched number of type size_t
size_t get_sizeBE(unsigned char *buf, int len);

/// \param buf pointer to buffer from where long long number should be fetched
/// \param len number of bytes in which long long is stored
/// \return fetched number of type long long
long long get_longBE(unsigned char *buf, int len);

/// \param num integer number to store in byte buffer
/// \return minimum number of bytes needed to store @param num
int get_num_of_bytes_for_int(int num);

/// \param num long long number to store in byte buffer
/// \return minimum number of bytes needed to store @param num
int get_num_of_bytes_for_long(long long num);

/// \param p pointer to string
/// \return length of string pointed by p or 0 if p is NULL
size_t get_len(const char *p);

/// \param jobj json object that needed to be decoded to io_message
/// \param key name of field in @param jobj
/// \param dest pointer where read bytes should be stored
void unpack_bytes_json(json_object *jobj, const char *key, char **dest);

int make_post_request(const char *url, const char *body_type, const char *body, json_object **res);

char *base64_encode(const char *data, size_t input_length);

char *base64_decode(const char *data, size_t input_length);

#endif //C_SDK_UTIL_H

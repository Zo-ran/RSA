#pragma once

typedef struct cipher cipher_t;

typedef enum mode {
    MODE_512,
    MODE_768,
    MODE_1024
} cmode_t;

cipher_t *create_cipher(cmode_t key_len);
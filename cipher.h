#pragma once

typedef struct cipher cipher_t;

typedef enum mode {
    MODE_512 = 512,
    MODE_768 = 768,
    MODE_1024 = 1024
} cmode_t;

struct cipher {
    bigInt_t p, q;
    bigInt_t d; // private key
    bigInt_t n, e; // public key
    cmode_t key_len;
};

cipher_t *create_cipher(cmode_t key_len);
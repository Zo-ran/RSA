#include "bigint.h"
#include "cipher.h"

#include <stdlib.h>
#include <string.h>

struct cipher {
    bigInt_t p, q;
    bigInt_t d; // private key
    bigInt_t n, e; // public key
    cmode_t key_len;
};

static inline void gen_cipher_pq(cipher_t *cipher) {
    int pq_bitlen = cipher->key_len / 2;
    bigInt_from_bitlen(&cipher->p, pq_bitlen);
    bigInt_from_bitlen(&cipher->q, pq_bitlen);
    bigInt_t const2;
    const2.data[0] = 2;
    const2.len = 1;
    
    // while (1) {
    //     print_bigInt_int(&cipher->p);
    //     bigInt_Add(&cipher->p, &cipher->p, &const2);
    // }
}

cipher_t *create_cipher(cmode_t key_len) {
    cipher_t *ret = (cipher_t *)malloc(sizeof(cipher_t));
    memset(ret, 0, sizeof(cipher_t));

    ret->key_len = key_len;
    gen_cipher_pq(ret);

    return ret;
}
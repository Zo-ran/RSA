#include "bigint.h"
#include "cipher.h"

#include <stdlib.h>
#include <string.h>

static inline void gen_cipher_pq(cipher_t *cipher) {
    int pq_bitlen = cipher->key_len / 2;
    bigInt_from_bitlen(&cipher->p, pq_bitlen);
    bigInt_from_bitlen(&cipher->q, pq_bitlen);
    bigInt_t const2;
    const2.data[0] = 2;
    const2.len = 1;
    
    while (!bigInt_isPrime(&cipher->p)) {
        print_bigInt_int(&cipher->p);
        bigInt_Add(&cipher->p, &cipher->p, &const2);
    }

    while (!bigInt_isPrime(&cipher->q)) {
        print_bigInt_int(&cipher->q);
        bigInt_Add(&cipher->q, &cipher->q, &const2);
    }
}

cipher_t *create_cipher(cmode_t key_len) {
    cipher_t *ret = (cipher_t *)malloc(sizeof(cipher_t));
    memset(ret, 0, sizeof(cipher_t));

    ret->key_len = key_len;
    gen_cipher_pq(ret);

    bigInt_Mul(&ret->n, &ret->p, &ret->q);
    assert(ret->p.data[0] > 0);
    assert(ret->q.data[0] > 0);
    bigInt_t phi_n;
    ret->p.data[0] -= 1;
    ret->q.data[0] -= 1;

    // phi_n = (p - 1) * (q - 1)
    bigInt_Mul(&phi_n, &ret->p, &ret->q);
    ret->p.data[0] += 1;
    ret->q.data[0] += 1;

    // e = 65537
    ret->e.data[0] = 1;
    ret->e.data[1] = 1;
    ret->e.len = 2;

    

    return ret;
}
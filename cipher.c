#include "bigint.h"
#include "cipher.h"

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

void *gen_prime_thread(void *len) {
    int bitlen = (int)len;
    bigInt_t bi, const2;
    const2.data[0] = 2;
    const2.len = 1;
    bigInt_from_bitlen(&bi, len);
    while (!bigInt_isPrime(&bi)) {
        bigInt_Add(&bi, &bi, &const2);
    }
    pthread_exit(&bi);
}

static inline void gen_cipher_pq(cipher_t *cipher) {
    int pq_bitlen = cipher->key_len / 2;
    pthread_t thread1, thread2;
    bigInt_t *p, *q;
    pthread_create(&thread1, NULL, gen_prime_thread, (void *)pq_bitlen);
    pthread_create(&thread2, NULL, gen_prime_thread, (void *)pq_bitlen);
    pthread_join(thread1, &p);
    pthread_join(thread2, &q);
    CP_BIGINT(&cipher->p, p);
    CP_BIGINT(&cipher->q, q);
}

static inline void gen_cipher_pq_new(cipher_t *cipher) {
    int pq_bitlen = cipher->key_len / 2;
    bigInt_t const2;
    const2.data[0] = 2;
    const2.len = 1;
    bigInt_from_bitlen(&cipher->p, pq_bitlen);
    bigInt_from_bitlen(&cipher->q, pq_bitlen);
    
    while (!bigInt_isPrime(&cipher->p)) {
        bigInt_Add(&cipher->p, &cipher->p, &const2);
    }

    while (!bigInt_isPrime(&cipher->q)) {
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

    bigInt_Inverse(&ret->d, &ret->e, &phi_n);

    return ret;
}

// n ^ e mod n

void encrypt(bigInt_t *plain, cipher_t *cipher) {
    uint8_t bits[cipher->e.len * 16];
    int cur = 0;
    for (int i = 0; i < cipher->e.len; ++i) {
        uint16_t t = cipher->e.data[i];
        for (int j = 0; j < 16; ++j) {
            bits[cur++] = t & 0x1;
            t >>= 1;
        }
    }
    while (bits[cur - 1] == 0) cur -= 1;

    bigInt_ModPow(plain, plain, &cipher->n, bits, cur);
}

void decrypt(bigInt_t *plain, cipher_t *cipher) {
    uint8_t bits[cipher->d.len * 16];
    int cur = 0;
    for (int i = 0; i < cipher->d.len; ++i) {
        uint16_t t = cipher->d.data[i];
        for (int j = 0; j < 16; ++j) {
            bits[cur++] = t & 0x1;
            t >>= 1;
        }
    }
    while (bits[cur - 1] == 0) cur -= 1;
    bigInt_ModPow(plain, plain, &cipher->n, bits, cur);
}   
#pragma once

#include <stdint.h>

#define MAX_bigInt_LEN 200
#define MAX_INPUT_LEN 2 * MAX_bigInt_LEN
#define NEW_BIGINT (bigInt_t *)malloc(sizeof(bigInt_t))
#define CP_BIGINT(dst, src) memcpy(dst, src, sizeof(bigInt_t))
#define CLEAR(x) memset(x, 0, sizeof(bigInt_t))

typedef struct bigInt bigInt_t;
struct bigInt {
    int len;
    uint16_t data[MAX_bigInt_LEN];
};

// constructor
void bigInt_from_string(bigInt_t *dest, const char *str);
void bigInt_from_bitlen(bigInt_t *dest, int bitlen);

// operators
uint8_t bigInt_isPrime(const bigInt_t *bi);
void bigInt_Add(bigInt_t *dst, const bigInt_t *a, const bigInt_t *b);
int bigInt_Sub(bigInt_t *dst, const bigInt_t *a, const bigInt_t *b);
void bigInt_Mul(bigInt_t *dst, const bigInt_t *a, const bigInt_t *b);
void bigInt_Div(bigInt_t *dst, const bigInt_t *a, const bigInt_t *b);
void bigInt_Mod(bigInt_t *dst, const bigInt_t *a, const bigInt_t *b);
void bigInt_RShift(bigInt_t *dst, const uint16_t bits);
void bigInt_LShift(bigInt_t *dst, const uint16_t bits);
int bigInt_Cmp(const bigInt_t *a, const bigInt_t *b);
void bigInt_Mul_int(bigInt_t *dst, const bigInt_t *a, const uint16_t d);
static inline void bigInt_MulEq(bigInt_t *dst, const bigInt_t *a) {
    bigInt_t *t1 = NEW_BIGINT;
    CLEAR(t1);
    if (a == dst) {
        bigInt_t *t2 = NEW_BIGINT;
        CP_BIGINT(t2, a);
        bigInt_Mul(t1, dst, t2);
        free(t2);
    } else {
        bigInt_Mul(t1, dst, a);
    }
    CP_BIGINT(dst, t1);
    free(t1);
}
static inline void bigInt_ModEq(bigInt_t *dst, const bigInt_t *a) {
    assert(a != dst);
    bigInt_t *t1 = NEW_BIGINT;
    CLEAR(t1);
    bigInt_Mod(t1, dst, a);
    CP_BIGINT(dst, t1);
    free(t1);
}

// formatter
void print_bigInt_string(const bigInt_t *bi);
void print_bigInt_int(const bigInt_t *bi);
#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_bigInt_LEN 200
#define MAX_INPUT_LEN 2 * MAX_bigInt_LEN
#define NEW_BIGINT (bigInt_t *)malloc(sizeof(bigInt_t))
#define CP_BIGINT(dst, src) memcpy(dst, src, sizeof(bigInt_t))
#define CLEAR(x) memset(x, 0, sizeof(bigInt_t))

typedef struct bigInt bigInt_t;
struct bigInt {
    uint16_t data[MAX_bigInt_LEN];
    int len;
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

// formatter
void print_bigInt_string(const bigInt_t *bi);
void print_bigInt_int(const bigInt_t *bi);

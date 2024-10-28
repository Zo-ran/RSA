#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_bigInt_LEN 200
#define MAX_INPUT_LEN 2 * MAX_bigInt_LEN
#define NEW_BIGINT (bigInt_t *)malloc(sizeof(bigInt_t))
#define CP_BIGINT(dst, src) do { for (int i = 0; i < (src)->len; ++i) (dst)->data[i] = (src)->data[i]; (dst)->len = (src)->len; } while(0)
#define CLEAR(x) memset(x, 0, sizeof(bigInt_t))

typedef struct bigInt bigInt_t;
struct bigInt {
    uint16_t data[MAX_bigInt_LEN];
    int len;
};

// constructor
void bigInt_from_string(bigInt_t *dest, char *str);
void bigInt_from_bitlen(bigInt_t *dest, int bitlen);
void init_bigInt();


// operators
uint8_t bigInt_isPrime(bigInt_t *bi);
void bigInt_Add(bigInt_t *dst, bigInt_t *a, bigInt_t *b);
int bigInt_Sub(bigInt_t *dst, bigInt_t *a, bigInt_t *b);
void bigInt_Mul(bigInt_t *dst, bigInt_t *a, bigInt_t *b);
void bigInt_Mul_New(bigInt_t *dst, bigInt_t *a, bigInt_t *b);
void bigInt_Div(bigInt_t *dst, bigInt_t *a, bigInt_t *b);
void bigInt_Mod(bigInt_t *dst, bigInt_t *a, bigInt_t *b);
void bigInt_RShift(bigInt_t *dst, uint16_t bits);
void bigInt_LShift(bigInt_t *dst, uint16_t bits);
int bigInt_Cmp(bigInt_t *a, bigInt_t *b);
void bigInt_Mul_int(bigInt_t *dst, bigInt_t *a, uint16_t d);

// formatter
void print_bigInt_string(bigInt_t *bi);
void print_bigInt_int(bigInt_t *bi);

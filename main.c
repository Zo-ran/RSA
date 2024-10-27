#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "config.h"

#include "bigint.h"
#include "cipher.h"

#if DEBUG == 0

int main() {
    srand((unsigned)time(NULL));
    char input[MAX_INPUT_LEN + 1] = "\0";
    char buf[64] = "\0";
    cmode_t key_len;
    bigInt_t bigInt;

    printf("Please select the key length (512, 768, 1024): ");
    scanf("%d", &key_len);
    assert(key_len == MODE_512 || key_len == MODE_768 || key_len == MODE_1024);

    printf("Please type \"gen\" to generate key: ");
    scanf("%s", buf);
    assert(strcmp(buf, "gen") == 0);

    cipher_t *cipher = create_cipher(key_len);

    printf("Please type the string to be encrypted: ");
    scanf("%s", input);
    bigInt_from_string(&bigInt, input);
    print_bigInt_string(&bigInt);
}

#else

bigInt_t a, b, c, d, e, f, g;

void test_add() {
    bigInt_from_bitlen(&a, 512);
    print_bigInt_int(&a);
    bigInt_from_bitlen(&b, 1024);
    print_bigInt_int(&b);
    bigInt_Add(&c, &a, &b);
    print_bigInt_int(&c);
}

void test_mul() {
    bigInt_from_bitlen(&a, 32);
    print_bigInt_int(&a);
    print_bigInt_int(&a);
}

void test_div() {
    bigInt_from_bitlen(&a, 32);
    print_bigInt_int(&a);
    bigInt_from_bitlen(&b, 32);
    print_bigInt_int(&b);
    bigInt_Mod(&c, &a, &b);
    print_bigInt_int(&c);
}

void test() {
    bigInt_from_bitlen(&a, 256);
    print_bigInt_int(&a);
    bigInt_isPrime(&a);
}

int main() {
    srand((unsigned)time(NULL));
    test();
}
#endif
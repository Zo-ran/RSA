#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "config.h"

#include "bigint.h"
#include "cipher.h"

extern const uint16_t prime_table[];

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
    bigInt_from_bitlen(&b, 16);
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
    bigInt_from_bitlen(&a, 1024);
    a.len /= 2;
    bigInt_from_bitlen(&b, 512);
    b.len /= 2;
    print_bigInt_int(&a);
    print_bigInt_int(&b);
    bigInt_Div(&c, &a, &b);
    print_bigInt_int(&c);
    // bigInt_Mod(&a, &a, &b);
    bigInt_Mul(&c, &c, &b);
    // print_bigInt_int(&c);
    // bigInt_Add(&c, &a, &c);
    // print_bigInt_int(&c);

    assert(bigInt_Cmp(&a, &b) < 0);
}

void test_prime() {
    bigInt_t const2;
    CLEAR(&const2);
    const2.data[0] = 8;
    const2.len = 1;
    // for (int i = 0; i < 166; ++i) {
    //     const2.data[0] = prime_table[i];
    //     printf("%d\n", bigInt_isPrime(&const2));
    // }
    bigInt_from_bitlen(&a, 256);
    print_bigInt_int(&a);
    while (!bigInt_isPrime(&a)) {
        print_bigInt_int(&a);
        bigInt_Add(&a, &a, &const2);
    }
}

void test_calc() {
    bigInt_from_bitlen(&a, 512);
    bigInt_from_bitlen(&b, 512);
    // b.len = 1;
    print_bigInt_int(&a);
    print_bigInt_int(&b);
    bigInt_Mul(&c, &a, &b);
    print_bigInt_int(&c);
}

int main() {
    // srand((unsigned)time(NULL));
    test_mul();
}
#endif
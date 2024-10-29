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

    printf("Please select the key length (512, 768, 1024): ");
    scanf("%d", &key_len);
    assert(key_len == MODE_512 || key_len == MODE_768 || key_len == MODE_1024);

    printf("Please type \"gen\" to generate key: ");
    scanf("%s", buf);
    assert(strcmp(buf, "gen") == 0);

    clock_t start, end;
    double cpu_time_used;
    start = clock();
    cipher_t *cipher = create_cipher(key_len);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("%f\n", cpu_time_used);

    bigInt_t bi;
    printf("Please type the string to be encrypted: ");
    scanf("%s", input);
    bigInt_from_string(&bi, input);
    print_bigInt_string(&bi);
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
    clock_t start, end;
    double cpu_time_used;

    bigInt_from_bitlen(&a, 1024);
    bigInt_from_bitlen(&b, 1024);


    start = clock();  
    for (int i = 0; i < 20000; ++i)
        bigInt_Mul(&c, &a, &b);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("%f\n", cpu_time_used);


    start = clock();
    for (int i = 0; i < 20000; ++i)
        bigInt_Mul_New(&d, &a, &b);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("%f\n", cpu_time_used);

    assert(bigInt_Cmp(&c, &d) == 0);
}

void test_div() {
    clock_t start, end;
    double cpu_time_used;

    bigInt_from_bitlen(&a, 1024);
    bigInt_from_bitlen(&b, 512);


    start = clock();  
    for (int i = 0; i < 200000; ++i)
        bigInt_Div(&c, &a, &b);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("%f\n", cpu_time_used);
}

void test() {
    bigInt_from_bitlen(&a, 1024);
    a.len /= 2;
    CP_BIGINT(&d, &a);
    bigInt_from_bitlen(&b, 512);
    b.len /= 2;
    print_bigInt_int(&a);
    print_bigInt_int(&b);
    bigInt_Div(&c, &a, &b);
    bigInt_Mod(&a, &a, &b);
    print_bigInt_int(&a);
    bigInt_Mul(&c, &c, &b);
    bigInt_Add(&c, &a, &c);
    assert(bigInt_Cmp(&d, &c) == 0);
    assert(bigInt_Cmp(&a, &b) < 0);
}

void test_calc() {
    a.len = 16;
    a.data[16] = 30348;
    a.data[15] = 5381;
    b.len = 16;
    b.data[15] = 40204;
    b.data[16] = 0;
    printf("%d", bigInt_Cmp(&a, &b));
}

void test_mod(){
    bigInt_from_bitlen(&a, 16);
    // a.len /= 2;
    bigInt_from_bitlen(&b, 16);
    // b.len /= 2;
    print_bigInt_int(&a);
    print_bigInt_int(&b);
    bigInt_Mod(&a, &a, &b);
    print_bigInt_int(&a);
}


void test_prime() {
    bigInt_t const2;
    const2.data[0] = 2;
    const2.len = 1;
    clock_t start, end;
    double cpu_time_used;
    bigInt_from_bitlen(&a, 512);
    start = clock();
    while (!bigInt_isPrime(&a)) {
        print_bigInt_int(&a);
        bigInt_Add(&a, &a, &const2);
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("%f\n", cpu_time_used);
    print_bigInt_int(&a);
}

void test_inverse() {
    bigInt_from_bitlen(&a, 1024);
    print_bigInt_int(&a);
    b.data[0] = 1;
    b.data[1] = 1;
    b.len = 2;
    bigInt_Inverse(&c, &b, &a);
    bigInt_Mul(&d, &b, &c);
    bigInt_Mod(&d, &d, &a);
    print_bigInt_int(&d);
}

int main() {
    srand((unsigned)time(NULL));
    // rand();
    test_prime();
}
#endif
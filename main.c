#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#include "bigint.h"
#include "cipher.h"
#include "sha256.h"

extern const uint16_t prime_table[];

char buf[64];

void Encrypt_Decrypt(cipher_t *cipher) {
    char input[MAX_INPUT_LEN + 1] = "\0";
    bigInt_t plain;
    printf("Please input the string to be encrypted: ");
    scanf("%s", input);
    bigInt_from_string(&plain, input);
    encrypt(&plain, cipher);
    printf("The encrypted text: ");
    print_bigInt_hex(&plain);
    printf("\n");
    printf("Please type \"decrypt\" to generate key: ");
    scanf("%s", buf);
    assert(strcmp(buf, "decrypt") == 0);
    decrypt(&plain, cipher);
    printf("The decrypted text: ");
    print_bigInt_string(&plain);
    printf("\n=================================================\n\n");
}

void Sign_Verify(cipher_t *cipher) {
    bigInt_t plain;
    char input[MAX_INPUT_LEN + 1] = "\0";
    unsigned char hash_out[32] = "\0";
    printf("Please input the message to be signed: ");
    scanf("%s", input);
    printf("Please type \"hash\" to hash the message: ");
    scanf("%s", buf);
    assert(strcmp(buf, "hash") == 0);
    sha256(input, strlen(input), hash_out);
    bigInt_from_string(&plain, hash_out);
    printf("The message after hash: ");
    print_bigInt_hex(&plain);
    printf("\n");
    printf("The signature: ");
    decrypt(&plain, cipher);
    print_bigInt_hex(&plain);
    printf("\n");
    printf("Please type \"verify\" to verify the signature: ");
    scanf("%s", buf);
    assert(strcmp(buf, "verify") == 0);
    printf("The result: ");
    encrypt(&plain, cipher);
    print_bigInt_hex(&plain);
    printf("\n=================================================\n\n");
}

int main() {
    srand((unsigned)time(NULL));
    cmode_t key_len;

    printf("Please select the key length (512, 768, 1024): ");
    scanf("%d", &key_len);
    assert(key_len == MODE_512 || key_len == MODE_768 || key_len == MODE_1024);

    printf("Please type \"gen\" to generate key: ");
    scanf("%s", buf);
    assert(strcmp(buf, "gen") == 0);

    clock_t start, end;
    double cpu_time_used;
    cipher_t *cipher;
    start = clock();
    cipher = create_cipher(key_len);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("generated in %fs\n", cpu_time_used);
    printf("Your public key (in hexadecimal)  is: ");
    print_bigInt_hex(&cipher->n);
    printf(" ");
    print_bigInt_hex(&cipher->e);
    printf("\n");
    printf("Your private key (in hexadecimal) is: ");
    print_bigInt_hex(&cipher->d);
    printf("\n\n");
    printf("=================================================\n0. exit   1. encrypt/decrypt   2. sign/verify\n=================================================\n\n");
    uint8_t func = 100;
    while (func) {
        printf("Please select the function: ");
        scanf("%d", &func);
        if (func == 1) {
            Encrypt_Decrypt(cipher);
        } else if (func == 2) {
            Sign_Verify(cipher);
        }
    }
}
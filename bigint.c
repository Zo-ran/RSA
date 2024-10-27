#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "bigint.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define true 1
#define false 0

#define PRIME_CONFIDENCE 15

const uint16_t prime_table[] = {
        3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41,
        43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 113,
        193, 241, 257, 337, 353, 401, 433, 449, 577, 593, 641,
        673, 769, 881, 929, 977, 1009, 1153, 1201, 1217, 1249,
        1297, 1361, 1409, 1489, 1553, 1601, 1697, 1777, 1873,
        1889, 2017, 2081, 2113, 2129, 2161, 2273, 2417, 2593,
        2609, 2657, 2689, 2753, 2801, 2833, 2897, 3041, 3089,
        3121, 3137, 3169, 3217, 3313, 3329, 3361, 3457, 3617,
        3697, 3761, 3793, 3889, 4001, 4049, 4129, 4177, 4241,
        4273, 4289, 4337, 4481, 4513, 4561, 4657, 4673, 4721,
        4801, 4817, 4993, 5009, 5153, 5233, 5281, 5297, 5393,
        5441, 5521, 5569, 5857, 5953, 6113, 6257, 6337, 6353,
        6449, 6481, 6529, 6577, 6673, 6689, 6737, 6833, 6961,
        6977, 7057, 7121, 7297, 7393, 7457, 7489, 7537, 7649,
        7681, 7793, 7841, 7873, 7937, 8017, 8081, 8161, 8209,
        8273, 8353, 8369, 8513, 8609, 8641, 8689, 8737, 8753,
        8849, 8929, 9041, 9137, 9281, 9377, 9473, 9521, 9601,
        9649, 9697, 9857
};

void bigInt_from_string(bigInt_t *dest, const char *str) {
    int len = strlen(str);
    assert(len <= MAX_INPUT_LEN && len > 0);
    
    uint16_t tmp = 0;
    memset(dest, 0, sizeof(bigInt_t));

    for (int i = 0; i < len; ++i) {
        uint16_t curr = str[i];
        if (i % 2 == 0) {
            tmp = curr;
        } else {
            tmp |= (curr << 8);
            dest->data[dest->len] = tmp;
            dest->len += 1;
        }
    }

    if (len % 2 != 0) {
        dest->data[dest->len] = tmp;
        dest->len += 1;
    }
}

void bigInt_from_bitlen(bigInt_t *dest, int bitlen) {
    uint8_t bits[bitlen];
    dest->len = 0;
    for (int i = 0; i < bitlen; ++i) {
        bits[i] = rand() & 0x1;
    }
    bits[bitlen - 1] = 1;
    bits[0] = 1;
    for (int i = 0; i < bitlen; i += 16) {
        uint16_t tmp = 0;
        for (int j = i; j < i + 16 && j < bitlen; ++j)
            tmp |= (bits[j] << (j - i));
        dest->data[dest->len] = tmp;
        dest->len += 1;
    }
}

void bigInt_ModPow(bigInt_t *dst, const bigInt_t *a, const bigInt_t *b, const bigInt_t *mod) {
    bigInt_t *ap = NEW_BIGINT;
    CP_BIGINT(ap, a);

    CLEAR(dst);
    dst->data[0] = 1;
    dst->len = 1;
    uint8_t bits[b->len * 16];
    int cur = 0;
    for (int i = 0; i < b->len; ++i) {
        uint16_t t = b->data[i];
        for (int j = 0; j < 16; ++j) {
            bits[cur++] = t & 0x1;
            t >>= 1;
        }
    }
    
    for (int i = 0; i < cur; ++i) {
        if (bits[cur]) {
            bigInt_MulEq(dst, ap);
            bigInt_ModEq(dst, mod);
        }
        bigInt_MulEq(ap, ap);
        bigInt_ModEq(ap, mod);
    }
    free(ap);
}

uint8_t bigInt_isPrime(const bigInt_t *bi) {
    if (bi->len == 1) {
        if (bi->data[0] == 1)
            return false;
        if (bi->data[0] == 2)
            return true;
    }
    if (bi->data[0] % 2 == 0)
        return false;
    bigInt_t *t1 = NEW_BIGINT;
    bigInt_t *t2 = NEW_BIGINT;
    CLEAR(t2);
    for (int i = 0; i < sizeof(prime_table) / sizeof(prime_table[0]); ++i) {
        CLEAR(t1);
        t2->data[0] = prime_table[i];
        t2->len = 1;
        bigInt_Mod(t1, bi, t2);
        assert(t1->len == 1);
        if (t1->data[0] == 0)
            return false;
    }
    // n - 1 = 2^s * d
    // a0 + a1 * 2^16 + a2 * 2^32 + a3 * 2^48
    int s = 0;
    bigInt_t *d = NEW_BIGINT;
    CP_BIGINT(d, bi);
    d->data[0] -= 1;
    while ((d->data[0] & 0x1) == 0) {
        s += 1;
        bigInt_RShift(d, 1);
    }

    // a ^ (n - 1) = 1 (mod n)
    // n - 1 = 2^s * d
    // a ^ d
    bigInt_t *a = NEW_BIGINT;
    for (int i = 0; i < PRIME_CONFIDENCE; ++i) {
        CLEAR(a);
        bigInt_from_bitlen(a, 16);
        bigInt_ModPow(a, a, d, bi);

        if (a->len == 0 && a->data[0] == 1)
            continue;
        bigInt_t *tmp = NEW_BIGINT;
        for (int i = 0; i < s; ++i) {
            bigInt_Sub(tmp, bi, a);
            if (tmp->data[0] != 1 || tmp->len != 1)
                return false;
            
        }
    }

    return true;
}

void bigInt_Add_shift(bigInt_t *dst, const bigInt_t *a, const bigInt_t *b, int bits) {
    int n = MAX(a->len, b->len + bits);
    bigInt_t *f = NEW_BIGINT;
    CP_BIGINT(f, a);
    uint16_t carry = 0;
    for (int i = 0; i < bits; ++i)
        dst->data[i] = a->data[i];
    for (int i = bits; i < n; ++i) {
        uint32_t tmp = a->data[i] + b->data[i - bits] + carry;
        carry = (tmp & 0xffff0000) >> 16;
        dst->data[i] = tmp & 0xffff;
    }
    if (!carry) {
        dst->len = n;
    } else {
        dst->data[n] = carry;
        dst->len = n + 1;
    }
    while (dst->data[dst->len - 1] == 0 && dst->len != 1) dst->len -= 1;
}

void bigInt_Add(bigInt_t *dst, const bigInt_t *a, const bigInt_t *b) {
    int n = MAX(a->len, b->len);
    uint16_t carry = 0;
    for (int i = 0; i < n; ++i) {
        uint32_t tmp = a->data[i] + b->data[i] + carry;
        carry = (tmp & 0xffff0000) >> 16;
        dst->data[i] = tmp & 0xffff;
    }
    if (!carry) {
        dst->len = n;
    } else {
        dst->data[n] = carry;
        dst->len = n + 1;
    } 
    while (dst->data[dst->len - 1] == 0 && dst->len != 1) dst->len -= 1;
}

int bigInt_Sub(bigInt_t *dst, const bigInt_t *a, const bigInt_t *b) {
    int n = MAX(a->len, b->len);
    uint16_t carry = 0;
    dst->len = a->len;
    for (int i = 0; i < n; ++i) {
        uint32_t tmp = a->data[i] - b->data[i] - carry;
        carry = !!((tmp & 0xffff0000) >> 16);
        dst->data[i] = tmp & 0xffff;
    }
    while (dst->data[dst->len - 1] == 0 && dst->len != 1) dst->len -= 1;
    return carry ? -1 : 1;
}

void bigInt_Mul(bigInt_t *dst, const bigInt_t *a, const bigInt_t *b) {
    int n = MAX(a->len, b->len);
    if (n == 0) {
        dst->len = 1;
        dst->data[0] = 0;
    } else if (n == 1) {
        uint32_t tmp = a->data[0] * b->data[0];
        dst->data[0] = tmp & 0xffff;
        if (tmp & 0xffff0000) {
            dst->len = 2;
            dst->data[1] = (tmp >> 16) & 0xffff;
        } else {
            dst->len = 1;
        }
    } else {
        int half = n / 2;
        bigInt_t *a0 = NEW_BIGINT;
        bigInt_t *a1 = NEW_BIGINT;
        bigInt_t *b0 = NEW_BIGINT;
        bigInt_t *b1 = NEW_BIGINT;
        bigInt_t *z1 = NEW_BIGINT;
        bigInt_t *z2 = NEW_BIGINT;
        memset(a1, 0, sizeof(bigInt_t));
        memset(a0, 0, sizeof(bigInt_t));
        memset(b1, 0, sizeof(bigInt_t));
        memset(b0, 0, sizeof(bigInt_t));
        memset(z2, 0, sizeof(bigInt_t));
        memset(z1, 0, sizeof(bigInt_t));
        a0->len = half; b0->len = half;
        for (int i = 0; i < half; ++i) {
            a0->data[i] = a->data[i];
            b0->data[i] = b->data[i];
        }
        a1->len = n - half, b1->len = n - half;
        for (int i = half; i < n; ++i) {
            a1->data[i - half] = a->data[i];
            b1->data[i - half] = b->data[i];
        }
        bigInt_Mul(dst, a0, b0); // BD 
        bigInt_Mul(z2, a1, b1); // AC

        bigInt_Add(a1, a1, a0); // (A + B)
        bigInt_Add(b1, b1, b0); // (C + D)
        bigInt_Mul(z1, a1, b1); // (A + B)(C + D)
        bigInt_Sub(z1, z1, dst);
        bigInt_Sub(z1, z1, z2); // (A + B)(C + D) - AC - BD = AD + BC

        bigInt_Add_shift(dst, dst, z1, half);
        bigInt_Add_shift(dst, dst, z2, 2 * half);
        free(a0); free(a1); free(b0); free(b1); free(z1); free(z2); 
    }
    while (dst->data[dst->len - 1] == 0 && dst->len != 1) dst->len -= 1;
}

void bigInt_Mul_int(bigInt_t *dst, const bigInt_t *a, const uint16_t d) {
    if (d == 0) {
        dst->data[0] = 0;
        dst->len = 1;
    } else {
        uint16_t carry = 0;
        dst->len = a->len;
        for (int i = 0; i < a->len; ++i) {
            uint32_t tmp = a->data[i] * d + carry;
            carry = (tmp & 0xffff0000) >> 16;
            dst->data[i] = tmp & 0xffff;
        }
        if (carry != 0) {
            dst->data[dst->len] = carry;
            dst->len += 1;
        }
    }
    while (dst->data[dst->len - 1] == 0 && dst->len != 1) dst->len -= 1;
}

void bigInt_Div(bigInt_t *dst, const bigInt_t *a, const bigInt_t *b) {
    assert(b->len != 0);
    assert(b->len != 1 || b->data[0] != 0);
    int cmp = bigInt_Cmp(a, b);
    if (cmp < 0) {
        dst->len = 1;
        dst->data[0] = 0;
    } else if (cmp == 0) {
        dst->len = 1;
        dst->data[0] = 1;
    } else {
        // mul on new var
        bigInt_t *a0 = NEW_BIGINT;
        bigInt_t *b0 = NEW_BIGINT;
        memcpy(a0, a, sizeof(bigInt_t));
        memcpy(b0, b, sizeof(bigInt_t));

        // ensure highest >= base / 2, base = 65536
        uint16_t d = 65536 / (b->data[b->len - 1] + 1);
        if (d != 1) {
            bigInt_Mul_int(a0, a0, d);
            bigInt_Mul_int(b0, b0, d);
        }
        uint16_t highest = b0->data[b0->len - 1];

        if (a0->len == b0->len) {
            dst->len = 1;
            dst->data[0] = 1;
        } else {
            int Ipos = a0->len - b0->len;
            bigInt_t *remainder = NEW_BIGINT; // remainder slide window
            CLEAR(remainder);
            bigInt_t *tmp = NEW_BIGINT;
            for (int i = Ipos; i < a0->len; ++i)
                remainder->data[i - Ipos] = a0->data[i];
            remainder->len = b0->len;
            if (bigInt_Cmp(remainder, b0) < 0) {
                dst->len = a0->len - b0->len;
            } else {
                bigInt_Sub(remainder, remainder, b0);
                dst->len = a0->len - b0->len + 1;
                dst->data[Ipos] = 1;
            }
            for (int i = Ipos - 1; i >= 0; --i) {
                for (int j = remainder->len; j >= 1; --j)
                    remainder->data[j] = remainder->data[j - 1];
                remainder->data[0] = a0->data[i];
                remainder->len += 1;
                while (remainder->data[remainder->len - 1] == 0 && remainder->len != 1) remainder->len -= 1;
                int quotient;
                if (remainder->len <= 1)
                    quotient = 0;
                else 
                    quotient = (remainder->data[remainder->len - 1] * 65536 + remainder->data[remainder->len - 2]) / highest - 2; // estimated quotient
                quotient = quotient > 0 ? quotient : 0;
                assert((quotient & 0xffff0000) == 0);
                // r = a - pq
                CLEAR(tmp);
                bigInt_Mul_int(tmp, b0, quotient);
                bigInt_Sub(remainder, remainder, tmp);
                while (bigInt_Cmp(remainder, b0) >= 0) {
                    bigInt_Sub(remainder, remainder, b0);
                    quotient += 1;
                }
                dst->data[i] = quotient;
            }
            free(remainder); free(tmp);
        }
        free(a0); free(b0);
    }
}

void bigInt_Mod(bigInt_t *dst, const bigInt_t *a, const bigInt_t *b) {
    bigInt_t *tmp = NEW_BIGINT;
    CLEAR(tmp);
    bigInt_Div(tmp, a, b);
    bigInt_Mul(dst, tmp, b);
    bigInt_Sub(dst, a, dst);
    free(tmp);
}

void bigInt_RShift(bigInt_t *dst, const uint16_t bits) {
    for (uint16_t i = 0; i < bits; ++i)
        for (int j = 0; j < dst->len; ++j) {
            dst->data[j] >>= 1;
            if (
                dst->data[j + 1] & 0x1)
                dst->data[j] |= 0x8000;
        }
    while (dst->data[dst->len - 1] == 0 && dst->len != 1) dst->len -= 1;   
}

void bigInt_LShift(bigInt_t *dst, const uint16_t bits) {
    for (uint16_t i = 0; i < bits; ++i)
        for (int j = dst->len - 1; j >= 0; j -= 1) {
            if (dst->data[j] & 0x8000)
                dst->data[j + 1] |= 0x1;
            dst->data[j] <<= 1;
        }
    if (dst->data[dst->len] != 0)
        dst->len += 1;
}

int bigInt_Cmp(const bigInt_t *a, const bigInt_t *b) {
    int ret = 0;
    if (a->len > b->len) {
        ret = 1;
    } else if (a->len < b->len) {
        ret = -1;
    } else {
        for (int i = a->len - 1; i >= 0; --i)
            if (a->data[i] > b->data[i]) {
                ret = 1;
                break;
            } else if (a->data[i] < b->data[i]) {
                ret = -1;
                break;
            }
    }
    return ret;
}

void print_bigInt_string(const bigInt_t *bi) {
    char buf[MAX_INPUT_LEN + 1] = "\0";
    int cur_pos = 0;
    for (int i = 0; i < bi->len; ++i) {
        buf[cur_pos++] = bi->data[i] & 0xFF;
        buf[cur_pos++] = bi->data[i] >> 8;
    }
    printf("%s\n", buf);
}

void print_bigInt_int(const bigInt_t *bi) {
    for (int i = 0; i < bi->len; ++i)
        printf("%d ", bi->data[i]);
    printf("\n");
}
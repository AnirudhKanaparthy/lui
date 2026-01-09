/**
 * This is just a tester program I made for uint128_t.
 * For the time being it is quick and dirty.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define UINT128_T_IMPLEMENTATION
#include "uint128_t.h"

void u128_sprintf(char* out, uint128_t n) {
    if (!out) return;
    
    if (n.high) {
        sprintf(out, "%lx%0*lx", n.high, 64/4, n.low);
    } else {
        sprintf(out, "%lx", n.low);
    }
}

void u128_printf(uint128_t n) {
    static char buffer[33] = {0};
    memset(buffer, 0, sizeof(buffer));
    u128_sprintf(buffer, n);
    printf("%s", buffer);
}

uint8_t hex_bits(const char c[2]) {
    uint8_t res = 0;
    if (c[1] >= '0' && c[1] <= '9') {
        res |= c[1] - '0';
    } else if (c[1] >= 'a' && c[1] <= 'f') {
        res |= c[1] - 'a' + 10;
    } else if (c[1] >= 'A' && c[1] <= 'F') {
        res |= c[1] - 'A' + 10;
    }
    
    if (c[0] >= '0' && c[0] <= '9') {
        res |= (c[0] - '0') << 4;
    } else if (c[0] >= 'a' && c[0] <= 'f') {
        res |= (c[0] - 'a' + 10) << 4;
    } else if (c[0] >= 'A' && c[0] <= 'F') {
        res |= (c[0] - 'A' + 10) << 4;
    }
    
    return res;
}

uint128_t u128_from_str(const char* str, int size) {
    // TODO: Improve on this so that it can deal with shorter strings than 32
    if (size != 32) {
        return (uint128_t){};
    }
    uint128_t res;
    uint8_t* cur = (uint8_t*)&res;
    
    uint16_t* str16 = (uint16_t*)str;
    const int len = size / 2;
    for (int i = 0; i < len; ++i) {
        const int idx = len - i - 1;
        cur[i] = hex_bits((char*)(str16 + idx));
    }
    return res;
}

int lpad(char* dest, const char* data, size_t len, char pad_char, size_t padding) {
    if (dest == NULL || data == NULL) return -1;
    if (len > padding) return -1;

    memset(dest, 0, padding);
    for (size_t i = 0; i < len; ++i) {
        dest[padding - i - 1] = data[len - i - 1];
    }
    int count = 0;
    while (*dest == '\0') {
        *dest = pad_char;
        ++dest;
        ++count;
    }
    return count;
}

void print_usage(const char* prog) {
    printf("Usage:\n");
    printf("  %s arith <hex_a> <hex_b>       - test +, -, *, /, %%\n", prog);
    printf("  %s arith_u64 <hex_a> <hex_b>   - test +, -, *, /, %% with u64\n", prog);
    printf("  %s cmp <hex_a> <hex_b>         - test <, <=, ==, >=, >\n", prog);
    printf("  %s cmp_u64 <hex_a> <hex_b>     - test comparisons with u64\n", prog);
    printf("  %s shift <hex_a> <amount>      - test <<, >>\n", prog);
    printf("  %s bitwise <hex_a> <hex_b>     - test &, |\n", prog);
    printf("  %s bitwise_u64 <hex_a> <hex_b> - test &, | with u64\n", prog);
    printf("  %s unary <hex_a>               - test inc, dec, not\n", prog);
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        print_usage(argv[0]);
        return 1;
    }
    
    const char* mode = argv[1];
    char buffer_a[33] = {0};
    char buffer_b[33] = {0};
    
    lpad(buffer_a, argv[2], strlen(argv[2]), '0', 32);
    uint128_t a = u128_from_str(buffer_a, 32);
    
    if (strcmp(mode, "arith") == 0 && argc == 4) {
        lpad(buffer_b, argv[3], strlen(argv[3]), '0', 32);
        uint128_t b = u128_from_str(buffer_b, 32);
        
        printf("add="); u128_printf(u128_add(a, b)); printf("\n");
        printf("sub="); u128_printf(u128_sub(a, b)); printf("\n");
        printf("mul="); u128_printf(u128_mul(a, b)); printf("\n");
        if (b.low != 0 || b.high != 0) {
            printf("div="); u128_printf(u128_div(a, b)); printf("\n");
            printf("mod="); u128_printf(u128_mod(a, b)); printf("\n");
        }
    }
    else if (strcmp(mode, "arith_u64") == 0 && argc == 4) {
        uint64_t b = strtoull(argv[3], NULL, 16);
        
        printf("add="); u128_printf(u128_add_u64(a, b)); printf("\n");
        printf("sub="); u128_printf(u128_sub_u64(a, b)); printf("\n");
        printf("mul="); u128_printf(u128_mul_u64(a, b)); printf("\n");
        if (b != 0) {
            printf("div="); u128_printf(u128_div_u64(a, b)); printf("\n");
            printf("mod="); u128_printf(u128_mod_u64(a, b)); printf("\n");
        }
    }
    else if (strcmp(mode, "cmp") == 0 && argc == 4) {
        lpad(buffer_b, argv[3], strlen(argv[3]), '0', 32);
        uint128_t b = u128_from_str(buffer_b, 32);
        
        printf("lt=%d\n", u128_lt(a, b));
        printf("le=%d\n", u128_le(a, b));
        printf("eq=%d\n", u128_eq(a, b));
        printf("ge=%d\n", u128_ge(a, b));
        printf("gt=%d\n", u128_gt(a, b));
    }
    else if (strcmp(mode, "cmp_u64") == 0 && argc == 4) {
        uint64_t b = strtoull(argv[3], NULL, 16);
        
        printf("lt=%d\n", u128_lt_u64(a, b));
        printf("le=%d\n", u128_le_u64(a, b));
        printf("eq=%d\n", u128_eq_u64(a, b));
        printf("ge=%d\n", u128_ge_u64(a, b));
        printf("gt=%d\n", u128_gt_u64(a, b));
    }
    else if (strcmp(mode, "shift") == 0 && argc == 4) {
        uint8_t amount = (uint8_t)atoi(argv[3]);
        
        printf("lshift="); u128_printf(u128_lshift(a, amount)); printf("\n");
        printf("rshift="); u128_printf(u128_rshift(a, amount)); printf("\n");
    }
    else if (strcmp(mode, "bitwise") == 0 && argc == 4) {
        lpad(buffer_b, argv[3], strlen(argv[3]), '0', 32);
        uint128_t b = u128_from_str(buffer_b, 32);
        
        printf("and="); u128_printf(u128_bit_and(a, b)); printf("\n");
        printf("or="); u128_printf(u128_bit_or(a, b)); printf("\n");
    }
    else if (strcmp(mode, "bitwise_u64") == 0 && argc == 4) {
        uint64_t b = strtoull(argv[3], NULL, 16);
        
        printf("and="); u128_printf(u128_bit_and_u64(a, b)); printf("\n");
        printf("or="); u128_printf(u128_bit_or_u64(a, b)); printf("\n");
    }
    else if (strcmp(mode, "unary") == 0 && argc == 3) {
        printf("inc="); u128_printf(u128_inc(a)); printf("\n");
        printf("dec="); u128_printf(u128_dec(a)); printf("\n");
        printf("not="); u128_printf(u128_bit_not(a)); printf("\n");
    }
    else {
        print_usage(argv[0]);
        return 1;
    }
    
    return 0;
}

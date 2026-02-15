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

#define UINT256_T_IMPLEMENTATION
#include "uint256_t.h"

#define UINT512_T_IMPLEMENTATION
#include "uint512_t.h"

#define LUI_UTILS_IMPLEMENTATION
#include "utils.h"

////////////////////////////////////////////////////////////////////////////////////////////////

#define CREATE_TEST(type, type_prefix)                                            \
    int type_prefix##_##test(int n_args, char *mode, char *a_str, char *b_str) {  \
        char buffer_a[sizeof(type) * 2 + 1] = {0};                                \
        char buffer_b[sizeof(type) * 2 + 1] = {0};                                \
                                                                                  \
        lpad(buffer_a, a_str, strlen(a_str), '0', sizeof(type) * 2);              \
        type a = type_prefix##_##from_hex(buffer_a, sizeof(type) * 2);            \
                                                                                  \
        if (strcmp(mode, "arith") == 0 && n_args == 3) {                          \
            lpad(buffer_b, b_str, strlen(b_str), '0', sizeof(type) * 2);          \
            type b = type_prefix##_##from_hex(buffer_b, sizeof(type) * 2);        \
                                                                                  \
            printf("add=");                                                       \
            type_prefix##_##printf(type_prefix##_##add(a, b));                    \
            printf("\n");                                                         \
            printf("sub=");                                                       \
            type_prefix##_##printf(type_prefix##_##sub(a, b));                    \
            printf("\n");                                                         \
            printf("mul=");                                                       \
            type_prefix##_##printf(type_prefix##_##mul(a, b));                    \
            printf("\n");                                                         \
            if (type_prefix##_##gt_u64(b, 0)) {                                   \
                printf("div=");                                                   \
                type_prefix##_##printf(type_prefix##_##div(a, b));                \
                printf("\n");                                                     \
                printf("mod=");                                                   \
                type_prefix##_##printf(type_prefix##_##mod(a, b));                \
                printf("\n");                                                     \
            }                                                                     \
        } else if (strcmp(mode, "arith_u64") == 0 && n_args == 3) {               \
            uint64_t b = strtoull(b_str, NULL, 16);                               \
                                                                                  \
            printf("add=");                                                       \
            type_prefix##_##printf(type_prefix##_##add_u64(a, b));                \
            printf("\n");                                                         \
            printf("sub=");                                                       \
            type_prefix##_##printf(type_prefix##_##sub_u64(a, b));                \
            printf("\n");                                                         \
            printf("mul=");                                                       \
            type_prefix##_##printf(type_prefix##_##mul_u64(a, b));                \
            printf("\n");                                                         \
            if (b != 0)                                                           \
            {                                                                     \
                printf("div=");                                                   \
                type_prefix##_##printf(type_prefix##_##div_u64(a, b));            \
                printf("\n");                                                     \
                printf("mod=");                                                   \
                type_prefix##_##printf(type_prefix##_##mod_u64(a, b));            \
                printf("\n");                                                     \
            }                                                                     \
        } else if (strcmp(mode, "cmp") == 0 && n_args == 3) {                     \
            lpad(buffer_b, b_str, strlen(b_str), '0', sizeof(type) * 2);          \
            type b = type_prefix##_##from_hex(buffer_b, sizeof(type) * 2);        \
                                                                                  \
            printf("lt=%d\n", type_prefix##_##lt(a, b));                          \
            printf("le=%d\n", type_prefix##_##le(a, b));                          \
            printf("eq=%d\n", type_prefix##_##eq(a, b));                          \
            printf("ge=%d\n", type_prefix##_##ge(a, b));                          \
            printf("gt=%d\n", type_prefix##_##gt(a, b));                          \
        } else if (strcmp(mode, "cmp_u64") == 0 && n_args == 3) {                 \
            uint64_t b = strtoull(b_str, NULL, 16);                               \
                                                                                  \
            printf("lt=%d\n", type_prefix##_##lt_u64(a, b));                      \
            printf("le=%d\n", type_prefix##_##le_u64(a, b));                      \
            printf("eq=%d\n", type_prefix##_##eq_u64(a, b));                      \
            printf("ge=%d\n", type_prefix##_##ge_u64(a, b));                      \
            printf("gt=%d\n", type_prefix##_##gt_u64(a, b));                      \
        } else if (strcmp(mode, "shift") == 0 && n_args == 3) {                   \
            uint16_t amount = (uint16_t)atoi(b_str);                              \
                                                                                  \
            printf("lshift=");                                                    \
            type_prefix##_##printf(type_prefix##_##lshift(a, amount));            \
            printf("\n");                                                         \
            printf("rshift=");                                                    \
            type_prefix##_##printf(type_prefix##_##rshift(a, amount));            \
            printf("\n");                                                         \
        } else if (strcmp(mode, "bitwise") == 0 && n_args == 3) {                 \
            lpad(buffer_b, b_str, strlen(b_str), '0', sizeof(type) * 2);          \
            type b = type_prefix##_##from_hex(buffer_b, sizeof(type) * 2);        \
                                                                                  \
            printf("and=");                                                       \
            type_prefix##_##printf(type_prefix##_##bit_and(a, b));                \
            printf("\n");                                                         \
            printf("or=");                                                        \
            type_prefix##_##printf(type_prefix##_##bit_or(a, b));                 \
            printf("\n");                                                         \
        } else if (strcmp(mode, "bitwise_u64") == 0 && n_args == 3) {             \
            uint64_t b = strtoull(b_str, NULL, 16);                               \
                                                                                  \
            printf("and=");                                                       \
            type_prefix##_##printf(type_prefix##_##bit_and_u64(a, b));            \
            printf("\n");                                                         \
            printf("or=");                                                        \
            type_prefix##_##printf(type_prefix##_##bit_or_u64(a, b));             \
            printf("\n");                                                         \
        } else if (strcmp(mode, "unary") == 0 && n_args == 2) {                   \
            printf("inc=");                                                       \
            type_prefix##_##printf(type_prefix##_##inc(a));                       \
            printf("\n");                                                         \
            printf("dec=");                                                       \
            type_prefix##_##printf(type_prefix##_##dec(a));                       \
            printf("\n");                                                         \
            printf("not=");                                                       \
            type_prefix##_##printf(type_prefix##_##bit_not(a));                   \
            printf("\n");                                                         \
        } else {                                                                  \
            return 1;                                                             \
        }                                                                         \
                                                                                  \
        return 0;                                                                 \
    }

DECLARE_LUI_UTILS(128);
DECLARE_LUI_UTILS(256);
DECLARE_LUI_UTILS(512);

CREATE_TEST(uint128_t, u128);
CREATE_TEST(uint256_t, u256);
CREATE_TEST(uint512_t, u512);

void print_usage(const char* prog) {
    printf("Usage:\n");
    printf("  %s <bits> arith <hex_a> <hex_b>       - test +, -, *, /, %%\n", prog);
    printf("  %s <bits> arith_u64 <hex_a> <hex_b>   - test +, -, *, /, %% with u64\n", prog);
    printf("  %s <bits> cmp <hex_a> <hex_b>         - test <, <=, ==, >=, >\n", prog);
    printf("  %s <bits> cmp_u64 <hex_a> <hex_b>     - test comparisons with u64\n", prog);
    printf("  %s <bits> shift <hex_a> <amount>      - test <<, >>\n", prog);
    printf("  %s <bits> bitwise <hex_a> <hex_b>     - test &, |\n", prog);
    printf("  %s <bits> bitwise_u64 <hex_a> <hex_b> - test &, | with u64\n", prog);
    printf("  %s <bits> unary <hex_a>               - test inc, dec, not\n", prog);
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        printf("Missing params\n");
        print_usage(argv[0]);
        return 1;
    }
   
    if (strncmp("128", argv[1], 3) == 0) {
        if(u128_test(argc - 2, argv[2], argv[3], argv[4]) == 1) {
            print_usage(argv[0]);
            return 1;
        }
    } else if (strncmp("256", argv[1], 3) == 0) {
        if(u256_test(argc - 2, argv[2], argv[3], argv[4]) == 1) {
            print_usage(argv[0]);
            return 1;
        }
    } else if (strncmp("512", argv[1], 3) == 0) {
        if(u512_test(argc - 2, argv[2], argv[3], argv[4]) == 1) {
            print_usage(argv[0]);
            return 1;
        }
    } else {
        printf("Bits can be either 128, 256 or 512\n");
        print_usage(argv[0]);
        return 1;
    }
    return 0;
}

DEFINE_LUI_UTILS(128);
DEFINE_LUI_UTILS(256);
DEFINE_LUI_UTILS(512);

#ifndef UINTN_T_H_
#define UINTN_T_H_

#include <string.h>
#include <stdbool.h>
// As a rule I create macros in such a way that all the arguments for a particular
// macro call are all either types on values (that are to be passed to a function).
// If the macro only takes values for a type definition, we know the result is a 
// definition of sorts. If we pass in value, we know the result is a value.

#define UINTN_T(bits) uint##bits##_t
#define UINTN_MAX(bits) UINT##bits##_MAX
#define UINTN_0(bits) UINT##bits##_0
#define UINTN_1(bits) UINT##bits##_1

#define UN_PREFIX(bits, after) u##bits##_##after
#define UN_CTOR(bits, from_bits) u##bits##_u##from_bits
#define UN_MAKE(bits) make_u##bits
#define UN_LOW(bits) U##bits##_LOW
#define UN_HIGH(bits) U##bits##_HIGH
#define UN_LEFTMOST_U64(bits) U##bits##_LEFTMOST_U64

#define DEFINE_UINTN_T_STRUCT(bits, half_bits)  \
    typedef struct {                            \
        UINTN_T(half_bits) low;                 \
        UINTN_T(half_bits) high;                \
    } UINTN_T(bits)

#define DEFINE_DIVRES(bits)       \
    typedef struct {              \
        UINTN_T(bits) quotient;   \
        UINTN_T(bits) remainder;  \
    } UN_PREFIX(bits, divres);

#define DECLARE_ALL_FUNCS(bits, half_bits, quarter_bits)                                    \
    UINTN_T(bits) UN_PREFIX(bits, add)(UINTN_T(bits) lhs, UINTN_T(bits) rhs);               \
    UINTN_T(bits) UN_PREFIX(bits, sub)(UINTN_T(bits) lhs, UINTN_T(bits) rhs);               \
    UINTN_T(bits) UN_PREFIX(bits, add_u64)(UINTN_T(bits) lhs, UINTN_T(64) rhs);             \
    UINTN_T(bits) UN_PREFIX(bits, sub_u64)(UINTN_T(bits) lhs, UINTN_T(64) rhs);             \
    UINTN_T(bits) UN_PREFIX(bits, mul)(UINTN_T(bits) lhs, UINTN_T(bits) rhs);               \
    UINTN_T(bits) UN_PREFIX(bits, mul_u64)(UINTN_T(bits) lhs, UINTN_T(64) rhs);             \
    UN_PREFIX(bits, divres) UN_PREFIX(bits, divmod)(UINTN_T(bits) lhs, UINTN_T(bits) rhs);  \
    UINTN_T(bits) UN_PREFIX(bits, div)(UINTN_T(bits) lhs, UINTN_T(bits) rhs);               \
    UINTN_T(bits) UN_PREFIX(bits, div_u64)(UINTN_T(bits) lhs, UINTN_T(64) rhs);             \
    UINTN_T(bits) UN_PREFIX(bits, mod)(UINTN_T(bits) lhs, UINTN_T(bits) rhs);               \
    UINTN_T(bits) UN_PREFIX(bits, mod_u64)(UINTN_T(bits) lhs, UINTN_T(64) rhs);             \
    UINTN_T(bits) UN_PREFIX(bits, lshift)(UINTN_T(bits) n, uint16_t amount);                \
    UINTN_T(bits) UN_PREFIX(bits, rshift)(UINTN_T(bits) n, UINTN_T(16) amount);             \
    UINTN_T(bits) UN_PREFIX(bits, bit_not)(UINTN_T(bits) op);                               \
    UINTN_T(bits) UN_PREFIX(bits, bit_and)(UINTN_T(bits) lhs, UINTN_T(bits) rhs);           \
    UINTN_T(bits) UN_PREFIX(bits, bit_or)(UINTN_T(bits) lhs, UINTN_T(bits) rhs);            \
    UINTN_T(bits) UN_PREFIX(bits, bit_and_u64)(UINTN_T(bits) lhs, UINTN_T(64) rhs);         \
    UINTN_T(bits) UN_PREFIX(bits, bit_or_u64)(UINTN_T(bits) lhs, UINTN_T(64) rhs);          \
    UINTN_T(bits) UN_PREFIX(bits, inc)(UINTN_T(bits) a);                                    \
    UINTN_T(bits) UN_PREFIX(bits, dec)(UINTN_T(bits) a);                                    \
    bool UN_PREFIX(bits, gt)(UINTN_T(bits) lhs, UINTN_T(bits) rhs);                         \
    bool UN_PREFIX(bits, lt)(UINTN_T(bits) lhs, UINTN_T(bits) rhs);                         \
    bool UN_PREFIX(bits, ge)(UINTN_T(bits) lhs, UINTN_T(bits) rhs);                         \
    bool UN_PREFIX(bits, le)(UINTN_T(bits) lhs, UINTN_T(bits) rhs);                         \
    bool UN_PREFIX(bits, eq)(UINTN_T(bits) lhs, UINTN_T(bits) rhs);                         \
    bool UN_PREFIX(bits, gt_u64)(UINTN_T(bits) lhs, UINTN_T(64) rhs);                       \
    bool UN_PREFIX(bits, lt_u64)(UINTN_T(bits) lhs, UINTN_T(64) rhs);                       \
    bool UN_PREFIX(bits, eq_u64)(UINTN_T(bits) lhs, UINTN_T(64) rhs);                       \
    bool UN_PREFIX(bits, ge_u64)(UINTN_T(bits) lhs, UINTN_T(64) rhs);                       \
    bool UN_PREFIX(bits, le_u64)(UINTN_T(bits) lhs, UINTN_T(64) rhs);                       \
    bool UN_PREFIX(bits, bit_get)(UINTN_T(bits) lhs, UINTN_T(16) rhs);                      \
    void UN_PREFIX(bits, bit_set)(UINTN_T(bits) lhs, UINTN_T(16) rhs);                      \
    void UN_PREFIX(bits, bit_clear)(UINTN_T(bits) lhs, UINTN_T(16) rhs);

#define UN_UL(bits) U##bits##_UL
#define UN_UL_PTR(bits) U##bits##_UL_PTR

#endif // UINTN_T_H_

#define UINTN_T_IMPLEMENTATION
#if defined(UINTN_T_IMPLEMENTATION) && !defined(UINTN_T_IMPLEMENTATION_DEFINED)
#define UINTN_T_IMPLEMENTATION_DEFINED

#define DEFINE_ALL_FUNCS(bits, half_bits, quarter_bits)  \
    DEFINE_ADD_FUNC(bits, half_bits);                    \
    DEFINE_SUB_FUNC(bits, half_bits);                    \
    DEFINE_ADD_U64_FUNC(bits, half_bits);                \
    DEFINE_SUB_U64_FUNC(bits, half_bits);                \
    DEFINE_MUL_FUNC(bits, half_bits, quarter_bits);      \
    DEFINE_MUL_U64_FUNC(bits)                            \
    DEFINE_DIVMOD_FUNC(bits);                            \
    DEFINE_DIV_FUNC(bits);                               \
    DEFINE_DIV_U64_FUNC(bits);                           \
    DEFINE_MOD_FUNC(bits);                               \
    DEFINE_MOD_U64_FUNC(bits);                           \
    DEFINE_LSHIFT_FUNC(bits, half_bits);                 \
    DEFINE_RSHIFT_FUNC(bits, half_bits);                 \
    DEFINE_BIT_NOT_FUNC(bits, half_bits);                \
    DEFINE_BIT_AND_FUNC(bits, half_bits);                \
    DEFINE_BIT_OR_FUNC(bits, half_bits);                 \
    DEFINE_BIT_AND_U64_FUNC(bits, half_bits);            \
    DEFINE_BIT_OR_U64_FUNC(bits, half_bits);             \
    DEFINE_INC_FUNC(bits);                               \
    DEFINE_DEC_FUNC(bits);                               \
    DEFINE_GT_FUNC(bits, half_bits);                     \
    DEFINE_LT_FUNC(bits, half_bits);                     \
    DEFINE_GE_FUNC(bits);                                \
    DEFINE_LE_FUNC(bits);                                \
    DEFINE_EQ_FUNC(bits, half_bits);                     \
    DEFINE_GT_U64_FUNC(bits, half_bits);                 \
    DEFINE_LT_U64_FUNC(bits, half_bits);                 \
    DEFINE_EQ_U64_FUNC(bits, half_bits);                 \
    DEFINE_GE_U64_FUNC(bits, half_bits);                 \
    DEFINE_LE_U64_FUNC(bits, half_bits);                 \
    DEFINE_BIT_GET(bits);                                \
    DEFINE_BIT_SET(bits);                                \
    DEFINE_BIT_CLEAR(bits);

#define DEFINE_ADD_FUNC(bits, half_bits)                                                \
    UINTN_T(bits) UN_PREFIX(bits, add)(UINTN_T(bits) lhs, UINTN_T(bits) rhs) {          \
        UINTN_T(bits) res = (UINTN_T(bits)){                                            \
            .low = UN_PREFIX(half_bits, add)(UN_LOW(bits)(lhs), UN_LOW(bits)(rhs)),     \
            .high = UN_PREFIX(half_bits, add)(UN_HIGH(bits)(lhs), UN_HIGH(bits)(rhs)),  \
        };                                                                              \
        if(UN_PREFIX(half_bits, gt_u64)(UN_LOW(bits)(lhs), 0) &&                        \
           UN_PREFIX(half_bits, gt)(UN_LOW(bits)(rhs),                                  \
               UN_PREFIX(half_bits, sub)(UINTN_MAX(half_bits), UN_LOW(bits)(lhs)))) {   \
            UN_HIGH(bits)(res) = UN_PREFIX(half_bits, inc)(UN_HIGH(bits)(res));         \
        }                                                                               \
        return res;                                                                     \
    }

#define DEFINE_SUB_FUNC(bits, half_bits)                                                \
    UINTN_T(bits) UN_PREFIX(bits, sub)(UINTN_T(bits) lhs, UINTN_T(bits) rhs) {          \
        UINTN_T(bits) res = (UINTN_T(bits)){                                            \
            .low = UN_PREFIX(half_bits, sub)(UN_LOW(bits)(lhs), UN_LOW(bits)(rhs)),     \
            .high = UN_PREFIX(half_bits, sub)(UN_HIGH(bits)(lhs), UN_HIGH(bits)(rhs)),  \
        };                                                                              \
        if(UN_PREFIX(half_bits, lt)(UN_LOW(bits)(lhs), UN_LOW(bits)(rhs))) {            \
            UN_HIGH(bits)(res) = UN_PREFIX(half_bits, dec)(UN_HIGH(bits)(res));         \
        }                                                                               \
        return res;                                                                     \
    }

#define DEFINE_ADD_U64_FUNC(bits, half_bits)                                                                         \
    UINTN_T(bits) UN_PREFIX(bits, add_u64)(UINTN_T(bits) lhs, UINTN_T(64) rhs) {                                     \
        UINTN_T(bits) res = (UINTN_T(bits)){                                                                         \
            .low = UN_PREFIX(half_bits, add_u64)(UN_LOW(bits)(lhs), rhs),                                            \
            .high = UN_HIGH(bits)(lhs),                                                                              \
        };                                                                                                           \
        if(UN_PREFIX(half_bits, gt_u64)(UN_LOW(bits)(lhs), 0) &&                                                     \
           UN_PREFIX(half_bits, le_u64)(UN_PREFIX(half_bits, sub)(UINTN_MAX(half_bits), UN_LOW(bits)(lhs)), rhs)) {  \
            UN_HIGH(bits)(res) = UN_PREFIX(half_bits, inc)(UN_HIGH(bits)(res));                                      \
        }                                                                                                            \
        return res;                                                                                                  \
    }

#define DEFINE_SUB_U64_FUNC(bits, half_bits)                                            \
    UINTN_T(bits) UN_PREFIX(bits, sub_u64)(UINTN_T(bits) lhs, UINTN_T(64) rhs) {        \
        UINTN_T(bits) res = (UINTN_T(bits)){                                            \
            .low = UN_PREFIX(half_bits, sub_u64)(UN_LOW(bits)(lhs), rhs),               \
            .high = UN_HIGH(bits)(lhs),                                                 \
        };                                                                              \
        if(UN_PREFIX(half_bits, lt_u64)(UN_LOW(bits)(lhs), rhs)) {                      \
            UN_HIGH(bits)(res) = UN_PREFIX(half_bits, dec)(UN_HIGH(bits)(res));         \
        }                                                                               \
        return res;                                                                     \
    }

#define DEFINE_MUL_FUNC(bits, half_bits, quarter_bits)                                                                                                              \
    UINTN_T(bits) UN_PREFIX(bits, mul)(UINTN_T(bits) lhs, UINTN_T(bits) rhs) {                                                                                      \
        UINTN_T(quarter_bits) lhs_hh = UN_HIGH(half_bits)(UN_HIGH(bits)(lhs));                                                                                      \
        UINTN_T(quarter_bits) lhs_hl = UN_LOW(half_bits)(UN_HIGH(bits)(lhs));                                                                                       \
        UINTN_T(quarter_bits) lhs_lh = UN_HIGH(half_bits)(UN_LOW(bits)(lhs));                                                                                       \
        UINTN_T(quarter_bits) lhs_ll = UN_LOW(half_bits)(UN_LOW(bits)(lhs));                                                                                        \
                                                                                                                                                                    \
        UINTN_T(half_bits) rhs_hh = UN_CTOR(half_bits, quarter_bits)(UN_HIGH(half_bits)(UN_HIGH(bits)(rhs)));                                                       \
        UINTN_T(half_bits) rhs_hl = UN_CTOR(half_bits, quarter_bits)(UN_LOW(half_bits)(UN_HIGH(bits)(rhs)));                                                        \
        UINTN_T(half_bits) rhs_lh = UN_CTOR(half_bits, quarter_bits)(UN_HIGH(half_bits)(UN_LOW(bits)(rhs)));                                                        \
        UINTN_T(half_bits) rhs_ll = UN_CTOR(half_bits, quarter_bits)(UN_LOW(half_bits)(UN_LOW(bits)(rhs)));                                                         \
                                                                                                                                                                    \
        UINTN_T(half_bits) first = UN_PREFIX(half_bits, mul)(rhs_ll, UN_CTOR(half_bits, quarter_bits)(lhs_ll));                                                     \
                                                                                                                                                                    \
        UINTN_T(half_bits) s_0 = UN_PREFIX(half_bits, mul)(rhs_ll, UN_CTOR(half_bits, quarter_bits)(lhs_lh));                                                       \
        UINTN_T(half_bits) s_1 = UN_PREFIX(half_bits, mul)(rhs_lh, UN_CTOR(half_bits, quarter_bits)(lhs_ll));                                                       \
        UINTN_T(half_bits) s_carry = UN_CTOR(half_bits, quarter_bits)(UN_PREFIX(half_bits, gt_u64)(s_0, 0) &&                                                       \
                                                                               UN_PREFIX(half_bits, gt)(s_1, UN_PREFIX(half_bits, sub)(UINTN_MAX(half_bits), s_0))  \
                                                                           ? UINTN_1(quarter_bits)                                                                  \
                                                                           : UINTN_0(quarter_bits));                                                                \
        UINTN_T(half_bits)                                                                                                                                          \
        second = UN_PREFIX(half_bits, add)(s_0, s_1);                                                                                                               \
                                                                                                                                                                    \
        UINTN_T(half_bits) t_0 = UN_PREFIX(half_bits, mul)(rhs_ll, UN_CTOR(half_bits, quarter_bits)(lhs_hl));                                                       \
        UINTN_T(half_bits) t_1 = UN_PREFIX(half_bits, mul)(rhs_lh, UN_CTOR(half_bits, quarter_bits)(lhs_lh));                                                       \
        UINTN_T(half_bits) t_2 = UN_PREFIX(half_bits, mul)(rhs_hl, UN_CTOR(half_bits, quarter_bits)(lhs_ll));                                                       \
        UINTN_T(half_bits) t_carry = UINTN_0(half_bits);                                                                                                            \
        UINTN_T(half_bits) third = t_0;                                                                                                                             \
        t_carry = UN_PREFIX(half_bits, add_u64)(t_carry, (UN_PREFIX(half_bits, gt_u64)(third, 0) &&                                                                 \
                                                          UN_PREFIX(half_bits, gt)(t_1, UN_PREFIX(half_bits, sub)(UINTN_MAX(half_bits), third)))                    \
                                                             ? 1                                                                                                    \
                                                             : 0);                                                                                                  \
        third = UN_PREFIX(half_bits, add)(third, t_1);                                                                                                              \
        t_carry = UN_PREFIX(half_bits, add_u64)(t_carry, (UN_PREFIX(half_bits, gt_u64)(third, 0) &&                                                                 \
                                                          UN_PREFIX(half_bits, gt)(t_2, UN_PREFIX(half_bits, sub)(UINTN_MAX(half_bits), third)))                    \
                                                             ? 1                                                                                                    \
                                                             : 0);                                                                                                  \
        third = UN_PREFIX(half_bits, add)(third, t_2);                                                                                                              \
                                                                                                                                                                    \
        UINTN_T(half_bits) fourth = UN_PREFIX(half_bits, add)(                                                                                                      \
                                        UN_PREFIX(half_bits, add)(                                                                                                  \
                                            UN_PREFIX(half_bits, mul)(rhs_ll, UN_CTOR(half_bits, quarter_bits)(lhs_hh)),                                            \
                                            UN_PREFIX(half_bits, mul)(rhs_lh, UN_CTOR(half_bits, quarter_bits)(lhs_hl))),                                           \
                                        UN_PREFIX(half_bits, add)(                                                                                                  \
                                            UN_PREFIX(half_bits, mul)(rhs_hl, UN_CTOR(half_bits, quarter_bits)(lhs_lh)),                                            \
                                            UN_PREFIX(half_bits, mul)(rhs_hh, UN_CTOR(half_bits, quarter_bits)(lhs_ll))));                                          \
                                                                                                                                                                    \
        UINTN_T(half_bits) temp = UN_CTOR(half_bits, quarter_bits)(UN_HIGH(half_bits)(first));                                                                      \
        t_carry = UN_PREFIX(half_bits, add_u64)(t_carry, (UN_PREFIX(half_bits, gt_u64)(second, 0) &&                                                                \
                                                          UN_PREFIX(half_bits, gt)(temp, UN_PREFIX(half_bits, sub)(UINTN_MAX(half_bits), second)))                  \
                                                             ? 1                                                                                                    \
                                                             : 0);                                                                                                  \
        second = UN_PREFIX(half_bits, add)(second, temp);                                                                                                           \
                                                                                                                                                                    \
        t_carry = UN_PREFIX(half_bits, add_u64)(t_carry, (UN_PREFIX(half_bits, gt_u64)(third, 0) &&                                                                 \
                                                          UN_PREFIX(half_bits, gt)(s_carry, UN_PREFIX(half_bits, sub)(UINTN_MAX(half_bits), third)))                \
                                                             ? 1                                                                                                    \
                                                             : 0);                                                                                                  \
        third = UN_PREFIX(half_bits, add)(third, UN_MAKE(half_bits)(UINTN_0(quarter_bits), UN_LOW(half_bits)(s_carry)));                                            \
        temp = UN_MAKE(half_bits)(UN_HIGH(half_bits)(second), UINTN_0(quarter_bits));                                                                               \
        t_carry = UN_PREFIX(half_bits, add_u64)(t_carry, (UN_PREFIX(half_bits, gt_u64)(third, 0) &&                                                                 \
                                                          UN_PREFIX(half_bits, gt)(temp, UN_PREFIX(half_bits, sub)(UINTN_MAX(half_bits), third)))                   \
                                                             ? 1                                                                                                    \
                                                             : 0);                                                                                                  \
        third = UN_PREFIX(half_bits, add)(third, temp);                                                                                                             \
                                                                                                                                                                    \
        fourth = UN_PREFIX(half_bits, add)(fourth, UN_MAKE(half_bits)(UINTN_0(quarter_bits), UN_LOW(half_bits)(t_carry)));                                          \
        fourth = UN_PREFIX(half_bits, add)(fourth, UN_MAKE(half_bits)(UN_HIGH(half_bits)(third), UINTN_0(quarter_bits)));                                           \
                                                                                                                                                                    \
        return UN_MAKE(bits)(                                                                                                                                       \
            UN_MAKE(half_bits)(                                                                                                                                     \
                UN_LOW(half_bits)(first),                                                                                                                           \
                UN_LOW(half_bits)(second)),                                                                                                                         \
            UN_MAKE(half_bits)(                                                                                                                                     \
                UN_LOW(half_bits)(third),                                                                                                                           \
                UN_LOW(half_bits)(fourth)));                                                                                                                        \
    }

#define DEFINE_DIVMOD_FUNC(bits)                                                                                   \
    UN_PREFIX(bits, divres) UN_PREFIX(bits, divmod)(UINTN_T(bits) lhs, UINTN_T(bits) rhs) {                        \
        UINTN_T(bits) quotient = {0};                                                                              \
        UINTN_T(bits) remainder = {0};                                                                             \
                                                                                                                   \
        memset(&quotient, 0, sizeof(UINTN_T(bits)));                                                               \
        memset(&remainder, 0, sizeof(UINTN_T(bits)));                                                              \
                                                                                                                   \
        for(int i = bits; i > 0; --i) {                                                                            \
            quotient = UN_PREFIX(bits, lshift)(quotient, 1);                                                       \
            remainder = UN_PREFIX(bits, lshift)(remainder, 1);                                                     \
                                                                                                                   \
            if(UN_PREFIX(bits, bit_get)(lhs, i - 1)) {                                                             \
                remainder = UN_PREFIX(bits, inc)(remainder);                                                       \
            }                                                                                                      \
                                                                                                                   \
            if(UN_PREFIX(bits, ge)(remainder, rhs)) {                                                              \
                remainder = UN_PREFIX(bits, sub)(remainder, rhs);                                                  \
                quotient = UN_PREFIX(bits, inc)(quotient);                                                         \
            }                                                                                                      \
        }                                                                                                          \
                                                                                                                   \
        return (UN_PREFIX(bits, divres)){                                                                          \
            .quotient = quotient,                                                                                  \
            .remainder = remainder,                                                                                \
        };                                                                                                         \
    }

#define DEFINE_MUL_U64_FUNC(bits)                                                 \
    UINTN_T(bits) UN_PREFIX(bits, mul_u64)(UINTN_T(bits) lhs, UINTN_T(64) rhs) {  \
        UINTN_T(bits) r = UINTN_0(bits);                                          \
        UN_LEFTMOST_U64(bits)(r) = rhs;                                           \
        return UN_PREFIX(bits, mul)(lhs,  r);                                     \
    }

#define DEFINE_DIV_FUNC(bits)                                                   \
    UINTN_T(bits) UN_PREFIX(bits, div)(UINTN_T(bits) lhs, UINTN_T(bits) rhs) {  \
        return UN_PREFIX(bits, divmod)(lhs, rhs).quotient;                      \
    }

#define DEFINE_DIV_U64_FUNC(bits)                                                 \
    UINTN_T(bits) UN_PREFIX(bits, div_u64)(UINTN_T(bits) lhs, UINTN_T(64) rhs) {  \
        UINTN_T(bits) r = UINTN_0(bits);                                          \
        UN_LEFTMOST_U64(bits)(r) = rhs;                                           \
        return UN_PREFIX(bits, div)(lhs,  r);                                     \
    }

#define DEFINE_MOD_FUNC(bits)                                                   \
    UINTN_T(bits) UN_PREFIX(bits, mod)(UINTN_T(bits) lhs, UINTN_T(bits) rhs) {  \
        return UN_PREFIX(bits, divmod)(lhs, rhs).remainder;                     \
    }

#define DEFINE_MOD_U64_FUNC(bits)                                                 \
    UINTN_T(bits) UN_PREFIX(bits, mod_u64)(UINTN_T(bits) lhs, UINTN_T(64) rhs) {  \
        UINTN_T(bits) r = UINTN_0(bits);                                          \
        UN_LEFTMOST_U64(bits)(r) = rhs;                                           \
        return UN_PREFIX(bits, mod)(lhs,  r);                                     \
    }

#define DEFINE_LSHIFT_FUNC(bits, half_bits)                                                                                                                                               \
    UINTN_T(bits) UN_PREFIX(bits, lshift)(UINTN_T(bits) n, uint16_t amount) {                                                                                                             \
        UINTN_T(bits) res = (UINTN_T(bits)){                                                                                                                                              \
            .low = UN_LOW(bits)(n),                                                                                                                                                       \
            .high = UN_HIGH(bits)(n),                                                                                                                                                     \
        };                                                                                                                                                                                \
                                                                                                                                                                                          \
        for(int i = 0; i < amount; ++i) {                                                                                                                                                 \
            UN_HIGH(bits)(res) = UN_PREFIX(half_bits, lshift)(UN_HIGH(bits)(res), 1);                                                                                                     \
            UN_HIGH(bits)(res) = UN_PREFIX(half_bits, bit_or)(UN_HIGH(bits)(res), UN_PREFIX(half_bits, bit_and_u64)(UN_PREFIX(half_bits, rshift)(UN_LOW(bits)(res), half_bits - 1), 1));  \
            UN_LOW(bits)(res) = UN_PREFIX(half_bits, lshift)(UN_LOW(bits)(res), 1);                                                                                                       \
        }                                                                                                                                                                                 \
                                                                                                                                                                                          \
        return res;                                                                                                                                                                       \
    }

#define DEFINE_RSHIFT_FUNC(bits, half_bits)                                                                                                                                              \
    UINTN_T(bits) UN_PREFIX(bits, rshift)(UINTN_T(bits) n, UINTN_T(16) amount) {                                                                                                         \
        UINTN_T(bits) res = (UINTN_T(bits)){                                                                                                                                             \
            .low = UN_LOW(bits)(n),                                                                                                                                                      \
            .high = UN_HIGH(bits)(n),                                                                                                                                                    \
        };                                                                                                                                                                               \
                                                                                                                                                                                         \
        for(int i = 0; i < amount; ++i) {                                                                                                                                                \
            UN_LOW(bits)(res) = UN_PREFIX(half_bits, rshift)(UN_LOW(bits)(res), 1);                                                                                                      \
            UN_LOW(bits)(res) = UN_PREFIX(half_bits, bit_or)(UN_LOW(bits)(res), UN_PREFIX(half_bits, lshift)(UN_PREFIX(half_bits, bit_and_u64)(UN_HIGH(bits)(res), 1), half_bits - 1));  \
            UN_HIGH(bits)(res) = UN_PREFIX(half_bits, rshift)(UN_HIGH(bits)(res), 1);                                                                                                    \
        }                                                                                                                                                                                \
                                                                                                                                                                                         \
        return res;                                                                                                                                                                      \
    }

#define DEFINE_BIT_NOT_FUNC(bits, half_bits)                                                                                      \
    UINTN_T(bits) UN_PREFIX(bits, bit_not)(UINTN_T(bits) op) {                                                                    \
        return UN_MAKE(bits)(UN_PREFIX(half_bits, bit_not)(UN_LOW(bits)(op)), UN_PREFIX(half_bits, bit_not)(UN_HIGH(bits)(op)));  \
    }

#define DEFINE_BIT_AND_FUNC(bits, half_bits)                                                \
    UINTN_T(bits) UN_PREFIX(bits, bit_and)(UINTN_T(bits) lhs, UINTN_T(bits) rhs) {          \
        return (UINTN_T(bits)){                                                             \
            .low = UN_PREFIX(half_bits, bit_and)(UN_LOW(bits)(lhs), UN_LOW(bits)(rhs)),     \
            .high = UN_PREFIX(half_bits, bit_and)(UN_HIGH(bits)(lhs), UN_HIGH(bits)(rhs)),  \
        };                                                                                  \
    }

#define DEFINE_BIT_OR_FUNC(bits, half_bits)                                                \
    UINTN_T(bits) UN_PREFIX(bits, bit_or)(UINTN_T(bits) lhs, UINTN_T(bits) rhs) {          \
        return (UINTN_T(bits)){                                                            \
            .low = UN_PREFIX(half_bits, bit_or)(UN_LOW(bits)(lhs), UN_LOW(bits)(rhs)),     \
            .high = UN_PREFIX(half_bits, bit_or)(UN_HIGH(bits)(lhs), UN_HIGH(bits)(rhs)),  \
        };                                                                                 \
    }

#define DEFINE_BIT_AND_U64_FUNC(bits, half_bits)                                        \
    UINTN_T(bits) UN_PREFIX(bits, bit_and_u64)(UINTN_T(bits) lhs, UINTN_T(64) rhs) {  \
        return (UINTN_T(bits)){                                                         \
            .low = UN_PREFIX(half_bits, bit_and_u64)(UN_LOW(bits)(lhs), rhs),           \
            .high = UINTN_0(half_bits),                                                 \
        };                                                                              \
    }

#define DEFINE_BIT_OR_U64_FUNC(bits, half_bits)                                        \
    UINTN_T(bits) UN_PREFIX(bits, bit_or_u64)(UINTN_T(bits) lhs, UINTN_T(64) rhs) {  \
        return (UINTN_T(bits)){                                                        \
            .low = UN_PREFIX(half_bits, bit_or_u64)(UN_LOW(bits)(lhs), rhs),           \
            .high = UN_HIGH(bits)(lhs),                                                \
        };                                                                             \
    }

#define DEFINE_INC_FUNC(bits)                              \
    UINTN_T(bits) UN_PREFIX(bits, inc)(UINTN_T(bits) a) {  \
        return UN_PREFIX(bits, add_u64)(a, 1);             \
    }

#define DEFINE_DEC_FUNC(bits)                              \
    UINTN_T(bits) UN_PREFIX(bits, dec)(UINTN_T(bits) a) {  \
        return UN_PREFIX(bits, sub_u64)(a, 1);             \
    }

#define DEFINE_GT_FUNC(bits, half_bits)                                           \
    bool UN_PREFIX(bits, gt)(UINTN_T(bits) lhs, UINTN_T(bits) rhs) {              \
        return UN_PREFIX(half_bits, gt)(UN_HIGH(bits)(lhs), UN_HIGH(bits)(rhs)) || (  \
                UN_PREFIX(half_bits, eq)(UN_HIGH(bits)(lhs), UN_HIGH(bits)(rhs)) &&   \
                UN_PREFIX(half_bits, gt)(UN_LOW(bits)(lhs), UN_LOW(bits)(rhs)));      \
    }

#define DEFINE_LT_FUNC(bits, half_bits)                                           \
    bool UN_PREFIX(bits, lt)(UINTN_T(bits) lhs, UINTN_T(bits) rhs) {              \
        return UN_PREFIX(half_bits, lt)(UN_HIGH(bits)(lhs), UN_HIGH(bits)(rhs)) || (  \
                UN_PREFIX(half_bits, eq)(UN_HIGH(bits)(lhs), UN_HIGH(bits)(rhs)) &&   \
                UN_PREFIX(half_bits, lt)(UN_LOW(bits)(lhs), UN_LOW(bits)(rhs)));      \
    }

#define DEFINE_GE_FUNC(bits)                                                      \
    bool UN_PREFIX(bits, ge)(UINTN_T(bits) lhs, UINTN_T(bits) rhs) {              \
        return UN_PREFIX(bits, gt)(lhs, rhs) || UN_PREFIX(bits, eq)(lhs, rhs);    \
    }

#define DEFINE_LE_FUNC(bits)                                                      \
    bool UN_PREFIX(bits, le)(UINTN_T(bits) lhs, UINTN_T(bits) rhs) {              \
        return UN_PREFIX(bits, lt)(lhs, rhs) || UN_PREFIX(bits, eq)(lhs, rhs);    \
    }

#define DEFINE_EQ_FUNC(bits, half_bits)                                             \
    bool UN_PREFIX(bits, eq)(UINTN_T(bits) lhs, UINTN_T(bits) rhs) {                \
        return UN_PREFIX(half_bits, eq)(UN_HIGH(bits)(lhs), UN_HIGH(bits)(rhs)) &&  \
               UN_PREFIX(half_bits, eq)(UN_LOW(bits)(lhs), UN_LOW(bits)(rhs));      \
    }

#define DEFINE_GT_U64_FUNC(bits, half_bits)                                   \
    bool UN_PREFIX(bits, gt_u64)(UINTN_T(bits) lhs, UINTN_T(64) rhs) {        \
        return UN_PREFIX(half_bits, gt_u64)(UN_HIGH(bits)(lhs), UINT64_0) ||  \
                UN_PREFIX(half_bits, gt_u64)(UN_LOW(bits)(lhs), rhs);         \
    }

#define DEFINE_LT_U64_FUNC(bits, half_bits)                                     \
    bool UN_PREFIX(bits, lt_u64)(UINTN_T(bits) lhs, UINTN_T(64) rhs) {          \
        return !(UN_PREFIX(half_bits, gt_u64)(UN_HIGH(bits)(lhs), UINT64_0) ||  \
                 !UN_PREFIX(half_bits, lt_u64)(UN_LOW(bits)(lhs), rhs));        \
    }

#define DEFINE_EQ_U64_FUNC(bits, half_bits)                                   \
    bool UN_PREFIX(bits, eq_u64)(UINTN_T(bits) lhs, UINTN_T(64) rhs) {        \
        return UN_PREFIX(half_bits, eq_u64)(UN_HIGH(bits)(lhs), UINT64_0) &&  \
               UN_PREFIX(half_bits, eq_u64)(UN_LOW(bits)(lhs), rhs);          \
    }

#define DEFINE_GE_U64_FUNC(bits, half_bits)                                   \
    bool UN_PREFIX(bits, ge_u64)(UINTN_T(bits) lhs, UINTN_T(64) rhs) {        \
        return UN_PREFIX(half_bits, gt_u64)(UN_HIGH(bits)(lhs), UINT64_0) ||  \
                UN_PREFIX(half_bits, ge_u64)(UN_LOW(bits)(lhs), rhs);         \
    }

#define DEFINE_LE_U64_FUNC(bits, half_bits)                                     \
    bool UN_PREFIX(bits, le_u64)(UINTN_T(bits) lhs, UINTN_T(64) rhs) {          \
        return !(UN_PREFIX(half_bits, gt_u64)(UN_HIGH(bits)(lhs), UINT64_0) ||  \
                 !UN_PREFIX(half_bits, le_u64)(UN_LOW(bits)(lhs), rhs));        \
    }

#define DEFINE_BIT_GET(bits)                                                 \
    bool UN_PREFIX(bits, bit_get)(UINTN_T(bits) lhs, UINTN_T(16) rhs) {      \
        uint16_t idx = rhs / 64;                                             \
        uint16_t ul_idx = rhs % 64;                                          \
        return (UN_UL(bits)(lhs, idx) & (1UL << ul_idx));                    \
    }

#define DEFINE_BIT_SET(bits)                                             \
    void UN_PREFIX(bits, bit_set)(UINTN_T(bits) lhs, UINTN_T(16) rhs) {  \
        uint16_t idx = rhs / 64;                                         \
        uint16_t ul_idx = rhs % 64;                                      \
        *(UN_UL_PTR(bits)(lhs, idx)) |= (1UL << ul_idx);                 \
    }

#define DEFINE_BIT_CLEAR(bits)                                                    \
    void UN_PREFIX(bits, bit_clear)(UINTN_T(bits) lhs, UINTN_T(16) rhs) {         \
        uint16_t idx = rhs / 64;                                                  \
        uint16_t ul_idx = rhs % 64;                                               \
        *(UN_UL_PTR(bits)(lhs, idx)) = UN_UL(bits)(lhs, idx) & ~(1UL << ul_idx);  \
    }

#endif // UINTN_T_IMPLEMENTATION
#ifndef UINT128_T_H_
#define UINT128_T_H_

#include <limits.h>
#include <stdbool.h>
#include <stdint.h>

#define LOW_64  0x00000000fffffffful
#define HIGH_64 0xffffffff00000000ul

typedef struct {
    // Order is like this because of little endian
    uint64_t low;
    uint64_t high;
} uint128_t;

#define UINT128_MAX ((uint128_t){.low=ULLONG_MAX, .high=ULLONG_MAX})
#define UINT128_0 ((uint128_t){.low=0, .high=0})
#define UINT128_1 ((uint128_t){.low=1, .high=0})

#define U64_UL(x, n) (x)
#define U128_UL(x, n) ((n) < (sizeof(uint128_t) / sizeof(uint64_t) / 2) ? U64_UL((x).low, (n)) : U64_UL((x).high, (n) - (sizeof(uint128_t) / sizeof(uint64_t) / 2)))

typedef struct {
    uint128_t quotient;
    uint128_t remainder;
} u128_divres;

uint128_t u128_add(uint128_t a, uint128_t b);
uint128_t u128_sub(uint128_t a, uint128_t b);
uint128_t u128_mul(uint128_t a, uint128_t b);
u128_divres u128_divmod(uint128_t a, uint128_t b);
uint128_t u128_div(uint128_t a, uint128_t b);
uint128_t u128_mod(uint128_t a, uint128_t b);

uint128_t u128_add_u64(uint128_t a, uint64_t b);
uint128_t u128_sub_u64(uint128_t a, uint64_t b);
uint128_t u128_mul_u64(uint128_t a, uint64_t b);
u128_divres u128_divmod_u64(uint128_t a, uint64_t b);
uint128_t u128_div_u64(uint128_t a, uint64_t b);
uint128_t u128_mod_u64(uint128_t a, uint64_t b);

// 
uint128_t u128_bit_not(uint128_t n);
uint128_t u128_bit_or(uint128_t a, uint128_t b);
uint128_t u128_bit_and(uint128_t a, uint128_t b);

uint128_t u128_bit_or_u64(uint128_t a, uint64_t b);
uint128_t u128_bit_and_u64(uint128_t a, uint64_t b);

//
uint128_t u128_rshift(uint128_t n, uint16_t amount);
uint128_t u128_lshift(uint128_t n, uint16_t amount);

//
bool u128_le(uint128_t a, uint128_t b);
bool u128_ge(uint128_t a, uint128_t b);
bool u128_eq(uint128_t a, uint128_t b);
bool u128_lt(uint128_t a, uint128_t b);
bool u128_gt(uint128_t a, uint128_t b);

bool u128_le_u64(uint128_t a, uint64_t b);
bool u128_ge_u64(uint128_t a, uint64_t b);
bool u128_eq_u64(uint128_t a, uint64_t b);
bool u128_lt_u64(uint128_t a, uint64_t b);
bool u128_gt_u64(uint128_t a, uint64_t b);

//
void u128_dec_in(uint128_t* a);
uint128_t u128_dec(uint128_t a);

void u128_inc_in(uint128_t* a);
uint128_t u128_inc(uint128_t a);

#endif // UINT128_T_H_

#if defined(UINT128_T_IMPLEMENTATION) && !defined(UINT128_T_IMPLEMENTATION_DEFINED)
#define UINT128_T_IMPLEMENTATION_DEFINED

uint128_t u128_add(uint128_t a, uint128_t b) {
    uint128_t res = (uint128_t){
        .low = a.low + b.low,
        .high = a.high + b.high,
    };
    if (a.low > 0 && b.low > ULLONG_MAX - a.low) {
        ++res.high;
    }
    return res;
}


uint128_t u128_sub(uint128_t a, uint128_t b) {
    // Long way of doing it using 2s complement
    // return u128_add(a, u128_add(u128_not(b), (uint128_t){0, 1}));

    uint128_t res = (uint128_t){
        .low = a.low - b.low,
        .high = a.high - b.high,
    };
    if (a.low < b.low) {
        --res.high;
    }
    return res;
}

uint128_t u128_add_u64(uint128_t a, uint64_t b) {
    uint128_t res = (uint128_t){
        .low = a.low + b,
        .high = a.high,
    };
    if (a.low > 0 && b > ULLONG_MAX - a.low) {
        ++res.high;
    }
    return res;
}

uint128_t u128_sub_u64(uint128_t a, uint64_t b) {
    uint128_t res = (uint128_t){
        .low = a.low - b,
        .high = a.high,
    };
    if (a.low < b) {
        --res.high;
    }
    return res;
}

uint128_t u128_mul_u64(uint128_t a, uint64_t b) {
    return u128_mul(a, (uint128_t){.low=b, .high=0});
}

u128_divres u128_divmod_u64(uint128_t a, uint64_t b) {
    return u128_divmod(a, (uint128_t){.low=b, .high=0});
}

uint128_t u128_div_u64(uint128_t a, uint64_t b) {
    return u128_divmod_u64(a, b).quotient;
}

uint128_t u128_mod_u64(uint128_t a, uint64_t b) {
    return u128_divmod_u64(a, b).remainder;
}

uint128_t u128_inc(uint128_t a) {
    return u128_add_u64(a, 1);
}

uint128_t u128_dec(uint128_t a) {
    return u128_sub_u64(a, 1);
}

void u128_inc_in(uint128_t* a) {
    *a = u128_add_u64(*a, 1);
}

void u128_dec_in(uint128_t* a) {
    *a = u128_sub_u64(*a, 1);
}

uint128_t u128_mul(uint128_t a, uint128_t b) {
    /**
     *     abcd
     *     pqrs
     *     ....
     *   .|...
     *  ..|..
     * ...|.
     *     wxyz
     */
    uint32_t a_hh = (a.high & (HIGH_64)) >> 32;
    uint32_t a_hl = a.high & (LOW_64);
    uint32_t a_lh = (a.low & (HIGH_64)) >> 32;
    uint32_t a_ll = a.low & (LOW_64);

    uint64_t b_hh = (b.high & (HIGH_64)) >> 32;
    uint64_t b_hl = b.high & (LOW_64);
    uint64_t b_lh = (b.low & (HIGH_64)) >> 32;
    uint64_t b_ll = b.low & (LOW_64);

    uint64_t first = b_ll * a_ll;

    // Separating out to capture the carry

    // Second
    uint64_t s_0 = b_ll * a_lh;
    uint64_t s_1 = b_lh * a_ll;
    uint64_t s_carry = (s_0 > 0 && s_1 > ULLONG_MAX - s_0) ? 1 : 0;
    uint64_t second = s_0 + s_1;

    // Third
    uint64_t t_0 = b_ll * a_hl;
    uint64_t t_1 = b_lh * a_lh;
    uint64_t t_2 = b_hl * a_ll;
    uint64_t t_carry = 0;
    uint64_t third = t_0;
    t_carry += (third > 0 && t_1 > ULLONG_MAX - third) ? 1 : 0;
    third += t_1;
    t_carry += (third > 0 && t_2 > ULLONG_MAX - third) ? 1 : 0;
    third += t_2;

    // Fourth
    uint64_t fourth = b_ll * a_hh + b_lh * a_hl + b_hl * a_lh + b_hh * a_ll;

    // Carry forward

    // Second
    uint64_t temp = (first >> 32);
    s_carry += (second > 0 && temp > ULLONG_MAX - second) ? 1 : 0;
    second += temp;

    // Third
    t_carry += (third > 0 && s_carry > ULLONG_MAX - third) ? 1 : 0;
    third += (s_carry << 32);
    temp = (second >> 32);
    t_carry += (third > 0 && temp > ULLONG_MAX - third) ? 1 : 0;
    third  += temp;

    // Fourth
    fourth += (t_carry << 32);
    fourth += (third  >> 32);

    first  &= LOW_64;
    second &= LOW_64;
    third  &= LOW_64;
    fourth &= LOW_64;

    return (uint128_t){
        .low  = second << 32 | first,
        .high = fourth << 32 | third,
    };
}

bool u128_gt(uint128_t a, uint128_t b) {
    return a.high > b.high || (a.high == b.high && a.low > b.low);
}

bool u128_lt(uint128_t a, uint128_t b) {
    return a.high < b.high || (a.high == b.high && a.low < b.low);
}

bool u128_eq(uint128_t a, uint128_t b) {
    return a.high == b.high && a.low == b.low;
}

bool u128_ge(uint128_t a, uint128_t b) {
    return u128_gt(a, b) || u128_eq(a, b);
}

bool u128_le(uint128_t a, uint128_t b) {
    return u128_lt(a, b) || u128_eq(a, b);
}

bool u128_le_u64(uint128_t a, uint64_t b) {
    return a.high ? false : a.low <= b;
}
bool u128_ge_u64(uint128_t a, uint64_t b) {
    return a.high ? true : a.low >= b;
}
bool u128_eq_u64(uint128_t a, uint64_t b) {
    return a.high ? false : a.low == b;
}
bool u128_lt_u64(uint128_t a, uint64_t b) {
    return a.high ? false : a.low < b;
}
bool u128_gt_u64(uint128_t a, uint64_t b) {
    return a.high ? true : a.low > b;
}

uint128_t u128_lshift(uint128_t n, uint16_t amount) {
    uint128_t res = (uint128_t){
        .high = n.high,
        .low = n.low,
    };

    for(int i = 0; i < amount; ++i) {
        res.high = res.high << 1;
        res.high = res.high | ((res.low >> 63) & 1);
        res.low = res.low << 1;
    }

    return res;
}

uint128_t u128_rshift(uint128_t n, uint16_t amount) {
    uint128_t res = (uint128_t){
        .low = n.low,
        .high = n.high,
    };

    for(int i = 0; i < amount; ++i) {
        res.low = res.low >> 1;
        res.low = res.low | ((res.high << 63) & ((uint64_t)1 << 63));
        res.high = res.high >> 1;
    }

    return res;
}

uint128_t u128_bit_and(uint128_t a, uint128_t b) {
    return (uint128_t){
        .low = a.low & b.low,
        .high = a.high & b.high,
    };
}

uint128_t u128_bit_or(uint128_t a, uint128_t b) {
    return (uint128_t){
        .low = a.low | b.low,
        .high = a.high | b.high,
    };
}

uint128_t u128_bit_or_u64(uint128_t a, uint64_t b) {
    return (uint128_t) {
        .low = a.low | b,
        .high = a.high | 0ul,
    };
}
uint128_t u128_bit_and_u64(uint128_t a, uint64_t b) {
    return (uint128_t) {
        .low = a.low & b,
        .high = a.high & 0ul,
    };
}

uint128_t u128_bit_not(uint128_t n) {
    return (uint128_t){
        .low = ~n.low,
        .high = ~n.high,
    };
}

u128_divres u128_divmod(uint128_t a, uint128_t b) {
    /**
     * lshift
     * rshift
     * &
     * +
     * -
     * >
     * 
     * =
     */
    uint128_t quotient = {0};
    uint128_t remainder = {0};
    for(int i = 128; i > 0; --i) {
        quotient = u128_lshift(quotient, 1);
        remainder = u128_lshift(remainder, 1);

        if(u128_bit_and_u64(u128_rshift(a, i - 1), 1).low & 1) {
            remainder = u128_inc(remainder);
        }

        if(u128_ge(remainder, b)) {
            remainder = u128_sub(remainder, b);
            quotient = u128_inc(quotient);
        }
    }

    return (u128_divres){
        .quotient = quotient,
        .remainder = remainder,
    };
}

uint128_t u128_div(uint128_t a, uint128_t b) {
    return u128_divmod(a, b).quotient;
}

uint128_t u128_mod(uint128_t a, uint128_t b) {
    return u128_divmod(a, b).remainder;
}

#endif // UINT128_T_IMPLEMENTATION
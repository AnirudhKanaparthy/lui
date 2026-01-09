#ifndef UINT512_T_H_
#define UINT512_T_H_

#include "uint256_t.h"

typedef struct {
    // Order is like this because of little endian
    uint256_t low;
    uint256_t high;
} uint512_t;

#define UINT512_MAX ((uint512_t){.low=UINT256_MAX, .high=UINT256_MAX})
#define UINT512_0 ((uint512_t){.low=UINT256_0, .high=UINT256_0})
#define UINT512_1 ((uint512_t){.low=UINT256_1, .high=UINT256_0})

typedef struct {
    uint512_t quotient;
    uint512_t remainder;
} u512_divres;

uint512_t u512_add(uint512_t a, uint512_t b);
uint512_t u512_sub(uint512_t a, uint512_t b);
uint512_t u512_mul(uint512_t a, uint512_t b);
u512_divres u512_divmod(uint512_t a, uint512_t b);
uint512_t u512_div(uint512_t a, uint512_t b);
uint512_t u512_mod(uint512_t a, uint512_t b);

uint512_t u512_add_u64(uint512_t a, uint64_t b);
uint512_t u512_sub_u64(uint512_t a, uint64_t b);
uint512_t u512_mul_u64(uint512_t a, uint64_t b);
u512_divres u512_divmod_u64(uint512_t a, uint64_t b);
uint512_t u512_div_u64(uint512_t a, uint64_t b);
uint512_t u512_mod_u64(uint512_t a, uint64_t b);

//
uint512_t u512_bit_not(uint512_t n);
uint512_t u512_bit_or(uint512_t a, uint512_t b);
uint512_t u512_bit_and(uint512_t a, uint512_t b);

uint512_t u512_bit_or_u64(uint512_t a, uint64_t b);
uint512_t u512_bit_and_u64(uint512_t a, uint64_t b);

//
uint512_t u512_rshift(uint512_t n, uint16_t amount);
uint512_t u512_lshift(uint512_t n, uint16_t amount);

//
bool u512_le(uint512_t a, uint512_t b);
bool u512_ge(uint512_t a, uint512_t b);
bool u512_eq(uint512_t a, uint512_t b);
bool u512_lt(uint512_t a, uint512_t b);
bool u512_gt(uint512_t a, uint512_t b);

bool u512_le_u64(uint512_t a, uint64_t b);
bool u512_ge_u64(uint512_t a, uint64_t b);
bool u512_eq_u64(uint512_t a, uint64_t b);
bool u512_lt_u64(uint512_t a, uint64_t b);
bool u512_gt_u64(uint512_t a, uint64_t b);

//
void u512_dec_in(uint512_t* a);
uint512_t u512_dec(uint512_t a);

void u512_inc_in(uint512_t* a);
uint512_t u512_inc(uint512_t a);

#endif // UINT512_T_H_

#define UINT512_T_IMPLEMENTATION
#if defined(UINT512_T_IMPLEMENTATION) && !defined(UINT512_T_IMPLEMENTATION_DEFINED)
#define UINT512_T_IMPLEMENTATION_DEFINED

#define UINT128_T_IMPLEMENTATION
#include "uint256_t.h"

uint512_t u512_add(uint512_t a, uint512_t b) {
    uint512_t res = (uint512_t){
        .low = u256_add(a.low, b.low),
        .high = u256_add(a.high, b.high),
    };
    if(u256_gt_u64(a.low, 0) && u256_gt(b.low, u256_sub(UINT256_MAX, a.low))) {
        res.high = u256_inc(res.high);
    }
    return res;
}

uint512_t u512_sub(uint512_t a, uint512_t b) {
    // Long way of doing it using 2s complement
    // return u256_add(a, u256_add(u256_not(b), (uint512_t){0, 1}));
    uint512_t res = (uint512_t){
        .low = u256_sub(a.low, b.low),
        .high = u256_sub(a.high, b.high),
    };
    if(u256_lt(a.low, b.low)) {
        res.high = u256_dec(res.high);
    }
    return res;
}

uint512_t u512_add_u64(uint512_t a, uint64_t b) {
    uint512_t res = (uint512_t){
        .low = u256_add_u64(a.low, b),
        .high = a.high,
    };
    if(u256_gt_u64(a.low, 0) && u256_le_u64(u256_sub(UINT256_MAX, a.low), b)) {
        res.high = u256_inc(res.high);
    }
    return res;
}

uint512_t u512_sub_u64(uint512_t a, uint64_t b) {
    uint512_t res = (uint512_t){
        .low = u256_sub_u64(a.low, b),
        .high = a.high,
    };
    if(u256_lt_u64(a.low, b)) {
        res.high = u256_dec(res.high);
    }
    return res;
}

uint512_t u512_mul_u64(uint512_t a, uint64_t b) {
    return u512_mul(a, (uint512_t){.low=b, .high=UINT256_0});
}

u512_divres u512_divmod_u64(uint512_t a, uint64_t b) {
    return u512_divmod(a, (uint512_t){.low=b, .high=UINT256_0});
}

uint512_t u512_div_u64(uint512_t a, uint64_t b) {
    return u512_divmod_u64(a, b).quotient;
}

uint512_t u512_mod_u64(uint512_t a, uint64_t b) {
    return u512_divmod_u64(a, b).remainder;
}

uint512_t u512_inc(uint512_t a) {
    return u512_add_u64(a, 1);
}

uint512_t u512_dec(uint512_t a) {
    return u512_sub_u64(a, 1);
}

void u512_inc_in(uint512_t* a) {
    *a = u512_add_u64(*a, 1);
}

void u512_dec_in(uint512_t* a) {
    *a = u512_sub_u64(*a, 1);
}

uint512_t u512_mul(uint512_t a, uint512_t b) {
    /**
     *     abcd
     *     pqrs
     *     ....
     *   .|...
     *  ..|..
     * ...|.
     *     wxyz
     */
    uint128_t a_hh = a.high.high;
    uint128_t a_hl = a.high.low;
    uint128_t a_lh = a.low.high;
    uint128_t a_ll = a.low.low;

    uint256_t b_hh = (uint256_t){.low=b.high.high, .high=UINT128_0};
    uint256_t b_hl = (uint256_t){.low=b.high.low, .high=UINT128_0};
    uint256_t b_lh = (uint256_t){.low=b.low.high, .high=UINT128_0};
    uint256_t b_ll = (uint256_t){.low=b.low.low, .high=UINT128_0};

    uint256_t first = u256_mul(b_ll, U128_TO_U256(a_ll));

    // Separating out to capture the carry

    // Second
    uint256_t s_0 = u256_mul(b_ll, U128_TO_U256(a_lh));
    uint256_t s_1 = u256_mul(b_lh, U128_TO_U256(a_ll));
    uint256_t s_carry = (uint256_t){.low=((u256_gt_u64(s_0, 0) && u256_gt(s_1, u256_sub(UINT256_MAX, s_0))) ? 1 : 0), .high=UINT128_0};
    uint256_t second = u256_add(s_0, s_1);

    // Third
    uint256_t t_0 = u256_mul(b_ll, U128_TO_U256(a_hl));
    uint256_t t_1 = u256_mul(b_lh, U128_TO_U256(a_lh));
    uint256_t t_2 = u256_mul(b_hl, U128_TO_U256(a_ll));
    uint256_t t_carry = UINT256_0;
    uint256_t third = t_0;
    t_carry =  u256_add_u64(t_carry, (u256_gt_u64(third, 0) && u256_gt(t_1, u256_sub(UINT256_MAX, third))) ? 1 : 0);
    third = u256_add(third, t_1);
    t_carry =  u256_add_u64(t_carry, (u256_gt_u64(third, 0) && u256_gt(t_2, u256_sub(UINT256_MAX, third))) ? 1 : 0);
    third = u256_add(third, t_2);

    // Fourth
    uint256_t fourth = u256_add(
        u256_add(
            u256_mul(b_ll, U128_TO_U256(a_hh)),
            u256_mul(b_lh, U128_TO_U256(a_hl))),
        u256_add(
            u256_mul(b_hl, U128_TO_U256(a_lh)),
            u256_mul(b_hh, U128_TO_U256(a_ll))));

    // Carry forward

    // Second
    uint256_t temp = (uint256_t){.low=first.high, .high=UINT128_0};
    t_carry =  u256_add_u64(t_carry, (u256_gt_u64(second, 0) && u256_gt(temp, u256_sub(UINT256_MAX, second))) ? 1 : 0);
    second = u256_add(second, temp);

    // Third
    t_carry =  u256_add_u64(t_carry, (u256_gt_u64(third, 0) && u256_gt(s_carry, u256_sub(UINT256_MAX, third))) ? 1 : 0);
    third = u256_add(third, (uint256_t){.low=UINT128_0, .high=s_carry.low});
    temp = (uint256_t){.low=second.high, .high=UINT128_0};
    t_carry =  u256_add_u64(t_carry, (u256_gt_u64(third, 0) && u256_gt(temp, u256_sub(UINT256_MAX, third))) ? 1 : 0);
    third = u256_add(third, temp);

    // Fourth
    fourth = u256_add(fourth, (uint256_t){.low=UINT128_0, .high=t_carry.low});
    fourth = u256_add(fourth, (uint256_t){.low=third.high, .high=UINT128_0});

    return (uint512_t){
        .low  = (uint256_t){.low=first.low, .high=second.low},
        .high = (uint256_t){.low=third.low, .high=fourth.low},
    };
}

bool u512_gt(uint512_t a, uint512_t b) {
    return u256_gt(a.high, b.high) || (u256_eq(a.high, b.high) && u256_gt(a.low, b.low));
}

bool u512_lt(uint512_t a, uint512_t b) {
    return u256_lt(a.high, b.high) || (u256_eq(a.high, b.high) && u256_lt(a.low, b.low));
}

bool u512_eq(uint512_t a, uint512_t b) {
    return u256_eq(a.high, b.high) && u256_eq(a.low, b.low);
}

bool u512_ge(uint512_t a, uint512_t b) {
    return u512_gt(a, b) || u512_eq(a, b);
}

bool u512_le(uint512_t a, uint512_t b) {
    return u512_lt(a, b) || u512_eq(a, b);
}

bool u512_le_u64(uint512_t a, uint64_t b) {
    return (a.high.high.high ||
            a.high.high.low  ||
            a.high.low.high  ||
            a.high.low.low
        ) ? false : u256_le_u64(a.low, b);
}
bool u512_ge_u64(uint512_t a, uint64_t b) {
    return (a.high.high.high ||
            a.high.high.low  ||
            a.high.low.high  ||
            a.high.low.low
        )  ? true : u256_ge_u64(a.low, b);
}
bool u512_eq_u64(uint512_t a, uint64_t b) {
    return (a.high.high.high ||
            a.high.high.low  ||
            a.high.low.high  ||
            a.high.low.low
        )  ? false : u256_eq_u64(a.low, b);
}
bool u512_lt_u64(uint512_t a, uint64_t b) {
    return (a.high.high.high ||
            a.high.high.low  ||
            a.high.low.high  ||
            a.high.low.low
        )  ? false : u256_lt_u64(a.low, b);
}
bool u512_gt_u64(uint512_t a, uint64_t b) {
    return (a.high.high.high ||
            a.high.high.low  ||
            a.high.low.high  ||
            a.high.low.low
        )  ? true : u256_gt_u64(a.low, b);
}

uint512_t u512_lshift(uint512_t n, uint16_t amount) {
    uint512_t res = (uint512_t){
        .low = n.low,
        .high = n.high,
    };

    for(int i = 0; i < amount; ++i) {
        res.high = u256_lshift(res.high, 1);
        res.high = u256_bit_or(res.high, u256_bit_and_u64(u256_rshift(res.low, 255), 1));
        res.low = u256_lshift(res.low, 1);
    }

    return res;
}

uint512_t u512_rshift(uint512_t n, uint16_t amount) {
    uint512_t res = (uint512_t){
        .low = n.low,
        .high = n.high,
    };

    // const uint256_t lmb = u256_lshift(UINT256_1, 255);
    for(int i = 0; i < amount; ++i) {
        res.low = u256_rshift(res.low, 1);
        res.low = u256_bit_or(res.low, u256_lshift(u256_bit_and_u64(res.high, 1), 255));
        res.high = u256_rshift(res.high, 1);
    }

    return res;
}

uint512_t u512_bit_and(uint512_t a, uint512_t b) {
    return (uint512_t){
        .low = u256_bit_and(a.low, b.low),
        .high = u256_bit_and(a.high, b.high),
    };
}

uint512_t u512_bit_or(uint512_t a, uint512_t b) {
    return (uint512_t){
        .low = u256_bit_or(a.low, b.low),
        .high = u256_bit_or(a.high, b.high),
    };
}

uint512_t u512_bit_or_u64(uint512_t a, uint64_t b) {
    return (uint512_t) {
        .low = u256_bit_or_u64(a.low, b),
        .high = u256_bit_or_u64(a.high, 0ul),
    };
}
uint512_t u512_bit_and_u64(uint512_t a, uint64_t b) {
    return (uint512_t) {
        .low = u256_bit_and_u64(a.low, b),
        .high = u256_bit_and_u64(a.high, 0ul),
    };
}

uint512_t u512_bit_not(uint512_t n) {
    return (uint512_t){
        .low = u256_bit_not(n.low),
        .high = u256_bit_not(n.high),
    };
}

u512_divres u512_divmod(uint512_t a, uint512_t b) {
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
    uint512_t quotient = {0};
    uint512_t remainder = {0};

    memset(&quotient, 0, sizeof(uint512_t));
    memset(&remainder, 0, sizeof(uint512_t));

    for(int i = 512; i > 0; --i) {
        quotient = u512_lshift(quotient, 1);
        remainder = u512_lshift(remainder, 1);
        
        if(u512_bit_and_u64(u512_rshift(a, i - 1), 1).low.low.low & 1) {
            remainder = u512_inc(remainder);
        }
        
        if(u512_ge(remainder, b)) {
            remainder = u512_sub(remainder, b);
            quotient = u512_inc(quotient);
        }
    }

    return (u512_divres){
        .quotient = quotient,
        .remainder = remainder,
    };
}

uint512_t u512_div(uint512_t a, uint512_t b) {
    return u512_divmod(a, b).quotient;
}

uint512_t u512_mod(uint512_t a, uint512_t b) {
    return u512_divmod(a, b).remainder;
}

#endif // UINT512_T_IMPLEMENTATION

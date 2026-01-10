#ifndef UINT256_T_H_
#define UINT256_T_H_

#include "uint128_t.h"

typedef struct {
    // Order is like this because of little endian
    uint128_t low;
    uint128_t high;
} uint256_t;

#define UINT256_MAX ((uint256_t){.low=UINT128_MAX, .high=UINT128_MAX})
#define UINT256_0 ((uint256_t){.low=UINT128_0, .high=UINT128_0})
#define UINT256_1 ((uint256_t){.low=UINT128_1, .high=UINT128_0})

#define U128_TO_U256(x) ((uint256_t){.low=(x), .high=UINT128_0})
#define U256_UL(x, n) ((n) < (sizeof(uint256_t) / sizeof(uint64_t) / 2) ? U128_UL((x).low, (n)) : U128_UL((x).high, (n) - (sizeof(uint256_t) / sizeof(uint64_t) / 2)))

typedef struct {
    uint256_t quotient;
    uint256_t remainder;
} u256_divres;

uint256_t u256_add(uint256_t a, uint256_t b);
uint256_t u256_sub(uint256_t a, uint256_t b);
uint256_t u256_mul(uint256_t a, uint256_t b);
u256_divres u256_divmod(uint256_t a, uint256_t b);
uint256_t u256_div(uint256_t a, uint256_t b);
uint256_t u256_mod(uint256_t a, uint256_t b);

uint256_t u256_add_u64(uint256_t a, uint64_t b);
uint256_t u256_sub_u64(uint256_t a, uint64_t b);
uint256_t u256_mul_u64(uint256_t a, uint64_t b);
u256_divres u256_divmod_u64(uint256_t a, uint64_t b);
uint256_t u256_div_u64(uint256_t a, uint64_t b);
uint256_t u256_mod_u64(uint256_t a, uint64_t b);

//
uint256_t u256_bit_not(uint256_t n);
uint256_t u256_bit_or(uint256_t a, uint256_t b);
uint256_t u256_bit_and(uint256_t a, uint256_t b);

uint256_t u256_bit_or_u64(uint256_t a, uint64_t b);
uint256_t u256_bit_and_u64(uint256_t a, uint64_t b);

//
uint256_t u256_rshift(uint256_t n, uint16_t amount);
uint256_t u256_lshift(uint256_t n, uint16_t amount);

//
bool u256_le(uint256_t a, uint256_t b);
bool u256_ge(uint256_t a, uint256_t b);
bool u256_eq(uint256_t a, uint256_t b);
bool u256_lt(uint256_t a, uint256_t b);
bool u256_gt(uint256_t a, uint256_t b);

bool u256_le_u64(uint256_t a, uint64_t b);
bool u256_ge_u64(uint256_t a, uint64_t b);
bool u256_eq_u64(uint256_t a, uint64_t b);
bool u256_lt_u64(uint256_t a, uint64_t b);
bool u256_gt_u64(uint256_t a, uint64_t b);

//
void u256_dec_in(uint256_t* a);
uint256_t u256_dec(uint256_t a);

void u256_inc_in(uint256_t* a);
uint256_t u256_inc(uint256_t a);

#endif // UINT256_T_H_

#if defined(UINT256_T_IMPLEMENTATION) && !defined(UINT256_T_IMPLEMENTATION_DEFINED)
#define UINT256_T_IMPLEMENTATION_DEFINED

#define UINT128_T_IMPLEMENTATION
#include "uint128_t.h"

uint256_t u256_add(uint256_t a, uint256_t b) {
    uint256_t res = (uint256_t){
        .low = u128_add(a.low, b.low),
        .high = u128_add(a.high, b.high),
    };
    if(u128_gt_u64(a.low, 0) && u128_gt(b.low, u128_sub(UINT128_MAX, a.low))) {
        res.high = u128_inc(res.high);
    }
    return res;
}

uint256_t u256_sub(uint256_t a, uint256_t b) {
    // Long way of doing it using 2s complement
    // return u256_add(a, u256_add(u256_not(b), (uint256_t){0, 1}));
    uint256_t res = (uint256_t){
        .low = u128_sub(a.low, b.low),
        .high = u128_sub(a.high, b.high),
    };
    if(u128_lt(a.low, b.low)) {
        res.high = u128_dec(res.high);
    }
    return res;
}

uint256_t u256_add_u64(uint256_t a, uint64_t b) {
    uint256_t res = (uint256_t){
        .low = u128_add_u64(a.low, b),
        .high = a.high,
    };
    if(u128_gt_u64(a.low, 0) && u128_le_u64(u128_sub(UINT128_MAX, a.low), b)) {
        res.high = u128_inc(res.high);
    }
    return res;
}

uint256_t u256_sub_u64(uint256_t a, uint64_t b) {
    uint256_t res = (uint256_t){
        .low = u128_sub_u64(a.low, b),
        .high = a.high,
    };
    if(u128_lt_u64(a.low, b)) {
        res.high = u128_dec(res.high);
    }
    return res;
}

uint256_t u256_mul_u64(uint256_t a, uint64_t b) {
    return u256_mul(a, (uint256_t){.low=b, .high=UINT128_0});
}

u256_divres u256_divmod_u64(uint256_t a, uint64_t b) {
    return u256_divmod(a, (uint256_t){.low=b, .high=UINT128_0});
}

uint256_t u256_div_u64(uint256_t a, uint64_t b) {
    return u256_divmod_u64(a, b).quotient;
}

uint256_t u256_mod_u64(uint256_t a, uint64_t b) {
    return u256_divmod_u64(a, b).remainder;
}

uint256_t u256_inc(uint256_t a) {
    return u256_add_u64(a, 1);
}

uint256_t u256_dec(uint256_t a) {
    return u256_sub_u64(a, 1);
}

void u256_inc_in(uint256_t* a) {
    *a = u256_add_u64(*a, 1);
}

void u256_dec_in(uint256_t* a) {
    *a = u256_sub_u64(*a, 1);
}

uint256_t u256_mul(uint256_t a, uint256_t b) {
    /**
     *     abcd
     *     pqrs
     *     ....
     *   .|...
     *  ..|..
     * ...|.
     *     wxyz
     */
    uint64_t a_hh = a.high.high;
    uint64_t a_hl = a.high.low;
    uint64_t a_lh = a.low.high;
    uint64_t a_ll = a.low.low;

    uint128_t b_hh = (uint128_t){.low=b.high.high, .high=0};
    uint128_t b_hl = (uint128_t){.low=b.high.low, .high=0};
    uint128_t b_lh = (uint128_t){.low=b.low.high, .high=0};
    uint128_t b_ll = (uint128_t){.low=b.low.low, .high=0};

    uint128_t first = u128_mul_u64(b_ll, a_ll);

    // Separating out to capture the carry

    // Second
    uint128_t s_0 = u128_mul_u64(b_ll, a_lh);
    uint128_t s_1 = u128_mul_u64(b_lh, a_ll);
    uint128_t s_carry = (uint128_t){.low=((u128_gt_u64(s_0, 0) && u128_gt(s_1, u128_sub(UINT128_MAX, s_0))) ? 1 : 0), .high=0};
    uint128_t second = u128_add(s_0, s_1);

    // Third
    uint128_t t_0 = u128_mul_u64(b_ll, a_hl);
    uint128_t t_1 = u128_mul_u64(b_lh, a_lh);
    uint128_t t_2 = u128_mul_u64(b_hl, a_ll);
    uint128_t t_carry = UINT128_0;
    uint128_t third = t_0;
    t_carry =  u128_add_u64(t_carry, (u128_gt_u64(third, 0) && u128_gt(t_1, u128_sub(UINT128_MAX, third))) ? 1 : 0);
    third = u128_add(third, t_1);
    t_carry =  u128_add_u64(t_carry, (u128_gt_u64(third, 0) && u128_gt(t_2, u128_sub(UINT128_MAX, third))) ? 1 : 0);
    third = u128_add(third, t_2);

    // Fourth
    uint128_t fourth = u128_add(u128_add(u128_mul_u64(b_ll, a_hh), u128_mul_u64(b_lh, a_hl)), u128_add(u128_mul_u64(b_hl, a_lh), u128_mul_u64(b_hh, a_ll)));

    // Carry forward

    // Second
    uint128_t temp = (uint128_t){.low=first.high, .high=0};
    t_carry =  u128_add_u64(t_carry, (u128_gt_u64(second, 0) && u128_gt(temp, u128_sub(UINT128_MAX, second))) ? 1 : 0);
    second = u128_add(second, temp);

    // Third
    t_carry =  u128_add_u64(t_carry, (u128_gt_u64(third, 0) && u128_gt(s_carry, u128_sub(UINT128_MAX, third))) ? 1 : 0);
    third = u128_add(third, (uint128_t){.low=0, .high=s_carry.low});
    temp = (uint128_t){.low=second.high, .high=0};
    t_carry =  u128_add_u64(t_carry, (u128_gt_u64(third, 0) && u128_gt(temp, u128_sub(UINT128_MAX, third))) ? 1 : 0);
    third = u128_add(third, temp);

    // Fourth
    fourth = u128_add(fourth, (uint128_t){.low=0, .high=t_carry.low});
    fourth = u128_add(fourth, (uint128_t){.low=third.high, .high=0});

    return (uint256_t){
        .low  = (uint128_t){.low=first.low, .high=second.low},
        .high = (uint128_t){.low=third.low, .high=fourth.low},
    };
}

bool u256_gt(uint256_t a, uint256_t b) {
    return u128_gt(a.high, b.high) || (u128_eq(a.high, b.high) && u128_gt(a.low, b.low));
}

bool u256_lt(uint256_t a, uint256_t b) {
    return u128_lt(a.high, b.high) || (u128_eq(a.high, b.high) && u128_lt(a.low, b.low));
}

bool u256_eq(uint256_t a, uint256_t b) {
    return u128_eq(a.high, b.high) && u128_eq(a.low, b.low);
}

bool u256_ge(uint256_t a, uint256_t b) {
    return u256_gt(a, b) || u256_eq(a, b);
}

bool u256_le(uint256_t a, uint256_t b) {
    return u256_lt(a, b) || u256_eq(a, b);
}

bool u256_le_u64(uint256_t a, uint64_t b) {
    return (a.high.high || a.high.low) ? false : u128_le_u64(a.low, b);
}
bool u256_ge_u64(uint256_t a, uint64_t b) {
    return (a.high.high || a.high.low) ? true : u128_ge_u64(a.low, b);
}
bool u256_eq_u64(uint256_t a, uint64_t b) {
    return (a.high.high || a.high.low) ? false : u128_eq_u64(a.low, b);
}
bool u256_lt_u64(uint256_t a, uint64_t b) {
    return (a.high.high || a.high.low) ? false : u128_lt_u64(a.low, b);
}
bool u256_gt_u64(uint256_t a, uint64_t b) {
    return (a.high.high || a.high.low) ? true : u128_gt_u64(a.low, b);
}

uint256_t u256_lshift(uint256_t n, uint16_t amount) {
    uint256_t res = (uint256_t){
        .high = n.high,
        .low = n.low,
    };

    for(int i = 0; i < amount; ++i) {
        res.high = u128_lshift(res.high, 1);
        res.high = u128_bit_or(res.high, u128_bit_and_u64(u128_rshift(res.low, 127), 1));
        res.low = u128_lshift(res.low, 1);
    }

    return res;
}

uint256_t u256_rshift(uint256_t n, uint16_t amount) {
    uint256_t res = (uint256_t){
        .low = n.low,
        .high = n.high,
    };

    for(int i = 0; i < amount; ++i) {
        res.low = u128_rshift(res.low, 1);
        res.low = u128_bit_or(res.low, u128_lshift(u128_bit_and_u64(res.high, 1), 127));
        res.high = u128_rshift(res.high, 1);
    }

    return res;
}

uint256_t u256_bit_and(uint256_t a, uint256_t b) {
    return (uint256_t){
        .low = u128_bit_and(a.low, b.low),
        .high = u128_bit_and(a.high, b.high),
    };
}

uint256_t u256_bit_or(uint256_t a, uint256_t b) {
    return (uint256_t){
        .low = u128_bit_or(a.low, b.low),
        .high = u128_bit_or(a.high, b.high),
    };
}

uint256_t u256_bit_or_u64(uint256_t a, uint64_t b) {
    return (uint256_t) {
        .low = u128_bit_or_u64(a.low, b),
        .high = u128_bit_or_u64(a.high, 0ul),
    };
}
uint256_t u256_bit_and_u64(uint256_t a, uint64_t b) {
    return (uint256_t) {
        .low = u128_bit_and_u64(a.low, b),
        .high = u128_bit_and_u64(a.high, 0ul),
    };
}

uint256_t u256_bit_not(uint256_t n) {
    return (uint256_t){
        .low = u128_bit_not(n.low),
        .high = u128_bit_not(n.high),
    };
}

u256_divres u256_divmod(uint256_t a, uint256_t b) {
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
    uint256_t quotient = {0};
    uint256_t remainder = {0};
    for(int i = 256; i > 0; --i) {
        quotient = u256_lshift(quotient, 1);
        remainder = u256_lshift(remainder, 1);

        if(u256_bit_and_u64(u256_rshift(a, i - 1), 1).low.low & 1) {
            remainder = u256_inc(remainder);
        }
       
        if(u256_ge(remainder, b)) {
            remainder = u256_sub(remainder, b);
            quotient = u256_inc(quotient);
        }
    }

    return (u256_divres){
        .quotient = quotient,
        .remainder = remainder,
    };
}

uint256_t u256_div(uint256_t a, uint256_t b) {
    return u256_divmod(a, b).quotient;
}

uint256_t u256_mod(uint256_t a, uint256_t b) {
    return u256_divmod(a, b).remainder;
}

#endif // UINT256_T_IMPLEMENTATION

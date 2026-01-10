#ifndef UINT128_T_H_
#define UINT128_T_H_

#include <stdint.h>

#include "uintn_t.h"

///////////////////////////////////////////////////////////////////////////////////

#define u64_add(lhs, rhs) ((lhs) + (rhs))
#define u64_sub(lhs, rhs) ((lhs) - (rhs))
#define u64_mul(lhs, rhs) ((lhs) * (rhs))
#define u64_div(lhs, rhs) ((lhs) / (rhs))

#define u64_add_u64(lhs, rhs) ((lhs) + (rhs))
#define u64_sub_u64(lhs, rhs) ((lhs) - (rhs))
#define u64_mul_u64(lhs, rhs) ((lhs) * (rhs))
#define u64_div_u64(lhs, rhs) ((lhs) / (rhs))

#define u64_lshift(lhs, rhs) ((lhs) << (rhs))
#define u64_rshift(lhs, rhs) ((lhs) >> (rhs))

#define u64_inc(a) ((a) + 1)
#define u64_dec(a) ((a) - 1)

#define u64_bit_not(op) (~(op))
#define u64_bit_and(lhs, rhs) ((lhs) & (rhs))
#define u64_bit_or(lhs, rhs) ((lhs) | (rhs))

#define u64_bit_and_u64(lhs, rhs) ((lhs) & (rhs))
#define u64_bit_or_u64(lhs, rhs) ((lhs) | (rhs))

#define u64_gt(lhs, rhs) ((lhs) > (rhs))
#define u64_lt(lhs, rhs) ((lhs) < (rhs))
#define u64_ge(lhs, rhs) ((lhs) >= (rhs))
#define u64_le(lhs, rhs) ((lhs) <= (rhs))
#define u64_eq(lhs, rhs) ((lhs) == (rhs))

#define u64_gt_u64(lhs, rhs) ((lhs) > (rhs))
#define u64_lt_u64(lhs, rhs) ((lhs) < (rhs))
#define u64_ge_u64(lhs, rhs) ((lhs) >= (rhs))
#define u64_le_u64(lhs, rhs) ((lhs) <= (rhs))
#define u64_eq_u64(lhs, rhs) ((lhs) == (rhs))

#define UINT64_0 0
#define UINT64_1 1

#define U64_LOW(n) (uint32_t)((n) & 0x00000000FFFFFFFFUL)
#define U64_HIGH(n) (uint32_t)(((n) & 0xFFFFFFFF00000000UL) >> (64/2))

#define U32_LOW(n) (uint32_t)((n) & 0x00000000FFFFFFFFUL)
#define U32_HIGH(n) (uint32_t)(((n) & 0xFFFFFFFF00000000UL) >> (64/2))

#define UINT32_0 0
#define UINT32_1 1

#define make_u64(low, high) ((((uint64_t)(high)) << (64/2)) | (uint32_t)(low))
#define u64_u32(n) (n)

////////////////////////////////////////////////////////////////////////////////////

DEFINE_UINTN_T_STRUCT(128, 64);
DEFINE_DIVRES(128);
#define make_u128(tlow, thigh) (uint128_t){.low=(tlow), .high=(thigh)}
#define u128_u64(n) make_u128((n), UINT64_0)

#define UINT128_MAX make_u128(UINT64_MAX, UINT64_MAX)
#define UINT128_0 make_u128(UINT64_0, UINT64_0)
#define UINT128_1 make_u128(UINT64_1, UINT64_0)

#define U128_LOW(n) (n).low
#define U128_HIGH(n) (n).high
#define U128_LEFTMOST_U64(n) (n).low

DECLARE_ALL_FUNCS(128, 64, 32);

#endif // UINT128_T_H_

#if defined(UINT128_T_IMPLEMENTATION) && !defined(UINT128_T_IMPLEMENTATION_DEFINED)
#define UINT128_T_IMPLEMENTATION_DEFINED

#define UINTN_T_IMPLEMENTATION
#include "uintn_t.h"
DEFINE_ALL_FUNCS(128, 64, 32);

#endif // UINT128_T_IMPLEMENTATION
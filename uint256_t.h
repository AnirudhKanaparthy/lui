#ifndef UINT256_T_H_
#define UINT256_T_H_

#include "uint128_t.h"

DEFINE_UINTN_T_STRUCT(256, 128);
DEFINE_DIVRES(256);
#define make_u256(tlow, thigh) (uint256_t){.low=(tlow), .high=(thigh)}
#define u256_u128(n) make_u256((n), UINT128_0)

#define UINT256_MAX make_u256(UINT128_MAX, UINT128_MAX)
#define UINT256_0 make_u256(UINT128_0, UINT128_0)
#define UINT256_1 make_u256(UINT128_1, UINT128_0)

#define U256_LOW(n) (n).low
#define U256_HIGH(n) (n).high
#define U256_LEFTMOST_U64(n) ((n).low.low)

#define U256_UL(op, idx) (((idx) & 0b010) ? U128_UL((op).high, (idx)) : U128_UL((op).low, (idx)))
#define U256_UL_PTR(op, idx) (((idx) & 0b010) ? U128_UL_PTR((op).high, (idx)) : U128_UL_PTR((op).low, (idx)))

DECLARE_ALL_FUNCS(256, 128, 64);

#endif // UINT256_T_H_

#if defined(UINT256_T_IMPLEMENTATION) && !defined(UINT256_T_IMPLEMENTATION_DEFINED)
#define UINT256_T_IMPLEMENTATION_DEFINED

#define UINTN_T_IMPLEMENTATION
#include "uintn_t.h"
DEFINE_ALL_FUNCS(256, 128, 64);

#endif // UINT256_T_IMPLEMENTATION

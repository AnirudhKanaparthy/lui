#ifndef UINT512_T_H_
#define UINT512_T_H_

#include "uint256_t.h"

DEFINE_UINTN_T_STRUCT(512, 256);
DEFINE_DIVRES(512);
#define make_u512(tlow, thigh) (uint512_t){.low=(tlow), .high=(thigh)}
#define u512_u256(n) make_u512((n), UINT256_0)

#define UINT512_MAX make_u512(UINT256_MAX, UINT256_MAX)
#define UINT512_0 make_u512(UINT256_0, UINT256_0)
#define UINT512_1 make_u512(UINT256_1, UINT256_0)

#define U512_LOW(n) (n).low
#define U512_HIGH(n) (n).high
#define U512_LEFTMOST_U64(n) ((n).low.low.low)

#define U512_UL(op, idx) (((idx) & 0b100) ? U256_UL((op).high, (idx)) : U256_UL((op).low, (idx)))
#define U512_UL_PTR(op, idx) (((idx) & 0b100) ? U256_UL_PTR((op).high, (idx)) : U256_UL_PTR((op).low, (idx)))

DECLARE_ALL_FUNCS(512, 256, 128);

#endif // UINT512_T_H_

#if defined(UINT512_T_IMPLEMENTATION) && !defined(UINT512_T_IMPLEMENTATION_DEFINED)
#define UINT512_T_IMPLEMENTATION_DEFINED

#define UINTN_T_IMPLEMENTATION
#include "uintn_t.h"
DEFINE_ALL_FUNCS(512, 256, 128);

#endif // UINT512_T_IMPLEMENTATION

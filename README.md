# lui
LUI - Larger Unsigned Integers. This repository contains C implementations for larger unsigned integers, starting from uint128_t.

The uint128_t header is made like a stb library. So you may include the header anywhere (just like a normal header file). But you have the control over where to put the definitions. You can if you want just create a uint128_t.c and put all the definitions there. You can add definitions to any translation unit (you can just think of it as a file) via the below:

```c
#define UINT128_T_IMPLEMENTATION
#include "uint128_t.h"
```

>[note] Warning
> This is not production-grade code (obviously). I made this up in like a day. I wanted to represent bigger numbers in C and not be restricted to 64 bits, especially for cryptographic algorithms which deal with 256 or even 512 bits.
> This is only for educational purposes only.

## TODO Goals
1. Write validators for all
2. Optimize all uint128_t, uint256_t, uint512_t

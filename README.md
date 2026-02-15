# lui
LUI - Large Unsigned Integers. This repository contains C implementations for large unsigned integers, starting from uint128_t going all the way upto uint512_t. But, this can be easily further expanded to even uint1024_t and larger!

The uint128_t header is made like a stb library. So you may include the header anywhere (just like a normal header file). But you have the control over where to put the definitions. You can if you want just create a uint128_t.c and put all the definitions there. You can add definitions to any translation unit via the below:

## Quick Start
```c
#include <stdio.h>

#define UINT128_T_IMPLEMENTATION
#include "uint128_t.h"

#define LUI_UTILS_IMPLEMENTATION
#include "utils.h"

DECLARE_LUI_UTILS(128);

int main(void) {
    // Low part first then the higher one
    uint128_t a = make_u128(563, 233); // 233 0
    uint128_t b = make_u128(34, 2351); // 0 1
    uint128_t c = u128_mul(a, b);
    
    u128_printf(a);
    printf(" * ");
    u128_printf(b);
    printf(" = ");
    u128_printf(c);
    printf("\n");
}

DEFINE_LUI_UTILS(128);
```


## Contributions
I am open to contributions, if you are interested. Just fork it, add your changes and create a pull request! And if you find any bugs please let me know.
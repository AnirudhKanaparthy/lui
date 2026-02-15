#ifndef LUI_UTILS_H_
#define LUI_UTILS_H_

#include <stddef.h>
#include <stdint.h>

int lpad(char* dest, const char* data, size_t len, char pad_char, size_t padding);
uint8_t hex_bits(const char c[2]);

#define DECLARE_LUI_UTILS(bits)                                         \
    int UN_PREFIX(bits, sprintf)(char* out, UINTN_T(bits) n);           \
    UINTN_T(bits) UN_PREFIX(bits, from_hex)(const char* str, int size); \
    int UN_PREFIX(bits, printf)(UINTN_T(bits) n);

#endif // LUI_UTILS_H_

#if defined(LUI_UTILS_IMPLEMENTATION)

#include <string.h>

#define DEFINE_LUI_UTILS(bits)  \
    DEFINE_PRINTF_FUNC(bits)    \
    DEFINE_FROM_HEX_FUNC(bits)  \
    DEFINE_SPRINTF_FUNC(bits)

int lpad(char* dest, const char* data, size_t len, char pad_char, size_t padding) {
    if (dest == NULL || data == NULL) return -1;
    if (len > padding) return -1;

    memset(dest, 0, padding);
    for (size_t i = 0; i < len; ++i) {
        dest[padding - i - 1] = data[len - i - 1];
    }
    int count = 0;
    while (*dest == '\0') {
        *dest = pad_char;
        ++dest;
        ++count;
    }
    return count;
}

uint8_t hex_bits(const char c[2]) {
    uint8_t res = 0;
    if (c[1] >= '0' && c[1] <= '9') {
        res |= c[1] - '0';
    } else if (c[1] >= 'a' && c[1] <= 'f') {
        res |= c[1] - 'a' + 10;
    } else if (c[1] >= 'A' && c[1] <= 'F') {
        res |= c[1] - 'A' + 10;
    }
   
    if (c[0] >= '0' && c[0] <= '9') {
        res |= (c[0] - '0') << 4;
    } else if (c[0] >= 'a' && c[0] <= 'f') {
        res |= (c[0] - 'a' + 10) << 4;
    } else if (c[0] >= 'A' && c[0] <= 'F') {
        res |= (c[0] - 'A' + 10) << 4;
    }
   
    return res;
}

#define DEFINE_SPRINTF_FUNC(bits)                               \
    int UN_PREFIX(bits, sprintf)(char* out, UINTN_T(bits) n) {  \
        if (!out) return -1;                                    \
                                                                \
        bool inside = false;                                    \
        const long n_64s = (bits/64);                           \
        const char* start = out;                                \
                                                                \
        for(long i = 0; i < n_64s; ++i) {                       \
            const long idx = n_64s - i - 1;                     \
            const uint64_t val = UN_UL(bits)(n, idx);           \
                                                                \
            if(!inside && (val != UINT64_0 || idx == 0)) {      \
                inside = true;                                  \
                out += sprintf(out, "%lx", val);                \
            } else if(inside) {                                 \
                out += sprintf(out, "%0*lx", 64/4, val);        \
            }                                                   \
        }                                                       \
        return (int)(out - start);                              \
    }

#define DEFINE_FROM_HEX_FUNC(bits)                                        \
    UINTN_T(bits) UN_PREFIX(bits, from_hex)(const char* str, int size) {  \
        /* Little Endian, also str must contain '\0' at the end */        \
                                                                          \
        if (size > (bits / 4)) {                                          \
            return UINTN_0(bits);                                         \
        }                                                                 \
        UINTN_T(bits) res = {0};                                          \
        uint8_t* cur = (uint8_t*)&res;                                    \
                                                                          \
        int j = 0;                                                        \
        for (int i = 0; i < size; i += 2) {                               \
            const int idx = size - i - 1;                                 \
            const uint8_t h = hex_bits(str + (idx - 1));                  \
            cur[j++] = h;                                                 \
        }                                                                 \
                                                                          \
        return res;                                                       \
    }

#define DEFINE_PRINTF_FUNC(bits)                    \
    int UN_PREFIX(bits, printf)(UINTN_T(bits) n) {  \
        static char buffer[(bits / 4) + 1] = {0};   \
        memset(buffer, 0, sizeof(buffer));          \
        UN_PREFIX(bits, sprintf)(buffer, n);        \
        return printf("%s", buffer);                \
    }


#endif // LUI_UTILS_IMPLEMENTATION
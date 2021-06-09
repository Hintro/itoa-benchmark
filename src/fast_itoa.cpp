#include "fast_itoa.h"
#include <string.h>

const int32_t e40d10000 = (1ull << 40) / 10000 + 1, e19d100 = (1<<19) / 100 + 1, e10d10 = 103, e9d10 = 52;
const int64_t e32m10000 = (1ull << 32) * 10000, ascii0s = 0x3030303030303030ll, e57d8 = 1441151881;

const uint16_t dLut[93] = {
       0,  '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8',  '9',
    '01', '11', '21', '31', '41', '51', '61', '71', '81', '91',
    '02', '12', '22', '32', '42', '52', '62', '72', '82', '92',
    '03', '13', '23', '33', '43', '53', '63', '73', '83', '93',
    '04', '14', '24', '34', '44', '54', '64', '74', '84', '94',
    '05', '15', '25', '35', '45', '55', '65', '75', '85', '95',
    '06', '16', '26', '36', '46', '56', '66', '76', '86', '96',
    '07', '17', '27', '37', '47', '57', '67', '77', '87', '97',
    '08', '18', '28', '38', '48', '58', '68', '78', '88', '98',
    '09', '19', '29'
};

const int32_t powers[14] = {
    10000000, 10000000, 10000000, 1000000, 1000000, 100000, 10000, 10000, 1000, 1000, 100, 10, 10, 0
};

uint32_t const * const powersLut = (uint32_t *) powers - 2;

const uint64_t shiftLut = 0x7665443321100000ull;

// For sequential optimization
const uint64_t _198 = 0xc6c6c6c6c6c6c6c6ull, _208 = 0xd0d0d0d0d0d0d0d0ull, 
b10100000 = 0xa0a0a0a0a0a0a0a0ull, lo4bit = 0x0f0f0f0f0f0f0f0full, _192 = 0xc0c0c0c0c0c0c0c0ll;
const uint8_t init_mask = 0xff;


inline uint64_t itoa8lil(const uint32_t val) {
    // const uint32_t q = (uint64_t) val * e40d10000 >> 40;
    // const uint_fast64_t a = ((uint_fast64_t) val << 32 | q) - q * e32m10000;
    // const uint_fast64_t b = a * e19d100 & 0x3f8000003f80000ull;
    // const uint_fast64_t c = (a<<17 | b>>18) - b*25, d = c * e10d10 & 0x7800780078007800ull; 
    // return (c<<9 | d>>9) - d* 5;

    // May 12, 2021: simplify further using distributive and associative properties of multiplication
    // const uint_fast32_t q = (uint_fast64_t) val * e40d10000 >> 40, t0 = ((uint_fast64_t) val<<32 | q);
    // const uint_fast64_t b = t0*e19d100 - q*c0 & 0x3f8000003f80000ull, a = t0 - q*e32m10000;
    // const uint64_t t1 = (a<<17 | b>>18);
    // const uint_fast64_t d = t1*e10d10 - b*c1 & 0x7800780078007800ull, c = t1 - b*25;
    // return (c<<9 | d>>9) - d*5;

    // const int32_t q = (int64_t) val * e40d10000 >> 40;
    // const int64_t a = ((int64_t) val << 32 | q) - q * e32m10000;
    // const int64_t b = a * e19d100 & 0x3f8000003f80000ull;
    // const int64_t c = (a<<17 | b>>18) - b*25, d = c * e10d10 & 0x7800780078007800ull; 
    // return (c<<9 | d>>9) - d*5;

    const int32_t q = (int64_t) val * e40d10000 >> 40;
    const int64_t a = ((int64_t) val << 32 | q) - q * e32m10000;
    const int64_t b = a * e19d100 & 0x3f8000003f80000ll;
    const int64_t c = (a<<17 | b>>18) - b*25, d = c * e10d10 & 0x7800780078007800ll; 
    return (c<<9 | d>>9) - d*5;
}

inline void itoa_u32(const uint32_t val, char *buffer) {
    int64_t lo8a = 0x30;  // ASCII 0
    if (val >= 100000000) {
        const int32_t hi2 = val/100000000;
        memcpy(buffer, dLut+hi2, 2);
        const int32_t lo8 = val%100000000;
        buffer += val>=1000000000 ? 2 : 1;
        lo8a = itoa8lil(lo8)>>2 | ascii0s;
    } else if (val) {
        const int64_t decimals = itoa8lil(val);
        lo8a = (decimals>>2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
    }
    buffer[8] = 0;
    memcpy(buffer, &lo8a, 8);
}

Itoa32::Itoa32() {
    E = 0;
    // Hi2 = 0;
    last = '0';
    // Lo8a = '0';
    // mask = init_mask;
    off = 0;
}

void Itoa32::itoa(const int32_t val, char * buffer) {
    
}

void Itoa32::uitoa(const uint32_t val, char * buffer) {
    // if (val == E) {
    //     const uint64_t a = Lo8a+1 + (_198&mask);
    //     const uint64_t b = (a&lo4bit | ascii0s) - ((a&_192) >> 4);
    //     if (a > mask) mask = mask<<8 | mask;
    //     Lo8a = b&mask;
    //     if(a==0) {
    //         memcpy(buffer, dLut + ++Hi2, 2);
    //     }
    //     buffer += (val>=100000000) + (val>=1000000000);
    //     ++E;
    //     Lo8a = __builtin_bswap64(Lo8a); memcpy(buffer, &Lo8a, 8);
    // } else {
    //     int64_t lo8a = 0x30;  // ASCII 0
    //     Hi2 = 0; mask = -1ll;
    //     if (val >= 100000000) {
    //         const int32_t hi2 = val/100000000;
    //         Hi2 = dLut[hi2];
    //         memcpy(buffer, &Hi2, 2);
    //         const int32_t lo8 = val%100000000;
    //         buffer += val>=1000000000 ? 2 : 1;
    //         lo8a = itoa8lil(lo8)>>2 | ascii0s;
    //     } else if (val) {
    //         const int64_t decimals = itoa8lil(val);
    //         lo8a = (decimals>>2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
    //         mask >>= 56 & __builtin_ctzll(decimals);
    //     }
    //     Lo8a = __builtin_bswap64(lo8a);
    //     buffer[8] = 0;
    //     memcpy(buffer, &lo8a, 8);
    // }
    // E = val+1;

    if((val == E) & (last<='9')) {
        memcpy(buffer, &last, 2);
        ++last;
    } else {
        int64_t lo8a = 0x30;  // ASCII 0
        if (val >= 100000000) {
            const int32_t hi2 = val/100000000;
            memcpy(buffer, dLut+hi2, 2);
            const int32_t lo8 = val%100000000;
            buffer += val>=1000000000 ? 2 : 1;
            lo8a = itoa8lil(lo8)>>2 | ascii0s;
            off = 8 + (val>=1000000000 ? 2 : 1);
        } else if (val) {
            const int64_t decimals = itoa8lil(val);
            lo8a = (decimals>>2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
            off = 64 - (56 & __builtin_ctzll(decimals)) >> 3;
        }
        buffer[8] = 0;
        memcpy(buffer, &lo8a, 8);
        E = val+1;
        last = (uint8_t) lo8a+1;
    }
}
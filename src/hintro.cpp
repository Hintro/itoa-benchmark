/*
Copyright 2021 Junyu Liu

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <stdint.h>
#include "test.h"
#include <iostream>
#include "constants.h"
#include "digitslut.h"

// static Itoa32 itoa32;

// inline int64_t itoa8big(const uint_fast32_t val) {
//     const int32_t q = (int64_t) val * e40d10000 >> 40;
//     const int64_t a = ((int64_t) q << 32 | val) - q * 10000;
//     const int64_t b = a * e19d100 & 0x3f8000003f80000ll;
//     const int64_t c = (a | b>>3) - (b*25>>17), d = c * e10d10 & 0x3c003c003c003c00ll;
//     return (c | d>>2) - (d*5>>9);
// }

inline uint64_t itoa8lil(const uint32_t val) {
    // const uint64_t q = val * e40d10000 >> 40, a = ((uint64_t) val << 32 | q) - q * e32m10000;
    // const uint64_t mask1 = 0x3f8000003f80000ull, mask2 = 0x7800780078007800ull;
    // const uint64_t b = a * e19d100 & mask1;
    // const uint64_t c = (a<<17 | b>>18) - b*25, d = c * e10d10 & mask2;
    // return (c<<9 | d>>9) - d* 5;  

    // const uint_fast32_t q = val * e40d10000 >> 40;
    // const uint_fast64_t a = ((uint64_t) val << 32 | q) - q * e32m10000;
    // const uint64_t b = a * e19d100 & 0x3f8000003f80000ull;
    // const uint64_t c = (a<<17 | b>>18) - b*25, d = c * e10d10 & 0x7800780078007800ull;
    // return (c<<9 | d>>9) - d* 5;

    const int32_t q = (int64_t) val * e40d10000 >> 40;
    const int64_t a = ((int64_t) val << 32 | q) - q * e32m10000;
    const int64_t b = a * e19d100 & 0x3f8000003f80000ll;
    const int64_t c = (a<<17 | b>>18) - b*25, d = c * e10d10 & 0x7800780078007800ll; 
    return (c<<9 | d>>9) - d*5;

    // const int32_t q = (int64_t) val * e40d10000 >> 40;
    // const int64_t a = -e32m10000*q + ((int64_t) val << 32 | q);
    // const int64_t b = a * e19d100 & 0x3f8000003f80000ll, b25 = b*25;
    // const int64_t c = (a<<17 | b>>18) - b25, d = c * e10d10 & 0x7800780078007800ll; 
    // return (c<<9 | d>>9) - d*5;
}

inline uint64_t itoa8lil2(const uint32_t val) {
    // const uint64_t q = val * e40d10000 >> 40, a = ((uint64_t) val << 32 | q) - q * e32m10000;
    // const uint64_t mask1 = 0x3f8000003f80000ull, mask2 = 0x7800780078007800ull;
    // const uint64_t b = a * e19d100 & mask1;
    // const uint64_t c = (a<<17 | b>>18) - b*25, d = c * e10d10 & mask2;
    // return (c<<9 | d>>9) - d* 5;  

    // const uint_fast32_t q = val * e40d10000 >> 40;
    // const uint_fast64_t a = ((uint64_t) val << 32 | q) - q * e32m10000;
    // const uint64_t b = a * e19d100 & 0x3f8000003f80000ull;
    // const uint64_t c = (a<<17 | b>>18) - b*25, d = c * e10d10 & 0x7800780078007800ull;
    // return (c<<9 | d>>9) - d* 5;

    // const int32_t q = (int64_t) val * e40d10000 >> 40;
    // const int64_t a = ((int64_t) val << 32 | q) - q * e32m10000;
    // const int64_t b = a * e19d100 & 0x3f8000003f80000ll;
    // const int64_t c = (a<<17 | b>>18) - b*25, d = c * e10d10 & 0x7800780078007800ll; 
    // return (c<<9 | d>>9) + ((ascii0s<<2) - d*5);

    const int32_t q = (int64_t) val * e40d10000 >> 40;
    const int64_t a = -e32m10000*q + ((int64_t) val << 32 | q);
    const int64_t b = a * e19d100 & 0x3f8000003f80000ll, b25 = b*25;
    const int64_t c = (a<<17 | b>>18) - b25, d = c * e10d10 & 0x7800780078007800ll; 
    return (c<<9 | d>>9) + ((ascii0s<<2) - d*5);
}


inline void itoa8lut(const uint32_t val, char * buf) {
    // const int32_t q = (int64_t) val * e40d10000 >> 40;
    // const int64_t a = ((int64_t) val << 32 | q) - q * e32m10000;
    // const int64_t b = a * e19d100 & 0x3f8000003f80000ll;
    // const int64_t c = (a<<17) - b*25; 
    // memcpy(buf, gDigitsLut + ((int32_t)b>>18), 2);
    // memcpy(buf+4, gDigitsLut + (b>>50), 2);
    // memcpy(buf+2, gDigitsLut + ((int32_t)c>>16), 2);
    // memcpy(buf+6, gDigitsLut + (c>>48), 2);

    // const int32_t q = (int64_t) val * e40d10000 >> 40;
    // const int64_t a = ((int64_t) val << 32 | q);
    // const int64_t b = a * e19d100 - q*(e32m10000*e19d100) & 0x3f8000003f80000ll;
    // const int64_t c = ((a-q*e32m10000)<<17) - b*25; 
    // memcpy(buf, gDigitsLut + ((int32_t)b>>18), 2);
    // memcpy(buf+4, gDigitsLut + (b>>50), 2);
    // memcpy(buf+2, gDigitsLut + ((int32_t)c>>16), 2);
    // memcpy(buf+6, gDigitsLut + (c>>48), 2);

    // const int32_t q = (int64_t) val * e40d10000 >> 40;
    // const int64_t a = ((int64_t) val << 32 | q) - q * e32m10000;
    // const int64_t b = a * e19d100 & 0x3f8000003f80000ll;
    // const int64_t c = (a<<17) - b*25; 
    // memcpy(buf, gDigitsLut + ((int32_t)b>>18), 2);
    // memcpy(buf+4, gDigitsLut + (b>>50), 2);
    // memcpy(buf+2, gDigitsLut + ((int32_t)c>>16), 2);
    // memcpy(buf+6, gDigitsLut + (c>>48), 2);

    // const int32_t q = (int64_t) val * e40d10000 >> 40;
    // const int64_t a = -e32m10000*q + ((int64_t) val << 32 | q);
    // const int64_t b = a * e19d100 & 0x3f8000003f80000ll;
    // const int64_t c = (a<<17) - b*25; 
    // memcpy(buf, gDigitsLut + ((int32_t)b>>18), 2);
    // memcpy(buf+4, gDigitsLut + (b>>50), 2);
    // memcpy(buf+2, gDigitsLut + ((int32_t)c>>16), 2);
    // memcpy(buf+6, gDigitsLut + (c>>48), 2);

    // const int32_t q = (int64_t) val * e40d10000 >> 40;
    // const int64_t a = -e32m10000*q + ((int64_t) val << 32 | q);
    // const int64_t b = a * e19d100 & 0x3f8000003f80000ll;
    // const int64_t c = (a<<17) - b*25;
    // memcpy(buf, gDigitsLut + ((int32_t)b>>18), 2);
    // memcpy(buf+4, gDigitsLut + (b>>50), 2);
    // memcpy(buf+2, gDigitsLut + ((int32_t)c>>16), 2);
    // memcpy(buf+6, gDigitsLut + (c>>48), 2);

    // const int32_t q = (int64_t) val * e40d10000 >> 40;
    // const int64_t a = -e32m10000*q + ((int64_t) val << 32 | q);
    // const int64_t b = a * e19d100 & 0x3f8000003f80000ll;
    // const int64_t b25 = b*25, c = (a<<17) - b25;
    // // memcpy(buf, gDigitsLut + ((int32_t)b>>18), 2);
    // // memcpy(buf+4, gDigitsLut + (b>>50), 2);
    // // memcpy(buf+2, gDigitsLut + ((int32_t)c>>16), 2);
    // // memcpy(buf+6, gDigitsLut + (c>>48), 2);
    // memcpy(buf, gDigitsLut + ((int32_t)b>>18), 2);
    // memcpy(buf+2, gDigitsLut + ((int32_t)c>>16), 2);
    // memcpy(buf+4, gDigitsLut + (b>>50), 2);
    // memcpy(buf+6, gDigitsLut + (c>>48), 2);

    const int32_t q = (int64_t) val * e40d10000 >> 40;
    const int64_t a = -e32m10000*q + ((int64_t) val << 32 | q);
    const int64_t b = a * e19d100 & 0x3f8000003f80000ll;
    const int64_t b25 = b*25, c = (a<<17) - b25;
    // memcpy(buf, gDigitsLut + ((int32_t)b>>18), 2);
    // memcpy(buf+4, gDigitsLut + (b>>50), 2);
    // memcpy(buf+2, gDigitsLut + ((int32_t)c>>16), 2);
    // memcpy(buf+6, gDigitsLut + (c>>48), 2);
    memcpy(buf, gDigitsLut + ((int32_t)b>>18), 2);
    memcpy(buf+2, gDigitsLut + ((int32_t)c>>16), 2);
    memcpy(buf+4, gDigitsLut + (b>>50), 2);
    memcpy(buf+6, gDigitsLut + (c>>48), 2);
}


inline int64_t itoa8lut2(const uint32_t val) {
    const int32_t q = (int64_t) val * e40d10000 >> 40;
    const int64_t a = ((int64_t) val << 32 | q) - q * e32m10000;
    const int64_t b = a * e19d100 & 0x3f8000003f80000ll;
    const int64_t c = (a<<17) - b*25;
    int64_t _0, _2, _4, _6;
    memcpy(&_0, gDigitsLut + ((int32_t)b>>18), 2);
    memcpy(&_4, gDigitsLut + (b>>50), 2);
    memcpy(&_2, gDigitsLut + ((int32_t)c>>16), 2);
    memcpy(&_6, gDigitsLut + (c>>48), 2);
    return (_0 | _2<<16) | (_4<<32 | _6<<48);

    // const int32_t q = (int64_t) val * e40d10000 >> 40;
    // const int64_t a = ((int64_t) val << 32 | q) - q * e32m10000;
    // const int64_t b = a * e19d100 & 0x3f8000003f80000ll;
    // const int64_t c = (a<<17) - b*25;
    // int64_t ret;
    // memcpy(&((char&)ret) , gDigitsLut + ((int32_t)b>>18), 2);
    // memcpy(&((char&)ret) +4, gDigitsLut + (b>>50), 2);
    // memcpy(&((char&)ret) +2, gDigitsLut + ((int32_t)c>>16), 2);
    // memcpy(&((char&)ret) +6, gDigitsLut + (c>>48), 2);
    // return ret;
}


inline int64_t itoa8blut(const uint32_t val) {
    const int32_t q = (int64_t) val * e40d10000 >> 40;
    const int64_t a = -e32m10000*q + ((int64_t) val << 32 | q);
    const int64_t b = a * e19d100 & 0x3f8000003f80000ll;
    const int64_t b25 = b*25, c = (a<<17) - b25;
    int64_t _0, _2, _4, _6;
    memcpy(&_6, decLut + ((int32_t)b>>18), 2);
    memcpy(&_2, decLut + (b>>50), 2);
    memcpy(&_4, decLut + ((int32_t)c>>16), 2);
    memcpy(&_0, decLut + (c>>48), 2);
    return (_6<<48 | _2<<16) | (_4<<32 | _0);

    // const int32_t q = (int64_t) val * e40d10000 >> 40;
    // const int64_t a = ((int64_t) val << 32 | q) - q * e32m10000;
    // const int64_t b = a * e19d100 & 0x3f8000003f80000ll;
    // const int64_t c = (a<<17) - b*25;
    // int64_t ret;
    // memcpy(&((char&)ret) , gDigitsLut + ((int32_t)b>>18), 2);
    // memcpy(&((char&)ret) +4, gDigitsLut + (b>>50), 2);
    // memcpy(&((char&)ret) +2, gDigitsLut + ((int32_t)c>>16), 2);
    // memcpy(&((char&)ret) +6, gDigitsLut + (c>>48), 2);
    // return ret;
}


static uint32_t E = 0;
// static uint16_t last = '0';
// static char * lastPointer;
// static uint_fast64_t last_a = (char)ascii0s;
// static uint_fast8_t shift = 56;
// static uint_fast32_t power10 = 10;
static uint16_t hi2a = 0, off = 0;
static uint64_t lo8a = 0x2f;
static uint64_t step = 1, limit = 0x3a;
static int shifts = 56;


inline int64_t itoa8lil(const uint32_t val, const int64_t& ne32m10000) {
    const int32_t q = (int64_t) val * e40d10000 >> 40;
    const int64_t a = ((int64_t) val << 32 | q) + q * ne32m10000;
    const int64_t b = a * e19d100 & 0x3f8000003f80000ll;
    const int64_t c = (a<<17 | b>>18) - b*25, d = c * e10d10 & 0x7800780078007800ll; 
    return (c<<9 | d>>9) - d*5;
}

inline void itoa8lut2(const uint32_t val, char * buf, const int64_t& ne32m10000) {
    const int32_t q = (int64_t) val * e40d10000 >> 40;
    const int64_t a = ne32m10000*q + ((int64_t) val << 32 | q);
    const int64_t b = a * e19d100 & 0x3f8000003f80000ll;
    const int64_t b25 = b*25, c = (a<<17) - b25;
    memcpy(buf, gDigitsLut + ((int32_t)b>>18), 2);
    memcpy(buf+4, gDigitsLut + (b>>50), 2);
    memcpy(buf+2, gDigitsLut + ((int32_t)c>>16), 2);
    memcpy(buf+6, gDigitsLut + (c>>48), 2);
}


inline void u32toa_hintro(const uint32_t val, char* buffer) {
    // uint64_t lo8a;
    // uint32_t hi2a, hi2aSize = 2;
    // if (val >= 100000000) {
    //     const uint32_t hi2 = val * e57d8 >> 57, lo8 = val - hi2 * 100000000;
    //     lo8a = itoa8lil(lo8) >> 2 | ascii0s;
    //     const uint_fast8_t hi1d = hi2*e9d10 >> 9, shift = hi1d ? 0 : 8;
    //     // hi2aSize -= hi1d ? 0 : 1;
    //     hi2aSize -= !hi1d;
    //     hi2a = (((hi2 << 8) + (uint_fast16_t) ascii0s | hi1d)>>shift) - (hi2*e9d10 & 0xe00) * 5;
    //     memcpy(buffer, &hi2a, sizeof hi2a);
    //     // hi2aSize -= hi2 < 10;
    // } else {
    //     const uint64_t decimals = itoa8lil(val), tmp = (decimals >> 2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
    //     lo8a = decimals ? tmp : (uint_fast8_t) ascii0s;
    //     hi2aSize = 0;
    // }
    // memcpy(buffer+=hi2aSize, &lo8a, 8);
    // buffer[8] = '\0';


    // uint64_t lo8a;
    // uint32_t hi2a, hi2aSize = 2;
    // if (val >= 100000000) {
    //     const uint32_t hi2 = val * e57d8 >> 57, lo8 = val - hi2 * 100000000;
    //     lo8a = itoa8lil(lo8) >> 2 | ascii0s;
    //     const uint_fast8_t hi1d = hi2*e9d10 >> 9, shift = hi1d ? 0 : 8;
    //     hi2aSize -= hi1d ? 0 : 1;
    //     hi2a = (((hi2 << 8) + (uint_fast16_t) ascii0s | hi1d)>>shift) - (hi2*e9d10 & 0xe00) * 5;
    //     memcpy(buffer, &hi2a, 2);
    //     buffer += hi1d ? 2 : 1;
    //     // hi2aSize -= hi2 < 10;
    // } else {
    //     const uint64_t decimals = itoa8lil(val), tmp = (decimals >> 2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
    //     lo8a = decimals ? tmp : (uint_fast8_t) ascii0s;
    //     // hi2aSize = 0;
    // }
    // memcpy(buffer, &lo8a, 8);
    // buffer[8] = '\0';


    // uint64_t lo8a;
    // uint32_t hi2a;
    // if (val >= 100000000) {
    //     const uint32_t hi2 = val * e57d8 >> 57, lo8 = val - hi2 * 100000000;
    //     lo8a = itoa8lil(lo8) >> 2 | ascii0s;
    //     const uint_fast8_t hi1d = hi2*e9d10 >> 9, shift = hi1d ? 0 : 8;
    //     // hi2aSize -= hi1d ? 0 : 1;
    //     hi2a = (((hi2 << 8) + (uint_fast16_t) ascii0s | hi1d)>>shift) - (hi2*e9d10 & 0xe00) * 5;
    //     memcpy(buffer, &hi2a, sizeof hi2a);
    //     buffer += hi1d ? 2 : 1;
    //     // hi2aSize -= hi2 < 10;
    // } else {
    //     const uint64_t decimals = itoa8lil(val), tmp = (decimals >> 2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
    //     lo8a = decimals ? tmp : (uint_fast8_t) ascii0s;
    //     // hi2aSize = 0;
    // }
    // memcpy(buffer, &lo8a, 8);
    // buffer[8] = '\0';

    // uint64_t lo8a;
    // uint32_t hi2a, hi2aSize = 2;
    // if (val >= 100000000) {
    //     const uint32_t hi2 = val * e57d8 >> 57, lo8 = val - hi2 * 100000000;
    //     lo8a = itoa8lil(lo8) >> 2 | ascii0s;
    //     const uint32_t hi1d = hi2*e9d10 >> 9, shift = hi1d ? 0 : 8;
    //     hi2aSize -= !hi1d;
    //     hi2a = (((hi2 << 8) + (uint_fast16_t) ascii0s | hi1d)>>shift) - (hi2*e9d10 & 0xe00) * 5;
    //     memcpy(buffer, &hi2a, sizeof hi2a);
    // } else {
    //     const uint64_t decimals = itoa8lil(val), tmp = (decimals >> 2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
    //     lo8a = decimals ? tmp : (uint_fast8_t) ascii0s;
    //     hi2aSize = 0;
    // }
    // memcpy(buffer+=hi2aSize, &lo8a, 8);
    // buffer[8] = '\0';

    // int64_t lo8a;
    // int32_t hi2a, hi2aSize = 2;
    // if (val >= 100000000) {
    //     const uint32_t hi2 = val * e57d8 >> 57;
    //     const int32_t lo8 = val - hi2 * 100000000;
    //     lo8a = itoa8lil(lo8) >> 2 | ascii0s;
    //     const int32_t hi1d = hi2*e9d10 >> 9, shift = hi1d ? 0 : 8;
    //     hi2aSize -= !hi1d;
    //     hi2a = (((hi2 << 8) + (uint32_t) ascii0s | hi1d)>>shift) - (hi2*e9d10 & 0xe00) * 5;
    //     memcpy(buffer, &hi2a, sizeof hi2a);
    // } else {
    //     const int64_t decimals = itoa8lil(val), tmp = (decimals >> 2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
    //     lo8a = decimals ? tmp : (uint8_t) ascii0s;
    //     hi2aSize = 0;
    // }
    // memcpy(buffer+=hi2aSize, &lo8a, 8);
    // buffer[8] = '\0';

    // int64_t lo8a;
    // int32_t hi2aSize = 2;
    // if (val >= 100000000) {
    //     const uint32_t hi2 = val * e57d8 >> 57;
    //     const int32_t lo8 = val - hi2 * 100000000;
    //     lo8a = itoa8lil(lo8) >> 2 | ascii0s;
    //     memcpy(buffer, dLut+hi2, 2);
    //     hi2aSize -= hi2<10;
    // } else {
    //     const int64_t decimals = itoa8lil(val), tmp = (decimals >> 2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
    //     lo8a = decimals ? tmp : (uint8_t) ascii0s;
    //     hi2aSize = 0;
    // }
    // memcpy(buffer+=hi2aSize, &lo8a, 8);
    // buffer[8] = '\0';


    // int64_t lo8a;
    // if (val >= 100000000) {
    //     const uint32_t hi2 = val * e57d8 >> 57;
    //     memcpy(buffer, dLut+hi2, 2);
    //     buffer = hi2>=10 ? buffer+2 : buffer+1;
    //     const int32_t lo8 = val - hi2 * 100000000;
    //     lo8a = itoa8lil(lo8) >> 2 | ascii0s;
    //     // memcpy(buffer, dLut+hi2, 2); buffer = hi2>=10 ? buffer+2 : buffer+1;
    // } else {
    //     const int64_t decimals = itoa8lil(val), tmp = (decimals >> 2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
    //     lo8a = decimals ? tmp : (uint8_t) ascii0s;
    // }
    // buffer[8] = '\0';
    // memcpy(buffer, &lo8a, 8);

    // int64_t lo8a = (int_fast8_t) ascii0s;
    // if (val >= 100000000) {
    //     const uint32_t hi2 = val * e57d8 >> 57;
    //     memcpy(buffer, dLut+hi2, 2);
    //     buffer = hi2>=10 ? buffer+2 : buffer+1;
    //     const int32_t lo8 = val - hi2 * 100000000;
    //     lo8a = itoa8lil(lo8) >> 2 | ascii0s;
    //     // memcpy(buffer, dLut+hi2, 2); buffer = hi2>=10 ? buffer+2 : buffer+1;
    // } else if (val) {
    //     const int64_t decimals = itoa8lil(val);
    //     lo8a = (decimals >> 2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
    // }
    // buffer[8] = '\0';
    // memcpy(buffer, &lo8a, 8);

    // int64_t lo8a = (int_fast8_t) ascii0s;
    // if (val >= 100000000) {
    //     const int32_t hi2 = val/100000000, lo8 = val%100000000;
    //     memcpy(buffer, dLut+hi2, 2);
    //     buffer = hi2>=10 ? buffer+2 : buffer+1;
    //     lo8a = itoa8lil(lo8) >> 2 | ascii0s;
    //     // memcpy(buffer, dLut+hi2, 2); buffer = hi2>=10 ? buffer+2 : buffer+1;
    // } else if (val) {
    //     const int64_t decimals = itoa8lil(val);
    //     lo8a = (decimals >> 2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
    // }
    // buffer[8] = '\0';
    // memcpy(buffer, &lo8a, 8);

    // int64_t lo8a = (int_fast8_t) ascii0s;
    // if (val >= 100000000) {
    //     const int32_t hi2 = val/100000000;
    //     memcpy(buffer, dLut+hi2, 2);
    //     const int32_t lo8 = val%100000000;
    //     buffer = hi2>=10 ? buffer+2 : buffer+1;
    //     lo8a = itoa8lil(lo8) >> 2 | ascii0s;
    // } else if (val) {
    //     const int64_t decimals = itoa8lil(val);
    //     lo8a = (decimals >> 2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
    // }
    // buffer[8] = '\0';
    // memcpy(buffer, &lo8a, 8);

    // int64_t lo8a = 0x30;  // ASCII 0
    // if (val >= 100000000) {
    //     const int32_t hi2 = val/100000000;
    //     memcpy(buffer, dLut+hi2, 2);
    //     const int32_t lo8 = val%100000000;
    //     buffer = hi2>=10 ? buffer+2 : buffer+1;
    //     lo8a = itoa8lil(lo8) >> 2 | ascii0s;
    // } else if (val) {
    //     const int64_t decimals = itoa8lil(val);
    //     lo8a = (decimals >> 2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
    // }
    // buffer[8] = 0;
    // memcpy(buffer, &lo8a, 8);

    // int64_t lo8a = 0x30;  // ASCII 0
    // if (val >= 100000000) {
    //     const int32_t hi2 = val/100000000;
    //     memcpy(buffer, dLut+hi2, 2);
    //     const int32_t lo8 = val%100000000;
    //     buffer += val>=1000000000 ? 2 : 1;
    //     lo8a = itoa8lil(lo8) >> 2 | ascii0s;
    // } else if (val) {
    //     // const int64_t decimals = itoa8lil(val);
    //     const int32_t lzs = __builtin_clz(val)>>1, power = powersLut[lzs];
    //     const int64_t decimals = itoa8lil(val);
    //     const int32_t guess = shiftLut >> (lzs<<2), shifts = guess + (val<power) << 3;
    //     lo8a = (decimals >> 2 | ascii0s) >> shifts;
    // }
    // buffer[8] = 0;
    // memcpy(buffer, &lo8a, 8);


    // int64_t lo8a = 0x30;  // ASCII 0
    // if (val >= 100000000) {
    //     const int32_t hi2 = val/100000000;
    //     memcpy(buffer, dLut+hi2, 2);
    //     const int32_t lo8 = val%100000000;
    //     buffer += val>=1000000000 ? 2 : 1;
    //     lo8a = itoa8lil(lo8)>>2 | ascii0s;
    // } else if (val) {
    //     const int64_t decimals = itoa8lil(val);
    //     lo8a = (decimals>>2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
    // }
    // buffer[8] = 0;
    // memcpy(buffer, &lo8a, 8);

    
    // int64_t lo8a = 0x30;  // ASCII 0
    // if (val >= 100000000) {
    //     const int32_t hi2 = val/100000000;
    //     uint16_t hi2a = ascii0s;
    //     const int32_t tenth = val/1000000000;
    //     memcpy(buffer, dLut+hi2, 2);
    //     const int32_t lo8 = val%100000000;
    //     buffer += val>=1000000000 ? 2 : 1;
    //     lo8a = itoa8lil(lo8)>>2 | ascii0s;
    // } else if (val) {
    //     const int64_t decimals = itoa8lil(val);
    //     lo8a = (decimals>>2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
    // }
    // buffer[8] = 0;
    // memcpy(buffer, &lo8a, 8);


    // int64_t lo8a = 0x30;  // ASCII 0
    // if (val >= 100000000) {
    //     const int32_t hi2 = val/100000000;
    //     memcpy(buffer, dLut+hi2, 2);
    //     const int32_t lo8 = val%100000000;
    //     buffer += val>=1000000000 ? 2 : 1;
    //     lo8a = itoa8lil(lo8)>>2 | ascii0s;
    // } else if (val) {
    //     const int64_t decimals = itoa8lil(val);
    //     lo8a = (decimals>>2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
    // }
    // buffer[8] = 0;
    // memcpy(buffer, &lo8a, 8);


    // BIG
    // ENDIAN
    // int64_t lo8a = 0x3000000000000000ll;  // ASCII 0
    // if (val >= 100000000) {
    //     const int32_t hi2 = val/100000000;
    //     memcpy(buffer, dLut+hi2, 2);
    //     const int32_t lo8 = val%100000000;
    //     buffer += val>=1000000000 ? 2 : 1;
    //     lo8a = itoa8big(lo8) | ascii0s;
    // } else if (val) {
    //     lo8a = itoa8big(val);
    //     lo8a = (lo8a|ascii0s) << (56 & __builtin_clzll(lo8a));
    // }
    // buffer[8] = 0;
    // lo8a = __builtin_bswap64(lo8a);
    // memcpy(buffer, &lo8a, 8);


    // if ((val == E) & (lo8a<limit)) {
    //     memcpy(buffer, &hi2a, 2);
    //     buffer += off;
    //     lo8a += step;
    //     buffer[8] = 0;
    //     memcpy(buffer, &lo8a, 8);
    // }
    // else {
    //     lo8a = 0x30;  // ASCII 0
    //     hi2a = 0;
    //     shifts = 56;
    //     off = (val>=100000000);
    //     if (val >= 100000000) {
    //         const int32_t hi2 = val/100000000;
    //         hi2a = dLut[hi2];
    //         memcpy(buffer, &hi2a, 2);
    //         const int32_t lo8 = val%100000000;
    //         off += val >= 1000000000;
    //         buffer += off;
    //         lo8a = itoa8lil(lo8)>>2 | ascii0s;
    //         shifts = 0;
    //     } else if (val) {
    //         const int64_t decimals = itoa8lil(val);
    //         shifts = 56 & __builtin_ctzll(decimals);
    //         lo8a = (decimals>>2 | ascii0s) >> shifts;
    //     }
    //     buffer[8] = 0;
    //     memcpy(buffer, &lo8a, 8);
    //     step = 0x100000000000000ll >> shifts;
    //     limit = 0x3900000000000000ll >> shifts;
    // }
    // if(val+1) E = val+1;

    // if ((val == E) & (lo8a<limit)) {
    //     memcpy(buffer, &hi2a, 2);
    //     buffer += off;
    //     buffer[8] = 0;
    //     lo8a += step;
    //     // buffer[8] = 0; // memcpy(buffer, &lo8a, 8);
    // }
    // else {
    //     lo8a = 0x30;  // ASCII 0
    //     hi2a = 0;
    //     shifts = 56;
    //     off = (val>=100000000);
    //     if (val >= 100000000) {
    //         const int32_t hi2 = val/100000000;
    //         hi2a = dLut[hi2];
    //         memcpy(buffer, &hi2a, 2);
    //         const int32_t lo8 = val%100000000;
    //         off += val >= 1000000000;
    //         buffer += off;
    //         lo8a = itoa8lil(lo8)>>2 | ascii0s;
    //         shifts = 0;
    //     } else if (val) {
    //         const int64_t decimals = itoa8lil(val);
    //         shifts = 56 & __builtin_ctzll(decimals);
    //         lo8a = (decimals>>2 | ascii0s) >> shifts;
    //     }
    //     buffer[8] = 0; // memcpy(buffer, &lo8a, 8);
    //     step = 0x100000000000000ll >> shifts;
    //     limit = 0x3900000000000000ll >> shifts;
    // }
    // memcpy(buffer, &lo8a, 8);
    // if(val+1) E = val+1;

/**
    const int32_t q = (int64_t) val * e40d10000 >> 40;
    const int64_t a = ((int64_t) val << 32 | q) - q * e32m10000;
    const int64_t b = a * e19d100 & 0x3f8000003f80000ll;
    const int64_t c = (a<<17) - b*25; 
    memcpy(buf, gDigitsLut + ((int32_t)b>>18), 2);
    memcpy(buf+4, gDigitsLut + (b>>50), 2);
    memcpy(buf+2, gDigitsLut + ((int32_t)c>>16), 2);
    memcpy(buf+6, gDigitsLut + (c>>48), 2);
**/

    // int64_t lo8a = 0x30;  // ASCII 0
    // if (val >= 100000000) {
    //     const int32_t hi2 = val/100000000;
    //     memcpy(buffer, dLut+hi2, 2);
    //     const int32_t lo8 = val%100000000;
    //     buffer += val>=1000000000 ? 2 : 1;
    //     buffer[8] = 0;
    //     itoa8lut(lo8, buffer);
    // } else {
    //     buffer[8] = 0;
    //     if (val) {
    //         const int64_t decimals = itoa8lil(val);
    //         lo8a = (decimals>>2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
    //     }
    //     memcpy(buffer, &lo8a, 8);
    // }

    // buffer[8] = 0;

    // lo8a = 0x30;  // ASCII 0
    // memset(buffer+7, 0, 4);
    // if (val >= 100000000) {
    //     const int32_t hi2 = val/100000000;
    //     memcpy(buffer, dLut+hi2, 2);
    //     const int32_t lo8 = val%100000000;
    //     buffer += val>=1000000000 ? 2 : 1;
    //     itoa8lut(lo8, buffer);
    // } else {
    //     if (val) {
    //         const int64_t decimals = itoa8lil(val);
    //         lo8a = (decimals>>2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
    //     }
    //     memcpy(buffer, &lo8a, 8);
    // }


    // lo8a = 0x30;  // ASCII 0
    // memset(buffer+7, 0, 4);
    // if (val >= 100000000) {
    //     const int32_t hi2 = val/100000000;
    //     const char * lut = gDigitsLut;
    //     if (val<1000000000) lut += 200;
    //     memcpy(buffer, lut+hi2*2, 2);
    //     const int32_t lo8 = val%100000000;
    //     buffer += val>=1000000000 ? 2 : 1;
    //     itoa8lut(lo8, buffer);
    // } else {
    //     if (val) {
    //         const int64_t decimals = itoa8lil(val);
    //         lo8a = (decimals>>2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
    //     }
    //     memcpy(buffer, &lo8a, 8);
    // }

/**
    const int32_t q = (int64_t) val * e40d10000 >> 40;
    const int64_t a = ((int64_t) val << 32 | q) - q * e32m10000;
    const int64_t b = a * e19d100 & 0x3f8000003f80000ll;
    const int64_t c = (a<<17 | b>>18) - b*25, d = c * e10d10 & 0x7800780078007800ll; 
    return (c<<9 | d>>9) - d*5;
    **/

    // lo8a = 0x30;  // ASCII 0
    // memset(buffer+7, 0, 4);
    // if (val >= 100000000) {
    //     const int32_t hi2 = val/100000000;
    //     memcpy(buffer++, dLut+hi2, 2);
    //     const int32_t lo8 = val%100000000;
    //     itoa8lut(lo8, buffer+(val>=1000000000));
    // } else {
    //     if (val) {
    //         const int32_t q = (int64_t) val * e40d10000 >> 40;
    //         int32_t x = 100, y = 1000, shifts = 7;
    //         const int64_t a = ((int64_t) val << 32 | q) - q * e32m10000;
    //         shifts -= val>=10;
    //         const int64_t b = a * e19d100 & 0x3f8000003f80000ll;
    //         if(val>=10000) {x = 100000; y = 1000000; shifts-=4;}
    //         const int64_t c = (a<<17 | b>>18) - b*25, d = c * e10d10 & 0x7800780078007800ll;
    //         shifts -= (val>=x) + (val>=y);
    //         const uint64_t decimals = (c<<9 | d>>9) + ((ascii0s<<2)-d*5);
    //         shifts = shifts*8 + 2;
    //         lo8a = decimals >> shifts;
    //     }
    //     memcpy(buffer, &lo8a, 8);
    // }

    // lo8a = 0x30;  // ASCII 0
    // memset(buffer+7, 0, 4);
    // if (val >= 100000000) {
    //     const int32_t hi2 = val/100000000;
    //     memcpy(buffer, dLut+hi2, 2);
    //     const int32_t lo8 = val%100000000;
    //     buffer += val>=1000000000 ? 2 : 1;
    //     itoa8lut(lo8, buffer);
    // } else {
    //     if (val) {
    //         const int64_t decimals = itoa8lil(val);
    //         lo8a = (decimals>>2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
    //     }
    //     memcpy(buffer, &lo8a, 8);
    // }


    // FAST
    // lo8a = 0x30;  // ASCII 0
    // memset(buffer+7, 0, 4);
    // if (val >= 100000000) {
    //     const int32_t hi2 = val/100000000;
    //     memcpy(buffer, dLut+hi2, 2);
    //     const int32_t lo8 = val%100000000;
    //     buffer += val>=1000000000 ? 2 : 1;
    //     itoa8lut(lo8, buffer);
    // } else {
    //     if (val) {
    //         const int64_t decimals = itoa8lil(val);
    //         lo8a = (decimals>>2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
    //     }
    //     memcpy(buffer, &lo8a, 8);
    // }


    // lo8a = 0x30;  // ASCII 0
    // memset(buffer+7, 0, 4);
    // if (val >= 100000000) {
    //     const int32_t hi2 = val/100000000;
    //     memcpy(buffer, dLut+hi2, 2);
    //     const int32_t lo8 = val%100000000;
    //     buffer += val>=1000000000 ? 2 : 1;
    //     itoa8lut(lo8, buffer);
    // } else {
    //     if (val) {
    //         const int64_t decimals = itoa8blut(val);
    //         lo8a = __builtin_bswap64(decimals | ascii0s) >> (56 & __builtin_clzll(decimals));
    //     }
    //     memcpy(buffer, &lo8a, 8);
    // }

    // lo8a = 0x30;  // ASCII 0
    // memset(buffer+7, 0, 4);
    // int64_t E57d8 = e57d8, E40d10000 = e40d10000;
    // if (val >= 100000000) {
    //     const int32_t hi2 = val*E57d8 >> 57;
    //     memcpy(buffer, dLut+hi2, 2);
    //     const int32_t lo8 = val - hi2*100000000;
    //     buffer += val>=1000000000 ? 2 : 1;
    //     itoa8lut(lo8, buffer);
    // } else {
    //     if (val) {
    //         // const int64_t decimals = itoa8lil(val);
    //         const int32_t q = (int64_t) val * E40d10000 >> 40;
    //         const int64_t a = ((int64_t) val << 32 | q) - q * e32m10000;
    //         const int64_t b = a * e19d100 & 0x3f8000003f80000ll;
    //         const int64_t c = (a<<17 | b>>18) - b*25, d = c * e10d10 & 0x7800780078007800ll; 
    //         const int64_t decimals = (c<<9 | d>>9) - d*5;
    //         lo8a = (decimals>>2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
    //     }
    //     memcpy(buffer, &lo8a, 8);
    // }


    // lo8a = 0x30;  // ASCII 0
    // // volatile int64_t ne32m10000 = -e32m10000;
    // // __asm__ volatile ("sal $32 %%rdx;" :"=a"(ne32m10000));
    // // asm volatile ("movabs rdx -42949672960000;" :"=a"(ne32m10000));
    // // __asm__ volatile ("movq %%rdx -42949672960000;" :"=a"(ne32m10000));
    // int64_t ne32m10000;
    // asm volatile ("movabs $-42949672960000LL, %%rcx;":"=a"(ne32m10000));
    // memset(buffer+7, 0, 4);
    // if (val >= 100000000) {
    //     const int hi2 = val/100000000;
    //     memcpy(buffer, dLut+hi2, 2);
    //     const int32_t lo8 = val%100000000;
    //     buffer += val>=1000000000 ? 2 : 1;
    //     itoa8lut2(lo8, buffer, ne32m10000);
    // } else {
    //     if (val) {
    //         const int64_t decimals = itoa8lil(val, ne32m10000);
    //         lo8a = (decimals>>2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
    //     }
    //     memcpy(buffer, &lo8a, 8);
    // }

    lo8a = 0x30;  // ASCII 0
    memset(buffer+7, 0, 4);
    if (val >= 100000000) {
        const int32_t hi2 = val/100000000;
        memcpy(buffer, dLut+hi2, 2);
        const int32_t lo8 = val%100000000;
        buffer += val>=1000000000 ? 2 : 1;
        itoa8lut(lo8, buffer);
    } else {
        if (val) {
            const int64_t decimals = itoa8lil(val);
            lo8a = (decimals>>2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
        }
        memcpy(buffer, &lo8a, 8);
    }


    // lo8a = 0x30;  // ASCII 0
    // memset(buffer+7, 0, 4);
    // if (val >= 100000000) {
    //     const int32_t hi2 = val/100000000;
    //     memcpy(buffer, dLut+hi2, 2);
    //     const int32_t lo8 = val%100000000;
    //     buffer += val>=1000000000 ? 2 : 1;
    //     itoa8lut(lo8, buffer);
    // } else {
    //     if (val) {
    //         // // const int64_t decimals = itoa8lil2(val);
    //         // const int32_t q = (int64_t) val * e40d10000 >> 40;
    //         // const int64_t a = -e32m10000*q + ((int64_t) val << 32 | q);
    //         // const int64_t b = a * e19d100 & 0x3f8000003f80000ll, b25 = b*25;
    //         // const int64_t c = (a<<17 | b>>18) - b25, d = c * e10d10 & 0x7800780078007800ll; 
    //         // // lo8a = (c<<9 | d>>9) + ((ascii0s<<2) - d*5);
    //         // // lo8a >>= 2 | (56 & __builtin_ctzll(c<<9 | d>>9));
    //         // lo8a = (c<<7 | d>>11);
    //         // lo8a = (lo8a + ascii0s - (d*5>>2)) >> (56 & __builtin_ctzll(lo8a));
            
    //         const int32_t q = (int64_t) val * e40d10000 >> 40;
    //         const int64_t a = ((int64_t) val << 32 | q) - q * e32m10000;
    //         const int64_t b = a * e19d100 & 0x3f8000003f80000ll;
    //         const int64_t c = (a<<17 | b>>18) - b*25, d = c * e10d10 & 0x7800780078007800ll; 
    //         lo8a = (c<<9 | d>>9);
    //         lo8a = (lo8a - d*5 >> 2 | ascii0s) >> (__builtin_ctzll(lo8a) & 56);
    //     }
    //     memcpy(buffer, &lo8a, 8);
    // }


    // lo8a = 0x30;  // ASCII 0
    // memset(buffer+7, 0, 4);
    // if (val >= 100000000) {
    //     const int32_t hi2 = val/100000000;
    //     memcpy(buffer, dLut+hi2, 2);
    //     const int32_t lo8 = val%100000000;
    //     buffer += val>=1000000000 ? 2 : 1;
    //     itoa8lut(lo8, buffer);
    // } else {
    //     if (val) {
    //         // const int64_t decimals = itoa8lil2(val);
    //         const int32_t q = (int64_t) val * e40d10000 >> 40;
    //         const int64_t a = -e32m10000*q + ((int64_t) val << 32 | q);
    //         const int64_t b = a * e19d100 & 0x3f8000003f80000ll, b25 = b*25;
    //         const int64_t c = (a<<17 | b>>18) - b25, d = c * e10d10 >> 9; 
    //         // lo8a = (c<<9 | d>>9) + ((ascii0s<<2) - d*5);
    //         // lo8a >>= 2 | (56 & __builtin_ctzll(c<<9 | d>>9));
    //         lo8a = (c<<9 | d&0x3c003c003c003cll);
    //         lo8a = (lo8a>>2 | ascii0s) >> (56 & __builtin_ctzll(lo8a));
    //     }
    //     memcpy(buffer, &lo8a, 8);
    // }
}



inline void i32toa_hintro(int32_t value, char* buffer) {
    // uint32_t u = static_cast<uint32_t>(value);
    // if (value < 0) {
    //     *buffer++ = '-';
    //     u = ~u + 1;
    // }
    // u32toa_naive(u, buffer);

    // uint32_t u = static_cast<uint32_t>(value);
    // if (value < 0) {
    //     *buffer = '-';
    //     u = ~u + 1;
    // }
    // u32toa_naive(u, buffer+(value<0));

    // const uint_fast64_t val = value < 0 ? ~value + 1 : value;
    // const uint32_t negMask = value < 0 ? -1 : 0, negShift = 8 & negMask, negSign = '-' & negMask;
    // uint64_t lo8a;
    // uint32_t hi2a = 0, hi2aSize = 2 - negMask;
    // const uint64_t ascii0s = 0x3030303030303030ull;
    // if (val >= 100000000) {
    //     const uint32_t e9d10 = 52, hi2 = val * e57d8 >> 57, lo8 = val - hi2 * 100000000;
    //     lo8a = itoa8lil(lo8) >> 2 | ascii0s;
    //     const uint32_t hi1d = hi2*e9d10 >> 9, adjustSize = hi1d ? 0 : 1, shift = hi1d ? 0 : 8;
    //     hi2a = (((hi2 << 8) + (uint16_t) ascii0s | hi1d)>>shift) - (hi2*e9d10 & 0xe00) * 5;
    //     hi2aSize -= adjustSize;
    // } else {
    //     const uint64_t decimals = itoa8lil(val), tmp = (decimals >> 2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
    //     lo8a = decimals ? tmp : (char) ascii0s;
    //     hi2aSize -= 2;
    // }
    // hi2a = hi2a << negShift | negSign;
    // memcpy(buffer, &hi2a, sizeof hi2a);
    // memcpy(buffer+hi2aSize, &lo8a, sizeof lo8a);
    // buffer[hi2aSize+8] = '\0';

    // uint_fast64_t lo8a;
    // uint_fast32_t hi2a = 0;
    // uint_fast8_t hi2aSize = 2;
    // const uint_fast32_t val = value < 0 ? ~value + 1 : value;
    // const uint_fast8_t negMask = value < 0 ? -1 : 0, negShift = 8 & negMask, negSign = '-' & negMask;
    // const uint64_t ascii0s = 0x3030303030303030ull;
    // if (val >= 100000000) {
    //     const uint_fast32_t e9d10 = 52, hi2 = val * e57d8 >> 57, lo8 = val - hi2 * 100000000;
    //     lo8a = itoa8lil(lo8) >> 2 | ascii0s;
    //     const uint_fast8_t hi1d = hi2*e9d10 >> 9, adjustSize = hi1d ? 0 : 1, shift = hi1d ? 0 : 8;
    //     hi2a = (((hi2 << 8) + (uint_fast16_t) ascii0s | hi1d)>>shift) - (hi2*e9d10 & 0xe00) * 5;
    //     hi2aSize = hi2aSize - negMask - adjustSize;
    // } else {
    //     const uint64_t decimals = itoa8lil(val), tmp = (decimals >> 2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
    //     lo8a = decimals ? tmp : (uint_fast8_t) ascii0s;
    //     hi2aSize = -negMask;
    // }
    // hi2a = hi2a << negShift | negSign;
    // memcpy(buffer, &hi2a, sizeof hi2a);
    // memcpy(buffer+hi2aSize, &lo8a, sizeof lo8a);
    // buffer[hi2aSize+8] = '\0';

    // uint_fast64_t lo8a;
    // uint_fast32_t hi2a = 0;
    // uint_fast8_t hi2aSize = 2;
    // const uint_fast32_t val = value < 0 ? ~value + 1 : value;
    // const uint_fast32_t negMask = value < 0 ? -1 : 0, negShift = 8 & negMask, negSign = '-' & negMask;
    // const uint64_t ascii0s = 0x3030303030303030ull;
    // if (val >= 100000000) {
    //     const uint_fast32_t e9d10 = 52, hi2 = val * e57d8 >> 57, lo8 = val - hi2 * 100000000;
    //     lo8a = itoa8lil(lo8) >> 2 | ascii0s;
    //     const uint_fast8_t hi1d = hi2*e9d10 >> 9, adjustSize = hi1d ? 0 : 1, shift = hi1d ? 0 : 8;
    //     hi2a = (((hi2 << 8) + (uint_fast16_t) ascii0s | hi1d)>>shift) - (hi2*e9d10 & 0xe00) * 5;
    //     hi2aSize = hi2aSize - negMask - adjustSize;
    // } else {
    //     const uint64_t decimals = itoa8lil(val), tmp = (decimals >> 2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
    //     lo8a = decimals ? tmp : (uint_fast8_t) ascii0s;
    //     hi2aSize = -negMask;
    // }
    // hi2a = hi2a << negShift | negSign;
    // memcpy(buffer, &hi2a, sizeof hi2a);
    // memcpy(buffer+hi2aSize, &lo8a, sizeof lo8a);
    // buffer[hi2aSize+8] = '\0';

    // uint_fast64_t lo8a;
    // uint_fast32_t hi2a = 0;
    // uint_fast8_t hi2aSize = 2;
    // const uint_fast32_t val = value < 0 ? ~value + 1 : value;
    // const uint_fast32_t negShift = value < 0 ? 8 : 0, negSign = value < 0 ? '-' : 0;
    // const uint64_t ascii0s = 0x3030303030303030ull;
    // if (val >= 100000000) {
    //     const uint_fast32_t e9d10 = 52, hi2 = val * e57d8 >> 57, lo8 = val - hi2 * 100000000;
    //     lo8a = itoa8lil(lo8) >> 2 | ascii0s;
    //     const uint_fast8_t hi1d = hi2*e9d10 >> 9, adjustSize = hi1d ? 0 : 1, shift = hi1d ? 0 : 8;
    //     hi2a = (((hi2 << 8) + (uint_fast16_t) ascii0s | hi1d)>>shift) - (hi2*e9d10 & 0xe00) * 5;
    //     hi2aSize = hi2aSize + (negShift ? 1 : 0) - adjustSize;
    // } else {
    //     const uint64_t decimals = itoa8lil(val), tmp = (decimals >> 2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
    //     lo8a = decimals ? tmp : (uint_fast8_t) ascii0s;
    //     hi2aSize = negShift ? 1 : 0;
    // }
    // hi2a = hi2a << negShift | negSign;
    // memcpy(buffer, &hi2a, sizeof hi2a);
    // buffer[hi2aSize+8] = '\0';
    // memcpy(buffer+hi2aSize, &lo8a, sizeof lo8a);

    // uint_fast64_t lo8a;
    // uint_fast32_t hi2a = 0;
    // uint_fast8_t hi2aSize = 2;
    // const uint_fast32_t val = value < 0 ? ~value + 1 : value;
    // const uint_fast32_t negMask = value < 0 ? -1 : 0, negShift = 8 & negMask, negSign = '-' & negMask;
    // hi2aSize -= negMask;
    // const uint64_t ascii0s = 0x3030303030303030ull;
    // if (val >= 100000000) {
    //     const uint_fast32_t e9d10 = 52, hi2 = val * e57d8 >> 57, lo8 = val - hi2 * 100000000;
    //     lo8a = itoa8lil(lo8) >> 2 | ascii0s;
    //     const uint_fast8_t hi1d = hi2*e9d10 >> 9, adjustSize = hi1d ? 0 : 1, shift = hi1d ? 0 : 8;
    //     hi2a = (((hi2 << 8) + (uint_fast16_t) ascii0s | hi1d)>>shift) - (hi2*e9d10 & 0xe00) * 5;
    //     hi2aSize -= adjustSize;
    // } else {
    //     const uint64_t decimals = itoa8lil(val), tmp = (decimals >> 2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
    //     lo8a = decimals ? tmp : (uint_fast8_t) ascii0s;
    //     hi2aSize = -negMask;
    // }
    // hi2a = hi2a << negShift | negSign;
    // memcpy(buffer, &hi2a, sizeof hi2a);
    // buffer[hi2aSize+8] = '\0';
    // memcpy(buffer+hi2aSize, &lo8a, sizeof lo8a);

    // uint_fast64_t lo8a;
    // uint_fast32_t hi2a = 0;
    // uint_fast8_t hi2aSize = 2;
    // const uint_fast32_t val = value < 0 ? ~value + 1 : value;
    // const uint_fast32_t negMask = value < 0 ? -1 : 0, negShift = 8 & negMask, negSign = '-' & negMask;
    // const uint64_t ascii0s = 0x3030303030303030ull;
    // if (val >= 100000000) {
    //     const uint_fast32_t e9d10 = 52, hi2 = val * e57d8 >> 57, lo8 = val - hi2 * 100000000;
    //     lo8a = itoa8lil(lo8) >> 2 | ascii0s;
    //     const uint_fast8_t hi1d = hi2*e9d10 >> 9, adjustSize = hi1d ? 0 : 1, shift = hi1d ? 0 : 8;
    //     hi2a = (((hi2 << 8) + (uint_fast16_t) ascii0s | hi1d)>>shift) - (hi2*e9d10 & 0xe00) * 5;
    //     hi2aSize = hi2aSize - negMask - adjustSize;
    // } else {
    //     const uint64_t decimals = itoa8lil(val), tmp = (decimals >> 2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
    //     lo8a = decimals ? tmp : (uint_fast8_t) ascii0s;
    //     hi2aSize = -negMask;
    // }
    // hi2a = hi2a << negShift | negSign;
    // memcpy(buffer, &hi2a, sizeof hi2a);
    // buffer[hi2aSize+8] = '\0';
    // memcpy(buffer+hi2aSize, &lo8a, sizeof lo8a);

    // uint_fast64_t lo8a;
    // uint_fast32_t hi2a = 0;
    // uint_fast8_t hi2aSize = 2;
    // const uint_fast32_t val = value < 0 ? ~value + 1 : value;
    // const uint_fast32_t negMask = value < 0 ? -1 : 0, negShift = 8 & negMask;
    // const uint64_t ascii0s = 0x3030303030303030ull;
    // if (val >= 100000000) {
    //     const uint_fast32_t e9d10 = 52, hi2 = val * e57d8 >> 57, lo8 = val - hi2 * 100000000;
    //     lo8a = itoa8lil(lo8) >> 2 | ascii0s;
    //     const uint_fast8_t hi1d = hi2*e9d10 >> 9, adjustSize = hi1d ? 0 : 1, shift = hi1d ? 0 : 8;
    //     hi2a = (((hi2 << 8) + (uint_fast16_t) ascii0s | hi1d)>>shift) - (hi2*e9d10 & 0xe00) * 5;
    //     hi2aSize = hi2aSize - negMask - adjustSize;
    // } else {
    //     const uint64_t decimals = itoa8lil(val), tmp = (decimals >> 2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
    //     lo8a = decimals ? tmp : (uint_fast8_t) ascii0s;
    //     hi2aSize = -negMask;
    // }
    // hi2a = hi2a << negShift | ('-' & negMask);
    // memcpy(buffer, &hi2a, sizeof hi2a);
    // buffer[hi2aSize+8] = '\0';
    // memcpy(buffer+hi2aSize, &lo8a, sizeof lo8a);

    // uint_fast64_t lo8a;
    // uint_fast32_t hi2a = 0;
    // uint_fast8_t hi2aSize = 2;
    // const uint_fast32_t val = value < 0 ? ~value + 1 : value;
    // const uint_fast32_t negMask = value < 0 ? -1 : 0, negShift = 8 & negMask;
    // const uint_fast8_t negSign = '-' & negMask;
    // const uint64_t ascii0s = 0x3030303030303030ull;
    // if (val >= 100000000) {
    //     const uint_fast32_t e9d10 = 52, hi2 = val * e57d8 >> 57, lo8 = val - hi2 * 100000000;
    //     lo8a = itoa8lil(lo8) >> 2 | ascii0s;
    //     const uint_fast8_t hi1d = hi2*e9d10 >> 9, adjustSize = hi1d ? 0 : 1, shift = hi1d ? 0 : 8;
    //     hi2a = (((hi2 << 8) + (uint_fast16_t) ascii0s | hi1d)>>shift) - (hi2*e9d10 & 0xe00) * 5;
    //     hi2aSize = hi2aSize - negMask - adjustSize;
    // } else {
    //     const uint64_t decimals = itoa8lil(val), tmp = (decimals >> 2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
    //     lo8a = decimals ? tmp : (uint_fast8_t) ascii0s;
    //     hi2aSize = -negMask;
    // }
    // hi2a = hi2a << negShift | negSign;
    // buffer[hi2aSize+8] = '\0';
    // memcpy(buffer, &hi2a, sizeof hi2a);
    // memcpy(buffer+hi2aSize, &lo8a, sizeof lo8a);

    // uint_fast64_t lo8a;
    // uint_fast32_t hi2a = 0;
    // uint_fast8_t hi2aSize = 2;
    // const uint_fast32_t val = value < 0 ? ~value + 1 : value;
    // const uint_fast32_t negMask = value < 0 ? -1 : 0, negShift = 8 & negMask;
    // const uint_fast8_t negSign = '-' & negMask;
    // if (val >= 100000000) {
    //     const uint_fast32_t e9d10 = 52, hi2 = val * e57d8 >> 57, lo8 = val - hi2 * 100000000;
    //     lo8a = itoa8lil(lo8) >> 2 | ascii0s;
    //     const uint_fast8_t hi1d = hi2*e9d10 >> 9, adjustSize = hi1d ? 0 : 1, shift = hi1d ? 0 : 8;
    //     hi2a = (((hi2 << 8) + (uint_fast16_t) ascii0s | hi1d)>>shift) - (hi2*e9d10 & 0xe00) * 5;
    //     hi2aSize = hi2aSize - negMask - adjustSize;
    // } else {
    //     const uint64_t decimals = itoa8lil(val), tmp = (decimals >> 2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
    //     lo8a = decimals ? tmp : (uint_fast8_t) ascii0s;
    //     hi2aSize = -negMask;
    // }
    // hi2a = hi2a << negShift | negSign;
    // buffer[hi2aSize+8] = '\0';
    // memcpy(buffer, &hi2a, sizeof hi2a);
    // memcpy(buffer+hi2aSize, &lo8a, sizeof lo8a);

    // uint_fast64_t lo8a;
    // uint_fast32_t hi2a = 0;
    // uint_fast8_t hi2aSize = 2;
    // const uint_fast32_t val = value < 0 ? ~value + 1 : value;
    // const uint_fast32_t negMask = value < 0 ? -1 : 0, negShift = 8 & negMask;
    // const uint_fast8_t negSign = '-' & negMask;
    // const uint64_t ascii0s = 0x3030303030303030ull;
    // if (val >= 100000000) {
    //     const uint_fast32_t hi2 = val * e57d8 >> 57, lo8 = val - hi2 * 100000000;
    //     lo8a = itoa8lil(lo8) >> 2 | ascii0s;
    //     // const uint_fast8_t hi1d = hi2*e9d10 >> 9, adjustSize = hi1d ? 0 : 1, shift = hi1d ? 0 : 8;
    //     // hi2a = (((hi2 << 8) + (uint_fast16_t) ascii0s | hi1d)>>shift) - (hi2*e9d10 & 0xe00) * 5;
    //     // const uint_fast8_t hi1d = (hi2>=10 ? 512 : 0) + (hi2>=10), adjustSize = hi1d ? 0 : 1, shift = hi1d ? 0 : 8;
    //     // hi2a = (((hi2 << 8) + (uint_fast16_t) ascii0s | hi1d)>>shift) - (hi2*e9d10 & 0xe00) * 5;
    //     // const uint_fast32_t hi1d = (hi2 >= 10 ? 512 : 0) + (hi2 >= 20 ? 512 : 0), adjustSize = hi1d ? 0 : 1, shift = hi1d ? 0 : 8;
    //     // hi2a = (((hi2 << 8) + (uint_fast16_t) ascii0s | hi1d>>9)>>shift) - hi1d*5;
    //     const uint_fast32_t hi1d = (hi2 >= 10 ? 2 : 0) + (hi2 >= 20 ? 2 : 0), adjustSize = hi1d ? 0 : 1, shift = hi1d ? 8 : 0;
    //     hi2a = hi2 - 5*hi1d << shift | (uint_fast16_t) ascii0s | hi1d >> 1;
    //     hi2aSize = hi2aSize - negMask - adjustSize;
    // } else {
    //     const uint64_t decimals = itoa8lil(val), tmp = (decimals >> 2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
    //     lo8a = decimals ? tmp : (uint_fast8_t) ascii0s;
    //     hi2aSize = -negMask;
    // }
    // hi2a = hi2a << negShift | negSign;
    // buffer[hi2aSize+8] = '\0';
    // memcpy(buffer, &hi2a, sizeof hi2a);
    // memcpy(buffer+hi2aSize, &lo8a, sizeof lo8a);


    // *buffer = '-';
    // if (val<0) {
    //     val = -val;
    //     ++buffer;
    // }
    // u32toa_hintro(val, buffer);

    
    // if (val<0) {
    //     val = -val;
    // }
    // *buffer = '-';
    // u32toa_hintro(val, buffer+(val<0));

    // int off = 0;
    // if (val<0) {off = 1; val = -val;}
    // *buffer = '-';
    // u32toa_hintro(val, buffer+off);

    // const int64_t tmp = '-';
    // memcpy(buffer, &tmp, 8);
    // memset(buffer+8, 0, 4);
    // uint32_t val = (uint32_t&) value;
    // if (value<0) {
    //     val = -val;
    //     ++buffer;
    // }
    // lo8a = 0x30;  // ASCII 0
    // if (val >= 100000000) {
    //     const int32_t hi2 = val/100000000;
    //     memcpy(buffer, dLut+hi2, 2);
    //     const int32_t lo8 = val%100000000;
    //     buffer += val>=1000000000 ? 2 : 1;
    //     itoa8lut(lo8, buffer);
    // } else {
    //     if (val) {
    //         const int64_t decimals = itoa8lil(val);
    //         lo8a = (decimals>>2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
    //     }
    //     memcpy(buffer, &lo8a, 8);
    // }


    // const int64_t tmp = '-';
    // memcpy(buffer, &tmp, 4);
    // if (value<0) {
    //     value = -value;
    //     ++buffer;
    // }
    // u32toa_hintro(value, buffer);

    const int64_t tmp = '-';
    memcpy(buffer, &tmp, 8);
    memset(buffer+8, 0, 4);
    uint32_t val = (uint32_t&) value;
    if (value<0) {
        val = -val;
        ++buffer;
    }
    lo8a = 0x30;  // ASCII 0
    if (val >= 100000000) {
        const int32_t hi2 = val/100000000;
        memcpy(buffer, dLut+hi2, 2);
        const int32_t lo8 = val%100000000;
        buffer += val>=1000000000 ? 2 : 1;
        itoa8lut(lo8, buffer);
    } else {
        if (val) {
            const int64_t decimals = itoa8lil(val);
            lo8a = (decimals>>2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
        }
        memcpy(buffer, &lo8a, 8);
    }
}

void u64toa_hintro(uint64_t value, char* buffer) {
    // lo8a = 0x30;  // ASCII 0
    // memset(buffer+7, 0, 4);
    // if (val >= 100000000) {
    //     const int32_t hi2 = val/100000000;
    //     memcpy(buffer, dLut+hi2, 2);
    //     const int32_t lo8 = val%100000000;
    //     buffer += val>=1000000000 ? 2 : 1;
    //     itoa8lut(lo8, buffer);
    // } else {
    //     if (val) {
    //         const int64_t decimals = itoa8lil(val);
    //         lo8a = (decimals>>2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
    //     }
    //     memcpy(buffer, &lo8a, 8);
    // }

    // char temp[10];
    // char *p = temp;
    // do {
    //     *p++ = char(value % 10) + '0';
    //     value /= 10;
    // } while (value > 0);

    // do {
    //     *buffer++ = *--p;
    // } while (p != temp);

    // *buffer = '\0';
}

void i64toa_hintro(int64_t value, char* buffer) {
    uint64_t u = static_cast<uint64_t>(value);
    if (value < 0) {
        *buffer++ = '-';
        u = ~u + 1;
    }
    u64toa_hintro(u, buffer);
}

REGISTER_TEST(hintro);

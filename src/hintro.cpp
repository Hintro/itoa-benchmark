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

// inline ll itoa8big(const uint_fast32_t val) {
//     const i32 q = (ll) val * e40d10000 >> 40;
//     const ll a = ((ll) q << 32 | val) - q * 10000;
//     const ll b = a * e19d100 & 0x3f8000003f80000ll;
//     const ll c = (a | b>>3) - (b*25>>17), d = c * e10d10 & 0x3c003c003c003c00ll;
//     return (c | d>>2) - (d*5>>9);
// }

inline ull itoa8lil(const u32 val) {
    // const ull q = val * e40d10000 >> 40, a = ((ull) val << 32 | q) - q * e32m10000;
    // const ull mask1 = 0x3f8000003f80000ull, mask2 = 0x7800780078007800ull;
    // const ull b = a * e19d100 & mask1;
    // const ull c = (a<<17 | b>>18) - b*25, d = c * e10d10 & mask2;
    // return (c<<9 | d>>9) - d* 5;  

    // const uint_fast32_t q = val * e40d10000 >> 40;
    // const uint_fast64_t a = ((ull) val << 32 | q) - q * e32m10000;
    // const ull b = a * e19d100 & 0x3f8000003f80000ull;
    // const ull c = (a<<17 | b>>18) - b*25, d = c * e10d10 & 0x7800780078007800ull;
    // return (c<<9 | d>>9) - d* 5;

    const i32 q = (ll) val * e40d10000 >> 40;
    const ll a = ((ll) val << 32 | q) - q * e32m10000;
    const ll b = a * e19d100 & 0x3f8000003f80000ll;
    const ll c = (a<<17 | b>>18) - b*25, d = c * e10d10 & 0x7800780078007800ll; 
    return (c<<9 | d>>9) - d*5;

    // const i32 q = (ll) val * e40d10000 >> 40;
    // const ll a = -e32m10000*q + ((ll) val << 32 | q);
    // const ll b = a * e19d100 & 0x3f8000003f80000ll, b25 = b*25;
    // const ll c = (a<<17 | b>>18) - b25, d = c * e10d10 & 0x7800780078007800ll; 
    // return (c<<9 | d>>9) - d*5;
}

inline ull itoa8lil2(const u32 val) {
    // const ull q = val * e40d10000 >> 40, a = ((ull) val << 32 | q) - q * e32m10000;
    // const ull mask1 = 0x3f8000003f80000ull, mask2 = 0x7800780078007800ull;
    // const ull b = a * e19d100 & mask1;
    // const ull c = (a<<17 | b>>18) - b*25, d = c * e10d10 & mask2;
    // return (c<<9 | d>>9) - d* 5;  

    // const uint_fast32_t q = val * e40d10000 >> 40;
    // const uint_fast64_t a = ((ull) val << 32 | q) - q * e32m10000;
    // const ull b = a * e19d100 & 0x3f8000003f80000ull;
    // const ull c = (a<<17 | b>>18) - b*25, d = c * e10d10 & 0x7800780078007800ull;
    // return (c<<9 | d>>9) - d* 5;

    // const i32 q = (ll) val * e40d10000 >> 40;
    // const ll a = ((ll) val << 32 | q) - q * e32m10000;
    // const ll b = a * e19d100 & 0x3f8000003f80000ll;
    // const ll c = (a<<17 | b>>18) - b*25, d = c * e10d10 & 0x7800780078007800ll; 
    // return (c<<9 | d>>9) + ((ascii0s<<2) - d*5);

    const i32 q = (ll) val * e40d10000 >> 40;
    const ll a = -e32m10000*q + ((ll) val << 32 | q);
    const ll b = a * e19d100 & 0x3f8000003f80000ll, b25 = b*25;
    const ll c = (a<<17 | b>>18) - b25, d = c * e10d10 & 0x7800780078007800ll; 
    return (c<<9 | d>>9) + ((ascii0s<<2) - d*5);
}


inline void itoa8lut(const u32 val, char * buf) {
    // const i32 q = (ll) val * e40d10000 >> 40;
    // const ll a = ((ll) val << 32 | q) - q * e32m10000;
    // const ll b = a * e19d100 & 0x3f8000003f80000ll;
    // const ll c = (a<<17) - b*25; 
    // memcpy(buf, gDigitsLut + ((i32)b>>18), 2);
    // memcpy(buf+4, gDigitsLut + (b>>50), 2);
    // memcpy(buf+2, gDigitsLut + ((i32)c>>16), 2);
    // memcpy(buf+6, gDigitsLut + (c>>48), 2);

    // const i32 q = (ll) val * e40d10000 >> 40;
    // const ll a = ((ll) val << 32 | q);
    // const ll b = a * e19d100 - q*(e32m10000*e19d100) & 0x3f8000003f80000ll;
    // const ll c = ((a-q*e32m10000)<<17) - b*25; 
    // memcpy(buf, gDigitsLut + ((i32)b>>18), 2);
    // memcpy(buf+4, gDigitsLut + (b>>50), 2);
    // memcpy(buf+2, gDigitsLut + ((i32)c>>16), 2);
    // memcpy(buf+6, gDigitsLut + (c>>48), 2);

    // const i32 q = (ll) val * e40d10000 >> 40;
    // const ll a = ((ll) val << 32 | q) - q * e32m10000;
    // const ll b = a * e19d100 & 0x3f8000003f80000ll;
    // const ll c = (a<<17) - b*25; 
    // memcpy(buf, gDigitsLut + ((i32)b>>18), 2);
    // memcpy(buf+4, gDigitsLut + (b>>50), 2);
    // memcpy(buf+2, gDigitsLut + ((i32)c>>16), 2);
    // memcpy(buf+6, gDigitsLut + (c>>48), 2);

    // const i32 q = (ll) val * e40d10000 >> 40;
    // const ll a = -e32m10000*q + ((ll) val << 32 | q);
    // const ll b = a * e19d100 & 0x3f8000003f80000ll;
    // const ll c = (a<<17) - b*25; 
    // memcpy(buf, gDigitsLut + ((i32)b>>18), 2);
    // memcpy(buf+4, gDigitsLut + (b>>50), 2);
    // memcpy(buf+2, gDigitsLut + ((i32)c>>16), 2);
    // memcpy(buf+6, gDigitsLut + (c>>48), 2);

    // const i32 q = (ll) val * e40d10000 >> 40;
    // const ll a = -e32m10000*q + ((ll) val << 32 | q);
    // const ll b = a * e19d100 & 0x3f8000003f80000ll;
    // const ll c = (a<<17) - b*25;
    // memcpy(buf, gDigitsLut + ((i32)b>>18), 2);
    // memcpy(buf+4, gDigitsLut + (b>>50), 2);
    // memcpy(buf+2, gDigitsLut + ((i32)c>>16), 2);
    // memcpy(buf+6, gDigitsLut + (c>>48), 2);

    // const i32 q = (ll) val * e40d10000 >> 40;
    // const ll a = -e32m10000*q + ((ll) val << 32 | q);
    // const ll b = a * e19d100 & 0x3f8000003f80000ll;
    // const ll b25 = b*25, c = (a<<17) - b25;
    // // memcpy(buf, gDigitsLut + ((i32)b>>18), 2);
    // // memcpy(buf+4, gDigitsLut + (b>>50), 2);
    // // memcpy(buf+2, gDigitsLut + ((i32)c>>16), 2);
    // // memcpy(buf+6, gDigitsLut + (c>>48), 2);
    // memcpy(buf, gDigitsLut + ((i32)b>>18), 2);
    // memcpy(buf+2, gDigitsLut + ((i32)c>>16), 2);
    // memcpy(buf+4, gDigitsLut + (b>>50), 2);
    // memcpy(buf+6, gDigitsLut + (c>>48), 2);

    const i32 q = (ll) val * e40d10000 >> 40;
    const ll a = -e32m10000*q + ((ll) val << 32 | q);
    const ll b = a * e19d100 & 0x3f8000003f80000ll;
    const ll b25 = b*25, c = (a<<17) - b25;
    // memcpy(buf, gDigitsLut + ((i32)b>>18), 2);
    // memcpy(buf+4, gDigitsLut + (b>>50), 2);
    // memcpy(buf+2, gDigitsLut + ((i32)c>>16), 2);
    // memcpy(buf+6, gDigitsLut + (c>>48), 2);
    memcpy(buf, gDigitsLut + ((i32)b>>18), 2);
    memcpy(buf+2, gDigitsLut + ((i32)c>>16), 2);
    memcpy(buf+4, gDigitsLut + (b>>50), 2);
    memcpy(buf+6, gDigitsLut + (c>>48), 2);
}


inline ll itoa8lut2(const u32 val) {
    const i32 q = (ll) val * e40d10000 >> 40;
    const ll a = ((ll) val << 32 | q) - q * e32m10000;
    const ll b = a * e19d100 & 0x3f8000003f80000ll;
    const ll c = (a<<17) - b*25;
    ll _0, _2, _4, _6;
    memcpy(&_0, gDigitsLut + ((i32)b>>18), 2);
    memcpy(&_4, gDigitsLut + (b>>50), 2);
    memcpy(&_2, gDigitsLut + ((i32)c>>16), 2);
    memcpy(&_6, gDigitsLut + (c>>48), 2);
    return (_0 | _2<<16) | (_4<<32 | _6<<48);

    // const i32 q = (ll) val * e40d10000 >> 40;
    // const ll a = ((ll) val << 32 | q) - q * e32m10000;
    // const ll b = a * e19d100 & 0x3f8000003f80000ll;
    // const ll c = (a<<17) - b*25;
    // ll ret;
    // memcpy(&((char&)ret) , gDigitsLut + ((i32)b>>18), 2);
    // memcpy(&((char&)ret) +4, gDigitsLut + (b>>50), 2);
    // memcpy(&((char&)ret) +2, gDigitsLut + ((i32)c>>16), 2);
    // memcpy(&((char&)ret) +6, gDigitsLut + (c>>48), 2);
    // return ret;
}


inline ll itoa8blut(const u32 val) {
    const i32 q = (ll) val * e40d10000 >> 40;
    const ll a = -e32m10000*q + ((ll) val << 32 | q);
    const ll b = a * e19d100 & 0x3f8000003f80000ll;
    const ll b25 = b*25, c = (a<<17) - b25;
    ll _0, _2, _4, _6;
    memcpy(&_6, decLut + ((i32)b>>18), 2);
    memcpy(&_2, decLut + (b>>50), 2);
    memcpy(&_4, decLut + ((i32)c>>16), 2);
    memcpy(&_0, decLut + (c>>48), 2);
    return (_6<<48 | _2<<16) | (_4<<32 | _0);

    // const i32 q = (ll) val * e40d10000 >> 40;
    // const ll a = ((ll) val << 32 | q) - q * e32m10000;
    // const ll b = a * e19d100 & 0x3f8000003f80000ll;
    // const ll c = (a<<17) - b*25;
    // ll ret;
    // memcpy(&((char&)ret) , gDigitsLut + ((i32)b>>18), 2);
    // memcpy(&((char&)ret) +4, gDigitsLut + (b>>50), 2);
    // memcpy(&((char&)ret) +2, gDigitsLut + ((i32)c>>16), 2);
    // memcpy(&((char&)ret) +6, gDigitsLut + (c>>48), 2);
    // return ret;
}


static u32 E = 0;
// static uint16_t last = '0';
// static char * lastPointer;
// static uint_fast64_t last_a = (char)ascii0s;
// static uint_fast8_t shift = 56;
// static uint_fast32_t power10 = 10;
static uint16_t hi2a = 0, off = 0;
static ull lo8a = 0x2f;
static ull step = 1, limit = 0x3a;
static int shifts = 56;


inline ll itoa8lil(const u32 val, const ll& ne32m10000) {
    const i32 q = (ll) val * e40d10000 >> 40;
    const ll a = ((ll) val << 32 | q) + q * ne32m10000;
    const ll b = a * e19d100 & 0x3f8000003f80000ll;
    const ll c = (a<<17 | b>>18) - b*25, d = c * e10d10 & 0x7800780078007800ll; 
    return (c<<9 | d>>9) - d*5;
}

inline void itoa8lut2(const u32 val, char * buf, const ll& ne32m10000) {
    const i32 q = (ll) val * e40d10000 >> 40;
    const ll a = ne32m10000*q + ((ll) val << 32 | q);
    const ll b = a * e19d100 & 0x3f8000003f80000ll;
    const ll b25 = b*25, c = (a<<17) - b25;
    memcpy(buf, gDigitsLut + ((i32)b>>18), 2);
    memcpy(buf+4, gDigitsLut + (b>>50), 2);
    memcpy(buf+2, gDigitsLut + ((i32)c>>16), 2);
    memcpy(buf+6, gDigitsLut + (c>>48), 2);
}


inline void u32toa_hintro(const u32 val, char* buffer) {
    // ull lo8a;
    // u32 hi2a, hi2aSize = 2;
    // if (val >= 100000000) {
    //     const u32 hi2 = val * e57d8 >> 57, lo8 = val - hi2 * 100000000;
    //     lo8a = itoa8lil(lo8) >> 2 | ascii0s;
    //     const uint_fast8_t hi1d = hi2*e9d10 >> 9, shift = hi1d ? 0 : 8;
    //     // hi2aSize -= hi1d ? 0 : 1;
    //     hi2aSize -= !hi1d;
    //     hi2a = (((hi2 << 8) + (uint_fast16_t) ascii0s | hi1d)>>shift) - (hi2*e9d10 & 0xe00) * 5;
    //     memcpy(buffer, &hi2a, sizeof hi2a);
    //     // hi2aSize -= hi2 < 10;
    // } else {
    //     const ull decimals = itoa8lil(val), tmp = (decimals >> 2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
    //     lo8a = decimals ? tmp : (uint_fast8_t) ascii0s;
    //     hi2aSize = 0;
    // }
    // memcpy(buffer+=hi2aSize, &lo8a, 8);
    // buffer[8] = '\0';


    // ull lo8a;
    // u32 hi2a, hi2aSize = 2;
    // if (val >= 100000000) {
    //     const u32 hi2 = val * e57d8 >> 57, lo8 = val - hi2 * 100000000;
    //     lo8a = itoa8lil(lo8) >> 2 | ascii0s;
    //     const uint_fast8_t hi1d = hi2*e9d10 >> 9, shift = hi1d ? 0 : 8;
    //     hi2aSize -= hi1d ? 0 : 1;
    //     hi2a = (((hi2 << 8) + (uint_fast16_t) ascii0s | hi1d)>>shift) - (hi2*e9d10 & 0xe00) * 5;
    //     memcpy(buffer, &hi2a, 2);
    //     buffer += hi1d ? 2 : 1;
    //     // hi2aSize -= hi2 < 10;
    // } else {
    //     const ull decimals = itoa8lil(val), tmp = (decimals >> 2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
    //     lo8a = decimals ? tmp : (uint_fast8_t) ascii0s;
    //     // hi2aSize = 0;
    // }
    // memcpy(buffer, &lo8a, 8);
    // buffer[8] = '\0';


    // ull lo8a;
    // u32 hi2a;
    // if (val >= 100000000) {
    //     const u32 hi2 = val * e57d8 >> 57, lo8 = val - hi2 * 100000000;
    //     lo8a = itoa8lil(lo8) >> 2 | ascii0s;
    //     const uint_fast8_t hi1d = hi2*e9d10 >> 9, shift = hi1d ? 0 : 8;
    //     // hi2aSize -= hi1d ? 0 : 1;
    //     hi2a = (((hi2 << 8) + (uint_fast16_t) ascii0s | hi1d)>>shift) - (hi2*e9d10 & 0xe00) * 5;
    //     memcpy(buffer, &hi2a, sizeof hi2a);
    //     buffer += hi1d ? 2 : 1;
    //     // hi2aSize -= hi2 < 10;
    // } else {
    //     const ull decimals = itoa8lil(val), tmp = (decimals >> 2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
    //     lo8a = decimals ? tmp : (uint_fast8_t) ascii0s;
    //     // hi2aSize = 0;
    // }
    // memcpy(buffer, &lo8a, 8);
    // buffer[8] = '\0';

    // ull lo8a;
    // u32 hi2a, hi2aSize = 2;
    // if (val >= 100000000) {
    //     const u32 hi2 = val * e57d8 >> 57, lo8 = val - hi2 * 100000000;
    //     lo8a = itoa8lil(lo8) >> 2 | ascii0s;
    //     const u32 hi1d = hi2*e9d10 >> 9, shift = hi1d ? 0 : 8;
    //     hi2aSize -= !hi1d;
    //     hi2a = (((hi2 << 8) + (uint_fast16_t) ascii0s | hi1d)>>shift) - (hi2*e9d10 & 0xe00) * 5;
    //     memcpy(buffer, &hi2a, sizeof hi2a);
    // } else {
    //     const ull decimals = itoa8lil(val), tmp = (decimals >> 2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
    //     lo8a = decimals ? tmp : (uint_fast8_t) ascii0s;
    //     hi2aSize = 0;
    // }
    // memcpy(buffer+=hi2aSize, &lo8a, 8);
    // buffer[8] = '\0';

    // ll lo8a;
    // i32 hi2a, hi2aSize = 2;
    // if (val >= 100000000) {
    //     const u32 hi2 = val * e57d8 >> 57;
    //     const i32 lo8 = val - hi2 * 100000000;
    //     lo8a = itoa8lil(lo8) >> 2 | ascii0s;
    //     const i32 hi1d = hi2*e9d10 >> 9, shift = hi1d ? 0 : 8;
    //     hi2aSize -= !hi1d;
    //     hi2a = (((hi2 << 8) + (u32) ascii0s | hi1d)>>shift) - (hi2*e9d10 & 0xe00) * 5;
    //     memcpy(buffer, &hi2a, sizeof hi2a);
    // } else {
    //     const ll decimals = itoa8lil(val), tmp = (decimals >> 2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
    //     lo8a = decimals ? tmp : (uint8_t) ascii0s;
    //     hi2aSize = 0;
    // }
    // memcpy(buffer+=hi2aSize, &lo8a, 8);
    // buffer[8] = '\0';

    // ll lo8a;
    // i32 hi2aSize = 2;
    // if (val >= 100000000) {
    //     const u32 hi2 = val * e57d8 >> 57;
    //     const i32 lo8 = val - hi2 * 100000000;
    //     lo8a = itoa8lil(lo8) >> 2 | ascii0s;
    //     memcpy(buffer, dLut+hi2, 2);
    //     hi2aSize -= hi2<10;
    // } else {
    //     const ll decimals = itoa8lil(val), tmp = (decimals >> 2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
    //     lo8a = decimals ? tmp : (uint8_t) ascii0s;
    //     hi2aSize = 0;
    // }
    // memcpy(buffer+=hi2aSize, &lo8a, 8);
    // buffer[8] = '\0';


    // ll lo8a;
    // if (val >= 100000000) {
    //     const u32 hi2 = val * e57d8 >> 57;
    //     memcpy(buffer, dLut+hi2, 2);
    //     buffer = hi2>=10 ? buffer+2 : buffer+1;
    //     const i32 lo8 = val - hi2 * 100000000;
    //     lo8a = itoa8lil(lo8) >> 2 | ascii0s;
    //     // memcpy(buffer, dLut+hi2, 2); buffer = hi2>=10 ? buffer+2 : buffer+1;
    // } else {
    //     const ll decimals = itoa8lil(val), tmp = (decimals >> 2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
    //     lo8a = decimals ? tmp : (uint8_t) ascii0s;
    // }
    // buffer[8] = '\0';
    // memcpy(buffer, &lo8a, 8);

    // ll lo8a = (int_fast8_t) ascii0s;
    // if (val >= 100000000) {
    //     const u32 hi2 = val * e57d8 >> 57;
    //     memcpy(buffer, dLut+hi2, 2);
    //     buffer = hi2>=10 ? buffer+2 : buffer+1;
    //     const i32 lo8 = val - hi2 * 100000000;
    //     lo8a = itoa8lil(lo8) >> 2 | ascii0s;
    //     // memcpy(buffer, dLut+hi2, 2); buffer = hi2>=10 ? buffer+2 : buffer+1;
    // } else if (val) {
    //     const ll decimals = itoa8lil(val);
    //     lo8a = (decimals >> 2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
    // }
    // buffer[8] = '\0';
    // memcpy(buffer, &lo8a, 8);

    // ll lo8a = (int_fast8_t) ascii0s;
    // if (val >= 100000000) {
    //     const i32 hi2 = val/100000000, lo8 = val%100000000;
    //     memcpy(buffer, dLut+hi2, 2);
    //     buffer = hi2>=10 ? buffer+2 : buffer+1;
    //     lo8a = itoa8lil(lo8) >> 2 | ascii0s;
    //     // memcpy(buffer, dLut+hi2, 2); buffer = hi2>=10 ? buffer+2 : buffer+1;
    // } else if (val) {
    //     const ll decimals = itoa8lil(val);
    //     lo8a = (decimals >> 2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
    // }
    // buffer[8] = '\0';
    // memcpy(buffer, &lo8a, 8);

    // ll lo8a = (int_fast8_t) ascii0s;
    // if (val >= 100000000) {
    //     const i32 hi2 = val/100000000;
    //     memcpy(buffer, dLut+hi2, 2);
    //     const i32 lo8 = val%100000000;
    //     buffer = hi2>=10 ? buffer+2 : buffer+1;
    //     lo8a = itoa8lil(lo8) >> 2 | ascii0s;
    // } else if (val) {
    //     const ll decimals = itoa8lil(val);
    //     lo8a = (decimals >> 2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
    // }
    // buffer[8] = '\0';
    // memcpy(buffer, &lo8a, 8);

    // ll lo8a = 0x30;  // ASCII 0
    // if (val >= 100000000) {
    //     const i32 hi2 = val/100000000;
    //     memcpy(buffer, dLut+hi2, 2);
    //     const i32 lo8 = val%100000000;
    //     buffer = hi2>=10 ? buffer+2 : buffer+1;
    //     lo8a = itoa8lil(lo8) >> 2 | ascii0s;
    // } else if (val) {
    //     const ll decimals = itoa8lil(val);
    //     lo8a = (decimals >> 2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
    // }
    // buffer[8] = 0;
    // memcpy(buffer, &lo8a, 8);

    // ll lo8a = 0x30;  // ASCII 0
    // if (val >= 100000000) {
    //     const i32 hi2 = val/100000000;
    //     memcpy(buffer, dLut+hi2, 2);
    //     const i32 lo8 = val%100000000;
    //     buffer += val>=1000000000 ? 2 : 1;
    //     lo8a = itoa8lil(lo8) >> 2 | ascii0s;
    // } else if (val) {
    //     // const ll decimals = itoa8lil(val);
    //     const i32 lzs = __builtin_clz(val)>>1, power = powersLut[lzs];
    //     const ll decimals = itoa8lil(val);
    //     const i32 guess = shiftLut >> (lzs<<2), shifts = guess + (val<power) << 3;
    //     lo8a = (decimals >> 2 | ascii0s) >> shifts;
    // }
    // buffer[8] = 0;
    // memcpy(buffer, &lo8a, 8);


    // ll lo8a = 0x30;  // ASCII 0
    // if (val >= 100000000) {
    //     const i32 hi2 = val/100000000;
    //     memcpy(buffer, dLut+hi2, 2);
    //     const i32 lo8 = val%100000000;
    //     buffer += val>=1000000000 ? 2 : 1;
    //     lo8a = itoa8lil(lo8)>>2 | ascii0s;
    // } else if (val) {
    //     const ll decimals = itoa8lil(val);
    //     lo8a = (decimals>>2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
    // }
    // buffer[8] = 0;
    // memcpy(buffer, &lo8a, 8);

    
    // ll lo8a = 0x30;  // ASCII 0
    // if (val >= 100000000) {
    //     const i32 hi2 = val/100000000;
    //     uint16_t hi2a = ascii0s;
    //     const i32 tenth = val/1000000000;
    //     memcpy(buffer, dLut+hi2, 2);
    //     const i32 lo8 = val%100000000;
    //     buffer += val>=1000000000 ? 2 : 1;
    //     lo8a = itoa8lil(lo8)>>2 | ascii0s;
    // } else if (val) {
    //     const ll decimals = itoa8lil(val);
    //     lo8a = (decimals>>2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
    // }
    // buffer[8] = 0;
    // memcpy(buffer, &lo8a, 8);


    // ll lo8a = 0x30;  // ASCII 0
    // if (val >= 100000000) {
    //     const i32 hi2 = val/100000000;
    //     memcpy(buffer, dLut+hi2, 2);
    //     const i32 lo8 = val%100000000;
    //     buffer += val>=1000000000 ? 2 : 1;
    //     lo8a = itoa8lil(lo8)>>2 | ascii0s;
    // } else if (val) {
    //     const ll decimals = itoa8lil(val);
    //     lo8a = (decimals>>2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
    // }
    // buffer[8] = 0;
    // memcpy(buffer, &lo8a, 8);


    // BIG
    // ENDIAN
    // ll lo8a = 0x3000000000000000ll;  // ASCII 0
    // if (val >= 100000000) {
    //     const i32 hi2 = val/100000000;
    //     memcpy(buffer, dLut+hi2, 2);
    //     const i32 lo8 = val%100000000;
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
    //         const i32 hi2 = val/100000000;
    //         hi2a = dLut[hi2];
    //         memcpy(buffer, &hi2a, 2);
    //         const i32 lo8 = val%100000000;
    //         off += val >= 1000000000;
    //         buffer += off;
    //         lo8a = itoa8lil(lo8)>>2 | ascii0s;
    //         shifts = 0;
    //     } else if (val) {
    //         const ll decimals = itoa8lil(val);
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
    //         const i32 hi2 = val/100000000;
    //         hi2a = dLut[hi2];
    //         memcpy(buffer, &hi2a, 2);
    //         const i32 lo8 = val%100000000;
    //         off += val >= 1000000000;
    //         buffer += off;
    //         lo8a = itoa8lil(lo8)>>2 | ascii0s;
    //         shifts = 0;
    //     } else if (val) {
    //         const ll decimals = itoa8lil(val);
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
    const i32 q = (ll) val * e40d10000 >> 40;
    const ll a = ((ll) val << 32 | q) - q * e32m10000;
    const ll b = a * e19d100 & 0x3f8000003f80000ll;
    const ll c = (a<<17) - b*25; 
    memcpy(buf, gDigitsLut + ((i32)b>>18), 2);
    memcpy(buf+4, gDigitsLut + (b>>50), 2);
    memcpy(buf+2, gDigitsLut + ((i32)c>>16), 2);
    memcpy(buf+6, gDigitsLut + (c>>48), 2);
**/

    // ll lo8a = 0x30;  // ASCII 0
    // if (val >= 100000000) {
    //     const i32 hi2 = val/100000000;
    //     memcpy(buffer, dLut+hi2, 2);
    //     const i32 lo8 = val%100000000;
    //     buffer += val>=1000000000 ? 2 : 1;
    //     buffer[8] = 0;
    //     itoa8lut(lo8, buffer);
    // } else {
    //     buffer[8] = 0;
    //     if (val) {
    //         const ll decimals = itoa8lil(val);
    //         lo8a = (decimals>>2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
    //     }
    //     memcpy(buffer, &lo8a, 8);
    // }

    // buffer[8] = 0;

    // lo8a = 0x30;  // ASCII 0
    // memset(buffer+7, 0, 4);
    // if (val >= 100000000) {
    //     const i32 hi2 = val/100000000;
    //     memcpy(buffer, dLut+hi2, 2);
    //     const i32 lo8 = val%100000000;
    //     buffer += val>=1000000000 ? 2 : 1;
    //     itoa8lut(lo8, buffer);
    // } else {
    //     if (val) {
    //         const ll decimals = itoa8lil(val);
    //         lo8a = (decimals>>2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
    //     }
    //     memcpy(buffer, &lo8a, 8);
    // }


    // lo8a = 0x30;  // ASCII 0
    // memset(buffer+7, 0, 4);
    // if (val >= 100000000) {
    //     const i32 hi2 = val/100000000;
    //     const char * lut = gDigitsLut;
    //     if (val<1000000000) lut += 200;
    //     memcpy(buffer, lut+hi2*2, 2);
    //     const i32 lo8 = val%100000000;
    //     buffer += val>=1000000000 ? 2 : 1;
    //     itoa8lut(lo8, buffer);
    // } else {
    //     if (val) {
    //         const ll decimals = itoa8lil(val);
    //         lo8a = (decimals>>2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
    //     }
    //     memcpy(buffer, &lo8a, 8);
    // }

/**
    const i32 q = (ll) val * e40d10000 >> 40;
    const ll a = ((ll) val << 32 | q) - q * e32m10000;
    const ll b = a * e19d100 & 0x3f8000003f80000ll;
    const ll c = (a<<17 | b>>18) - b*25, d = c * e10d10 & 0x7800780078007800ll; 
    return (c<<9 | d>>9) - d*5;
    **/

    // lo8a = 0x30;  // ASCII 0
    // memset(buffer+7, 0, 4);
    // if (val >= 100000000) {
    //     const i32 hi2 = val/100000000;
    //     memcpy(buffer++, dLut+hi2, 2);
    //     const i32 lo8 = val%100000000;
    //     itoa8lut(lo8, buffer+(val>=1000000000));
    // } else {
    //     if (val) {
    //         const i32 q = (ll) val * e40d10000 >> 40;
    //         i32 x = 100, y = 1000, shifts = 7;
    //         const ll a = ((ll) val << 32 | q) - q * e32m10000;
    //         shifts -= val>=10;
    //         const ll b = a * e19d100 & 0x3f8000003f80000ll;
    //         if(val>=10000) {x = 100000; y = 1000000; shifts-=4;}
    //         const ll c = (a<<17 | b>>18) - b*25, d = c * e10d10 & 0x7800780078007800ll;
    //         shifts -= (val>=x) + (val>=y);
    //         const ull decimals = (c<<9 | d>>9) + ((ascii0s<<2)-d*5);
    //         shifts = shifts*8 + 2;
    //         lo8a = decimals >> shifts;
    //     }
    //     memcpy(buffer, &lo8a, 8);
    // }

    // lo8a = 0x30;  // ASCII 0
    // memset(buffer+7, 0, 4);
    // if (val >= 100000000) {
    //     const i32 hi2 = val/100000000;
    //     memcpy(buffer, dLut+hi2, 2);
    //     const i32 lo8 = val%100000000;
    //     buffer += val>=1000000000 ? 2 : 1;
    //     itoa8lut(lo8, buffer);
    // } else {
    //     if (val) {
    //         const ll decimals = itoa8lil(val);
    //         lo8a = (decimals>>2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
    //     }
    //     memcpy(buffer, &lo8a, 8);
    // }


    // FAST
    // lo8a = 0x30;  // ASCII 0
    // memset(buffer+7, 0, 4);
    // if (val >= 100000000) {
    //     const i32 hi2 = val/100000000;
    //     memcpy(buffer, dLut+hi2, 2);
    //     const i32 lo8 = val%100000000;
    //     buffer += val>=1000000000 ? 2 : 1;
    //     itoa8lut(lo8, buffer);
    // } else {
    //     if (val) {
    //         const ll decimals = itoa8lil(val);
    //         lo8a = (decimals>>2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
    //     }
    //     memcpy(buffer, &lo8a, 8);
    // }


    // lo8a = 0x30;  // ASCII 0
    // memset(buffer+7, 0, 4);
    // if (val >= 100000000) {
    //     const i32 hi2 = val/100000000;
    //     memcpy(buffer, dLut+hi2, 2);
    //     const i32 lo8 = val%100000000;
    //     buffer += val>=1000000000 ? 2 : 1;
    //     itoa8lut(lo8, buffer);
    // } else {
    //     if (val) {
    //         const ll decimals = itoa8blut(val);
    //         lo8a = __builtin_bswap64(decimals | ascii0s) >> (56 & __builtin_clzll(decimals));
    //     }
    //     memcpy(buffer, &lo8a, 8);
    // }

    // lo8a = 0x30;  // ASCII 0
    // memset(buffer+7, 0, 4);
    // ll E57d8 = e57d8, E40d10000 = e40d10000;
    // if (val >= 100000000) {
    //     const i32 hi2 = val*E57d8 >> 57;
    //     memcpy(buffer, dLut+hi2, 2);
    //     const i32 lo8 = val - hi2*100000000;
    //     buffer += val>=1000000000 ? 2 : 1;
    //     itoa8lut(lo8, buffer);
    // } else {
    //     if (val) {
    //         // const ll decimals = itoa8lil(val);
    //         const i32 q = (ll) val * E40d10000 >> 40;
    //         const ll a = ((ll) val << 32 | q) - q * e32m10000;
    //         const ll b = a * e19d100 & 0x3f8000003f80000ll;
    //         const ll c = (a<<17 | b>>18) - b*25, d = c * e10d10 & 0x7800780078007800ll; 
    //         const ll decimals = (c<<9 | d>>9) - d*5;
    //         lo8a = (decimals>>2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
    //     }
    //     memcpy(buffer, &lo8a, 8);
    // }


    // lo8a = 0x30;  // ASCII 0
    // // volatile ll ne32m10000 = -e32m10000;
    // // __asm__ volatile ("sal $32 %%rdx;" :"=a"(ne32m10000));
    // // asm volatile ("movabs rdx -42949672960000;" :"=a"(ne32m10000));
    // // __asm__ volatile ("movq %%rdx -42949672960000;" :"=a"(ne32m10000));
    // ll ne32m10000;
    // asm volatile ("movabs $-42949672960000LL, %%rcx;":"=a"(ne32m10000));
    // memset(buffer+7, 0, 4);
    // if (val >= 100000000) {
    //     const int hi2 = val/100000000;
    //     memcpy(buffer, dLut+hi2, 2);
    //     const i32 lo8 = val%100000000;
    //     buffer += val>=1000000000 ? 2 : 1;
    //     itoa8lut2(lo8, buffer, ne32m10000);
    // } else {
    //     if (val) {
    //         const ll decimals = itoa8lil(val, ne32m10000);
    //         lo8a = (decimals>>2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
    //     }
    //     memcpy(buffer, &lo8a, 8);
    // }

    // lo8a = 0x30;  // ASCII 0
    // memset(buffer+7, 0, 4);
    // if (val >= 100000000) {
    //     const u32 hi8 = val/100;
    //     char * const buf = val >= 1000000000 ? buffer : buffer-1;
    //     const i32 q = (ll) hi8 * e40d10000 >> 40;
    //     const i32 lo2 = val%100;
    //     memcpy(buf+8, gDigitsLut+lo2*2, 2);
    //     const ll a = -e32m10000*q + ((ll) hi8 << 32 | q);
    //     const ll b = a * e19d100 & 0x3f8000003f80000ll;
    //     const ll b25 = b*25, c = (a<<17) - b25;
    //     memcpy(buf, gDigitsLut + ((i32)b>>18), 2);
    //     // if(buffer == buf) memcpy(buffer, gDigitsLut + ((i32)b>>18), 2);
    //     // else memcpy(buffer, gDigitsLut+1 + ((i32)b>>18), 1);
    //     memcpy(buf+2, gDigitsLut + ((i32)c>>16), 2);
    //     memcpy(buf+4, gDigitsLut + (b>>50), 2);
    //     memcpy(buf+6, gDigitsLut + (c>>48), 2);

    //     // memcpy(buf+8, gDigitsLut+lo2*2, 2);
    // } else {
    //     if (val) {
    //         const ll decimals = itoa8lil(val);
    //         lo8a = (decimals>>2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
    //     }
    //     memcpy(buffer, &lo8a, 8);
    // }

    // FAST and safe
    // ll lo8a = 0x30;  // ASCII 0
    // memset(buffer+7, 0, 4);
    // if (val >= 100000000) {
    //     const u32 hi8 = val/100;
    //     char * const buf = val >= 1000000000 ? buffer : buffer-1;
    //     const i32 q = (ll) hi8 * e40d10000 >> 40;
    //     const i32 lo2 = val%100;
    //     memcpy(buf+8, gDigitsLut+lo2*2, 2);
    //     const ll a = -e32m10000*q + ((ll) hi8 << 32 | q);
    //     const ll b = a * e19d100 & 0x3f8000003f80000ll;
    //     const ll b25 = b*25, c = (a<<17) - b25;
    //     // memcpy(buffer, dLut + ((i32)b>>19), 2);
    //     memcpy(buffer, (char *) dLut + ((i32)b>>18), 2);
    //     memcpy(buf+2, gDigitsLut + ((i32)c>>16), 2);
    //     memcpy(buf+4, gDigitsLut + (b>>50), 2);
    //     memcpy(buf+6, gDigitsLut + (c>>48), 2);

    //     // memcpy(buf+8, gDigitsLut+lo2*2, 2);
    // } else {
    //     if (val) {
    //         const ll decimals = itoa8lil(val);
    //         lo8a = (decimals>>2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
    //     }
    //     memcpy(buffer, &lo8a, 8);
    // }

    // ll lo8a = '0'; 
    // memset(buffer+7, 0, 4);
    // if (val >= 100000000) {
    //     const u32 hi8 = val/100;
    //     char * const buf = val >= 1000000000 ? buffer : buffer-1;
    //     const i32 q = (ll) hi8 * e40d10000 >> 40;
    //     const i32 lo2 = val%100;
    //     memcpy(buf+8, gDigitsLut+lo2*2, 2);
    //     const ll a = -e32m10000*q + ((ll) hi8 << 32 | q);
    //     const ll b = a * e19d100 & 0x3f8000003f80000ll;
    //     const ll b25 = b*25, c = (a<<17) - b25;
    //     // memcpy(buffer, dLut + ((i32)b>>19), 2);
    //     memcpy(buffer, (char *) dLut + ((i32)b>>18), 2);
    //     memcpy(buf+2, gDigitsLut + ((i32)c>>16), 2);
    //     memcpy(buf+4, gDigitsLut + (b>>50), 2);
    //     memcpy(buf+6, gDigitsLut + (c>>48), 2);
    // } else {
    //     if (val) {
    //         const ll decimals = itoa8lil(val);
    //         lo8a = (decimals>>2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
    //     }
    //     memcpy(buffer, &lo8a, 8);
    // }

    // almost branchless
    // memset(buffer+7, 0, 4);
    // if (val < 100) {
    //     const i32 digit = val | '0';
    //     i32 toWrite;
    //     memcpy(&toWrite, gDigitsLut+val*2, 2);
    //     if (val < 10) toWrite = digit;
    //     memcpy(buffer, &toWrite, 4);
    //     return;
    // }
    // // memset(buffer+7, 0, 4);
    // const i32 hi8 = val/100;
    // const i32 q = (ll) hi8 * e40d10000 >> 40;
    // const i32 lo2 = val%100;
    // short lo2a; memcpy(&lo2a, gDigitsLut+lo2*2, 2);
    // const ll a = ((ll) hi8 << 32 | q) - q * e32m10000;
    // const ll b = a * e19d100 & 0x3f8000003f80000ll;
    // const ll c = (a<<17 | b>>18) - b*25, d = c * e10d10 & 0x7800780078007800ll;
    // const ll hi8d = (c<<9 | d>>9) - d*5;
    // const int shifts = 56 & __builtin_ctzll(hi8d);
    // const ll lo8a = (hi8d>>2 | ascii0s) >> shifts;
    // memcpy(buffer, &lo8a, 8);
    // memcpy(buffer+8-(shifts>>3), &lo2a, 2);


    // BETTER trailing zero count
    // memset(buffer+7, 0, 4);
    // if (val < 100) {
    //     const i32 digit = val | '0';
    //     i32 toWrite;
    //     memcpy(&toWrite, gDigitsLut+val*2, 2);
    //     if (val < 10) toWrite = digit;
    //     memcpy(buffer, &toWrite, 4);
    //     return;
    // }
    // // memset(buffer+7, 0, 4);
    // const i32 hi8 = val/100;
    // const i32 q = (ll) hi8 * e40d10000 >> 40;
    // const i32 lo2 = val%100;
    // short lo2a; memcpy(&lo2a, gDigitsLut+lo2*2, 2);
    // const ll a = ((ll) hi8 << 32 | q) - q * e32m10000;
    // const ll b = a * e19d100 & 0x3f8000003f80000ll;
    // const ll c = (a<<17 | b>>18) - b*25, d = c * e10d10 & 0x7800780078007800ll;
    // const ll hi8d = (c<<9 | d>>9) - d*5;
    // const int shifts = 56 & __builtin_ctzll(c<<9 | d>>9);
    // const ll lo8a = (hi8d>>2 | ascii0s) >> shifts;
    // memcpy(buffer, &lo8a, 8);
    // memcpy(buffer+8-(shifts>>3), &lo2a, 2);


    // memset(buffer+7, 0, 4);
    // if (val < 100) {
    //     const i32 digit = val | '0';
    //     i32 toWrite;
    //     memcpy(&toWrite, gDigitsLut+val*2, 2);
    //     if (val < 10) toWrite = digit;
    //     memcpy(buffer, &toWrite, 4);
    //     return;
    // }
    // // memset(buffer+7, 0, 4);
    // const i32 hi8 = val/100;
    // const i32 q = (ll) hi8 * e40d10000 >> 40;
    // const i32 lo2 = val%100;
    // short lo2a; memcpy(&lo2a, gDigitsLut+lo2*2, 2);
    // const ll a = ((ll) hi8 << 32 | q) - q * e32m10000;
    // const ll b = a * e19d100 & 0x3f8000003f80000ll;
    // const ll c = (a<<17 | b>>18) - b*25, d = c * e10d10 & 0x7800780078007800ll;
    // const ll hi8d = (c<<9 | d>>9) - d*5;
    // const int shifts = 56 & __builtin_ctzll(c<<9 | d>>9);
    // const ll lo8a = (hi8d>>2 | ascii0s) >> shifts;
    // memcpy(buffer, &lo8a, 8);
    // memcpy(buffer+8-(shifts>>3), &lo2a, 2);

    // ll lo8a = '0'; 
    // memset(buffer+7, 0, 4);
    // if (val >= 100000000) {
    //     const u32 hi8 = val/100;
    //     char * const buf = val >= 1000000000 ? buffer : buffer-1;
    //     const i32 q = (ll) hi8 * e40d10000 >> 40;
    //     const i32 lo2 = val%100;
    //     memcpy(buf+8, gDigitsLut+lo2*2, 2);
    //     const ll a = -e32m10000*q + ((ll) hi8 << 32 | q);
    //     const ll b = a * e19d100 & 0x3f8000003f80000ll;
    //     const ll b25 = b*25, c = (a<<17) - b25;
    //     memcpy(buffer, (char *) dLut + ((i32)b>>18), 2);
    //     memcpy(buf+2, gDigitsLut + ((i32)c>>16), 2);
    //     memcpy(buf+4, gDigitsLut + (b>>50), 2);
    //     memcpy(buf+6, gDigitsLut + (c>>48), 2);
    // } else {
    //     if (val) {
    //         const i32 q = (ll) val * e40d10000 >> 40;
    //         const ll a = ((ll) val << 32 | q) - q * e32m10000;
    //         const ll b = a * e19d100 & 0x3f8000003f80000ll;
    //         const ll c = (a<<17 | b>>18) - b*25, d = c * e10d10 & 0x7800780078007800ll;
    //         const ll decimals = (c<<9 | d>>9) - d*5;
    //         const int shifts = 56 & __builtin_ctzll(c<<9 | d>>9);
    //         lo8a = (decimals>>2 | ascii0s) >> shifts;
    //     }
    //     memcpy(buffer, &lo8a, 8);
    // }

    // ll lo8a = '0'; 
    // memset(buffer+7, 0, 4);
    // if (val >= 100000000) {
    //     const u32 hi8 = val/100;
    //     char * const buf = val >= 1000000000 ? buffer : buffer-1;
    //     const i32 q = (ll) hi8 * e40d10000 >> 40;
    //     const i32 lo2 = val%100;
    //     memcpy(buf+8, gDigitsLut+lo2*2, 2);
    //     const ll a = -e32m10000*q + ((ll) hi8 << 32 | q);
    //     const ll b = a * e19d100 & 0x3f8000003f80000ll;
    //     const ll b25 = b*25, c = (a<<17) - b25;
    //     memcpy(buffer, (char *) dLut + ((i32)b>>18), 2);
    //     memcpy(buf+2, gDigitsLut + ((i32)c>>16), 2);
    //     memcpy(buf+4, gDigitsLut + (b>>50), 2);
    //     memcpy(buf+6, gDigitsLut + (c>>48), 2);
    // } else {
    //     if (val) {
    //         const i32 q = (ll) val * e40d10000 >> 40;
    //         const ll a = ((ll) val << 32 | q) - q * e32m10000;
    //         const ll b = a * e19d100 & 0x3f8000003f80000ll;
    //         const ll c = (a<<17 | b>>18) - b*25, d = c * e10d10 & 0x7800780078007800ll;
    //         const ll decimals = (c<<9 | d>>9) - d*5;
    //         const int shifts = 56 & __builtin_ctzll(c<<9 | d>>9);
    //         lo8a = (decimals>>2 | ascii0s) >> shifts;
    //         memcpy(buffer, &lo8a, 8);
    //     }
    //     else memcpy(buffer, &lo8a, 2);
    // }

    // ll lo8a = '0';
    // memset(buffer+7, 0, 4);
    // if (val > 99999999) {
    //     const u32 hi8 = val/100;
    //     char * const buf = val >= 1000000000 ? buffer : buffer-1;
    //     const i32 q = (ll) hi8 * e40d10000 >> 40;
    //     const i32 lo2 = val%100;
    //     memcpy(buf+8, gDigitsLut+lo2*2, 2);
    //     const ll a = -e32m10000*q + ((ll) hi8 << 32 | q);
    //     const ll b = a * e19d100 & 0x3f8000003f80000ll;
    //     const ll b25 = b*25, c = (a<<17) - b25;
    //     memcpy(buffer, (char *) dLut + ((i32)b>>18), 2);
    //     memcpy(buf+2, gDigitsLut + ((i32)c>>16), 2);
    //     memcpy(buf+4, gDigitsLut + (b>>50), 2);
    //     memcpy(buf+6, gDigitsLut + (c>>48), 2);
    // } else if (val) {
    //     const i32 q = (ll) val * e40d10000 >> 40;
    //     const ll a = ((ll) val << 32 | q) - q * e32m10000;
    //     const ll b = a * e19d100 & 0x3f8000003f80000ll;
    //     const ll c = (a<<17 | b>>18) - b*25, d = c * e10d10 & 0x7800780078007800ll;
    //     const ll decimals = (c<<9 | d>>9) - d*5;
    //     const int shifts = 56 & __builtin_ctzll(c<<9 | d>>9);
    //     lo8a = (decimals>>2 | ascii0s) >> shifts;
    //     memcpy(buffer, &lo8a, 8);
    // }
    // else memcpy(buffer, &lo8a, 4);

    // FAST on Jun 11
    // ll lo8a = '0';
    // memset(buffer+7, 0, 4);
    // if (val > 99999999) {
    //     const u32 hi8 = val/100;
    //     char * const buf = val >= 1000000000 ? buffer : buffer-1;
    //     const i32 q = (ll) hi8 * e40d10000 >> 40;
    //     const i32 lo2 = val%100;
    //     memcpy(buf+8, gDigitsLut+lo2*2, 2);
    //     const ll a = -e32m10000*q + ((ll) hi8 << 32 | q);
    //     const ll b = a * e19d100 & 0x3f8000003f80000ll;
    //     const ll b25 = b*25, c = (a<<17) - b25;
    //     memcpy(buffer, (char *) dLut + ((i32)b>>18), 2);
    //     memcpy(buf+2, gDigitsLut + ((i32)c>>16), 2);
    //     memcpy(buf+4, gDigitsLut + (b>>50), 2);
    //     memcpy(buf+6, gDigitsLut + (c>>48), 2);
    // } else if (val) {
    //     const i32 q = (ll) val * e40d10000 >> 40;
    //     const ll a = ((ll) val << 32 | q) - q * e32m10000;
    //     const ll b = a * e19d100 & 0x3f8000003f80000ll;
    //     const ll c = (a<<17 | b>>18) - b*25, d = c * e10d10 & 0x7800780078007800ll;
    //     const ll decimals = (c<<9 | d>>9) - d*5;
    //     lo8a = (decimals>>2 | ascii0s) >> (56 & __builtin_ctzll(c<<9 | d>>9));
    //     memcpy(buffer, &lo8a, 8);
    // }
    // else memcpy(buffer, &lo8a, 4);

    // ll lo8a = '0';
    // memset(buffer+7, 0, 4);
    // if (val > 99999999) {
    //     const u32 hi8 = val/100;
    //     char * const buf = val >= 1000000000 ? buffer : buffer-1;
    //     const i32 q = (ll) hi8 * e40d10000 >> 40;
    //     const i32 lo2 = val%100;
    //     memcpy(buf+8, gDigitsLut+lo2*2, 2);
    //     const ll a = -e32m10000*q + ((ll) hi8 << 32 | q);
    //     const ll b = a * e19d100 & 0x3f8000003f80000ll;
    //     const ll b25 = b*25, c = (a<<17) - b25;
    //     memcpy(buffer, (char *) dLut + ((i32)b>>18), 2);
    //     memcpy(buf+2, gDigitsLut + ((i32)c>>16), 2);
    //     memcpy(buf+4, gDigitsLut + (b>>50), 2);
    //     memcpy(buf+6, gDigitsLut + (c>>48), 2);
    // } else if (val) {
    //     const i32 q = (ll) val * e40d10000 >> 40;
    //     const ll a = ((ll) val << 32 | q) - q * e32m10000;
    //     const ll b = a * e19d100 & 0x3f8000003f80000ll;
    //     const ll c = (a<<17 | b>>18) - b*25, d = c * e10d10 & 0x7800780078007800ll;
    //     const ll decimals = (c<<9 | d>>9) - d*5;
    //     lo8a = (decimals>>2 | ascii0s) >> (56 & __builtin_ctzll(c<<9 | d>>9));
    //     memcpy(buffer, &lo8a, 8);
    // }
    // else memcpy(buffer, &lo8a, 4);

    // FAST on Jun 11
    // ll lo8a = '0';
    // memset(buffer+7, 0, 4);
    // if (val > 99999999) {
    //     const u32 hi8 = val/100;
    //     char * const buf = val >= 1000000000 ? buffer : buffer-1;
    //     const i32 q = (ll) hi8 * e40d10000 >> 40;
    //     const i32 lo2 = val%100;
    //     short lo2a; memcpy(&lo2a, gDigitsLut+lo2*2, 2);
    //     const ll a = -e32m10000*q + ((ll) hi8 << 32 | q);
    //     memcpy(buf+8, &lo2a, 2);
    //     const ll b = a * e19d100 & 0x3f8000003f80000ll;
    //     const ll b25 = b*25, c = (a<<17) - b25;
    //     memcpy(buffer, (char *) dLut + ((i32)b>>18), 2);
    //     memcpy(buf+2, gDigitsLut + ((i32)c>>16), 2);
    //     memcpy(buf+4, gDigitsLut + (b>>50), 2);
    //     memcpy(buf+6, gDigitsLut + (c>>48), 2);
    // } else if (val) {
    //     const i32 q = (ll) val * e40d10000 >> 40;
    //     const ll a = ((ll) val << 32 | q) - q * e32m10000;
    //     const ll b = a * e19d100 & 0x3f8000003f80000ll;
    //     const ll c = (a<<17 | b>>18) - b*25, d = c * e10d10 & 0x7800780078007800ll;
    //     const ll decimals = (c<<9 | d>>9) - d*5;
    //     lo8a = (decimals>>2 | ascii0s) >> (56 & __builtin_ctzll(c<<9 | d>>9));
    //     memcpy(buffer, &lo8a, 8);
    // }
    // else memcpy(buffer, &lo8a, 4);

    // ll lo8a = '0';
    // memset(buffer+7, 0, 4);
    // if (val > 99999999) {
    //     const u32 hi8 = val/100;
    //     char * const buf = val >= 1000000000 ? buffer : buffer-1;
    //     const i32 q = (ll) hi8 * e40d10000 >> 40;
    //     const i32 lo2 = val%100;
    //     short lo2a; memcpy(&lo2a, gDigitsLut+lo2*2, 2);
    //     const ll a = -e32m10000*q + ((ll) hi8 << 32 | q);
    //     memcpy(buf+8, &lo2a, 2);
    //     const ll b = a * e19d100 & 0x3f8000003f80000ll;
    //     const ll b25 = b*25, c = (a<<17) - b25;
    //     memcpy(buffer, (char *) dLut + ((i32)b>>18), 2);
    //     memcpy(buf+2, gDigitsLut + ((i32)c>>16), 2);
    //     memcpy(buf+4, gDigitsLut + (b>>50), 2);
    //     memcpy(buf+6, gDigitsLut + (c>>48), 2);
    // } else if (val) {
    //     const i32 q = (ll) val * e40d10000 >> 40;
    //     const ll a = ((ll) val << 32 | q) - q * e32m10000;
    //     const ll b = a * e19d100 & 0x3f8000003f80000ll;
    //     const ll c = (a<<17 | b>>18) - b*25, d = c * e10d10 & 0x7800780078007800ll;
    //     const ll decimals = (c<<9 | d>>9) - d*5;
    //     lo8a = (decimals>>2 | ascii0s) >> (56 & __builtin_ctzll(c<<9 | d>>9));
    //     memcpy(buffer, &lo8a, 8);
    // }
    // else memcpy(buffer, &lo8a, 4);

    // ll lo8a = '0';
    // memset(buffer+7, 0, 4);
    // if (val > 99999999) {
    //     const u32 hi8 = val/100;
    //     char * const buf = val >= 1000000000 ? buffer : buffer-1;
    //     const i32 q = (ll) hi8 * e40d10000 >> 40;
    //     // const i32 lo2 = val%100;
    //     short lo2a; memcpy(&lo2a, (gDigitsLut+(ll)val*2)+((ll)hi8*(-200)), 2);
    //     const ll a = -e32m10000*q + ((ll) hi8 << 32 | q);
    //     memcpy(buf+8, &lo2a, 2);
    //     const ll b = a * e19d100 & 0x3f8000003f80000ll;
    //     const ll b25 = b*25, c = (a<<17) - b25;
    //     memcpy(buffer, (char *) dLut + ((i32)b>>18), 2);
    //     memcpy(buf+2, gDigitsLut + ((i32)c>>16), 2);
    //     memcpy(buf+4, gDigitsLut + (b>>50), 2);
    //     memcpy(buf+6, gDigitsLut + (c>>48), 2);
    // } else if (val) {
    //     const i32 q = (ll) val * e40d10000 >> 40;
    //     const ll a = ((ll) val << 32 | q) - q * e32m10000;
    //     const ll b = a * e19d100 & 0x3f8000003f80000ll;
    //     const ll c = (a<<17 | b>>18) - b*25, d = c * e10d10 & 0x7800780078007800ll;
    //     const ll decimals = (c<<9 | d>>9) - d*5;
    //     lo8a = (decimals>>2 | ascii0s) >> (56 & __builtin_ctzll(c<<9 | d>>9));
    //     memcpy(buffer, &lo8a, 8);
    // }
    // else memcpy(buffer, &lo8a, 4);

    // memcpy(buf+2, gDigitsLut + ((i32)a<<1) - ((i32)b25>>16), 2);

    // Might be faster on newer archs
    // ll lo8a = '0';
    // memset(buffer+7, 0, 4);
    // if (val > 99999999) {
    //     const u32 hi8 = val/100;
    //     char * const buf = val >= 1000000000 ? buffer : buffer-1;
    //     const i32 q = (ll) hi8 * e40d10000 >> 40;
    //     short lo2a; memcpy(&lo2a, gDigitsLut+(ll)val*2 + ((ll)hi8*(-200)), 2);
    //     memcpy(buf+8, &lo2a, 2);
    //     const ll a = -e32m10000*q + ((ll) hi8 << 32 | q);
    //     const ll b = a * e19d100 & 0x3f8000003f80000ll;
    //     const ll b25 = b*25, c = (a<<17) - b25;
    //     memcpy(buffer, (char *) dLut + ((i32)b>>18), 2);
    //     memcpy(buf+2, gDigitsLut + ((i32)c>>16), 2);
    //     memcpy(buf+4, gDigitsLut + (b>>50), 2);
    //     memcpy(buf+6, gDigitsLut + (c>>48), 2);
    // } else if (val) {
    //     const i32 q = (ll) val * e40d10000 >> 40;
    //     const ll a = ((ll) val << 32 | q) - q * e32m10000;
    //     const ll b = a * e19d100 & 0x3f8000003f80000ll;
    //     const ll c = (a<<17 | b>>18) - b*25, d = c * e10d10 & 0x7800780078007800ll;
    //     const ll decimals = (c<<9 | d>>9) - d*5;
    //     lo8a = (decimals>>2 | ascii0s) >> (56 & __builtin_ctzll(c<<9 | d>>9));
    //     memcpy(buffer, &lo8a, 8);
    // }
    // else memcpy(buffer, &lo8a, 4);

    ll lo8a = '0';
    memset(buffer+7, 0, 4);
    if (val > 99999999) {
        const u32 hi8 = val/100;
        char * const buf = val >= 1000000000 ? buffer : buffer-1;
        const i32 q = (ll) hi8 * e40d10000 >> 40;
        const i32 lo2 = val%100;
        short lo2a; memcpy(&lo2a, gDigitsLut+lo2*2, 2);
        const ll a = -e32m10000*q + ((ll) hi8 << 32 | q);
        memcpy(buf+8, &lo2a, 2);
        const ll b = a * e19d100 & 0x3f8000003f80000ll;
        const ll b25 = b*25, c = (a<<17) - b25;
        memcpy(buffer, (char *) dLut + ((i32)b>>18), 2);
        memcpy(buf+2, gDigitsLut + ((i32)c>>16), 2);
        memcpy(buf+4, gDigitsLut + (b>>50), 2);
        memcpy(buf+6, gDigitsLut + (c>>48), 2);
    } else if (val) {
        const i32 q = (ll) val * e40d10000 >> 40;
        const ll a = ((ll) val << 32 | q) - q * e32m10000;
        const ll b = a * e19d100 & 0x3f8000003f80000ll;
        const ll c = (a<<17 | b>>18) - b*25, d = c * e10d10 & 0x7800780078007800ll;
        const ll decimals = (c<<9 | d>>9) - d*5;
        lo8a = (decimals>>2 | ascii0s) >> (56 & __builtin_ctzll(c<<9 | d>>9));
        memcpy(buffer, &lo8a, 8);
    }
    else memcpy(buffer, &lo8a, 4);
}



inline void i32toa_hintro(i32 value, char* buffer) {
    // u32 u = static_cast<u32>(value);
    // if (value < 0) {
    //     *buffer++ = '-';
    //     u = ~u + 1;
    // }
    // u32toa_naive(u, buffer);

    // u32 u = static_cast<u32>(value);
    // if (value < 0) {
    //     *buffer = '-';
    //     u = ~u + 1;
    // }
    // u32toa_naive(u, buffer+(value<0));

    // const uint_fast64_t val = value < 0 ? ~value + 1 : value;
    // const u32 negMask = value < 0 ? -1 : 0, negShift = 8 & negMask, negSign = '-' & negMask;
    // ull lo8a;
    // u32 hi2a = 0, hi2aSize = 2 - negMask;
    // const ull ascii0s = 0x3030303030303030ull;
    // if (val >= 100000000) {
    //     const u32 e9d10 = 52, hi2 = val * e57d8 >> 57, lo8 = val - hi2 * 100000000;
    //     lo8a = itoa8lil(lo8) >> 2 | ascii0s;
    //     const u32 hi1d = hi2*e9d10 >> 9, adjustSize = hi1d ? 0 : 1, shift = hi1d ? 0 : 8;
    //     hi2a = (((hi2 << 8) + (uint16_t) ascii0s | hi1d)>>shift) - (hi2*e9d10 & 0xe00) * 5;
    //     hi2aSize -= adjustSize;
    // } else {
    //     const ull decimals = itoa8lil(val), tmp = (decimals >> 2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
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
    // const ull ascii0s = 0x3030303030303030ull;
    // if (val >= 100000000) {
    //     const uint_fast32_t e9d10 = 52, hi2 = val * e57d8 >> 57, lo8 = val - hi2 * 100000000;
    //     lo8a = itoa8lil(lo8) >> 2 | ascii0s;
    //     const uint_fast8_t hi1d = hi2*e9d10 >> 9, adjustSize = hi1d ? 0 : 1, shift = hi1d ? 0 : 8;
    //     hi2a = (((hi2 << 8) + (uint_fast16_t) ascii0s | hi1d)>>shift) - (hi2*e9d10 & 0xe00) * 5;
    //     hi2aSize = hi2aSize - negMask - adjustSize;
    // } else {
    //     const ull decimals = itoa8lil(val), tmp = (decimals >> 2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
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
    // const ull ascii0s = 0x3030303030303030ull;
    // if (val >= 100000000) {
    //     const uint_fast32_t e9d10 = 52, hi2 = val * e57d8 >> 57, lo8 = val - hi2 * 100000000;
    //     lo8a = itoa8lil(lo8) >> 2 | ascii0s;
    //     const uint_fast8_t hi1d = hi2*e9d10 >> 9, adjustSize = hi1d ? 0 : 1, shift = hi1d ? 0 : 8;
    //     hi2a = (((hi2 << 8) + (uint_fast16_t) ascii0s | hi1d)>>shift) - (hi2*e9d10 & 0xe00) * 5;
    //     hi2aSize = hi2aSize - negMask - adjustSize;
    // } else {
    //     const ull decimals = itoa8lil(val), tmp = (decimals >> 2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
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
    // const ull ascii0s = 0x3030303030303030ull;
    // if (val >= 100000000) {
    //     const uint_fast32_t e9d10 = 52, hi2 = val * e57d8 >> 57, lo8 = val - hi2 * 100000000;
    //     lo8a = itoa8lil(lo8) >> 2 | ascii0s;
    //     const uint_fast8_t hi1d = hi2*e9d10 >> 9, adjustSize = hi1d ? 0 : 1, shift = hi1d ? 0 : 8;
    //     hi2a = (((hi2 << 8) + (uint_fast16_t) ascii0s | hi1d)>>shift) - (hi2*e9d10 & 0xe00) * 5;
    //     hi2aSize = hi2aSize + (negShift ? 1 : 0) - adjustSize;
    // } else {
    //     const ull decimals = itoa8lil(val), tmp = (decimals >> 2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
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
    // const ull ascii0s = 0x3030303030303030ull;
    // if (val >= 100000000) {
    //     const uint_fast32_t e9d10 = 52, hi2 = val * e57d8 >> 57, lo8 = val - hi2 * 100000000;
    //     lo8a = itoa8lil(lo8) >> 2 | ascii0s;
    //     const uint_fast8_t hi1d = hi2*e9d10 >> 9, adjustSize = hi1d ? 0 : 1, shift = hi1d ? 0 : 8;
    //     hi2a = (((hi2 << 8) + (uint_fast16_t) ascii0s | hi1d)>>shift) - (hi2*e9d10 & 0xe00) * 5;
    //     hi2aSize -= adjustSize;
    // } else {
    //     const ull decimals = itoa8lil(val), tmp = (decimals >> 2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
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
    // const ull ascii0s = 0x3030303030303030ull;
    // if (val >= 100000000) {
    //     const uint_fast32_t e9d10 = 52, hi2 = val * e57d8 >> 57, lo8 = val - hi2 * 100000000;
    //     lo8a = itoa8lil(lo8) >> 2 | ascii0s;
    //     const uint_fast8_t hi1d = hi2*e9d10 >> 9, adjustSize = hi1d ? 0 : 1, shift = hi1d ? 0 : 8;
    //     hi2a = (((hi2 << 8) + (uint_fast16_t) ascii0s | hi1d)>>shift) - (hi2*e9d10 & 0xe00) * 5;
    //     hi2aSize = hi2aSize - negMask - adjustSize;
    // } else {
    //     const ull decimals = itoa8lil(val), tmp = (decimals >> 2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
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
    // const ull ascii0s = 0x3030303030303030ull;
    // if (val >= 100000000) {
    //     const uint_fast32_t e9d10 = 52, hi2 = val * e57d8 >> 57, lo8 = val - hi2 * 100000000;
    //     lo8a = itoa8lil(lo8) >> 2 | ascii0s;
    //     const uint_fast8_t hi1d = hi2*e9d10 >> 9, adjustSize = hi1d ? 0 : 1, shift = hi1d ? 0 : 8;
    //     hi2a = (((hi2 << 8) + (uint_fast16_t) ascii0s | hi1d)>>shift) - (hi2*e9d10 & 0xe00) * 5;
    //     hi2aSize = hi2aSize - negMask - adjustSize;
    // } else {
    //     const ull decimals = itoa8lil(val), tmp = (decimals >> 2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
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
    // const ull ascii0s = 0x3030303030303030ull;
    // if (val >= 100000000) {
    //     const uint_fast32_t e9d10 = 52, hi2 = val * e57d8 >> 57, lo8 = val - hi2 * 100000000;
    //     lo8a = itoa8lil(lo8) >> 2 | ascii0s;
    //     const uint_fast8_t hi1d = hi2*e9d10 >> 9, adjustSize = hi1d ? 0 : 1, shift = hi1d ? 0 : 8;
    //     hi2a = (((hi2 << 8) + (uint_fast16_t) ascii0s | hi1d)>>shift) - (hi2*e9d10 & 0xe00) * 5;
    //     hi2aSize = hi2aSize - negMask - adjustSize;
    // } else {
    //     const ull decimals = itoa8lil(val), tmp = (decimals >> 2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
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
    //     const ull decimals = itoa8lil(val), tmp = (decimals >> 2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
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
    // const ull ascii0s = 0x3030303030303030ull;
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
    //     const ull decimals = itoa8lil(val), tmp = (decimals >> 2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
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

    // const ll tmp = '-';
    // memcpy(buffer, &tmp, 8);
    // memset(buffer+8, 0, 4);
    // u32 val = (u32&) value;
    // if (value<0) {
    //     val = -val;
    //     ++buffer;
    // }
    // lo8a = 0x30;  // ASCII 0
    // if (val >= 100000000) {
    //     const i32 hi2 = val/100000000;
    //     memcpy(buffer, dLut+hi2, 2);
    //     const i32 lo8 = val%100000000;
    //     buffer += val>=1000000000 ? 2 : 1;
    //     itoa8lut(lo8, buffer);
    // } else {
    //     if (val) {
    //         const ll decimals = itoa8lil(val);
    //         lo8a = (decimals>>2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
    //     }
    //     memcpy(buffer, &lo8a, 8);
    // }


    // const ll tmp = '-';
    // memcpy(buffer, &tmp, 4);
    // if (value<0) {
    //     value = -value;
    //     ++buffer;
    // }
    // u32toa_hintro(value, buffer);

    const ll tmp = '-';
    memcpy(buffer, &tmp, 8);
    memset(buffer+8, 0, 4);
    u32 val = (u32&) value;
    if (value<0) {
        val = -val;
        ++buffer;
    }
    lo8a = 0x30;  // ASCII 0
    if (val >= 100000000) {
        const i32 hi2 = val/100000000;
        memcpy(buffer, dLut+hi2, 2);
        const i32 lo8 = val%100000000;
        buffer += val>=1000000000 ? 2 : 1;
        itoa8lut(lo8, buffer);
    } else {
        if (val) {
            const ll decimals = itoa8lil(val);
            lo8a = (decimals>>2 | ascii0s) >> (56 & __builtin_ctzll(decimals));
        }
        memcpy(buffer, &lo8a, 8);
    }
}

void u64toa_hintro(ull value, char* buffer) {
    char temp[20];
    char *p = temp;
    do {
        *p++ = char(value % 10) + '0';
        value /= 10;
    } while (value > 0);

    do {
        *buffer++ = *--p;
    } while (p != temp);

    *buffer = '\0';
}

void i64toa_hintro(ll value, char* buffer) {
    ull u = static_cast<ull>(value);
    if (value < 0) {
        *buffer++ = '-';
        u = ~u + 1;
    }
    u64toa_hintro(u, buffer);
}

REGISTER_TEST(hintro);

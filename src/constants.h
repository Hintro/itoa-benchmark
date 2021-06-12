#include <stdint.h>

using ll = long long;
using ull = unsigned long long;
using i32 = int32_t;
using u32 = uint32_t;

// template <class T>
// __attribute__((always_inline)) inline void DoNotOptimize(const T &value) {
//   asm volatile("" : "+r"(const_cast<T &>(value)));
// }

template <class T>
__attribute__((always_inline)) inline T& DoNotOptimize(const T &value) {
    asm volatile("" : "+r"(const_cast<T&>(value)));
    return const_cast<T&>(value);
}

const i32 e19d100 = (1<<19) /100 + 1, e10d10 = 103, e23d1e3 = (1<<23) /1000 + 1;
const ll e40d10000 = (1ll << 40) /10000 + 1, e32m10000 = (1ll << 32) * 10000, 
ascii0s = 0x3030303030303030ll, e32d1e4 = (1ll<<32) /10000 + 1;
// e57d8 = 1441151881, e9d10 = 52,

// const uint16_t dLut[] = {
//        0,  '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8',  '9',
//     '01', '11', '21', '31', '41', '51', '61', '71', '81', '91',
//     '02', '12', '22', '32', '42', '52', '62', '72', '82', '92',
//     '03', '13', '23', '33', '43', '53', '63', '73', '83', '93',
//     '04', '14', '24', '34', '44', '54', '64', '74', '84', '94',
//     '05', '15', '25', '35', '45', '55', '65', '75', '85', '95',
//     '06', '16', '26', '36', '46', '56', '66', '76', '86', '96',
//     '07', '17', '27', '37', '47', '57', '67', '77', '87', '97',
//     '08', '18', '28', '38', '48', '58', '68', '78', '88', '98',
//     '09', '19', '29'
// };

const short dLut[] = {
       0,  '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8',  '9',
    '01', '11', '21', '31', '41', '51', '61', '71', '81', '91',
    '02', '12', '22', '32', '42', '52', '62', '72', '82', '92',
    '03', '13', '23', '33', '43', '53', '63', '73', '83', '93',
    '04', '14', '24'
};

// const char const Luts[] = {
//      0 , 0 ,'1', 0 ,'2', 0 ,'3', 0 ,'4', 0 ,'5', 0 ,'6', 0 ,'7', 0 ,'8', 0 ,'9', 0 ,
//     '1','0','1','1','1','2','1','3','1','4','1','5','1','6','1','7','1','8','1','9',
//     '2','0','2','1','2','2','2','3','2','4','2','5','2','6','2','7','2','8','2','9',
//     '3','0','3','1','3','2','3','3','3','4','3','5','3','6','3','7','3','8','3','9',
//     '4','0','4','1','4','2',

//     '0','0','0','1','0','2','0','3','0','4','0','5','0','6','0','7','0','8','0','9',
//     '1','0','1','1','1','2','1','3','1','4','1','5','1','6','1','7','1','8','1','9',
//     '2','0','2','1','2','2','2','3','2','4','2','5','2','6','2','7','2','8','2','9',
//     '3','0','3','1','3','2','3','3','3','4','3','5','3','6','3','7','3','8','3','9',
//     '4','0','4','1','4','2','4','3','4','4','4','5','4','6','4','7','4','8','4','9',
//     '5','0','5','1','5','2','5','3','5','4','5','5','5','6','5','7','5','8','5','9',
//     '6','0','6','1','6','2','6','3','6','4','6','5','6','6','6','7','6','8','6','9',
//     '7','0','7','1','7','2','7','3','7','4','7','5','7','6','7','7','7','8','7','9',
//     '8','0','8','1','8','2','8','3','8','4','8','5','8','6','8','7','8','8','8','9',
//     '9','0','9','1','9','2','9','3','9','4','9','5','9','6','9','7','9','8','9','9'
// };

const char decLut[] = {
    0,0, 1,0, 2,0, 3,0, 4,0, 5,0, 6,0, 7,0, 8,0, 9,0,
    0,1, 1,1, 2,1, 3,1, 4,1, 5,1, 6,1, 7,1, 8,1, 9,1,
    0,2, 1,2, 2,2, 3,2, 4,2, 5,2, 6,2, 7,2, 8,2, 9,2,
    0,3, 1,3, 2,3, 3,3, 4,3, 5,3, 6,3, 7,3, 8,3, 9,3,
    0,4, 1,4, 2,4, 3,4, 4,4, 5,4, 6,4, 7,4, 8,4, 9,4,
    0,5, 1,5, 2,5, 3,5, 4,5, 5,5, 6,5, 7,5, 8,5, 9,5,
    0,6, 1,6, 2,6, 3,6, 4,6, 5,6, 6,6, 7,6, 8,6, 9,6,
    0,7, 1,7, 2,7, 3,7, 4,7, 5,7, 6,7, 7,7, 8,7, 9,7,
    0,8, 1,8, 2,8, 3,8, 4,8, 5,8, 6,8, 7,8, 8,8, 9,8,
    0,9, 1,9, 2,9, 3,9, 4,9, 5,9, 6,9, 7,9, 8,9, 9,9,
};

// const short * const hi2Lut = (short*) Luts;
// const char * const DPsLut = Luts + 86;


const i32 powers[14] = {
    10000000, 10000000, 10000000, 1000000, 1000000, 100000, 10000, 10000, 1000, 1000, 100, 10, 10, 0
};

u32 const * const powersLut = (u32 *) powers - 2;

const ull shiftLut = 0x7665443321100000ull;

// For sequential optimization
const ull _198 = 0xc6c6c6c6c6c6c6c6ull, _208 = 0xd0d0d0d0d0d0d0d0ull, 
b10100000 = 0xa0a0a0a0a0a0a0a0ull, lo4bit = 0x0f0f0f0f0f0f0f0full, _192 = 0xc0c0c0c0c0c0c0c0ll;
const int init_mask = 0xff;
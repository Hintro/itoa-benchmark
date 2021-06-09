#include <stdint.h>

inline uint64_t itoa8lil(const uint32_t val);

class Itoa32
{
private:
    uint32_t E;
    int16_t last, off;
    // uint16_t last, Hi2; // E is the expected value; last is the last ASCII digit;
    // uint64_t Lo8a, mask; // hi2 and lo8 store big-endian ASCII digits
public:
    Itoa32();
    void itoa(const int32_t val, char * buffer);
    void uitoa(const uint32_t val, char * buffer);
};

// const int32_t e40d10000, e19d100, e10d10, e9d10;
// const int64_t e32m10000, ascii0s, e57d8;

// const uint16_t dLut[93];

// const int32_t powers[14];

// uint32_t const * const powersLut;

// const uint64_t shiftLut;

// // For sequential optimization
// const uint64_t _198, _208, b10100000, lo4bit, _192;
// const uint8_t init_mask;
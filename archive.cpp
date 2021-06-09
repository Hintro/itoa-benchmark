 
 /**
  * uint32: 2.88, 3.01, 6.883
  * 
    if ((val == E) & (lo8a<limit)) {
        memcpy(buffer, &hi2a, 2);
        buffer += off;
        buffer[8] = 0;
        lo8a += step;
        // buffer[8] = 0; // memcpy(buffer, &lo8a, 8);
    }
    else {
        lo8a = 0x30;  // ASCII 0
        hi2a = 0;
        shifts = 56;
        off = (val>=100000000);
        if (val >= 100000000) {
            const int32_t hi2 = val/100000000;
            hi2a = dLut[hi2];
            memcpy(buffer, &hi2a, 2);
            const int32_t lo8 = val%100000000;
            off += val >= 1000000000;
            buffer += off;
            lo8a = itoa8lil(lo8)>>2 | ascii0s;
            shifts = 0;
        } else if (val) {
            const int64_t decimals = itoa8lil(val);
            shifts = 56 & __builtin_ctzll(decimals);
            lo8a = (decimals>>2 | ascii0s) >> shifts;
        }
        buffer[8] = 0; // memcpy(buffer, &lo8a, 8);
        step = 0x100000000000000ll >> shifts;
        limit = 0x3900000000000000ll >> shifts;
    }
    memcpy(buffer, &lo8a, 8);
    if(val+1) E = val+1;
**/


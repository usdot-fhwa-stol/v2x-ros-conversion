#include "cpp_message/Util.h"

namespace cpp_message {

    std::uint8_t reverseBitsUint8(std::uint8_t x) {
        // 1. Swap adjacent 4-bit nibbles (abcd efgh -> efgh abcd)
        x = static_cast<std::uint8_t>(((x & 0xF0) >> 4) | ((x & 0x0F) << 4));
        
        // 2. Swap adjacent 2-bit pairs (efgh abcd -> ghef cdab)
        x = static_cast<std::uint8_t>(((x & 0xCC) >> 2) | ((x & 0x33) << 2));
        
        // 3. Swap adjacent single bits (ghef cdab -> hgfe dcba)
        x = static_cast<std::uint8_t>(((x & 0xAA) >> 1) | ((x & 0x55) << 1));
        
        return x;
    }

}
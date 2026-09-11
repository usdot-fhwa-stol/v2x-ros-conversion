#ifndef __CPP_MESSAGE_UTIL_H__
#define __CPP_MESSAGE_UTIL_H__

#include <cstdint>

namespace cpp_message{

    /// @brief Reverses the bits of a uint8_t value
    /// @param x The input value
    /// @return The value with reversed bits
    /// @details This function is used to convert bit string (least significant bit) to ros message representation (most significant bit first). 
    std::uint8_t reverseBitsUint8(std::uint8_t x);
}


#endif // __CPP_MESSAGE_UTIL_H__
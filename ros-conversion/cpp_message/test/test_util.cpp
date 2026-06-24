/*
 * Copyright (C) 2019-2022 LEIDOS.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License. You may obtain a copy of
 * the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */

#include "cpp_message/Util.h"
#include <bitset>
#include <gtest/gtest.h>


TEST(UtilTest, testReverseBitsUint8)
{
    // Test cases: input and expected output pairs
    std::vector<std::pair<std::uint8_t, std::uint8_t>> test_cases = {
        {0b00000000, 0b00000000}, // All bits 0
        {0b11111111, 0b11111111}, // All bits 1
        {0b10101010, 0b01010101}, // Alternating bits starting with 1
        {0b01010101, 0b10101010}, // Alternating bits starting with 0
        {0b11010010, 0b01001011}, // Random pattern
        {0b00001111, 0b11110000}, // Lower half bits set
        {0b11110000, 0b00001111}  // Upper half bits set
    };

    for (const auto& test_case : test_cases) {
        std::uint8_t input = test_case.first;
        std::uint8_t expected_output = test_case.second;
        std::uint8_t actual_output = cpp_message::reverseBitsUint8(input);
        EXPECT_EQ(actual_output, expected_output) << "Failed for input: " << std::bitset<8>(input);
    }
}
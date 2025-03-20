//
// Created by kandu on 11.12.2024.
//

#include <array>
#include <gtest/gtest.h>
#include "asncpp/numeric_string.h"


static const std::vector<std::pair<std::vector<uint8_t>, std::string_view> > test_cases = {

    {{0x12, 0x05, 0x31, 0x32, 0x33, 0x34, 0x35}, "12345"},

    {{0x12, 0x07, 0x30, 0x30, 0x30, 0x20, 0x31, 0x31, 0x31}, "000 111"},

    {
        {
            0x12, 0x13, 0x39, 0x20, 0x38, 0x20, 0x37, 0x20, 0x36, 0x20, 0x35, 0x20, 0x34, 0x20, 0x33, 0x20, 0x32, 0x20,
            0x31, 0x20, 0x30
        },
        "9 8 7 6 5 4 3 2 1 0"
    }
};


static const std::vector<std::pair<std::vector<uint8_t>, std::string_view> > test_cases_invalid = {

    {{0x12, 0x05, 0x31, 0x32, 0x41, 0x34, 0x35}, "Contains invalid character 'A'"},

    {{0x12, 0x05, 0x31, 0x32, 0x0A, 0x34, 0x35}, "Contains invalid control character '\\n'"},

    {{0x12, 0x04, 0x31, 0x32, 0x33}, "Incorrect length field (declared 4, but has 3 bytes)"}
};

TEST(numeric_string_test, encode) {
    for (const auto &[encoded_expected, expected]: test_cases) {
        numeric_string_t obj(expected);
        const std::vector<uint8_t> encoded = serialize(&obj);
        EXPECT_EQ(encoded, encoded_expected);
    }
}

TEST(numeric_string_test, decode) {
    for (const auto &[encoded, expected]: test_cases) {
        auto deserialized = asncpp::base::deserialize_v(encoded);
        auto ppp = deserialized.get();
        auto *ptr{static_cast<numeric_string_t *>(ppp)};
        EXPECT_EQ(ptr->value(), expected);
    }
}

//
// Created by kandu on 24.12.2024.
//
#include <gtest/gtest.h>
#include "asncpp/boolean.h"

const std::vector<std::pair<std::vector<uint8_t>, bool> > test_cases{
    {{0x01, 0x01, 0x00}, false},
    {{0x01, 0x01, 0xFF}, true}
};

TEST(boolean_test, decode) {
    for (const auto &[encoded, expected]: test_cases) {
        auto deserialized{asncpp::base::deserialize_v(encoded)};
        const auto *ptr{dynamic_cast<asncpp::types::boolean_t *>(deserialized.get())};
        EXPECT_EQ(static_cast<asncpp::types::boolean_t::value_type>(*ptr),
                  expected) << "Failed for encoded value: " << expected;
    }
}

TEST(boolean_test, encode) {
    for (const auto &[encoded, expected]: test_cases) {
        asncpp::types::boolean_t obj{expected};
        EXPECT_EQ(serialize(&obj), encoded) << "Failed for encoded value: " << expected;
    }
}

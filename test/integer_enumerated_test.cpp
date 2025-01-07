//
// Created by kandu on 04.12.2024.
//
#include <gtest/gtest.h>
#include "asncpp/base/common.h"
#include "asncpp/integer.h"

static const std::vector<std::pair<std::vector<uint8_t>, int64_t>> test_cases{
        {{0x02, 0x01, 0x00},                   0},
        {{0x02, 0x01, 0x7F},                   127},
        {{0x02, 0x02, 0x00, 0x80},             128},
        {{0x02, 0x02, 0x04, 0xD2},             1234},
        {{0x02, 0x03, 0x01, 0xE2, 0x40},       123456},

        {{0x02, 0x01, 0xFF},                   -1},
        {{0x02, 0x01, 0x80},                   -128},
        {{0x02, 0x02, 0xFF, 0x7F},             -129},
        {{0x02, 0x02, 0xFB, 0x2E},             -1234},
        {{0x02, 0x03, 0xFE, 0x1D, 0xC0},       -123456},


        {{0x02, 0x04, 0x7F, 0xFF, 0xFF, 0xFF}, INT32_MAX},
        {{0x02, 0x04, 0x80, 0x00, 0x00, 0x00}, INT32_MIN},

        {{0x02, 0x08, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, INT64_MAX},
        {{0x02, 0x08, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, INT64_MIN}
};

TEST(integer_enumerated_test, deserialize_test) {
    for (const auto &[encoded, expected]: test_cases) {
        auto deserialized{ asncpp::base::deserialize_v(encoded)};
        const auto* ptr{dynamic_cast<asncpp::types::integer_t*>(deserialized.get())};
        EXPECT_EQ(deserialized.get()->get<asncpp::types::integer_t>()->get_value(), expected) << "Failed for encoded value: " << expected;
        //EXPECT_EQ(ptr->get_value(), expected) << "Failed for encoded value: " << expected;
    }
}

TEST(integer_enumerated_test, serialize_test) {
    for (const auto &[encoded, expected]: test_cases) {
        asncpp::types::integer_t obj{expected};
        EXPECT_EQ(serialize(&obj), encoded) << "Failed for encoded value: " << expected;
    }
}



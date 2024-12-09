//
// Created by kandu on 04.12.2024.
//
#include <gtest/gtest.h>
#include "asncpp/asn_base.h"
#include "integer.h"

std::vector<std::pair<std::vector<uint8_t>, int64_t>> integer_test_cases = {
        // Положительные числа
        {{0x02, 0x01, 0x00},                   0},             // INTEGER 0
        {{0x02, 0x01, 0x7F},                   127},          // INTEGER 127
        {{0x02, 0x02, 0x00, 0x80},             128},    // INTEGER 128
        {{0x02, 0x02, 0x04, 0xD2},             1234},   // INTEGER 1234
        {{0x02, 0x03, 0x01, 0xE2, 0x40},       123456}, // INTEGER 123456

        // Отрицательные числа
        {{0x02, 0x01, 0xFF},                   -1},           // INTEGER -1
        {{0x02, 0x01, 0x80},                   -128},         // INTEGER -128
        {{0x02, 0x02, 0xFF, 0x7F},             -129},   // INTEGER -129
        {{0x02, 0x02, 0xFB, 0x2E},             -1234},  // INTEGER -1234
        {{0x02, 0x03, 0xFE, 0x1D, 0xC0},       -123456}, // INTEGER -123456

        // Большие числа
        {{0x02, 0x04, 0x7F, 0xFF, 0xFF, 0xFF}, 2147483647}, // Max 32-bit int
        {{0x02, 0x04, 0x80, 0x00, 0x00, 0x00}, -2147483648}, // Min 32-bit int

        {{0x02, 0x08, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, INT64_MAX}, // INTEGER 9223372036854775807

        // Минимальное 64-битное отрицательное число
        {{0x02, 0x08, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, INT64_MIN}  // INTEGER -9223372036854775808
};

TEST(ASN1BaseTest, DeserializeIntegerMultipleCases) {
    for (const auto &[encoded, expected]: integer_test_cases) {
        auto deserialized = deserialize_v(std::span<const uint8_t>(encoded.data(), encoded.size()));
        const integer_t *ptr = dynamic_cast<integer_t *>(deserialized.get());
        EXPECT_EQ(static_cast<integer_t::value_t>(*ptr), expected) << "Failed for encoded value: " << expected;
    }
}

TEST(ASN1BaseTest, SerializeIntegerMultipleCases) {
    for (const auto &[encoded, expected]: integer_test_cases) {
        integer_t obj(expected);
        const auto serialized = serialize(&obj);
        EXPECT_EQ(serialized, encoded) << "Failed for encoded value: " << expected;
    }
}

TEST(ASN1BaseTest, OverflowType) {
    //integer_t obj;
    using integer_inv = integer_base<uint32_t, asn1_tag::INTEGER>;
    EXPECT_ANY_THROW(
            deserialize<integer_inv >(
                    (integer_test_cases.end() - 1)->first //is too long, require 64 bits. But we have only 32
            )
    );
}

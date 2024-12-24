//
// Created by kandu on 24.12.2024.
//
#include <gtest/gtest.h>
#include "asncpp/bit_string.h"

std::vector<std::pair<std::vector<uint8_t>, std::vector<bool> > > bit_string_test_data = {
    {
        {0x03, 0x04, 0x03, 0xA0, 0xC3, 0xF0}, // Полный пакет: BIT STRING с длиной 4 байта и 3 неиспользованными битами
        {
            1, 0, 1, 0, 0, 0, 0, 0, // A0 (10100000)
            1, 1, 0, 0, 0, 0, 1, 1, // C3 (11000011)
            1, 1, 1, 1, 0
        } // F0 (11110000) без последних 3 бит
    },
    {
        {0x03, 0x03, 0x00, 0xFF, 0x80}, // Полный пакет: BIT STRING с длиной 3 байта и 0 неиспользованных битов
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0} // BIT STRING: представление в виде вектора бит
    },
    {
        {0x03, 0x02, 0x04, 0xA0}, // Полный пакет: BIT STRING с длиной 2 байта и 4 неиспользованными битами
        {1, 0, 1, 0} // BIT STRING: представление значащих битов
    }

};

TEST(bit_string_test, encode) {
    for (const auto &[encoded, expected]: bit_string_test_data) {
        bit_string_t obj{expected};
        const auto att = asncpp::base::serialize(&obj);
        EXPECT_EQ(att, encoded);
    }
}

// TEST(bit_string_test, decode) {
//     for (const auto &[encoded, expected]: bit_string_test_data) {
//         auto deserialized{asncpp::base::deserialize_v(encoded)};
//         const auto *ptr{dynamic_cast<bit_string_t *>(deserialized.get())};
//         EXPECT_EQ(ptr->value(), std::vector<uint8_t>{});
//     }
// }


TEST(bit_string_test, consructed_uint8_vector) {
    std::vector<uint8_t> bytes = {0b10110010, 0b11001100};
    size_t bitLength = 15; // Last bit of the second byte is unused.
    bit_string_t bitStr(bytes, bitLength);

    EXPECT_EQ(bitStr.value(), bytes);
    EXPECT_EQ(bitStr.bit_length(), bitLength);

    auto encoded = bitStr.encode();
    EXPECT_EQ(encoded, std::vector<uint8_t>({0x01, 0b10110010, 0b11001100}));
}


TEST(bit_string_test, consructed_string_view) {
    std::string_view bitStrView = "101100101";
    bit_string_t bitStr(bitStrView);

    EXPECT_EQ(bitStr.value(), std::vector<uint8_t>({0b10110010, 0b10000000}));
    EXPECT_EQ(bitStr.bit_length(), bitStrView.size());

    auto encoded = bitStr.encode();
    EXPECT_EQ(encoded, std::vector<uint8_t>({0x07, 0b10110010, 0b10000000}));
}


TEST(bit_string_test, consructed_uint64) {
    uint64_t number = 0b101100101100;
    size_t bitLength = 12;
    bit_string_t bitStr(number, bitLength);

    EXPECT_EQ(bitStr.value(), std::vector<uint8_t>({0b00001011, 0b00101100}));
    EXPECT_EQ(bitStr.bit_length(), bitLength);

    auto encoded = bitStr.encode();
    EXPECT_EQ(encoded, std::vector<uint8_t>({0x04, 0b00001011, 0b00101100}));

}

TEST(bit_string_test, consructed_empty) {
    bit_string_t bitStr;

    EXPECT_EQ(bitStr.value(), std::vector<uint8_t>());
    EXPECT_EQ(bitStr.bit_length(), 0);

    auto encoded = bitStr.encode();
    EXPECT_EQ(encoded, std::vector<uint8_t>({0x00}));
}


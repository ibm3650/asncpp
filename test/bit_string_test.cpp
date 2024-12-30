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
        asncpp::types::bit_string_t obj{expected};
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
    asncpp::types::bit_string_t bitStr(bytes, bitLength);

    EXPECT_EQ(bitStr.value(), bytes);
    EXPECT_EQ(bitStr.bit_length(), bitLength);

    auto encoded = bitStr.encode();
    EXPECT_EQ(encoded, std::vector<uint8_t>({0x01, 0b10110010, 0b11001100}));
}


TEST(bit_string_test, consructed_string_view) {
    std::string_view bitStrView = "101100101";
    asncpp::types::bit_string_t bitStr(bitStrView);

    EXPECT_EQ(bitStr.value(), std::vector<uint8_t>({0b10110010, 0b10000000}));
    EXPECT_EQ(bitStr.bit_length(), bitStrView.size());

    auto encoded = bitStr.encode();
    EXPECT_EQ(encoded, std::vector<uint8_t>({0x07, 0b10110010, 0b10000000}));
}


TEST(bit_string_test, consructed_uint64) {
    uint64_t number = 0b101100101100;
    size_t bitLength = 12;
    asncpp::types::bit_string_t bitStr(number, bitLength);

    EXPECT_EQ(bitStr.value(), std::vector<uint8_t>({0b00001011, 0b00101100}));
    EXPECT_EQ(bitStr.bit_length(), bitLength);

    auto encoded = bitStr.encode();
    EXPECT_EQ(encoded, std::vector<uint8_t>({0x04, 0b00001011, 0b00101100}));

}

TEST(bit_string_test, consructed_empty) {
    asncpp::types::bit_string_t bitStr;

    EXPECT_EQ(bitStr.value(), std::vector<uint8_t>());
    EXPECT_EQ(bitStr.bit_length(), 0);

    auto encoded = bitStr.encode();
    EXPECT_EQ(encoded, std::vector<uint8_t>({0x00}));
}

TEST(bit_string_test, decoding_constructable) {
    // const std::vector<uint8_t> encoded{
    //     0x23, 0x0C, // BIT STRING tag (0x20 | 0x03 -> конструкционный) и длина (11 байт)
    //  0x03, 0x02, 0x04, 0xA0, // Вложенный BIT STRING (4 неиспользованных бита, значение: 0xA0)
    //  0x03, 0x02, 0x02, 0xC3, // Вложенный BIT STRING (2 неиспользованных бита, значение: 0xC3)
    //  0x03, 0x02, 0x00, 0xFF  // Вложенный BIT STRING (0 неиспользованных бит, значение: 0xFF)
    //
    //     };
    // const std::vector<uint8_t> encoded{
    //     0x23, 10, // BIT STRING tag and length (9 bytes total)
    //           0x23, 8, // Nested constructed BIT STRING (no unused bits)
    //           0x03, 0x02, 0x04, 0x5A, // Nested BIT STRING (4 unused bits, value: 0x5A)
    //           0x03, 0x02, 0x00, 0xE7  // Nested BIT STRING (0 unused bits, value: 0xE7)
    //     };
    const std::vector<uint8_t> encoded{
        0x23, 15, // BIT STRING tag and length (13 bytes total)
            0x23, 13, // Nested constructed BIT STRING
            0x03, 0x03, 0x00, 0x11, 0x22, // Nested BIT STRING (0 unused bits, value: 0x1122)
            0x03, 0x02, 0x01, 0xF0, // Nested BIT STRING (1 unused bit, value: 0xF0)
            0x03, 0x02, 0x02, 0x0F  // Nested BIT STRING (2 unused bits, value: 0x0F)
        };
    auto deserialized = asncpp::base::deserialize_v(encoded);
    const auto *ptr{dynamic_cast<asncpp::types::bit_string_t *>(deserialized.get())};
    auto childs{ ptr->get_children(0)};
    const auto ggg  = std::vector<uint8_t>{0x00, 0x11, 0x22};
    const auto ggg1  = std::vector<uint8_t>{0x01,0xF0};
    const auto ggg2  = std::vector<uint8_t>{0x02,0x0F};
    EXPECT_EQ(childs->get_children(0)->get_data(), ggg);
    EXPECT_EQ(childs->get_children(1)->get_data(), ggg1);
    EXPECT_EQ(childs->get_children(2)->get_data(), ggg2);
    // EXPECT_EQ(childs->bit_length(), 16);
    //
    // childs->get_children(0)->get_data() == std::vector<uint8_t>{0x11, 0x22};
    // childs->get_children(1)->get_data() == std::vector<uint8_t>{0xF0};
    // childs->get_children(2)->get_data() == std::vector<uint8_t>{0x0F};


    asncpp::types::bit_string_t bitStr;
    std::unique_ptr<asncpp::types::bit_string_t> t[] = {std::make_unique<asncpp::types::bit_string_t>(std::vector<uint8_t>{0x11, 0x22}, 16),
                std::make_unique<asncpp::types::bit_string_t>(std::vector<uint8_t>{0xF0}, 7),
                std::make_unique<asncpp::types::bit_string_t>(std::vector<uint8_t>{0x0F}, 6)};
    auto t1 = std::make_unique<asncpp::types::bit_string_t>();
    t1->append_child(std::move(t[0]));
    t1->append_child(std::move(t[1]));
    t1->append_child(std::move(t[2]));
    bitStr.append_child(std::move(t1));
    const auto serialized = asncpp::base::serialize(&bitStr);
        EXPECT_EQ(serialized, encoded);
    //for[0<=childs_count]

    // std::vector<uint8_t> bytes = {0b10110010, 0b11001100};
    // size_t bitLength = 15; // Last bit of the second byte is unused.
    // bit_string_t bitStr(bytes);
    //
    // EXPECT_EQ(bitStr.value(), bytes);
    // EXPECT_EQ(bitStr.bit_length(), bytes.size() * 8);
    //
    // auto encoded = bitStr.encode();
    // EXPECT_EQ(encoded, std::vector<uint8_t>({0x00, 0b10110010, 0b11001100}));
}

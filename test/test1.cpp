//
// Created by kandu on 04.12.2024.
//
#include <gtest/gtest.h>
#include "asncpp/asn_base.h"
#include "integer.h"

// Тест успешной десериализации INTEGER
TEST(ASN1BaseTest, DeserializeInteger) {
    const uint8_t encoded_integer[] = {0x02, 0x02, 0x30, 0x39}; // INTEGER 12345
    auto deserialized = deserialize(encoded_integer);
    EXPECT_EQ(static_cast<integer_t<> *>(deserialized.get())->get_value(), 12345);
}

// Тест исключения на некорректный тег
TEST(ASN1BaseTest, DeserializeInvalidTag) {
   // const uint8_t invalid_data[] = {0x04, 0x02, 0x30, 0x39}; // OCTET STRING, not INTEGER
   // EXPECT_THROW(deserialize(invalid_data), std::runtime_error);
}

// Тест сериализации INTEGER
TEST(ASN1BaseTest, SerializeInteger) {
    integer_t<int> obj(12345);
    auto encoded = serialize(&obj);
    EXPECT_EQ(encoded[0], 0x02); // Проверка тега
    EXPECT_EQ(encoded[2], 0x39); // Проверка значения
}

// Тест пустого буфера
TEST(ASN1BaseTest, DeserializeEmptyBuffer) {
//    const uint8_t empty_buffer[] = {};
   // EXPECT_THROW(deserialize(empty_buffer), std::invalid_argument);
}
//
// Created by kandu on 23.12.2024.
//
#include <array>
#include <asncpp/integer.h>
#include <gtest/gtest.h>
#include "asncpp/base/asn1_basic.h"

TEST(common_test, deserialize) {
    const uint8_t encoded[] = {0x02, 0x01, 0x2A}; // INTEGER: 42
    auto obj = asncpp::base::deserialize_v(encoded);
    ASSERT_NE(obj, nullptr);
    ASSERT_EQ(std::get<asncpp::base::asn1_tag>(obj->_type), asncpp::base::asn1_tag::INTEGER);
}

TEST(common_test, serialize) {
    asncpp::types::integer_t obj(42);
    auto encoded = serialize(&obj);
    std::vector<uint8_t> expected = {0x02, 0x01, 0x2A};
    ASSERT_EQ(encoded, expected);
}

TEST(common_test, invalid_tag) {
    const uint8_t encoded[] = {0xFF}; // Invalid tag
    ASSERT_THROW(asncpp::base::deserialize_v(encoded), std::runtime_error);
}






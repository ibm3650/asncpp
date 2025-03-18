//
// Created by kandu on 20.12.2024.
//
#include <array>
#include <asncpp/bit_string.h>
#include <gtest/gtest.h>
#include "asncpp/base/asn1_basic.h"

class TestASN1Basic final : public asncpp::base::asn1_basic {
public:
    using asn1_basic::asn1_basic;
    using asn1_basic::extract_type;
    using asn1_basic::encode_type;

    [[nodiscard]] constexpr uintmax_t get_tag() const noexcept override {
        if (std::holds_alternative<std::monostate>(_type)) {
            return 0;
        }
        return std::holds_alternative<uintmax_t>(_type)
                   ? std::get<uintmax_t>(_type)
                   : static_cast<uintmax_t>(std::get<asncpp::base::asn1_tag>(_type));
    }
};


TEST(asn_basic_test, encode_tag_empty) {
    TestASN1Basic const obj;
    ASSERT_THROW((void)obj.encode_type(), std::runtime_error);
}

TEST(asn_basic_test, encode_short_tag) {
    TestASN1Basic obj;
    obj._type = uintmax_t{0x02};
    const auto encoded = obj.encode_type();
    ASSERT_EQ(encoded, std::vector<uint8_t>{0x02});
}

TEST(asn_basic_test, encode_long_tag) {
    TestASN1Basic obj;
    obj._type = uintmax_t{0x1234};
    obj._cls = asncpp::base::asn1_class::CONTEXT_SPECIFIC;
    const asncpp::base::dynamic_array_t encoded = obj.encode_type();
    const asncpp::base::dynamic_array_t expected = {0x9F, 0xA4, 0x34};
    ASSERT_EQ(encoded, expected);
}

TEST(asn_basic_test, decode_short_tag) {
    constexpr std::array<uint8_t, 3> buffer{0x02, 0x01, 0x7F}; // INTEGER(127)
    const auto [type, size] = TestASN1Basic::extract_type(buffer);
    TestASN1Basic const obj(buffer);
    EXPECT_EQ(std::get<asncpp::base::asn1_tag>(type), asncpp::base::asn1_tag::INTEGER);
    EXPECT_EQ(size, 1);
}

TEST(asn_basic_test, decode_long_tag) {
    constexpr std::array<uint8_t, 3> buffer{0x9F, 0xA4, 0x34};
    const auto [type, size] = TestASN1Basic::extract_type(buffer);
    ASSERT_EQ(std::get<uintmax_t>(type), 0x1234);
    ASSERT_EQ(size, 3);
}


TEST(asn_basic_test, encode_short_length) {
    constexpr uint8_t length{127};
    const asncpp::base::dynamic_array_t encoded{asncpp::base::asn1_basic::encode_length(length)};
    const asncpp::base::dynamic_array_t expected{0x7F};
    ASSERT_EQ(encoded, expected);
}

TEST(asn_basic_test, encode_long_length) {
    constexpr size_t length{300};
    const asncpp::base::dynamic_array_t encoded{asncpp::base::asn1_basic::encode_length(length)};
    const asncpp::base::dynamic_array_t expected{0x82, 0x01, 0x2C};
    ASSERT_EQ(encoded, expected);
}

TEST(asn_basic_test, decode_short_length) {
    constexpr std::array<uint8_t, 1> buffer{0x7F};
    auto [length, size] = asncpp::base::asn1_basic::extract_length(buffer);
    ASSERT_EQ(length, 127);
    ASSERT_EQ(size, 1);
}

TEST(asn_basic_test, decode_long_length) {
    constexpr std::array<uint8_t, 3> buffer{0x82, 0x01, 0x2C};
    auto [length, size] = asncpp::base::asn1_basic::extract_length(buffer);
    ASSERT_EQ(length, 300);
    ASSERT_EQ(size, 3);
}

TEST(asn_basic_test, decode_invalid_length) {
    constexpr std::array<uint8_t, 3> buffer{0x84, 0xFF, 0xFF};
    ASSERT_THROW((void)asncpp::base::asn1_basic::extract_length(buffer), std::runtime_error);
}


TEST(asn_basic_test, decode_invalid_type) {
    constexpr std::array<uint8_t, 3> buffer{0x1F, 0x80, 0x80};
    ASSERT_THROW((void)TestASN1Basic::extract_type(buffer), std::runtime_error);
}

TEST(asn_basic_test, decode_empty_buffer) {
    ASSERT_THROW((void)TestASN1Basic::extract_type({}), std::runtime_error);
}


TEST(ASN1BasicTest, AppendAndGetChild) {
    const std::vector<uint8_t> encoded{
        0x23, 15, // BIT STRING tag and length (13 bytes total)
        0x23, 13, // Nested constructed BIT STRING
        0x03, 0x03, 0x00, 0x11, 0x22, // Nested BIT STRING (0 unused bits, value: 0x1122)
        0x03, 0x02, 0x01, 0xF0, // Nested BIT STRING (1 unused bit, value: 0xF0)
        0x03, 0x02, 0x02, 0x0F // Nested BIT STRING (2 unused bits, value: 0x0F)
    };

    const auto deserialized = asncpp::base::deserialize_v(encoded);
    EXPECT_EQ(deserialized->number_of_children(), 1);
    EXPECT_EQ(deserialized->get_child(0)->number_of_children(), 3);
    auto parent = deserialized->get_child(0)->get_child(0);
    EXPECT_EQ(reinterpret_cast<asncpp::types::bit_string_t*>(parent.get())->value(),
              (std::vector<uint8_t>{0x11, 0x22})
    );
    parent = deserialized->get_child(0)->get_child(1);
    EXPECT_EQ(reinterpret_cast<asncpp::types::bit_string_t*>(parent.get())->value(),
              (std::vector<uint8_t>{0xF0})
    );

    parent = deserialized->get_child(0)->get_child(2);
    EXPECT_EQ(reinterpret_cast<asncpp::types::bit_string_t*>(parent.get())->value(),
              (std::vector<uint8_t>{0x0F})
    );
}

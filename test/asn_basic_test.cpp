//
// Created by kandu on 20.12.2024.
//
#include <array>
#include <gtest/gtest.h>
#include "asncpp/base/asn1_basic.h"

class TestASN1Basic final : public asncpp::base::asn1_basic {
public:
    [[nodiscard]] constexpr uintmax_t get_tag() const noexcept override {
        return std::holds_alternative<uintmax_t>(_type) ?
            std::get<uintmax_t>(_type) :
        static_cast<uintmax_t>(std::get<asncpp::base::asn1_tag>(_type));
    }

};

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
    constexpr std::array<uint8_t, 1> buffer{0x02};
    auto [type, size] = asncpp::base::asn1_basic::extract_type(buffer);
    ASSERT_TRUE(std::holds_alternative<asncpp::base::asn1_tag>(type));
    ASSERT_EQ(std::get<asncpp::base::asn1_tag>(type), asncpp::base::asn1_tag::INTEGER);
    ASSERT_EQ(size, 1);
}

TEST(asn_basic_test, decode_long_tag) {
    constexpr std::array<uint8_t, 3> buffer{0x9F, 0xA4, 0x34};
    auto [type, size] = asncpp::base::asn1_basic::extract_type(buffer);
    ASSERT_TRUE(std::holds_alternative<uintmax_t>(type));
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
    ASSERT_THROW(asncpp::base::asn1_basic::extract_type(buffer), std::runtime_error);
}

TEST(asn_basic_test, decode_empty_buffer) {
    ASSERT_THROW(asncpp::base::asn1_basic::extract_type({}), std::runtime_error);
}




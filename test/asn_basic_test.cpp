//
// Created by kandu on 20.12.2024.
//
#include <array>
#include <gtest/gtest.h>
#include "asncpp/base/asn1_basic.h"

// class asn_basic_test : public ::testing::Test {
// protected:
//     class TestASN1Basic : public asncpp::base::asn1_basic {
//     public:
//         TestASN1Basic() {
//             _type = asncpp::base::asn1_tag::INTEGER;
//         }
//
//         constexpr uintmax_t get_tag() const noexcept override {
//             return tag_value;
//         }
//
//         void set_tag(uintmax_t tag) {
//             tag_value = tag;
//         }
//
//     private:
//         uintmax_t tag_value{0};
//     };
// };
class TestASN1Basic: public asncpp::base::asn1_basic {
public:
    constexpr uintmax_t get_tag() const noexcept override {
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
    auto encoded = obj.encode_type();

    std::vector<uint8_t> expected = {0x9F, 0xA4, 0x34}; // Long tag encoding
    ASSERT_EQ(encoded, expected);
}

TEST(asn_basic_test, decode_short_tag) {
    uint8_t buffer[] = {0x02};
    auto [type, size] = asncpp::base::asn1_basic::extract_type(buffer);

    ASSERT_TRUE(std::holds_alternative<asncpp::base::asn1_tag>(type));
    ASSERT_EQ(std::get<asncpp::base::asn1_tag>(type), asncpp::base::asn1_tag::INTEGER);
    ASSERT_EQ(size, 1);
}

TEST(asn_basic_test, decode_long_tag) {
    uint8_t buffer[] = {0x9F, 0xA4, 0x34};
    auto [type, size] = asncpp::base::asn1_basic::extract_type(buffer);

    ASSERT_TRUE(std::holds_alternative<uintmax_t>(type));
    ASSERT_EQ(std::get<uintmax_t>(type), 0x1234);
    ASSERT_EQ(size, 3);
}

TEST(asn_basic_test, encode_short_length) {
    uint8_t length = 127;
    auto encoded = asncpp::base::asn1_basic::encode_length(length);

    std::vector<uint8_t> expected = {0x7F};
    ASSERT_EQ(encoded, expected);
}

TEST(asn_basic_test, encode_long_length) {
    size_t length = 300;
    auto encoded = asncpp::base::asn1_basic::encode_length(length);

    std::vector<uint8_t> expected = {0x82, 0x01, 0x2C};
    ASSERT_EQ(encoded, expected);
}



TEST(asn_basic_test, decode_short_length) {
    uint8_t buffer[] = {0x7F};
    auto [length, size] = asncpp::base::asn1_basic::extract_length(buffer);

    ASSERT_EQ(length, 127);
    ASSERT_EQ(size, 1);
}

TEST(asn_basic_test, decode_long_length) {
    uint8_t buffer[] = {0x82, 0x01, 0x2C};
    auto [length, size] = asncpp::base::asn1_basic::extract_length(buffer);

    ASSERT_EQ(length, 300);
    ASSERT_EQ(size, 3);
}

// TEST(asn_basic_test, DecodeInvalidLength) {
//     uint8_t buffer[] = {0x84, 0xFF, 0xFF};
//     ASSERT_THROW(asncpp::base::asn1_basic::extract_length(buffer), std::runtime_error);
// }

TEST(asn_basic_test, decode_invalid_type) {
    uint8_t buffer[] = {0x1F, 0x80, 0x80};
    ASSERT_THROW(asncpp::base::asn1_basic::extract_type(buffer), std::runtime_error);
}

TEST(asn_basic_test, decode_empty_buffer) {
   // uint8_t buffer[] = {};
    ASSERT_THROW(asncpp::base::asn1_basic::extract_type({}), std::runtime_error);
}

// TEST(asn_basic_test, EncodeTypeInvalidState) {
//     TestASN1Basic obj;
//     ASSERT_THROW(obj.encode_type(), std::runtime_error);
// }

// TEST(asn_basic_test, EncodeTypeUninitializedTag) {
//     TestASN1Basic obj;
//     obj.set_tag(0xFFFFFFFFFFFFFFFFULL); // Invalid tag
//     ASSERT_THROW(obj.encode_type(), std::runtime_error);
// }


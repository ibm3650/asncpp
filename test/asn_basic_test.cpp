//
// Created by kandu on 20.12.2024.
//
#include <gtest/gtest.h>
#include "asncpp/base/asn1_basic.h"

class test_asn1_basic : public asncpp::base::asn1_basic
{
public:
    asncpp::base::dynamic_array_t encode() override {
        return asncpp::base::asn1_basic::encode();
    }
    void decode(std::span<const uint8_t> data) override {
        return asncpp::base::asn1_basic::decode(data);
    }

    constexpr uintmax_t get_tag() const noexcept override {
        return {};
    }

    // Empty - bridge to protected members for unit-testing
};


TEST(ASN1BasicTest, EncodeBasicType) {
    std::vector<uint8_t> expected_data = {0x02, 0x01, 0x05}; // INTEGER 5
    test_asn1_basic obj;
    obj.decode(expected_data);

    EXPECT_EQ(obj.get_cls(), asncpp::base::asn1_class::UNIVERSAL);
    EXPECT_EQ(obj.is_constructed(), false);
    EXPECT_EQ(obj.get_length(), 1);
    EXPECT_EQ(obj.get_data(), std::vector<uint8_t>{0x05});

    auto encoded_data = obj.encode();
    EXPECT_EQ(encoded_data, expected_data);
}

//
// TEST(ASN1BasicTest, DecodeInvalidData) {
//     std::vector<uint8_t> invalid_data = {0xFF}; // Некорректные данные
//
//     EXPECT_THROW(asncpp::base::asn1_basic obj(invalid_data), std::invalid_argument);
// }
//
//
// TEST(ASN1BasicTest, DecodeLongTags) {
//     std::vector<uint8_t> long_tag = {0x1F, 0x81, 0x00, 0x02, 0x01, 0x05}; // Длинный тег
//     asncpp::base::asn1_basic obj;
//     obj.decode(long_tag);
//
//     EXPECT_EQ(obj.get_cls(), asncpp::base::asn1_class::UNIVERSAL);
//     EXPECT_EQ(obj.get_length(), 1);
//     EXPECT_EQ(obj.get_data(), std::vector<uint8_t>{0x05});
// }
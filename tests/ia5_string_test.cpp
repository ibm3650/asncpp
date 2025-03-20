//
// Created by kandu on 13.12.2024.
//

#include <array>
#include <gtest/gtest.h>
#include "asncpp/ia5_string.h"


static std::vector<std::pair<std::vector<uint8_t>, std::string_view>> test_cases = {
    {{0x16, 0x05, 0x48, 0x65, 0x6C, 0x6C, 0x6F}, "Hello"}, // "Hello"
    {{0x16, 0x0C, 0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x77, 0x6F, 0x72, 0x6C, 0x64, 0x21}, "Hello world!"}, // "Hello world!"
    {{0x16, 0x07, 0x48, 0x65, 0x6C, 0x6C, 0x00, 0x0A, 0x21}, {"Hell\0\n!", 7}} // "Hello\n!"
};

static std::vector<std::pair<std::vector<uint8_t>, std::string>> ia5_string_invalid = {
    {{0x16, 0x05, 0x48, 0x65, 0x6C, 0xC0, 0x6F}, "Invalid character 0xC0"}, // Символ 0xC0 вне диапазона 0x00-0x7F
    {{0x16, 0x04, 0x48, 0x65, 0x6C}, "Invalid length"}, // Неверная длина
    {{0x17, 0x05, 0x48, 0x65, 0x6C, 0x6C, 0x6F}, "Invalid tag"}, // Неверный тег
};


TEST(ia5_string_test, encode) {
    for (const auto &[encoded_expected, expected]: test_cases) {
        //ia5_string_t obj{expected};
        ia5_string_t obj(expected);
        const std::vector<uint8_t> encoded = serialize(&obj);
        EXPECT_EQ(encoded, encoded_expected);
    }
    // const char data[] = {0x1F, 0x1F, 0x08, '2', '0', '2', '2', '1', '2', '2', '5'};
    // visible_string_t const date_obj(data);
    // const char32_t data1[] = {0x1F, 0x1F, 0x08, '2', '0', '2', '2', '1', '2', '2', '5'};
    // //universal_string_t date_o7bj(data1);
}

TEST(ia5_string_test, decode) {
    for (const auto &[encoded, expected]: test_cases) {
        const auto deserialized = asncpp::base::deserialize_v(encoded);
        const ia5_string_t *ptr = dynamic_cast<ia5_string_t *>(deserialized.get());
        EXPECT_EQ(ptr->value(), expected);
    }
}

TEST(ia5_string_test, encode_invalid_symbols) {
    const uint8_t invalid[] = {'H', 'e', 'l', 0xC0, 'o'};
   // const std::pair<std::vector<uint8_t>, std::string> invalid{{0x16, 0x05,'H', 'e', 'l', 0xC0, 'o'},};
    EXPECT_ANY_THROW(ia5_string_t var(std::string_view{reinterpret_cast<const char *>(invalid), sizeof(invalid)}));
}


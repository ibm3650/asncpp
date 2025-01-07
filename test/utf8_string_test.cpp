//
// Created by kandu on 06.01.2025.
//

#include <array>
#include <gtest/gtest.h>
#include "asncpp/utf8_string.h"


static const std::vector<std::pair<std::vector<uint8_t>, std::u8string_view> > test_cases = {
    {
        {0x0C, 13, 'H', 'e', 'l', 'l', 'o', ',', ' ', 0xE4, 0xB8, 0x96, 0xE7, 0x95, 0x8C},
        u8"Hello, 世界"
    }
};


//std::vector<std::pair<std::vector<uint8_t>, std::u16string_view> > bmp_string_invalid = {
//    {
//        {0x1E, 0x05, 0x00, 0x48, 0x00, 0x65, 0x00}, // Полный пакет: Ошибка - длина указана 5, но данных меньше
//        u"He" // Только значение: "He" (некорректные данные)
//    },
//    {
//        {0x1E, 0x06, 0x00, 0x48, 0x00, 0x65, 0x00, 0x6C, 0xD8, 0x00},
//        // Полный пакет: Ошибка - недопустимый UTF-16 surrogate
//        u"Hel" // Только значение: "Hel" (ошибка в строке)
//    },
//    {
//        {0x1E, 0x03, 0x00, 0xC3, 0x00}, // Полный пакет: Ошибка - неполная пара байт
//        u"\u00C3" // Только значение: "\u00C3" (ошибка в кодировке)
//    }
//};


// std::vector<std::pair<std::vector<uint8_t>, std::string>> test_data = {
//     // Корректные данные
//     // {
//     //     {0x1C, 0x08, 0x00, 0x61, 0x00, 0x62, 0x00, 0x63, 0x00, 0x64}, // Полный пакет: UniversalString "abcd"
//     //     {0x00, 0x61, 0x00, 0x62, 0x00, 0x63, 0x00, 0x64}              // Только значение: "abcd" std::u32string original = U"Test: ☀️🌙🌟";
//     // },
//     // {
//     //         {0x1E, 0x06, 0x00, 0x41, 0x00, 0x42, 0x00, 0x43},             // Полный пакет: BMPString "ABC"
//     //         {0x00, 0x41, 0x00, 0x42, 0x00, 0x43}                          // Только значение: "ABC"
//     // },
//     {
//             {0x1A, 0x05, 0x48, 0x65, 0x6C, 0x6C, 0x6F},                  // Полный пакет: PrintableString "Hello"
//             "Hello"                               // Только значение: "Hello"
//     },
//
//     // Некорректные данные
//     // {
//     //         {0x1C, 0x04, 0x61, 0x00, 0x62, 0x00},                        // Полный пакет: Ошибка - длина неверна
//     //         {0x61, 0x00, 0x62, 0x00}                                     // Только значение: "a\0b\0"
//     // },
//     // {
//     //         {0x1E, 0x08, 0x00, 0x41, 0x00, 0x42, 0x43},                  // Полный пакет: Ошибка - неверная структура
//     //         {0x00, 0x41, 0x00, 0x42, 0x43}                               // Только значение: "A\0BC" (некорректный BMPString)
//     // },
//   //  {
//             // {0x1A, 0x06, 0x48, 0x65, 0x6C, 0x6C, 0x6F},                  // Полный пакет: Ошибка - длина указана неверно
//             // "Hello"                               // Только значение: "Hello"
//  //   }
// };

TEST(utf8_string_test, encode) {
    for (const auto &[encoded_expected, expected]: test_cases) {
        utf8_string_t obj(expected);
        const std::vector<uint8_t> encoded = serialize(&obj);
        EXPECT_EQ(encoded, encoded_expected);
    }
    // const char data[] = {0x1F, 0x1F, 0x08, '2', '0', '2', '2', '1', '2', '2', '5'};
    // visible_string_t const date_obj(data);
    // const char32_t data1[] = {0x1F, 0x1F, 0x08, '2', '0', '2', '2', '1', '2', '2', '5'};
    // //universal_string_t date_o7bj(data1);
}
//
TEST(utf8_string_test, decode) {
    for (const auto &[encoded, expected]: test_cases) {
        const auto deserialized = asncpp::base::deserialize_v(encoded);
        const utf8_string_t *ptr = dynamic_cast<utf8_string_t *>(deserialized.get());
        EXPECT_EQ(ptr->value(), expected);
    }
}
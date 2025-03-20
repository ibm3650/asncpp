//
// Created by kandu on 11.12.2024.
//

#include <array>
#include <gtest/gtest.h>
#include "asncpp/visible_string.h"


static const std::vector<std::pair<std::vector<uint8_t>, std::string_view> > test_cases = {
    {
        {0x1A, 0x05, 0x48, 0x65, 0x6C, 0x6C, 0x6F},
        "Hello"
    },
    {
        {0x1A, 0x07, 0x57, 0x6F, 0x72, 0x6C, 0x64, 0x21, 0x3F},
        "World!?"
    },
    {
        {0x1A, 0x03, 0x41, 0x42, 0x43},
        "ABC"
    }
};


std::vector<std::pair<std::vector<uint8_t>, std::string_view> > test_cases_invalid = {
    {
        {0x1A, 0x05, 0x48, 0x65, 0x6C, 0x6C, 0x0A}, // Полный пакет: Ошибка - символ '\n' не допустим в VisibleString
        "Hell\n" // Только значение: "Hell\n" (ошибка в строке)
    },
    {
        {0x1A, 0x03, 0xC3, 0x28, 0x41}, // Полный пакет: Ошибка - содержит недопустимый символ
        "É(A" // Только значение: "É(A" (ошибка в кодировке)
    }
};


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

TEST(visible_string_test, encode) {
    for (const auto &[encoded_expected, expected]: test_cases) {
        visible_string_t obj(expected);
        const std::vector<uint8_t> encoded = serialize(&obj);
        EXPECT_EQ(encoded, encoded_expected);
    }
    // const char data[] = {0x1F, 0x1F, 0x08, '2', '0', '2', '2', '1', '2', '2', '5'};
    // visible_string_t const date_obj(data);
    // const char32_t data1[] = {0x1F, 0x1F, 0x08, '2', '0', '2', '2', '1', '2', '2', '5'};
    // //universal_string_t date_o7bj(data1);
}

TEST(visible_string_test, decode) {
    for (const auto &[encoded, expected]: test_cases) {
        const auto deserialized = asncpp::base::deserialize_v(encoded);
        const visible_string_t *ptr = static_cast<visible_string_t *>(deserialized.get());
        EXPECT_EQ(ptr->value(), expected);
    }
}

TEST(visible_string_test, encode_invalid_symbols) {
    for (const auto &[encoded, expected]: test_cases_invalid) {
        EXPECT_ANY_THROW(visible_string_t var(expected));
    }
}

// std::vector<std::tuple<std::tm, std::vector<uint8_t>, std::string> > date_time_tests = {
//     {
//         {30, 30, 12, 25, 11, 122}, // 2022-12-25T12:30:30
//         {0x1F, 0x14, '2', '0', '2', '2', '-', '1', '2', '-', '2', '5', 'T', '1', '2', ':', '3', '0', ':', '3', '0'},
//         "2022-12-25T12:30:30"
//     },
//     {
//             {0, 0, 0, 1, 0, 123}, // 2023-01-01T00:00:00
//             {0x1F, 0x14, '2', '0', '2', '3', '-', '0', '1', '-', '0', '1', 'T', '0', '0', ':', '0', '0', ':', '0', '0'},
//             "2023-01-01T00:00:00"
//         },
//         {
//             {59, 59, 23, 31, 11, 119}, // 2019-12-31T23:59:59
//             {0x1F, 0x14, '2', '0', '1', '9', '-', '1', '2', '-', '3', '1', 'T', '2', '3', ':', '5', '9', ':', '5', '9'},
//             "2019-12-31T23:59:59"
//         }
// };
//
// std::array time_of_day_tests{
//     test_item_t{
//             {30, 15, 12}, // 12:15:30
//             {0x20, 0x08, '1', '2', ':', '1', '5', ':', '3', '0'}, // DER encoding for TIME-OF-DAY
//             "12:15:30"
//         },
//         test_item_t{
//             {0, 0, 0}, // 00:00:00
//             {0x20, 0x08, '0', '0', ':', '0', '0', ':', '0', '0'}, // DER encoding for TIME-OF-DAY
//             "00:00:00"
//         },
//         test_item_t{
//             {59, 59, 23}, // 23:59:59
//             {0x20, 0x08, '2', '3', ':', '5', '9', ':', '5', '9'}, // DER encoding for TIME-OF-DAY
//             "23:59:59"
//         }
// };

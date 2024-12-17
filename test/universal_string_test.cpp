//
// Created by kandu on 11.12.2024.
//

#include <array>
#include <gtest/gtest.h>
#include "asncpp/universl_string.h"


static const std::vector<std::pair<std::vector<uint8_t>, std::u32string_view> > test_cases = {
    {
        {
            0x1C, 0x14, 0x00, 0x00, 0x00, 0x48, 0x00, 0x00, 0x00, 0x65,
            0x00, 0x00, 0x00, 0x6C, 0x00, 0x00, 0x00, 0x6C, 0x00, 0x00, 0x00, 0x6F
        },
        U"Hello"
    }, // "Hello"

    {
        {
            0x1C, 0x18, 0x00, 0x00, 0x00, 0x48, 0x00, 0x00, 0x00, 0x65,
            0x00, 0x00, 0x00, 0x6C, 0x00, 0x00, 0x00, 0x6C, 0x00, 0x00, 0x00, 0x6F,
            0x00, 0x00, 0x00, 0x21
        },
        U"Hello!"
    }, // "Hello!"

    {
        {
            0x1C, 0x18, 0x00, 0x00, 0x00, 0x48, 0x00, 0x00, 0x00, 0x65,
            0x00, 0x00, 0x00, 0x6C, 0x00, 0x00, 0x00, 0x6C, 0x00, 0x00, 0x00, 0x6F,
            0x00, 0x00, 0x00, 0x0A
        },
        U"Hello\n"
    } // "Hello\n"
};


TEST(universal_string_test, encode) {
    for (const auto &[encoded_expected, expected]: test_cases) {
        universal_string_t obj(expected);
        const std::vector<uint8_t> encoded = serialize(&obj);
        EXPECT_EQ(encoded, encoded_expected);
    }
    // const char data[] = {0x1F, 0x1F, 0x08, '2', '0', '2', '2', '1', '2', '2', '5'};
    // visible_string_t const date_obj(data);
    // const char32_t data1[] = {0x1F, 0x1F, 0x08, '2', '0', '2', '2', '1', '2', '2', '5'};
    // //universal_string_t date_o7bj(data1);
}

TEST(universal_string_test, decode) {
    for (const auto &[encoded, expected]: test_cases) {
        const auto deserialized = asncpp::base::deserialize_v(encoded);
        const universal_string_t *ptr = dynamic_cast<universal_string_t *>(deserialized.get());
        EXPECT_EQ(ptr->value(), expected);
    }
}

// TEST(universal_string_test, encode_invalid_symbols) {
//     for (const auto &[encoded, expected]: test_cases_invalid) {
//         EXPECT_ANY_THROW(visible_string_t var(expected));
//     }
// }

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

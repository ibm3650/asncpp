//
// Created by kandu on 10.12.2024.
//

#include <algorithm>
#include <array>
#include <ranges>
#include <gtest/gtest.h>
#include "asncpp/date.h"
#include "asncpp/bit_string.h"
#include "asncpp/octet_string.h"


using test_item_t = std::tuple<std::tm, std::vector<uint8_t>, std::string_view>;

const std::array date_tests = {
    test_item_t{
        {0, 0, 0, 25, 11, 122}, // 2022-12-25
        {0x1F, 0x1F, 0x08, '2', '0', '2', '2', '1', '2', '2', '5'},
        "2022-12-25"
    },
    test_item_t{
        {0, 0, 0, 1, 0, 123}, // 2023-01-01
        {0x1F, 0x1F, 0x08, '2', '0', '2', '3', '0', '1', '0', '1'},
        "2023-01-01"
    },
    test_item_t{
        {0, 0, 0, 15, 6, 117}, // 2017-07-15
        {0x1F, 0x1F, 0x08, '2', '0', '1', '7', '0', '7', '1', '5'},
        "2017-07-15"
    }
};

static bool operator==(const std::tm &lhs, const std::tm &rhs) {
    return std::memcmp(&lhs, &rhs, sizeof(std::tm)) == 0;
    // return lhs.tm_year == rhs.tm_year &&
    //        lhs.tm_mon == rhs.tm_mon &&
    //        lhs.tm_mday == rhs.tm_mday &&
    //        lhs.tm_hour == rhs.tm_hour &&
    //        lhs.tm_min == rhs.tm_min &&
    //        lhs.tm_sec == rhs.tm_sec;
}




TEST(date_test, encode) {
    for (const auto &[tm_value, encoded_expected, expected]: date_tests) {
        date_t date_obj(tm_value);
        const std::vector encoded{asncpp::base::serialize(&date_obj)};
        EXPECT_EQ(encoded, encoded_expected);
    }
}


TEST(date_test, decode) {
    for (const auto &[tm_value, encoded_expected, expected]: date_tests) {
        const auto ptr{asncpp::base::deserialize_v(encoded_expected)};
        const date_t *date_obj = dynamic_cast<date_t *>(ptr.get());
        EXPECT_EQ(date_obj->get_value(), tm_value);
    }
}

TEST(date_test, stringify) {
    for (const auto &[tm_value, encoded_expected, expected]: date_tests) {
        date_t date_obj(tm_value);
        EXPECT_EQ(date_obj.to_string(), expected);
    }
}




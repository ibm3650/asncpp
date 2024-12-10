//
// Created by kandu on 10.12.2024.
//

#include <gtest/gtest.h>
#include "asncpp/iso8601_base.h"

TEST(ISO8601BaseTest, EncodeDecode) {
    constexpr std::tm expected = {
        .tm_sec = 0,
        .tm_min = 0,
        .tm_hour = 12,
        .tm_mday = 25,
        .tm_mon = 11,
        .tm_year = 122
    }; // 2022-12-25 12:00:00
    date_t date_obj(expected);
    //auto encoded = serialize(&date_obj);

   // auto decoded_obj = date_t();
   // decoded_obj.decode(encoded);
    EXPECT_EQ(date_obj.to_string(), "20221225");
}
//
// TEST(ISO8601BaseTest, InvalidFormat) {
//     std::vector<uint8_t> invalid_data = {'2', '0', 'X', '0', '1'};
//     date_t obj;
//     EXPECT_THROW(obj.decode(invalid_data), std::invalid_argument);
// }
//
// TEST(ISO8601BaseTest, DefaultConstructor) {
//     date_t obj;
//     EXPECT_NO_THROW(obj.to_string());
// }

//
// Created by kandu on 10.12.2024.
//

#include <algorithm>
#include <array>
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

static auto operator==(const std::tm &lhs, const std::tm &rhs) -> bool {
    return lhs.tm_year == rhs.tm_year &&
           lhs.tm_mon == rhs.tm_mon &&
           lhs.tm_mday == rhs.tm_mday &&
           lhs.tm_hour == rhs.tm_hour &&
           lhs.tm_min == rhs.tm_min &&
           lhs.tm_sec == rhs.tm_sec;
}

#include <ranges>


TEST(ASN1DateTests, EncodeDate) {
    for (const auto &[tm_value, encoded_expected, expected]: date_tests) {
        date_t date_obj(tm_value);
        const std::vector encoded{asncpp::base::serialize(&date_obj)};
        EXPECT_EQ(encoded, encoded_expected);
    }
}


TEST(ASN1DateTests, DecodeDate) {
    for (const auto &[tm_value, encoded_expected, expected]: date_tests) {
        const auto ptr{asncpp::base::deserialize_v(encoded_expected)};
        const date_t *date_obj = dynamic_cast<date_t *>(ptr.get());
        EXPECT_EQ(date_obj->get_value(), tm_value);
    }
}

TEST(ASN1DateTests, StringifyDate) {
    for (const auto &[tm_value, encoded_expected, expected]: date_tests) {
        date_t date_obj(tm_value);
        EXPECT_EQ(date_obj.to_string(), expected);
    }
}

//
// TEST(ASN1DateTimeTests, EncodeDecodeDateTime) {
//     for (const auto &[tm_value, encoded, expected]: date_time_tests) {
//         date_time_t date_time_obj(tm_value);
//         EXPECT_EQ(date_time_obj.encode(), encoded);
//         date_time_obj.decode(encoded);
//         EXPECT_EQ(date_time_obj.to_string(), expected);
//     }
// }
//
// TEST(ASN1TimeOfDayTests, EncodeDecodeTimeOfDay) {
//     for (const auto &[tm_value, encoded, expected]: time_of_day_tests) {
//         time_of_day_t time_of_day_obj(tm_value);
//         EXPECT_EQ(time_of_day_obj.encode(), encoded);
//         time_of_day_obj.decode(encoded);
//         EXPECT_EQ(time_of_day_obj.to_string(), expected);
//     }
// }
TEST(ASN1TimeOfDayTests, EncodeDecodeTimeOfDay) {
    const std::vector<uint8_t> encoded{
        0x24, 0x05,
        0x04, 0x03, 0xA1, 0xB2, 0xC3
    };
    auto deserialized = asncpp::base::deserialize_v(encoded);
    const auto *ptr{dynamic_cast<octet_string_t *>(deserialized.get())};
    auto childs{ptr->get_children(0)};
    const auto ggg = std::vector<uint8_t>{0xA1, 0xB2, 0xC3};
    EXPECT_EQ(childs->get_data(), ggg);
}

TEST(ASN1TimeOfDayTests, EncodeDecodeTimeOfDay3) {
    const std::vector<uint8_t> encoded{
        0x24, 0x05,
        0x04, 0x03, 0xA1, 0xB2, 0xC3
    };
    octet_string_t bitStr;
    std::unique_ptr<octet_string_t> t[] = {
        std::make_unique<octet_string_t>(std::vector<uint8_t>{0xA1, 0xB2, 0xC3}),
        //std::make_unique<octet_string_t>(std::vector<uint8_t>{0xF0}, 7),
       // std::make_unique<octet_string_t>(std::vector<uint8_t>{0x0F}, 6)
    };
    auto t1 = std::make_unique<octet_string_t>();
    t1->append_child(std::move(t[0]));
   // t1->append_child(std::move(t[1]));
   // t1->append_child(std::move(t[2]));
   // bitStr.append_child(std::move(t1));
    const auto serialized = asncpp::base::serialize(t1.get());
    EXPECT_EQ(serialized, encoded);
}

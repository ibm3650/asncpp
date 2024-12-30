//
// Created by kandu on 30.12.2024.
//
#include <gtest/gtest.h>
#include "asncpp/octet_string.h"

TEST(octet_string_test, serialize) {
    const std::vector<uint8_t> encoded{
        0x24, 0x05,
        0x04, 0x03, 0xA1, 0xB2, 0xC3
    };
    //octet_string_t bitStr;
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

TEST(octet_string_test, deserialize) {
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
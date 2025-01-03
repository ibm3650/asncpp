//
// Created by kandu on 03.01.2025.
//
#include <gtest/gtest.h>
#include "asncpp/object_descriptor.h"

TEST(object_descriptor_test, serialize) {
    const std::vector<uint8_t> encoded{
        0x27, 20, // Constructed OBJECT_DESCRIPTOR tag + length
        0x07, 0x08, // Nested OBJECT_DESCRIPTOR tag + length
        'N', 'e', 's', 't', 'e', 'd', ' ', '1', // "Nested 1"
        0x07, 0x08, // Nested OBJECT_DESCRIPTOR tag + length
        'N', 'e', 's', 't', 'e', 'd', ' ', '2' // "Nested 2"
    };
    std::unique_ptr<object_descriptor_t> t[] = {
        std::make_unique<object_descriptor_t>("Nested 1"),
        std::make_unique<object_descriptor_t>("Nested 2"),
        //std::make_unique<octet_string_t>(std::vector<uint8_t>{0xF0}, 7),
        // std::make_unique<octet_string_t>(std::vector<uint8_t>{0x0F}, 6)
    };
    auto t1 = std::make_unique<object_descriptor_t>();
    t1->append_child(std::move(t[0]));
    t1->append_child(std::move(t[1]));
    // t1->append_child(std::move(t[1]));
    // t1->append_child(std::move(t[2]));
    // bitStr.append_child(std::move(t1));
    const auto serialized = asncpp::base::serialize(t1.get());
    EXPECT_EQ(serialized, encoded);
}

template<class T>
const T *get_children(const asncpp::base::asn1_basic *base, size_t index) {
    return static_cast<T *>(base->get_children(index));
}

TEST(object_descriptor_test, deserialize) {
    const std::vector<uint8_t> encoded{
        0x27, 20, // Constructed OBJECT_DESCRIPTOR tag + length
        0x07, 0x08, // Nested OBJECT_DESCRIPTOR tag + length
        'N', 'e', 's', 't', 'e', 'd', ' ', '1', // "Nested 1"
        0x07, 0x08, // Nested OBJECT_DESCRIPTOR tag + length
        'N', 'e', 's', 't', 'e', 'd', ' ', '2' // "Nested 2"
    };
    auto deserialized = asncpp::base::deserialize_v(encoded);
    const auto *ptr{static_cast<object_descriptor_t *>(deserialized.get())};
    EXPECT_EQ(get_children<object_descriptor_t>(ptr, 0)->value(), "Nested 1");
    EXPECT_EQ(get_children<object_descriptor_t>(ptr, 1)->value(), "Nested 2");

    // //auto childs{ptr->get_children(0)};
    // const auto ggg = std::vector<uint8_t>{0xA1, 0xB2, 0xC3};
    // EXPECT_EQ(childs->get_data(), ggg);
}


TEST(object_descriptor_test, invalid_symbol) {
    const char invalid_data[] = {'H', 'e', 'l', 1, 'o'};
    EXPECT_THROW((object_descriptor_t{std::string_view{invalid_data, std::size(invalid_data)}}), std::invalid_argument);
}

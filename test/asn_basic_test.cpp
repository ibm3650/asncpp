/**
 * @file asn_basic_test.cpp
 * @brief Test cases for the `asn1_basic` class.
 * @author Nikita Kanduba
 * @date 20.12.2024
 */
#include <array>
#include <asncpp/bit_string.h>
#include <gtest/gtest.h>
#include <ranges>
#include <asncpp/base/asn1_basic.h>

class TestASN1Basic final : public asncpp::base::asn1_basic {
public:
    using asn1_basic::asn1_basic;
    using asn1_basic::extract_type;
    using asn1_basic::encode_type;

    [[nodiscard]] constexpr uintmax_t get_tag() const noexcept override {
        if (std::holds_alternative<std::monostate>(_type)) {
            return 0;
        }
        return std::holds_alternative<uintmax_t>(_type)
                   ? std::get<uintmax_t>(_type)
                   : static_cast<uintmax_t>(std::get<asncpp::base::asn1_tag>(_type));
    }
};

/**
 * @brief Проверка исключения метода encode_type при пустом теге/буфере данных.
 */
TEST(asn_basic_test, encode_tag_empty) {
    TestASN1Basic const obj;
    ASSERT_THROW((void)obj.encode_type(), std::runtime_error);
}

/**
 * @brief Проверка метода encode_type при коротком теге.
 */
TEST(asn_basic_test, encode_short_tag) {
    const asncpp::base::dynamic_array_t expected{0x02};
    TestASN1Basic obj;
    obj._type = asncpp::base::asn1_tag::INTEGER;
    ASSERT_EQ(obj.encode_type(), expected);
}

/**
 * @brief Проверка метода encode_type при длинном теге.
 */
TEST(asn_basic_test, encode_long_tag) {
    const asncpp::base::dynamic_array_t expected{0x9F, 0xA4, 0x34};
    TestASN1Basic obj;
    obj._type = uintmax_t{0x1234};
    obj._cls = asncpp::base::asn1_class::CONTEXT_SPECIFIC;
    ASSERT_EQ(obj.encode_type(), expected);
}


/**
 * @brief Проверка метода extract_type при коротком теге.
 */
TEST(asn_basic_test, decode_short_tag) {
    constexpr auto type_expected{asncpp::base::asn1_tag::INTEGER};
    constexpr size_t size_expected{1};
    const auto [type, size] =
            TestASN1Basic::extract_type(std::array<uint8_t, 3>{0x02, 0x01, 0x7F});
    EXPECT_EQ(std::get<asncpp::base::asn1_tag>(type), type_expected);
    EXPECT_EQ(size, size_expected);
}


/**
 * @brief Проверка метода extract_type при длинном теге.
 */
TEST(asn_basic_test, decode_long_tag) {
    constexpr uintmax_t type_expected{0x1234};
    constexpr size_t size_expected{3};
    const auto [type, size] =
            TestASN1Basic::extract_type(std::array<uint8_t, 3>{0x9F, 0xA4, 0x34});
    ASSERT_EQ(std::get<uintmax_t>(type), type_expected);
    ASSERT_EQ(size, size_expected);
}


/**
 * @brief Проверка метода encode_length при короткой длине.
 */
TEST(asn_basic_test, encode_short_length) {
    const asncpp::base::dynamic_array_t expected{0x7F};
    const asncpp::base::dynamic_array_t encoded{asncpp::base::asn1_basic::encode_length(127)};
    ASSERT_EQ(encoded, expected);
}

/**
 * @brief Проверка метода encode_length при длинной длине.
 */
TEST(asn_basic_test, encode_long_length) {
    const asncpp::base::dynamic_array_t expected{0x82, 0x01, 0x2C};
    const asncpp::base::dynamic_array_t encoded{asncpp::base::asn1_basic::encode_length(300)};
    ASSERT_EQ(encoded, expected);
}


/**
 * @brief Проверка метода extract_length при короткой длине.
 */
TEST(asn_basic_test, decode_short_length) {
    constexpr size_t length_expected{127};
    constexpr size_t size_expected{1};
    auto [length, size] =
            asncpp::base::asn1_basic::extract_length(std::array<uint8_t, 1>{0x7F});
    ASSERT_EQ(length, length_expected);
    ASSERT_EQ(size, size_expected);
}


/**
 * @brief Проверка метода extract_length при длинной длине.
 */
TEST(asn_basic_test, decode_long_length) {
    constexpr size_t length_expected{300};
    constexpr size_t size_expected{3};
    auto [length, size] =
            asncpp::base::asn1_basic::extract_length(std::array<uint8_t, 3>{0x82, 0x01, 0x2C});
    ASSERT_EQ(length, length_expected);
    ASSERT_EQ(size, size_expected);
}


/**
 * @brief Проверка исключения метода extract_length при неверной длине.
 */
TEST(asn_basic_test, decode_invalid_length) {
    ASSERT_THROW((void)asncpp::base::asn1_basic::extract_length(std::array<uint8_t, 3>{0x84, 0xFF, 0xFF}),
                 std::runtime_error);
}


/**
 * @brief Проверка исключения метода extract_type при неверном типе.
 */
TEST(asn_basic_test, decode_invalid_type) {
    ASSERT_THROW((void)TestASN1Basic::extract_type(std::array<uint8_t, 3>{0x1F, 0x80, 0x80}), std::runtime_error);
}

/**
 * @brief Проверка исключения метода extract_type при пустом буфере.
 */
TEST(asn_basic_test, decode_empty_buffer) {
    ASSERT_THROW((void)TestASN1Basic::extract_type({}), std::runtime_error);
}


/**
 * @brief Проверка методов get_child и number_of_children при десериализации конструкционного объекта.
 */
TEST(asn_basic_test, children_operations) {
    const std::vector<std::vector<uint8_t> > expected{
        {0x11, 0x22},
        {0xF0},
        {0x0F}
    };
    const std::vector<uint8_t> encoded{
        0x23, 15, // BIT STRING tag and length (13 bytes total)
        0x23, 13, // Nested constructed BIT STRING
        0x03, 0x03, 0x00, 0x11, 0x22, // Nested BIT STRING (0 unused bits, value: 0x1122)
        0x03, 0x02, 0x01, 0xF0, // Nested BIT STRING (1 unused bit, value: 0xF0)
        0x03, 0x02, 0x02, 0x0F // Nested BIT STRING (2 unused bits, value: 0x0F)
    };

    const auto deserialized = asncpp::base::deserialize_v(encoded);
    EXPECT_EQ(deserialized->number_of_children(), 1);
    EXPECT_EQ(deserialized->get_child(0)->number_of_children(), 3);
    for (auto [index, expected_value]: std::views::enumerate(expected)) {
        const auto parent = deserialized->get_child(0)->get_child(index);
        EXPECT_EQ(reinterpret_cast<asncpp::types::bit_string_t*>(parent.get())->value(), expected_value);
    }
}

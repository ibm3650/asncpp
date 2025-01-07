//
// Created by kandu on 06.01.2025.
//
#include "asncpp/real.h" // Заголовочный файл с реализацией класса
#include "asncpp/null.h" // Заголовочный файл с реализацией класса
#include <gtest/gtest.h>

TEST(real_test, encode) {
    real_t real(123.456);
    auto encoded = asncpp::base::serialize(&real);

    // Ожидаемый результат в формате ASN.1 REAL
    auto ptr = asncpp::base::deserialize_v(encoded);
    auto real_ptr = dynamic_cast<real_t *>(ptr.get());
   // std::vector<uint8_t> expected = {0x09, 0x03, 0x80, 0x5E, 0x80};
    EXPECT_EQ(123.456, real_ptr->value());
    null_t null;
   // duration_t duration;
    //EXPECT_EQ(encoded, expected);
}
//
// TEST(RealTypeTest, EncodeNegativeValue) {
//     real_t real(-78.9);
//     auto encoded = real.encode();
//
//     // Ожидаемый результат для отрицательного числа
//     std::vector<uint8_t> expected = {0x09, 0x03, 0xC0, 0x4E, 0xA3};
//     EXPECT_EQ(encoded, expected);
// }
//
// TEST(RealTypeTest, EncodeInfinity) {
//     real_t positive_inf(std::numeric_limits<double>::infinity());
//     auto encoded = positive_inf.encode();
//     std::vector<uint8_t> expected_positive = {0x09, 0x01, 0x40};
//     EXPECT_EQ(encoded, expected_positive);
//
//     real_t negative_inf(-std::numeric_limits<double>::infinity());
//     auto encoded_negative = negative_inf.encode();
//     std::vector<uint8_t> expected_negative = {0x09, 0x01, 0x41};
//     EXPECT_EQ(encoded_negative, expected_negative);
// }
//
// TEST(RealTypeTest, EncodeZero) {
//     real_t zero(0.0);
//     auto encoded = zero.encode();
//
//     // Для нуля данные пустые
//     std::vector<uint8_t> expected = {0x09, 0x00};
//     EXPECT_EQ(encoded, expected);
// }
//
// TEST(RealTypeTest, DecodePositiveValue) {
//     std::vector<uint8_t> data = {0x09, 0x03, 0x80, 0x5E, 0x80};
//     real_t real;
//     real.decode(data);
//     EXPECT_NEAR(real.value(), 123.456, 0.001);
// }
//
// TEST(RealTypeTest, DecodeNegativeValue) {
//     std::vector<uint8_t> data = {0x09, 0x03, 0xC0, 0x4E, 0xA3};
//     real_t real;
//     real.decode(data);
//     EXPECT_NEAR(real.value(), -78.9, 0.001);
// }
//
// TEST(RealTypeTest, DecodeInfinity) {
//     std::vector<uint8_t> positive_inf_data = {0x09, 0x01, 0x40};
//     real_t positive_inf;
//     positive_inf.decode(positive_inf_data);
//     EXPECT_TRUE(std::isinf(positive_inf.value()));
//     EXPECT_GT(positive_inf.value(), 0.0);
//
//     std::vector<uint8_t> negative_inf_data = {0x09, 0x01, 0x41};
//     real_t negative_inf;
//     negative_inf.decode(negative_inf_data);
//     EXPECT_TRUE(std::isinf(negative_inf.value()));
//     EXPECT_LT(negative_inf.value(), 0.0);
// }
//
// TEST(RealTypeTest, DecodeZero) {
//     std::vector<uint8_t> data = {0x09, 0x00};
//     real_t zero;
//     zero.decode(data);
//     EXPECT_EQ(zero.value(), 0.0);
// }
//
// TEST(RealTypeTest, DecodeInvalidLength) {
//     std::vector<uint8_t> data = {0x09, 0x01}; // Отсутствует тело данных
//     real_t real;
//     EXPECT_THROW(real.decode(data), std::runtime_error);
// }
//
// TEST(RealTypeTest, DecodeInvalidSpecialRealValue) {
//     std::vector<uint8_t> data = {0x09, 0x01, 0x50}; // Неправильный специальный флаг
//     real_t real;
//     EXPECT_THROW(real.decode(data), std::runtime_error);
// }
//
// TEST(RealTypeTest, DecodeInvalidBaseOrExponent) {
//     std::vector<uint8_t> data = {0x09, 0x03, 0x80, 0xFF, 0x80}; // Некорректный порядок
//     real_t real;
//     EXPECT_THROW(real.decode(data), std::runtime_error);
// }
//
// Created by kandu on 26.11.2024.
//

#ifndef ASNCPP_OBJECT_IDENTIFIER_H
#define ASNCPP_OBJECT_IDENTIFIER_H

#include <string>
#include <stdexcept>
#include "../include/asn_base.h"
//
//class object_identifier_t
//        : public asn1_type<std::vector<uint32_t>, static_cast<uintmax_t>(asn1_tag::OBJECT_IDENTIFIER)> {
//public:
//    explicit object_identifier_t() = default;
//
//    explicit object_identifier_t(const std::vector<uint32_t> &data) {
//        this->_value = data;
//    }
//
//    [[nodiscard]] std::vector<uint8_t> encode() const override {
//        std::vector<uint8_t> result;
//
//        if (_value.size() >= 2) {
//            result.push_back(40 * _value[0] + _value[1]);
//        }
//
//        // Кодируем остальные идентификаторы (c, d, ...)
//        for (size_t i = 2; i < _value.size(); ++i) {
//            uint32_t value = _value[i];
//            std::vector<uint8_t> temp;
//
//            // Кодируем value в base-128
//            do {
//                temp.insert(temp.begin(), static_cast<uint8_t>(value & 0x7F));
//                value >>= 7;
//            } while (value > 0);
//
//            // Устанавливаем MSB для всех байтов, кроме последнего
//            for (size_t j = 0; j < temp.size() - 1; ++j) {
//                temp[j] |= 0x80;
//            }
//
//            // Добавляем закодированное значение в результат
//            result.insert(result.end(), temp.begin(), temp.end());
//        }
//        result.insert(result.begin(), result.size());
//        result.insert(result.begin(), static_cast<uint8_t>(asn1_tag::OBJECT_IDENTIFIER));
//        return result;
//    }
//
//    [[nodiscard]] std::string to_string() const override {
//        std::string result = "OBJECT IDENTIFIER: ";
//        for (size_t i = 0; i < _value.size(); ++i) {
//            result += std::to_string(_value[i]);
//            if (i != _value.size() - 1) {
//                result += ".";
//            }
//        }
//        return result;
//    }
//
//    void decode(const uint8_t *buffer) override {
//        const asn1_base raw = asn1_base({buffer, 0xFFFFFFFF});
//
//        if (raw.get_length() < 1) {
//            throw std::runtime_error("Invalid length for OBJECT IDENTIFIER value");
//        }
//
//        std::vector<uint32_t> result;
//        result.push_back(raw.get_data()[0] / 40);
//        result.push_back(raw.get_data()[0] % 40);
//        uint32_t value = 0;
//        for (size_t i = 1; i < raw.get_length(); ++i) {
//            if (raw.get_data()[i] & 0x80) {
//                value = (value << 7) | (raw.get_data()[i] & 0x7F);
//            } else {
//                value = (value << 7) | raw.get_data()[i];
//                result.push_back(value);
//                value = 0;
//            }
//        }
//
//        if (result.size() < 2) {
//            throw std::runtime_error("Invalid OBJECT IDENTIFIER value");
//        }
//        this->_value = result;
//    }
//};

#endif //ASNCPP_OBJECT_IDENTIFIER_H

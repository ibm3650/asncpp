//
// Created by kandu on 28.11.2024.
//

#ifndef ASNCPP_BIT_STRING_H
#define ASNCPP_BIT_STRING_H

#include <string>
#include <stdexcept>
#include <bitset>
#include "../asn1/base.h"

class bit_string_t : public asn1_type<std::vector<uint8_t>, static_cast<uintmax_t>(asn1_tag::BIT_STRING)> {
public:
    bit_string_t() = default;

//    explicit bit_string_t(const std::vector<uint8_t> &data) {
//        this->_value = data;
//    }
//
//    template <size_t N>
//    explicit bit_string_t(const std::bitset<N>& data) {
//        this->_value = data;
//    }
//
//    explicit bit_string_t(std::string_view data) {
//        this->_value = std::vector<uint8_t>(data.begin(), data.end());
//    }
//
//    explicit bit_string_t(const std::vector<uint8_t> &data, size_t unused_bits) {
//        this->_value = data;
//        this->_value.insert(this->_value.begin(), static_cast<uint8_t>(unused_bits));
//    }
//
//    explicit bit_string_t(const std::vector<uint8_t> &data, size_t unused_bits, size_t length) {
//        this->_value = data;
//        this->_value.insert(this->_value.begin(), static_cast<uint8_t>(unused_bits));
//        this->_value.insert(this->_value.begin(), static_cast<uint8_t>(length));
//    }
//
//    explicit bit_string_t(const std::vector<uint8_t> &data, size_t unused_bits, size_t length, bool is_last_byte) {
//        this->_value = data;
//        this->_value.insert(this->_value.begin(), static_cast<uint8_t>(unused_bits));
//        this->_value.insert(this->_value.begin(), static_cast<uint8_t>(length));
//        this->_value.insert(this->_value.begin(), static_cast<uint8_t>(is_last_byte));
//    }
//    void encode(uint64_t data) {
////    bit_string_t(uint64_t data, size_t unused_bits) {
//        size_t length = 0;
//        while (data) {
//            this->_value.insert(this->_value.begin(), static_cast<uint8_t>(data & 0xFF));
//            data >>= 8;
//        }
//        uint8_t last_byte = this->_value[0];
//        while (last_byte) {
//            last_byte >>= 1;
//            ++length;
//        }
//        this->_value.insert(this->_value.begin(), static_cast<uint8_t>(8 - length));
//        this->_value.insert(this->_value.begin(), static_cast<uint8_t>(this->_value.size()));
//        this->_value.insert(this->_value.begin(), static_cast<uint8_t>(asn1_tag::BIT_STRING));
////        this->_value = std::vector<uint8_t>(sizeof(data));
////        for (size_t i = 0; i < sizeof(data); ++i) {
////            this->_value[i] = (data >> (8 * i)) & 0xFF;
////        }
////        this->_value.insert(this->_value.begin(), static_cast<uint8_t>(unused_bits));
//    }

    bit_string_t(uint64_t data, size_t payload_bits) {
        _value.clear();
        while (data) {
            this->_value.insert(this->_value.begin(), static_cast<uint8_t>(data & 0xFF));
            data >>= 8;
        }
        this->_value.insert(this->_value.begin(), static_cast<uint8_t>(8 - (payload_bits % 8)));
    }

    void decode(const uint8_t *buffer) final {
//        const asn1_block raw = asn1_block(buffer);
//
//        if (raw.get_length() < 1) {
//            throw std::runtime_error("Invalid length for BIT STRING value");
//        }
//
//        this->_value.clear();
//        this->_value.insert(this->_value.end(), raw.get_data(), raw.get_data() + raw.get_length());
    }

    [[nodiscard]] std::vector<uint8_t> encode() const final {
        std::vector<uint8_t> result;
        result.insert(result.end(), this->_value.begin(), this->_value.end());
        result.insert(result.begin(), result.size());
        result.insert(result.begin(), static_cast<uint8_t>(asn1_tag::BIT_STRING));
        return result;
    }
    //TODO: хранить число фактически занимаемых битов
    [[nodiscard]] std::string to_string() const override {
        std::string result = "BIT STRING: ";
        for (size_t i = 1; i < this->_value.size(); ++i) {
            uint8_t value = this->_value[i];
            for (size_t j = 0; j < 8; ++j) {
                result += ((value & 0x80) == 0x80) + '0';
                value <<= 1;
            }
        }
        return result;
    }
};

#endif //ASNCPP_BIT_STRING_H

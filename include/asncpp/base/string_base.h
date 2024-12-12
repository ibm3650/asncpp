//
// Created by kandu on 12.12.2024.
//

#ifndef STRING_BASE_H
#define STRING_BASE_H

#include "asn_base.h"
#include <string>
#include <string_view>

template<class T, asn1_tag type, auto limitations = nullptr>
class string_base_t : public asn1_base {
public:
    string_base_t() {
        _type = type;
    }

    string_base_t(std::basic_string_view<T> data) {
        _type = type;
        if constexpr (limitations != nullptr) {
            for (const T symbol: data) {
                if (!limitations(symbol)) {
                    throw std::invalid_argument("Invalid character in VISIBLE_STRING");
                }
            }
        }

        this->_value = data;
    }

    [[nodiscard]] auto encode() -> std::vector<uint8_t> final {
        if constexpr(sizeof(T) == 1) {
            _data.append_range(_value);
            return _data;
        }
        for (auto symbol: _value) {
            // for (const auto& symbol : _value) {
            for (size_t i = 0; i < sizeof(T); ++i) {
                this->_data.insert(this->_data.begin(), static_cast<uint8_t>(symbol & 0xFFU));
                symbol >>= 8U;
                //uint8_t byte = (symbol >> (i * 8U)) & 0xFFU;
                // _data.insert(_data.begin(), byte);
            }
        }
        return _data;
    }


    //TODO: Implement long tag encoding
    //TODO: REserve space for length
    void decode(std::span<const uint8_t> data) final {
        for (size_t i = 0; i < _data.size(); i += sizeof(T)) {
            T symbol{};
            for (size_t j = 0; j < sizeof(T); ++j) {
                symbol = (symbol << 8U) | static_cast<T>(data[i + j]);
            }
            _value += symbol;
        }
    }
private:
    std::basic_string<T> _value{};
};

using bmp_string_t = string_base_t<char16_t, asn1_tag::BMP_STRING>;
using universal_string_t = string_base_t<char32_t, asn1_tag::UNIVERSAL_STRING>;
// using visible_string_t [[deprecated]] = string_base_t<char, asn1_tag::VISIBLE_STRING, [](char c) { return std::isprint(c); }>;

#endif //STRING_BASE_H

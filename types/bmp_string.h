//
// Created by kandu on 29.11.2024.
//

#ifndef ASNCPP_BMP_STRING_H
#define ASNCPP_BMP_STRING_H

#include "../asn1/base.h"
#include <string>
#include <string_view>


class bmp_string_t : public asn1_type<std::u16string, static_cast<uintmax_t>(asn1_tag::BMP_STRING)> {
public:
    bmp_string_t() = default;

    bmp_string_t(std::u16string_view data) {
        this->_value = data;
    }

    void decode(const uint8_t *buffer) final {
        const asn1_block raw = asn1_block(buffer);
        for (size_t i = 0; i < raw.get_data().size(); i += 2) {
            const char16_t symbol = static_cast<char16_t>(raw.get_data()[i + 1]) |
                                    (static_cast<char16_t>(raw.get_data()[i]) << 8);
            this->_value.push_back(symbol);
        }
        //this->_value.insert_range(this->_value.begin(), raw.get_data());
    }

    [[nodiscard]] std::vector<uint8_t> encode() const final {
        std::vector<uint8_t> result;
        for (const auto &symbol: this->_value) {
            result.emplace_back(static_cast<uint8_t>(symbol >> 8));
            result.emplace_back(static_cast<uint8_t>(symbol & 0xFF));
        }
        result.insert(result.begin(), result.size());
        result.insert(result.begin(), static_cast<uint8_t>(asn1_tag::BMP_STRING));
        return result;
    }

    [[nodiscard]] std::string to_string() const {
        return "BMP String";
    }

};

#endif //ASNCPP_BMP_STRING_H

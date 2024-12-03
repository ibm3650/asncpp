//
// Created by kandu on 02.12.2024.
//

#ifndef ASNCPP_IA5_STRING_H
#define ASNCPP_IA5_STRING_H
#include "../include/asn_base.h"
#include <stdexcept>
#include <string>
#include <string_view>


class ia5_string_t : public asn1_type<std::string, static_cast<uintmax_t>(asn1_tag::IA5_STRING)> {
public:
    ia5_string_t() = default;

    ia5_string_t(std::string_view data) { // NOLINT(*-explicit-constructor, *-explicit-conversions)
        for (const char symbol: data) {
            if (static_cast<uint8_t>(symbol) > 0x7F) {
                throw std::invalid_argument("Invalid character in VISIBLE_STRING");
            }
        }
        this->_value = data;
    }

    void decode(const uint8_t *buffer) final {
        const asn1_base raw = asn1_base({buffer, 0xFFFFFFFF});
        this->_value.insert_range(this->_value.begin(), raw.get_data());
    }

    [[nodiscard]] std::vector<uint8_t> encode() const final {
        std::vector<uint8_t> result;
        result.insert(result.end(), this->_value.begin(), this->_value.end());
        result.insert(result.begin(), result.size());
        result.insert(result.begin(), static_cast<uint8_t>(asn1_tag::PRINTABLE_STRING));
        return result;
    }

    [[nodiscard]] std::string to_string() const final {
        return "IA5 String: " + this->_value;
    }
};
#endif //ASNCPP_IA5_STRING_H

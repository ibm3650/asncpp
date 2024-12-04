//
// Created by kandu on 29.11.2024.
//

#ifndef ASNCPP_PRINTABLE_STRING_H
#define ASNCPP_PRINTABLE_STRING_H

#include "asncpp/asn_base.h"
#include <stdexcept>
#include <string>
#include <string_view>


class printable_string_t : public asn1_type<std::string, static_cast<uintmax_t>(asn1_tag::PRINTABLE_STRING)> {
public:
    printable_string_t() = default;

    printable_string_t(std::string_view data) { // NOLINT(*-explicit-constructor, *-explicit-conversions)
        using namespace std::string_view_literals;
        const auto allowed_specs = " '()+,-./:=?"sv;
        for (const char symbol: data) {
            if (!std::isalnum(symbol) && !allowed_specs.contains(symbol)) {
                throw std::invalid_argument("Invalid character in PRINTABLE_STRING");
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
        return "Printable String: " + this->_value;
    }
};

#endif //ASNCPP_PRINTABLE_STRING_H

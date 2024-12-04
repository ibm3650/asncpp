//
// Created by kandu on 29.11.2024.
//

#ifndef ASNCPP_NUMERIC_STRING_H
#define ASNCPP_NUMERIC_STRING_H

#include <string>
#include <stdexcept>
#include "asncpp/asn_base.h"

class numeric_string_t : public asn1_type<std::string, static_cast<uintmax_t>(asn1_tag::NUMERIC_STRING)> {
public:
    numeric_string_t() = default;

    explicit numeric_string_t(const std::string &data) {
        this->_value = data;
        //TODO: std::isdigit() check
    }

    void decode(const uint8_t *buffer) final {
        const asn1_base raw = asn1_base({buffer, 0xFFFFFFFF});
        this->_value.insert(this->_value.end(), raw.get_data().begin(), raw.get_data().end());
    }

    [[nodiscard]] std::vector<uint8_t> encode() const final {
        std::vector<uint8_t> result;
        result.insert(result.end(), this->_value.begin(), this->_value.end());
        result.insert(result.begin(), result.size());
        result.insert(result.begin(), static_cast<uint8_t>(asn1_tag::NUMERIC_STRING));
        return result;
    }

    [[nodiscard]] std::string to_string() const override {
        return "Numeric String: " + this->_value;
    }
};


#endif //ASNCPP_NUMERIC_STRING_H

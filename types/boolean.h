//
// Created by kandu on 26.11.2024.
//

#ifndef ASNCPP_BOOLEAN_H
#define ASNCPP_BOOLEAN_H

#include <stdexcept>
#include "asncpp/common.h"

using namespace std::string_literals;

class boolean_t : public asn1_type<bool, static_cast<uintmax_t>(asn1_tag::BOOLEAN)> {
public:
    explicit boolean_t(bool val) {
        this->_value = val;
    }

    void decode(const uint8_t *buffer) final {
        const asn1_base raw = asn1_base({buffer, 0xFFFFFFFF});

        if (raw.get_length() != 1) {
            throw std::runtime_error("Invalid length for BOOLEAN value");
        }

        this->_value = raw.get_data()[0] != 0x00;
    }

    [[nodiscard]] std::vector<uint8_t> encode() const final {
        std::vector<uint8_t> output;
        output.push_back(this->_value ? 0xFF : 0x00);
        output.insert(output.begin(), 1);
        output.insert(output.begin(), static_cast<uint8_t>(asn1_tag::INTEGER));
        return output;
    }

    [[nodiscard]] std::string to_string() const final {
        return "BOOLEAN: "s + (this->_value ? "TRUE" : "FALSE");
    }

    [[nodiscard]] explicit operator bool() const noexcept {
        return this->_value;
    }
};


#endif //ASNCPP_BOOLEAN_H

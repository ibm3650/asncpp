
//
// Created by kandu on 27.11.2024.
//

#ifndef ASNCPP_NULL_H
#define ASNCPP_NULL_H
#include <string>
#include <stdexcept>
#include "asncpp/common.h"

class null_t : public asn1_type<std::nullptr_t, static_cast<uintmax_t>(asn1_tag::Null)> {
public:
    null_t() = default;

    void decode(const uint8_t *buffer) final {
        const asn1_base raw = asn1_base({buffer, 0xFFFFFFFF});

        if (raw.get_length() != 0) {
            throw std::runtime_error("Invalid NULL value");
        }
    }


    [[nodiscard]] std::vector<uint8_t> encode() const final {
        const std::vector<uint8_t> result = {static_cast<uint8_t>(asn1_tag::Null), 0};
        return result;
    }

    [[nodiscard]] std::string to_string() const override {
        return "NULL";
    }


};
#endif //ASNCPP_NULL_H

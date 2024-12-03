//
// Created by kandu on 27.11.2024.
//

#ifndef ASNCPP_OCTET_STRING_H
#define ASNCPP_OCTET_STRING_H
#include <string>
#include <stdexcept>
#include "../include/asn_base.h"

class octet_string_t : public asn1_type<std::vector<uint8_t>, static_cast<uintmax_t>(asn1_tag::OCTET_STRING)> {
public:
    octet_string_t() = default;
    explicit octet_string_t(const std::vector<uint8_t> &data) {
        this->_value = data;
    }

    void decode(const uint8_t *buffer) final {
        const asn1_base raw = asn1_base({buffer, 0xFFFFFFFF});

        this->_value.clear();
        this->_value.insert(this->_value.end(), raw.get_data(), raw.get_data() + raw.get_length());
    }

    [[nodiscard]] std::vector<uint8_t> encode() const final {
        std::vector<uint8_t> result;
        result.insert(result.end(), this->_value.begin(), this->_value.end());
        result.insert(result.begin(), result.size());
        result.insert(result.begin(), static_cast<uint8_t>(asn1_tag::OCTET_STRING));
        return result;
    }

    [[nodiscard]] std::string to_string() const override {
        std::string result = "OCTET STRING: ";
        for (size_t i = 0; i < this->_value.size(); ++i) {
            result += std::to_string(this->_value[i]);
            if (i != this->_value.size() - 1) {
                result += " ";
            }
        }
        return result;
    }
};
#endif //ASNCPP_OCTET_STRING_H

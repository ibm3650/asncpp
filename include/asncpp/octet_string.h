//
// Created by kandu on 27.11.2024.
//

#ifndef ASNCPP_OCTET_STRING_H
#define ASNCPP_OCTET_STRING_H
#include <string>
#include <stdexcept>
#include "base/common.h"

class octet_string_t : public asn1_base {
public:
    octet_string_t() = default;

    octet_string_t(std::span<const uint8_t> data) {
        this->_value.assign_range(data);
    }

    void decode(std::span<const uint8_t> data) final {
        this->_value.append_range(this->_data);
    }

    [[nodiscard]] std::vector<uint8_t> encode() final {
        this->_data.append_range(this->_value);
        return _data;
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
private:
    std::vector<uint8_t> _value;
};
#endif //ASNCPP_OCTET_STRING_H

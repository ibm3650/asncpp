//
// Created by kandu on 28.11.2024.
//

#ifndef ASNCPP_OBJECT_DESCRIPTOR_H
#define ASNCPP_OBJECT_DESCRIPTOR_H
#include <string>
#include <stdexcept>
#include "asncpp/common.h"

class object_descriptor_t : public asn1_type<std::string, static_cast<uintmax_t>(asn1_tag::OBJECT_DESCRIPTOR)> {
public:
    object_descriptor_t() = default;

    explicit object_descriptor_t(const std::string &data) {
        this->_value = data;
    }

    void decode(const uint8_t *buffer) final {
        const asn1_base raw = asn1_base({buffer, 0xFFFFFFFF});

        this->_value.clear();
        this->_value.insert(this->_value.end(), raw.get_data().begin(), raw.get_data().end());
    }

    [[nodiscard]] std::vector<uint8_t> encode() const final {
        std::vector<uint8_t> result;
        result.insert(result.end(), this->_value.begin(), this->_value.end());
        result.insert(result.begin(), result.size());
        result.insert(result.begin(), static_cast<uint8_t>(asn1_tag::OBJECT_DESCRIPTOR));
        return result;
    }

    [[nodiscard]] std::string to_string() const override {
        return "Object Descriptor: " + this->_value;
    }
};
#endif //ASNCPP_OBJECT_DESCRIPTOR_H

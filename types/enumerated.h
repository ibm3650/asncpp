//
// Created by kandu on 27.11.2024.
//

#ifndef ASNCPP_ENUMERATED_H
#define ASNCPP_ENUMERATED_H

#include <string>
#include <stdexcept>
#include "../asn1/base.h"
using namespace std::string_literals;

class enumerated_t : public asn1_type<uintmax_t, static_cast<uintmax_t>(asn1_tag::ENUMERATED)> {
    public:
    enumerated_t() = default;
    explicit enumerated_t(uintmax_t val) {
        this->_value = val;
    }

    void decode(const uint8_t *buffer) final {
        const asn1_block raw = asn1_block(buffer);

        if (raw.get_length() > sizeof(uintmax_t)) {
            throw std::runtime_error("ENUMERATED value is too large");
        }

        this->_value = 0; // Обнуление перед началом декодирования

        // Собираем значение из байтов
        for (size_t i = 0; i < raw.get_length(); ++i) {
            this->_value = (this->_value << 8) | raw.get_data()[i];
        }
    }

    [[nodiscard]] explicit operator uintmax_t() const noexcept {
        return this->_value;
    }

    [[nodiscard]] std::vector<uint8_t> encode() const final {
        std::vector<uint8_t> output;
        uintmax_t temp = this->_value;

        // Кодирование значения в big-endian формате
        do {
            output.insert(output.begin(), static_cast<uint8_t>(temp & 0xFF));
            temp >>= 8;
        } while (temp != 0);
        output.insert(output.begin(), output.size());
        output.insert(output.begin(), static_cast<uint8_t>(asn1_tag::ENUMERATED));

        return output;
    }

    [[nodiscard]] std::string to_string() const override {
        return "ENUMERATED: "s + std::to_string(this->_value);
    }

};
#endif //ASNCPP_ENUMERATED_H

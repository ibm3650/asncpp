//
// Created by kandu on 06.01.2025.
//

#ifndef REAL_H
#define REAL_H
#include "base/asn1_basic.h"
#include <cmath>
#include <sstream>

class real_t : public asncpp::base::asn1_basic {
public:
    using value_type = double;

    explicit real_t(asncpp::base::asn1_basic &&base) : asncpp::base::asn1_basic(std::move(base)) {}

    real_t() noexcept = default;

    real_t(real_t &&) noexcept = default;

    real_t(const real_t &) noexcept = default;

    real_t &operator=(real_t &&) noexcept = default;

    real_t &operator=(const real_t &) noexcept = default;

    ~real_t() noexcept final = default;

    explicit real_t(value_type val) noexcept : _value{val} {}

    [[nodiscard]] constexpr uintmax_t get_tag() const noexcept final {
        return static_cast<uintmax_t>(asncpp::base::asn1_tag::REAL);
    }

    [[nodiscard]] value_type value() const noexcept {
        return _value;
    }

    void decode(std::span<const uint8_t>  /*data*/) final {
        if (_data.empty()) {
            throw std::invalid_argument("Invalid REAL encoding: empty data");
        }

        const uint8_t first_byte = _data[0];

        if (first_byte == 0x40) {
            _value = std::numeric_limits<value_type>::infinity();
        } else if (first_byte == 0x41) {
            _value = -std::numeric_limits<value_type>::infinity();
        } else if (first_byte == 0x42) {
            _value = 0.0; // Explicit zero
        } else {
            // Decode using binary base, exponent, and mantissa
            uint8_t base = (first_byte & 0x30) >> 4;
            uint8_t scale = first_byte & 0x0F;

            size_t exponent_length = (_data[1] & 0x80) ? (_data[1] & 0x7F) : 1;
            size_t offset = 2;

            int exponent = 0;
            for (size_t i = 0; i < exponent_length; ++i) {
                exponent = (exponent << 8) | _data[offset++];
            }

            double mantissa = 0.0;
            for (size_t i = offset; i < _data.size(); ++i) {
                mantissa = (mantissa * 256) + _data[i];
            }

            mantissa *= std::pow(2.0, -scale * 8);
            _value = mantissa * std::pow(base == 0 ? 2.0 : 10.0, exponent);
        }
    }

    [[nodiscard]] asncpp::base::dynamic_array_t encode() final {
        if (std::isinf(_value)) {
            _data.push_back(_value > 0 ? 0x40 : 0x41);
        } else if (_value == 0.0) {
            _data.push_back(0x42);
        } else {
            int exponent;
            double mantissa = std::frexp(_value, &exponent);

            uint8_t scale = 0;
            mantissa *= std::pow(2.0, scale * 8);

            _data.push_back(0x80 | (scale & 0x0F));

            // Encode exponent
            if (exponent < 128 && exponent > -128) {
                _data.push_back(static_cast<uint8_t>(exponent & 0xFF));
            } else {
                _data.push_back(0x81);
                _data.push_back(static_cast<uint8_t>(exponent & 0xFF));
            }

            // Encode mantissa
            mantissa = std::fabs(mantissa);
            while (mantissa != 0.0) {
                uint8_t byte = static_cast<uint8_t>(mantissa);
                _data.push_back(byte);
                mantissa -= byte;
                mantissa *= 256;
            }
        }

        return _data;
    }

    [[nodiscard]] std::string to_string() const final {
        std::ostringstream oss;
        oss << "REAL: " << _value;
        return oss.str();
    }

private:
    value_type _value = 0.0;
};

#endif //REAL_H

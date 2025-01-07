
//
// Created by kandu on 27.11.2024.
//

#ifndef ASNCPP_NULL_H
#define ASNCPP_NULL_H
#include <string>
#include <stdexcept>
#include "asncpp/base/asn1_basic.h"

class null_t : public asncpp::base::asn1_basic {
public:
  explicit null_t(asncpp::base::asn1_basic &&base) : asncpp::base::asn1_basic(std::move(base)) {}
    null_t() = default;

    void decode(std::span<const uint8_t> data) final {
        //const asn1_base raw = asn1_base({buffer, 0xFFFFFFFF});

        if (_data.size() != 0) {
            throw std::runtime_error("Invalid NULL value");
        }
    }


    [[nodiscard]] std::vector<uint8_t> encode() final {
        //const std::vector<uint8_t> result = {static_cast<uint8_t>(asn1_tag::Null)};
        return _data;
    }

    [[nodiscard]] std::string to_string() const override {
        return "NULL";
    }

     uintmax_t    get_tag() const noexcept final {
        return static_cast<uintmax_t>(asncpp::base::asn1_tag::Null);
    }
};
#endif //ASNCPP_NULL_H

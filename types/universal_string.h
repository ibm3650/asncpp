//
// Created by kandu on 02.12.2024.
//

#ifndef ASNCPP_UNIVERSAL_STRING_H
#define ASNCPP_UNIVERSAL_STRING_H
#include "../include/asn_base.h"
#include <stdexcept>
#include <string>
#include <string_view>


class universal_string_t : public asn1_type<std::u32string, static_cast<uintmax_t>(asn1_tag::UNIVERSAL_STRING)> {
public:
    universal_string_t() = default;

    universal_string_t(std::u32string_view data) { // NOLINT(*-explicit-constructor, *-explicit-conversions)
        this->_value = data;
    }

    void decode(const uint8_t *buffer) final {
        const asn1_base raw = asn1_base({buffer, 0xFFFFFFFF});
        for (size_t i = 0; i < raw.get_data().size(); i += 4) {
            const char32_t symbol = static_cast<char32_t>(raw.get_data()[i + 3]) |
                                    (static_cast<char32_t>(raw.get_data()[i + 2]) << 8) |
                                    (static_cast<char32_t>(raw.get_data()[i + 1]) << 16) |
                                    (static_cast<char32_t>(raw.get_data()[i]) << 24);
            this->_value.push_back(symbol);
        }
    }

    [[nodiscard]] std::vector<uint8_t> encode() const final {
        std::vector<uint8_t> result;
        for (const auto &symbol: this->_value) {
            result.emplace_back(static_cast<uint8_t>(symbol >> 24));
            result.emplace_back(static_cast<uint8_t>(symbol >> 16));
            result.emplace_back(static_cast<uint8_t>(symbol >> 8));
            result.emplace_back(static_cast<uint8_t>(symbol & 0xFF));
        }
        result.insert(result.begin(), result.size());
        result.insert(result.begin(), static_cast<uint8_t>(asn1_tag::UNIVERSAL_STRING));
        return result;
    }

    [[nodiscard]] std::string to_string() const final {
        return "IA5 String: ";
    }
};
#endif //ASNCPP_UNIVERSAL_STRING_H

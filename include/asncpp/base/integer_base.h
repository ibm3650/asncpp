//
// Created by kandu on 22.11.2024.
//

#ifndef ASNCPP_INTEGER_H
#define ASNCPP_INTEGER_H
#include <cstdint>
#include <string>
#include <stdexcept>
#include "asn1_basic.h"
#include "common.h"

namespace asncpp::base {
    /**
     * @class integer_basic
     * @brief Implements ASN.1 INTEGER and ENUMERATED types.
     *
     * Supports encoding and decoding integers with two's complement representation.
     * The class template accepts a numeric type and an ASN.1 tag type.
     *
     * @tparam T Integral type (e.g., int, long, intmax_t).
     * @tparam type ASN.1 tag (e.g., INTEGER, ENUMERATED).
     */
    template<std::integral T, asn1_tag type>
    class integer_basic : public asn1_basic {
    public:
        using value_type = T;
        using const_reference = const T &;

        integer_basic() noexcept = default;

        integer_basic(integer_basic &&) noexcept = default;

        integer_basic(const integer_basic &) noexcept = default;

        integer_basic &operator=(integer_basic &&) noexcept = default;

        integer_basic &operator=(const integer_basic &) noexcept = default;

        explicit integer_basic(value_type val) noexcept : _decoded{val} {
        }

        ~integer_basic() noexcept final = default;

        /**
         * @brief Encodes the integer value into ASN.1 format.
         * @return Encoded byte vector.
         */
        dynamic_array_t encode() final;

        /**
         * @brief Decodes the integer from ASN.1 byte data.
         * @param data Byte span containing encoded ASN.1 data.
         */
        void decode(std::span<const uint8_t> data) final;

        /**
         * @brief Retrieves the tag of the ASN.1 type.
         * @return Tag value as uintmax_t.
         */
        constexpr uintmax_t get_tag() const noexcept final {
            return static_cast<uintmax_t>(type);
        }

        /**
         * @brief Retrieves the decoded integer value.
         * @return The decoded integer value.
         */
        constexpr const_reference get_value() const noexcept {
            return _decoded;
        }

        /**
         * @brief Converts the integer value to a string representation.
         * @return String representation of the integer value.
         */
        std::string to_string() const final {
            return std::to_string(_decoded);
        }

    private:
        value_type _decoded{}; ///< Decoded integer value.
    };
} // namespace asncpp::base

#endif //ASNCPP_INTEGER_H

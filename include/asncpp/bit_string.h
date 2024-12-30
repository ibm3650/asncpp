//
// Created by kandu on 28.11.2024.
//

#ifndef ASNCPP_BIT_STRING_H
#define ASNCPP_BIT_STRING_H


#include "asncpp/base/asn1_basic.h"
namespace asncpp::types {
    class bit_string_t : public asncpp::base::asn1_basic {
    public:
        explicit bit_string_t(asn1_basic &&base) : asn1_basic(std::move(base)) {
        };

        bit_string_t() noexcept = default;

        bit_string_t(bit_string_t &&) noexcept = default;

        bit_string_t(const bit_string_t &) noexcept = default;

        bit_string_t &operator=(bit_string_t &&) noexcept = default;

        bit_string_t &operator=(const bit_string_t &) noexcept = default;

        ~bit_string_t() noexcept final = default;

        /**
         * @brief Constructs a BIT STRING from a vector of bools.
         * @param data A vector of boolean values representing bits.
         */
        explicit bit_string_t(const std::vector<bool> &data);

        /**
         * @brief Constructs a BIT STRING from a binary string.
         * @param data A string view containing '0' and '1' characters.
         * @throws std::invalid_argument if the string contains invalid characters.
         */
        bit_string_t(std::string_view data);

        /**
         * @brief Constructs a BIT STRING from an unsigned 64-bit integer.
         * @param data An integer value.
         * @param bit_length Number of significant bits.
         * @throws std::invalid_argument if bit_length > 64.
         */
        bit_string_t(uint64_t data, size_t bit_length);

        /**
         * @brief Constructs a BIT STRING from a byte vector and bit length.
         * @param data A vector of bytes.
         * @param bit_length Number of significant bits in the data.
         * @throws std::invalid_argument if bit_length exceeds data size in bits.
         */
        bit_string_t(const std::vector<uint8_t> &data, size_t bit_length);


        /**
         * @brief Decodes the BIT STRING from ASN.1 format.
         * @param data A span containing the encoded data.
         * @throws std::invalid_argument on invalid encoding.
         */
        void decode(std::span<const uint8_t> data) final;

        /**
         * @brief Encodes the BIT STRING into ASN.1 format.
         * @return A vector of bytes representing the encoded BIT STRING.
         */
        asncpp::base::dynamic_array_t encode() final;

        /**
         * @brief Converts the BIT STRING to a human-readable string.
         * @return A string representation of the BIT STRING.
         */
        [[nodiscard]] std::string to_string() const final;

        /**
         * @brief Gets the tag for BIT STRING.
         * @return The tag value for BIT STRING.
         */
        [[nodiscard]] constexpr uintmax_t get_tag() const noexcept final {
            return static_cast<uintmax_t>(asncpp::base::asn1_tag::BIT_STRING);
        }

        /**
         * @brief Gets the raw value of the BIT STRING.
         * @return A const reference to the internal byte vector.
         */
        [[nodiscard]] const std::vector<uint8_t> &value() const;

        /**
         * @brief Gets the bit length of the BIT STRING.
         * @return The number of significant bits.
         */
        [[nodiscard]] size_t bit_length() const;

    private:
        std::vector<uint8_t> _value; ///< The raw value of the BIT STRING.
        size_t _bit_length = 0; ///< The number of significant bits.
    };
}   // namespace asncpp::types



#endif //ASNCPP_BIT_STRING_H

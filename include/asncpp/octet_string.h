//
// Created by kandu on 27.11.2024.
//

// #ifndef ASNCPP_OCTET_STRING_H
// #define ASNCPP_OCTET_STRING_H
// #include <string>
// #include <stdexcept>
// #include "asncpp/base/asn1_basic.h"
//
// class octet_string_t : public asncpp::base::asn1_basic {
// public:
//     using value_type = std::vector<uint8_t>;
//
//     explicit octet_string_t(asn1_basic &&base) : asn1_basic(std::move(base)) {};
//
//     octet_string_t() noexcept = default;
//
//     octet_string_t(octet_string_t &&) noexcept = default;
//
//     octet_string_t(const octet_string_t &) noexcept = default;
//
//     octet_string_t &operator=(octet_string_t &&) noexcept = default;
//
//     octet_string_t &operator=(const octet_string_t &) noexcept = default;
//
//     ~octet_string_t() noexcept final = default;
//
//     octet_string_t(std::span<const uint8_t> data) {
//         this->_value.assign_range(data);
//     }
//
//     void decode(std::span<const uint8_t>  /*data*/) final {
//         _value.append_range(this->_data);
//     }
//
//
//     [[nodiscard]] std::vector<uint8_t> encode() final {
//         _data.append_range(this->_value);
//         return _data;
//     }
//
//     [[nodiscard]] std::string to_string() const override {
//         std::string result{};
//         for (size_t i = 0; i < this->_value.size(); ++i) {
//             result += std::to_string(this->_value[i]);
//             if (i != this->_value.size() - 1) {
//                 result += " ";
//             }
//         }
//         return result;
//     }
//
//     constexpr uintmax_t get_tag() const noexcept final {
//         return static_cast<uintmax_t>(asncpp::base::asn1_tag::OCTET_STRING);
//     }
//
// private:
//     std::vector<uint8_t> _value;
// };
// #endif //ASNCPP_OCTET_STRING_H
// //
// // Created by kandu on 27.11.2024.
// //
 #ifndef ASNCPP_OCTET_STRING_H
#define ASNCPP_OCTET_STRING_H

#include <vector>
#include <string>
#include <stdexcept>
#include <span>
#include <ranges>
#include <sstream>
#include "base/asn1_basic.h"

/**
 * @class octet_string_t
 * @brief Represents the ASN.1 OCTET STRING type.
 */
class octet_string_t : public asncpp::base::asn1_basic {
public:
    using value_type = std::vector<uint8_t>;


    /**
     * @brief Default constructor.
     */
    octet_string_t() noexcept = default;

    /**
     * @brief Constructor that takes a span of bytes.
     * @param data Span of bytes to initialize the octet string.
     */
    explicit octet_string_t(std::span<const uint8_t> data) : _value{data.begin(), data.end()} {}

    /**
     * @brief Constructor that takes a base ASN.1 object.
     * @param base ASN.1 basic object.
     */
    explicit octet_string_t(asn1_basic &&base) : asn1_basic(std::move(base)) {}

    /**
     * @brief Copy constructor.
     */
    octet_string_t(const octet_string_t &) noexcept = default;

    /**
     * @brief Move constructor.
     */
    octet_string_t(octet_string_t &&) noexcept = default;

    /**
     * @brief Copy assignment operator.
     */
    octet_string_t &operator=(const octet_string_t &) noexcept = default;

    /**
     * @brief Move assignment operator.
     */
    octet_string_t &operator=(octet_string_t &&) noexcept = default;

    /**
     * @brief Destructor.
     */
    ~octet_string_t() noexcept final = default;

    /**
     * @brief Decodes the OCTET STRING from ASN.1 format.
     * @param data Span containing the encoded data.
     * @throws std::invalid_argument if the data is invalid.
     */
    void decode(std::span<const uint8_t> data) final {
        _value.assign(_data.begin(), _data.end());
    }

    /**
     * @brief Encodes the OCTET STRING into ASN.1 format.
     * @return Encoded OCTET STRING as a vector of bytes.
     */
    [[nodiscard]] std::vector<uint8_t> encode() final {
        _data.append_range(this->_value);
       // _data = _value; // Direct copy for primitive OCTET STRING
        return _data;
    }

    /**
     * @brief Converts the OCTET STRING to a human-readable string.
     * @return Space-separated string representation of the byte values.
     */
    [[nodiscard]] std::string to_string() const final {
        std::ostringstream oss;
        for (size_t i = 0; i < _value.size(); ++i) {
            oss << static_cast<int>(_value[i]);
            if (i != _value.size() - 1) {
                oss << " ";
            }
        }
        return oss.str();
    }

    /**
     * @brief Gets the ASN.1 tag for OCTET STRING.
     * @return The tag value for OCTET STRING.
     */
    constexpr uintmax_t get_tag() const noexcept final {
        return static_cast<uintmax_t>(asncpp::base::asn1_tag::OCTET_STRING);
    }

    /**
     * @brief Gets the raw value of the OCTET STRING.
     * @return Const reference to the internal byte vector.
     */
    [[nodiscard]] const std::vector<uint8_t> &value() const noexcept {
        return _value;
    }

private:
    std::vector<uint8_t> _value; ///< Internal storage for the OCTET STRING value.
};

#endif //ASNCPP_OCTET_STRING_H











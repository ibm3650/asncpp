//
// Created by kandu on 21.11.2024.
//

#ifndef ASNCPP_ASN_BASE_H
#define ASNCPP_ASN_BASE_H

#include <vector>
#include <span>
#include <stdexcept>
#include <variant>
#include <iterator>
#include <memory>

/**
 * @enum asn1_tag
 * @brief Enumerates ASN.1 tags as defined in the DER/BER standards.
 *
 * This enumeration provides a comprehensive list of ASN.1 tags used
 * for identifying the type of encoded data. Tags are based on the
 * X.690 standard and include both modern and deprecated types.
 */
enum class asn1_tag : uint8_t {
    Reserved = 0x00, /**< Reserved tag, not used. */
    BOOLEAN = 0x01, /**< Boolean type: TRUE or FALSE. */
    INTEGER = 0x02, /**< Integer type: signed integer with two's complement encoding. */
    BIT_STRING = 0x03, /**< Bit string type: binary data with possible unused bits. */
    OCTET_STRING = 0x04, /**< Octet string type: arbitrary binary data. */
    Null = 0x05, /**< Null type: represents no value. */
    OBJECT_IDENTIFIER = 0x06, /**< Object Identifier (OID): unique identifier. */
    OBJECT_DESCRIPTOR = 0x07, /**< Object Descriptor: human-readable description. */
    EXTERNAL = 0x08, /**< EXTERNAL type: rarely used, for referencing external data. */
    REAL = 0x09, /**< REAL type: floating-point numbers. */
    ENUMERATED = 0x0A, /**< Enumerated type: enumeration of integer values. */
    EMBEDDED_PDV = 0x0B, /**< Embedded Presentation Data Value: complex structures. */
    UTF8_STRING = 0x0C, /**< UTF-8 string type: Unicode text encoded in UTF-8. */
    RELATIVE_OID = 0x0D, /**< Relative OID: compressed OID for shorter identifiers. */
    SEQUENCE = 0x10, /**< SEQUENCE type: ordered collection of elements. */
    SET = 0x11, /**< SET type: unordered collection of elements. */
    NUMERIC_STRING [[deprecated]] = 0x12, /**< Numeric String: digits and spaces. Deprecated. */
    PRINTABLE_STRING = 0x13, /**< Printable String: restricted to printable ASCII characters. */
    T61_STRING [[deprecated]] = 0x14, /**< T.61 String: legacy text format. Deprecated. */
    VIDEOTEX_STRING [[deprecated]] = 0x15, /**< Videotex String: legacy graphical text format. Deprecated. */
    IA5_STRING = 0x16, /**< IA5 String: ASCII-compatible text format. */
    UTC_TIME = 0x17, /**< UTC Time: date and time in `YYMMDDHHMM[SS]Z` format. */
    GENERALIZED_TIME = 0x18, /**< Generalized Time: extended date and time format. */
    GRAPHIC_STRING [[deprecated]] = 0x19, /**< Graphic String: graphical symbols. Deprecated. */
    VISIBLE_STRING [[deprecated]] = 0x1A, /**< Visible String: printable ASCII. Deprecated. */
    GENERAL_STRING [[deprecated]] = 0x1B, /**< General String: supports multiple alphabets. Deprecated. */
    UNIVERSAL_STRING = 0x1C, /**< Universal String: UTF-32 encoded text. */
    BMP_STRING = 0x1E, /**< BMP String: UTF-16 encoded text (Basic Multilingual Plane). */
    DATE = 0x1F, /**< Date: year, month, day. */
    TIME_OF_DAY = 0x20, /**< Time of Day: hour, minute, second. */
    DATE_TIME = 0x21, /**< Date-Time: combined date and time. */
    DURATION = 0x22, /**< Duration: time interval in ISO 8601 format. */
};

/**
 * @enum asn1_class
 * @brief Enumerates the classes of ASN.1 tags.
 *
 * ASN.1 tags are divided into four primary classes, which determine the scope
 * and application of the tag. Each class has a specific use case as defined
 * in the X.690 standard.
 */
enum class asn1_class : uint8_t {
    UNIVERSAL = 0x00, /**< Universal class: globally defined and applicable for all contexts. */
    APPLICATION = 0x01, /**< Application class: specific to a particular application. */
    CONTEXT_SPECIFIC = 0x02, /**< Context-specific class: interpreted relative to the context. */
    PRIVATE = 0x03 /**< Private class: used for application-specific purposes. */
};


class asn1_base {
public:
    using tag_t = std::variant<asn1_tag, uintmax_t>;
    using dynamic_data_t = std::vector<uint8_t>;

    asn1_base() = default;

    virtual ~asn1_base() = default;

    asn1_base(std::span<const uint8_t> data) {
        decode(data);
    }

    [[nodiscard]] bool is_constructed() const noexcept {
        return _constructed;
    }

    [[nodiscard]] tag_t get_type() const noexcept {
        return _type;
    }

    [[nodiscard]] asn1_class get_cls() const noexcept {
        return _cls;
    }

    [[nodiscard]] size_t get_length() const noexcept {
        return _length;
    }

    [[nodiscard]] const dynamic_data_t &get_data() const noexcept {
        return _data;
    }

    [[nodiscard]] virtual std::string to_string() const {
        return "ASN.1 Base";
    }


    friend std::vector<uint8_t> serialize(asn1_base *block);

    friend std::unique_ptr<asn1_base> deserialize_v(std::span<const uint8_t> data);

    template<typename T>
    friend std::unique_ptr<T> deserialize(std::span<const uint8_t> data) {
        if (data.empty()) {
            throw std::invalid_argument("Invalid ASN.1 data");
        }
        const auto type = asn1_base::extract_type(data).first;
        if (!std::get_if<asn1_tag>(&type)) {
            return nullptr;
        }
        std::unique_ptr ptr = std::make_unique<T>();
        (void) static_cast<asn1_base *>(ptr.get())->asn1_base::decode(data);
        ptr->decode(data);
        return ptr;
    }

protected:
    virtual void decode(std::span<const uint8_t> data) {
        size_t offset = 0;
        if (data.size_bytes() < 2) {
            throw std::invalid_argument("Invalid ASN.1 data. Length is too short");
        }

        this->_cls = extract_class(data[0]);
        this->_constructed = extract_is_constructed(data[0]);
        const auto type_decoded = extract_type(data);
        data = data.subspan(type_decoded.second);
        const auto length = extract_length(data);
        data = data.subspan(length.second);
        this->_type = type_decoded.first;
        this->_length = length.first;
        const auto it = data.begin();
        if (it + length.first > data.end()) {
            throw std::invalid_argument("Invalid ASN.1 data. Length exceeds buffer size");
        }
        _data = std::vector<uint8_t>(it, it + static_cast<std::ptrdiff_t>(length.first));
    }

    [[nodiscard]] virtual std::vector<uint8_t> encode() {
        dynamic_data_t output = _data;
        output.insert_range(output.begin(), asn1_base::encode_length(_data.size()));
        //TODO: Encode long tags
        output.insert(output.begin(), static_cast<uint8_t>(std::get<asn1_tag>(_type)));
        return output;
    }

    dynamic_data_t _data;
    tag_t _type;
    //value_t _decoded;
private:
    bool _constructed{};

    asn1_class _cls{};
    size_t _length{};

    //TODO: Проверка длинны массива
    [[nodiscard]] static std::pair<tag_t, size_t> extract_type(std::span<const uint8_t> buffer) {
        const uint8_t tag_decoded = buffer[0] & 0x1FU;
        if (tag_decoded != 0x1FU) {
            return std::make_pair(static_cast<asn1_tag>(tag_decoded), 1ULL);
        }
        auto it = std::next(buffer.begin());
        uintmax_t type_decoded{0};
        const auto push_bits = [&](const uint8_t byte) {
            if (it - buffer.begin() - 1 > sizeof(uintmax_t)) {
                throw std::runtime_error("Tag is too long");
            }
            type_decoded = (type_decoded << 7U) | (byte & 0x7FU);
        };
        while (*it & 0x80U) {
            push_bits(*it);
            ++it;
        }
        push_bits(*it);
        return std::make_pair(type_decoded, it - buffer.begin());
    }

    [[nodiscard]] static std::vector<uint8_t> encode_length(size_t length) {
        if (length <= 127) {
            return {static_cast<uint8_t>(length)};
        }
        std::vector<uint8_t> output;
        output.reserve(sizeof(size_t));
        while (length > 0) {
            output.emplace(output.begin(), static_cast<uint8_t>(length & 0xFFU));
            length >>= 8U;
        }
        output.emplace(output.begin(), static_cast<uint8_t>(output.size() | 0x80U));
        return output;
    }

    //TODO: Implement long tag encoding
    //TODO: Проверка длинны массива
    [[nodiscard]] static std::pair<size_t, size_t> extract_length(std::span<const uint8_t> buffer) {
        if (!(buffer[0] & 0x80U)) {
            return std::make_pair(buffer[0], 1ULL);
        }

        size_t length = 0;
        const uint8_t num_octets = buffer[0] & 0x7FU;
        if (num_octets > sizeof(size_t)) {
            throw std::runtime_error("Length is too long");
        }
        for (size_t i = 1; i <= num_octets; i++) {
            length = (length << 8U) | buffer[i];
        }
        return std::make_pair(length, num_octets + 1);
    }

    [[nodiscard]] static inline bool extract_is_constructed(const uint8_t tag) noexcept {
        return (tag & 0x20U);
    }

    [[nodiscard]] static inline asn1_class extract_class(const uint8_t tag) noexcept {
        return static_cast<asn1_class>(tag & 0xC0U);
    }
};

std::unique_ptr<asn1_base> deserialize_v(std::span<const uint8_t> data);

template<typename T>
std::unique_ptr<T> deserialize(std::span<const uint8_t> data);


#endif //ASNCPP_ASN_BASE_H

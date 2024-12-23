//
// Created by kandu on 21.11.2024.
//

#ifndef ASNCPP_ASN_BASE_H
#define ASNCPP_ASN_BASE_H

#include <algorithm>
#include <vector>
#include <span>
#include <variant>
#include <memory>

namespace asncpp::base {
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

    /**
     * @enum asn1_tag
     * @brief Defines various ASN.1 tag types.
     *
     * Each tag type represents a different data type or structure in ASN.1 encoding.
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
     * @typedef tag_t
     * @brief Represents a variant type for storing ASN.1 tag information.
     *
     * The tag can be either:
     * - `std::monostate` (uninitialized),
     * - `asn1_tag` (standard ASN.1 tag type),
     * - `uintmax_t` (for extended tags).
     */
    using tag_t = std::variant<std::monostate, asn1_tag, uintmax_t>;

    /**
     * @typedef dynamic_array_t
     * @brief Represents a dynamic byte array used for ASN.1 encoding and decoding.
     */
    using dynamic_array_t = std::vector<uint8_t>;

    /**
     * @class asn1_basic
     * @brief Abstract base class for representing ASN.1 data types.
     *
     * Provides a foundation for encoding, decoding, and managing ASN.1 data.
     */
    class asn1_basic;

    /**
     * @brief Deserializes a byte stream into an ASN.1 object.
     *
     * @param data The byte stream to deserialize.
     * @return A unique pointer to the deserialized ASN.1 object.
     * @throws std::runtime_error If the data cannot be parsed.
     */
    std::unique_ptr<asn1_basic> deserialize_v(std::span<const uint8_t> data);

    /**
     * @brief Serializes an ASN.1 object into a byte stream.
     *
     * @param block The ASN.1 object to serialize.
     * @return A byte stream representing the serialized ASN.1 object.
     */
    dynamic_array_t serialize(asn1_basic *block);
} // namespace asncpp::base

#endif //ASNCPP_ASN_BASE_H

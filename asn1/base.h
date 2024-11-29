//
// Created by kandu on 21.11.2024.
//

#ifndef ASNCPP_BASE_H
#define ASNCPP_BASE_H

#include <vector>


enum class asn1_tag : uint8_t {
    Reserved = 0x00,
    BOOLEAN = 0x01, //+
    INTEGER = 0x02, //+
    BIT_STRING = 0x03, //+-
    OCTET_STRING = 0x04, //+
    Null = 0x05, //+
    OBJECT_IDENTIFIER = 0x06, //+
    OBJECT_DESCRIPTOR = 0x07, //+
    External = 0x08,
    Real = 0x09,
    ENUMERATED = 0x0A, //+
    EmbeddedPDV = 0x0B,
    UTF8_STRING = 0x0C, //+
    RELATIVE_OID = 0x0D, //+
    Sequence = 0x10,
    Set = 0x11,
    NUMERIC_STRING = 0x12, //+
    PRINTABLE_STRING = 0x13, //+
    T61String = 0x14,
    VideotexString = 0x15,
    IA5String = 0x16,
    UTCTime = 0x17,
    GeneralizedTime = 0x18,
    GraphicString = 0x19,
    VisibleString = 0x1A,
    GeneralString = 0x1B,
    UniversalString = 0x1C,
    BMP_STRING = 0x1E //+
};

enum class asn1_class : uint8_t {
    UNIVERSAL = 0x00,
    APPLICATION = 0x01,
    CONTEXT_SPECIFIC = 0x02,
    PRIVATE = 0x03
};

class asn1_block {
public:
    asn1_block(const uint8_t *buffer) {
        size_t offset = 0;
        this->_cls = extract_class(*buffer);
        this->_constructed = extract_is_constructed(*buffer);
        const auto type = extract_type(buffer);
        offset += type.second;
        const auto length = extract_length(buffer + type.second);
        offset += length.second;
        //FIXME: Добавить обработку для тегов вне перечисления asn1_tag
        this->_type = type.first;
        this->_length = length.first;
        _data = std::vector<uint8_t>(buffer + offset,
                                     buffer + offset + this->_length);
    }

    [[nodiscard]] bool is_constructed() const noexcept {
        return _constructed;
    }

    [[nodiscard]] asn1_tag get_type() const noexcept {
        return _type;
    }

    [[nodiscard]] asn1_class get_cls() const noexcept {
        return _cls;
    }

    [[nodiscard]] size_t get_length() const noexcept {
        return _length;
    }

    [[nodiscard]] const std::vector<uint8_t> &get_data() const noexcept {
        return _data;
    }

    [[nodiscard]] static std::vector<uint8_t> encode_length(size_t length) {
        std::vector<uint8_t> output;
        if (length <= 127) {
            output.push_back(static_cast<uint8_t>(length));
            return output;
        }
        while (length > 0) {
            output.insert(output.begin(), static_cast<uint8_t>(length & 0xFF));
            length >>= 8;
        }
        output.insert(output.begin(), static_cast<uint8_t>(output.size() | 0x80));
        return output;
    }

    static std::pair<size_t, size_t> extract_length(const uint8_t *buffer) {
        if (!(*buffer & 0x80)) {
            return std::make_pair(*buffer, 1LL);
        }

        size_t length = 0;
        const uint8_t num_octets = *buffer & 0x7F;
        buffer++;
        for (size_t i = 0; i < num_octets; i++) {
            length = (length << 8) | *buffer;
            buffer++;
        }
        return std::make_pair(length, num_octets + 1);
    }

private:
    bool _constructed;
    asn1_tag _type;
    asn1_class _cls;
    size_t _length;
    std::vector<uint8_t> _data;

    static std::pair<asn1_tag, size_t> extract_type(const uint8_t *buffer) {
        const uint8_t *ptr_old = buffer;
        const uint8_t tag = *buffer & 0x1F;
        if (tag != 0x1F) {
            return std::make_pair(static_cast<asn1_tag>(tag), 1LL);
        }

        buffer++;
        //FIXME: Добавить обработку длинных тегов
        uint16_t type = 0;
        while (*buffer & 0x80) {
            type = (type << 7) | (*buffer & 0x7F);
            buffer++;
        }
        type = (type << 7) | (*buffer & 0x7F);
        return std::make_pair(static_cast<asn1_tag>(type), buffer - ptr_old + 1);
    }


    static inline bool extract_is_constructed(uint8_t tag) {
        return (tag & 0x20);
    }

    static inline asn1_class extract_class(uint8_t tag) {
        return static_cast<asn1_class>(tag & 0xC0);
    }
};

template<typename T, uintmax_t U>
class asn1_type {
public:
    static constexpr uintmax_t tag = U;
    using value_type = T;

    virtual ~asn1_type() = default;

    virtual void decode(const uint8_t *buffer) = 0;

    [[nodiscard]] virtual std::vector<uint8_t> encode() const = 0;

    [[nodiscard]] virtual std::string to_string() const = 0;


//
//    [[nodiscard]] virtual T get_value() const {
//        return _value;
//    }
//
//    [[nodiscard]] virtual operator T() const {
//        return get_value();
//    }
//
//    size_t children_count() const {
//        return 0;
//    }
//
//    [[nodiscard]] virtual asn1_type *get_child(size_t index) {
//        return nullptr;
//    }
//
//    [[nodiscard]] virtual const asn1_type *get_child(size_t index) const {
//        return nullptr;
//    }

protected:
    T _value{};
    std::vector<uint8_t> _buffer;
};


#endif //ASNCPP_BASE_H

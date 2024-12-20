//
// Created by kandu on 17.12.2024.
//

#ifndef ASN1_BASIC_H
#define ASN1_BASIC_H
#include <string>
#include "common.h"


class asncpp::base::asn1_basic {
public:
    asn1_basic() noexcept = default;

    asn1_basic(asn1_basic &&) = default;

    asn1_basic(const asn1_basic &) = default;

    asn1_basic &operator=(asn1_basic &&) = default;

    asn1_basic &operator=(const asn1_basic &) = default;

    virtual ~asn1_basic() = default;

    asn1_basic(std::span<const uint8_t> data){ // NOLINT(*-explicit-constructor)
        asn1_basic::decode(data);
    }

    constexpr bool is_constructed() const noexcept {
        return _constructed;
    }

    constexpr asn1_class get_cls() const noexcept {
        return _cls;
    }

    constexpr size_t get_length() const noexcept {
        return _length;
    }

    constexpr const dynamic_array_t &get_data() const noexcept {
        return _data;
    }

    static std::pair<tag_t, size_t> extract_type(std::span<const uint8_t> buffer);

    friend dynamic_array_t serialize(asn1_basic *block);

    friend std::unique_ptr<asn1_basic> deserialize_v(std::span<const uint8_t> data);


protected:
    constexpr virtual uintmax_t get_tag() const noexcept = 0;

    virtual void decode(std::span<const uint8_t> data);

    virtual dynamic_array_t encode();

    [[nodiscard, maybe_unused]] virtual std::string to_string() const {
        return "ASN.1 basic";
    }

    dynamic_array_t _data; // NOLINT(*-non-private-member-variables-in-classes)
private:
    bool _constructed{};
    asn1_class _cls{};
    size_t _length{};
    tag_t _type;

    [[nodiscard]] dynamic_array_t encode_type() const;

    [[nodiscard]] static dynamic_array_t encode_length(size_t length);

    [[nodiscard]] static std::pair<size_t, size_t> extract_length(std::span<const uint8_t> buffer);

    [[nodiscard]] constexpr static bool extract_is_constructed(uint8_t tag) noexcept;

    [[nodiscard]] constexpr static asn1_class extract_class(uint8_t tag) noexcept;
};

#endif //ASN1_BASIC_H

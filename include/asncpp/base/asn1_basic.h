//
// Created by kandu on 17.12.2024.
//

#ifndef ASN1_BASIC_H
#define ASN1_BASIC_H
#include "common.h"

class asncpp::base::asn1_basic {
public:
    asn1_basic() = default;

    asn1_basic(asn1_basic &&) = default;

    asn1_basic(const asn1_basic &) = default;

    asn1_basic &operator=(asn1_basic &&) = default;

    asn1_basic &operator=(const asn1_basic &) = default;

    virtual ~asn1_basic() = default;

    asn1_basic(std::span<const uint8_t> data) { // NOLINT(*-explicit-constructor)
        asn1_basic::decode(data);
    }

    [[nodiscard]] auto is_constructed() const noexcept -> bool {
        return _constructed;
    }

    [[nodiscard]] auto get_type() const noexcept -> tag_t {
        return _type;
    }

    [[nodiscard]] auto get_cls() const noexcept -> asn1_class {
        return _cls;
    }

    [[nodiscard]] auto get_length() const noexcept -> size_t {
        return _length;
    }

    [[nodiscard]] auto get_data() const noexcept -> const dynamic_array_t & {
        return _data;
    }

    [[nodiscard]] virtual auto to_string() const -> std::string {
        return "ASN.1 Base";
    }

    friend auto serialize(asn1_basic *block) -> std::vector<uint8_t>;

    friend auto deserialize_v(std::span<const uint8_t> data) -> std::unique_ptr<asn1_basic>;

protected:
    virtual void decode(std::span<const uint8_t> data);

    virtual std::vector<uint8_t> encode();

    dynamic_array_t _data;

    tag_t _type;
private:
    bool _constructed{};
    asn1_class _cls{};
    size_t _length{};


};

#endif //ASN1_BASIC_H

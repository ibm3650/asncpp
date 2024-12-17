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

    [[nodiscard]] virtual auto encode() -> std::vector<uint8_t>;

    dynamic_array_t _data;

    tag_t _type;
private:
    bool _constructed{};

    asn1_class _cls{};
    size_t _length{};

    //TODO: Проверка длинны массива
    [[nodiscard]] static auto extract_type(std::span<const uint8_t> buffer) -> std::pair<tag_t, size_t> {
        const uint8_t tag_decoded = buffer[0] & 0x1FU;
        if (tag_decoded < 0x1FU) {
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
        ++it;
        if (type_decoded <= 0x22U) {
            return std::make_pair(static_cast<asn1_tag>(type_decoded), it - buffer.begin());
        }
        return std::make_pair(type_decoded, it - buffer.begin());
    }

    [[nodiscard]] static auto encode_length(size_t length) -> std::vector<uint8_t> {
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

    [[nodiscard]] auto encode_type() -> std::vector<uint8_t> {
        std::vector<uint8_t> output;
        uintmax_t raw_type{};

        if (std::get_if<asn1_tag>(&_type)) {
            raw_type = static_cast<uintmax_t>(std::get<asn1_tag>(_type));
        } else {
            raw_type = std::get<uintmax_t>(_type);
        }
        if (raw_type < 0x1FU) {
            return {
                static_cast<unsigned char>(static_cast<uint8_t>(get_cls()) << 6U |
                                           static_cast<uint8_t>(is_constructed()) << 5U |
                                           static_cast<uint8_t>(raw_type))
            };
            //output.push_back(static_cast<uint8_t>(raw_type))};
            //return output;
        }


        std::vector<uint8_t> result;
        result.push_back(static_cast<unsigned char>(static_cast<uint8_t>(get_cls()) << 6U |
                                                    static_cast<uint8_t>(is_constructed()) << 5U |
                                                    static_cast<uint8_t>(0x1FU)));
        do {
            result.push_back(static_cast<uint8_t>(raw_type & 0x7FU));
            raw_type >>= 7U;
        } while (raw_type > 0);

        std::transform(std::next(result.begin()),
                       std::prev(result.end()),
                       std::next(result.begin()),
                       [](const uint8_t byte) {
                           return byte | 0x80U;
                       });
        return result;
    }

    //TODO: Implement long tag encoding
    //TODO: Проверка длинны массива
    [[nodiscard]] static auto extract_length(std::span<const uint8_t> buffer) -> std::pair<size_t, size_t> {
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

    [[nodiscard]] static auto extract_is_constructed(const uint8_t tag) noexcept -> bool {
        return (tag & 0x20U) >> 5U;
    }

    [[nodiscard]] static auto extract_class(const uint8_t tag) noexcept -> asn1_class {
        return static_cast<asn1_class>((tag & 0xC0U) >> 6U);
    }
};

#endif //ASN1_BASIC_H

//
// Created by kandu on 17.12.2024.
//

#include "asncpp/base/asn1_basic.h"

void asncpp::base::asn1_basic::decode(std::span<const uint8_t> data) {
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

asncpp::base::dynamic_array_t asncpp::base::asn1_basic::encode() {
    dynamic_array_t output = _data;
    output.insert_range(output.begin(), asn1_basic::encode_length(_data.size()));
    output.insert_range(output.begin(), encode_type());
    return output;
}

//TODO: Проверка длинны массива
std::pair<asncpp::base::tag_t, size_t> asncpp::base::extract_type(std::span<const uint8_t> buffer) {
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

asncpp::base::dynamic_array_t asncpp::base::encode_length(size_t length) {
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

asncpp::base::dynamic_array_t asncpp::base::encode_type() {
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
std::pair<size_t, size_t> asncpp::base::extract_length(std::span<const uint8_t> buffer) {
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

bool asncpp::base::extract_is_constructed(const uint8_t tag) noexcept {
    return (tag & 0x20U) >> 5U;
}

asncpp::base::asn1_class asncpp::base::extract_class(const uint8_t tag) noexcept {
    return static_cast<asncpp::base::asn1_class>((tag & 0xC0U) >> 6U);
}

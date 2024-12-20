//
// Created by kandu on 17.12.2024.
//

#include "asncpp/base/asn1_basic.h"
#include <stdexcept>
//TODO: USE ALGORITHM
//TODO: USE ATTRIBUTE
void asncpp::base::asn1_basic::decode(std::span<const uint8_t> data) {
    if (data.size_bytes() < 2) {
        throw std::invalid_argument("Invalid ASN.1 data. Length is too short");
    }

    _cls = extract_class(data[0]);

    _constructed = extract_is_constructed(data[0]);

    data = data.subspan(extract_type(data).second);

    const auto length{extract_length(data)};
    data = data.subspan(length.second);
    _length = length.first;

    if (std::distance(data.cbegin() + static_cast<std::ptrdiff_t>(length.first), data.end()) < 0) {
        throw std::invalid_argument("Invalid ASN.1 data. Length exceeds buffer size");
    }
    _data.assign(data.cbegin(), data.cbegin() + static_cast<std::ptrdiff_t>(length.first));
}

//TODO: USE ALGORITHM
//TODO: USE ATTRIBUTE
asncpp::base::dynamic_array_t asncpp::base::asn1_basic::encode() {
    dynamic_array_t output{_data};
    output.insert_range(output.begin(), encode_length(_data.size()));
    output.insert_range(output.begin(), encode_type());
    return output;
}

//TODO: USE ALGORITHM
//TODO: USE ATTRIBUTE
std::pair<asncpp::base::tag_t, size_t> asncpp::base::asn1_basic::extract_type(std::span<const uint8_t> buffer) {
    const uint8_t tag_decoded = buffer[0] & 0x1FU;

    if (tag_decoded < 0x1FU) {
        //[[likely]]
        return std::make_pair(static_cast<asn1_tag>(tag_decoded), 1ULL);
    }

    if (buffer.size_bytes() < 2) {
        throw std::runtime_error("Data is too short for the long encoding");
    }

    auto type_begin = std::next(buffer.cbegin());
    uintmax_t type_decoded{0};
    for (; *type_begin & 0x80U && type_begin != buffer.cend(); ++type_begin) {
        type_decoded = (type_decoded << 7U) | (*type_begin & 0x7FU);
    }
    if (type_begin == buffer.cend()) {
        throw std::runtime_error("Data is too short for the long encoding");
    }
    type_decoded = (type_decoded << 7U) | (*type_begin++ & 0x7FU);
    if (type_decoded <= 0x22U) {
        return std::make_pair(static_cast<asn1_tag>(type_decoded), type_begin - buffer.cbegin());
    }
    return std::make_pair(type_decoded, type_begin - buffer.cbegin());
}

//TODO: USE ALGORITHM
//TODO: USE ATTRIBUTE
asncpp::base::dynamic_array_t asncpp::base::asn1_basic::encode_length(size_t length) {
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

//TODO: USE ALGORITHM
//TODO: USE ATTRIBUTE
asncpp::base::dynamic_array_t asncpp::base::asn1_basic::encode_type() const {
    uintmax_t raw_type{get_tag()};
    const uint8_t base = static_cast<uint8_t>(get_cls()) << 6U |
                         static_cast<uint8_t>(is_constructed()) << 5U;

    if (raw_type < 0x1FU) {
        return {static_cast<unsigned char>(base | static_cast<uint8_t>(raw_type))};
    }

    std::vector<uint8_t> result{static_cast<uint8_t>(base | static_cast<uint8_t>(0x1FU))};
    do {
        result.push_back(static_cast<uint8_t>(raw_type & 0x7FU));
        raw_type >>= 7U;
    } while (raw_type > 0);

    std::transform(std::next(result.cbegin()),
                   std::prev(result.cend()),
                   std::next(result.begin()),
                   [](const uint8_t byte) { return byte | 0x80U; });
    return result;
}

//TODO: USE ALGORITHM
//TODO: USE ATTRIBUTE
std::pair<size_t, size_t> asncpp::base::asn1_basic::extract_length(std::span<const uint8_t> buffer) {
    if (!(buffer[0] & 0x80U)) {
        return std::make_pair(buffer[0], 1ULL);
    }
    size_t length = 0;
    const uint8_t num_octets = buffer[0] & 0x7FU;
    if (num_octets > sizeof(size_t)) {
        throw std::runtime_error("Length is too long");
    }
    if (num_octets > buffer.size_bytes() - 1) {
        throw std::runtime_error("Data is too short for the long encoding");
    }
    for (size_t i = 1; i <= num_octets; i++) {
        length = (length << 8U) | buffer[i];
    }
    return std::make_pair(length, num_octets + 1);
}

bool asncpp::base::asn1_basic::extract_is_constructed(const uint8_t tag) noexcept {
    return (tag & 0x20U) >> 5U;
}

asncpp::base::asn1_class asncpp::base::asn1_basic::extract_class(const uint8_t tag) noexcept {
    return static_cast<asn1_class>((tag & 0xC0U) >> 6U);
}

//
// Created by kandu on 17.12.2024.
//

#include "asncpp/base/asn1_basic.h"

#include <bit>
#include <ranges>
#include <stdexcept>

namespace views = std::ranges::views;

void asncpp::base::asn1_basic::decode(std::span<const uint8_t> data) {
    if (data.size_bytes() < 2) {
        throw std::invalid_argument("Invalid ASN.1 data. Length is too short");
    }

    _cls = extract_class(data[0]);

    _constructed = extract_is_constructed(data[0]);
    const auto type = extract_type(data);
    _type = type.first;
    data = data.subspan(type.second);

    const auto length{extract_length(data)};
    data = data.subspan(length.second);
    _length = length.first;

    if (std::distance(data.cbegin() + static_cast<std::ptrdiff_t>(length.first), data.end()) < 0) {
        throw std::invalid_argument("Invalid ASN.1 data. Length exceeds buffer size");
    }
    _data.assign(data.cbegin(), data.cbegin() + static_cast<std::ptrdiff_t>(length.first));
}


asncpp::base::dynamic_array_t asncpp::base::asn1_basic::encode() {
    dynamic_array_t output{encode_type()};
    output.append_range(encode_length(_data.size()));
    output.append_range(_data);
    return output;
}


std::pair<asncpp::base::tag_t, size_t> asncpp::base::asn1_basic::extract_type(std::span<const uint8_t> buffer) {
    if (buffer.empty()) {
        throw std::runtime_error("Data is too short for the long encoding");
    }
    size_t count{0};
    const uint8_t tag_decoded = buffer[count++] & 0x1FU;
    if (tag_decoded < 0x1FU) {
        return std::make_pair(static_cast<asn1_tag>(tag_decoded), 1ULL);
    }

    if (buffer.size_bytes() < 2) {
        throw std::runtime_error("Data is too short for the long encoding");
    }

    uintmax_t type_decoded{0};
    const auto type_view{
        buffer | views::drop(count) | views::take_while(
            [](uint8_t byte) { return byte & 0x80U; })
    };


    for (const uint8_t byte: type_view) {
        type_decoded = (type_decoded << 7U) | (byte & 0x7FU);
        count++;
    }

    if (count == 0) {
        throw std::runtime_error("Data is too short for the long encoding");
    }
    if (count == buffer.size_bytes()) {
        throw std::runtime_error("Data is too short for the long encoding");
    }
    type_decoded = (type_decoded << 7U) | (buffer[count++] & 0x7FU);

    if (type_decoded <= 0x22U) {
        return std::make_pair(static_cast<asn1_tag>(type_decoded), count);
    }
    return std::make_pair(type_decoded, count);
}


asncpp::base::dynamic_array_t asncpp::base::asn1_basic::encode_length(size_t length) {
    if (length <= 127) {
        return {static_cast<uint8_t>(length)};
    }
    std::vector<uint8_t> output;
    while (length > 0) {
        output.emplace(output.begin(), static_cast<uint8_t>(length & 0xFFU));
        length >>= 8U;
    }
    output.emplace(output.begin(), static_cast<uint8_t>(output.size() | 0x80U));
    return output;
}


asncpp::base::dynamic_array_t asncpp::base::asn1_basic::encode_type() const {
    uintmax_t raw_type{0};
    std::visit([&](auto &&arg) {
                   using T = std::decay_t<decltype(arg)>;
                   if constexpr (std::is_same_v<T, std::monostate>) {
                       raw_type = get_tag();
                   } else if constexpr (std::is_same_v<T, asn1_tag>) {
                       raw_type = static_cast<uintmax_t>(arg);
                   } else if constexpr (std::is_same_v<T, uintmax_t>) {
                       raw_type = arg;
                   }
               },
               _type);
    const uint8_t base = (static_cast<uint8_t>(get_cls()) << 6U) |
                         (static_cast<uint8_t>(is_constructed()) << 5U);

    if (raw_type < 0x1FU) {
        return {static_cast<unsigned char>(base | static_cast<uint8_t>(raw_type))};
    }

    std::vector<uint8_t> result;
    do {
        result.insert(result.begin(), static_cast<uint8_t>(raw_type & 0x7FU));
        raw_type >>= 7U;
    } while (raw_type > 0);
    result.insert(result.begin(), static_cast<uint8_t>(base | static_cast<uint8_t>(0x1FU)));

    // const auto out = result | views::drop(1) |
    //     views::take(result.size() - 2) |
    //         views::transform([](uint8_t byte) { return byte | 0x80U; });
    // return std::ranges::to<std::vector<uint8_t>>(out);
    std::transform(std::next(result.cbegin()),
                   std::prev(result.cend()),
                   std::next(result.begin()),
                   [](const uint8_t byte) { return byte | 0x80U; });
    return result;
}


std::pair<size_t, size_t> asncpp::base::asn1_basic::extract_length(std::span<const uint8_t> buffer) {
    if (buffer.empty()) {
        throw std::runtime_error("Buffer is empty");
    }

    // Если длина закодирована в одном байте
    if (!(buffer[0] & 0x80U)) {
        return std::make_pair(buffer[0], 1ULL);
    }

    // Определяем количество байт, использованных для длины
    size_t length{0};
    const uint8_t num_octets = buffer[0] & 0x7FU;

    // Проверка на слишком большую длину
    if (num_octets > sizeof(size_t)) {
        throw std::runtime_error("Length is too long for this platform");
    }

    // Проверка на доступность байт для длины
    if (num_octets > buffer.size_bytes() - 1) {
        throw std::runtime_error("Data is too short for the specified length encoding");
    }

    // Проверяем, что длина не равна нулю
    if (num_octets == 0) {
        throw std::runtime_error("Length encoding specifies zero octets");
    }

    // Собираем длину из октетов
    for (size_t i = 1; i <= num_octets; ++i) {
        length = (length << 8U) | buffer[i];
    }

    // Проверяем, что длина больше нуля (дополнительная проверка)
    if (length == 0) {
        throw std::runtime_error("Length cannot be zero");
    }

    return std::make_pair(length, num_octets + 1);
}

constexpr bool asncpp::base::asn1_basic::extract_is_constructed(uint8_t tag) noexcept {
    return (tag & 0x20U) >> 5U;
}

constexpr asncpp::base::asn1_class asncpp::base::asn1_basic::extract_class(uint8_t tag) noexcept {
    return static_cast<asn1_class>((tag & 0xC0U) >> 6U);
}

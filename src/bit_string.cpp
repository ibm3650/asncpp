//
// Created by kandu on 27.12.2024.
//
#include "asncpp/bit_string.h"
#include <ranges>
#include <numeric>

asncpp::types::bit_string_t::bit_string_t(const std::vector<bool> &data) : _bit_length{data.size()} {
    for (const auto &chunk: data | std::views::chunk(8)) {
        uint8_t byte{0};
        for (const bool bit: chunk) {
            byte <<= 1U;
            byte |= static_cast<uint8_t>(bit);
        }
        if (chunk.size() < 8) {
            byte <<= 8 - chunk.size();
        }
        _value.push_back(byte);
    }
}

asncpp::types::bit_string_t::bit_string_t(std::string_view data) : _bit_length{data.size()} {
    for (const auto &chunk: data | std::views::chunk(8)) {
        uint8_t byte{0};
        for (const char bit: chunk) {
            if (bit != '0' && bit != '1') {
                throw std::invalid_argument("Invalid character in bit string");
            }
            byte <<= 1U;
            byte |= static_cast<uint8_t>(bit == '1');
        }
        if (chunk.size() < 8) {
            byte <<= 8 - chunk.size();
        }
        _value.push_back(byte);
    }
}

asncpp::types::bit_string_t::bit_string_t(uint64_t data, size_t bit_length) : _bit_length{bit_length} {
    if (bit_length > 64) {
        throw std::invalid_argument("Bit length exceeds 64 bits");
    }
    while (bit_length > 0) {
        size_t chunk = std::min<size_t>(bit_length, 8);
        _value.insert(_value.begin(), static_cast<uint8_t>(data & 0xFFU));
        data >>= chunk;
        bit_length -= chunk;
    }
}

void asncpp::types::bit_string_t::decode(std::span<const uint8_t> data) {
    if (_data.empty()) {
        if (!_children.empty()) {
            return;
        }
        throw std::invalid_argument("Invalid BIT STRING: empty data");
    }

    uint8_t unused_bits = _data[0];
    if (unused_bits > 7) {
        throw std::invalid_argument("Invalid BIT STRING: unused bits exceed 7");
    }
    _bit_length = (_data.size() - 1) * 8 - unused_bits;
    _value.assign(_data.begin() + 1, _data.end());
}


asncpp::base::dynamic_array_t asncpp::types::bit_string_t::encode() {
    if (!_children.empty()) {
        return _data;
    }
    _data.clear();
    _data.push_back(static_cast<uint8_t>((8 - (_bit_length % 8)) % 8));
    _data.insert(_data.end(), _value.begin(), _value.end());
    return _data;
}

asncpp::types::bit_string_t::bit_string_t(const std::vector<uint8_t> &data, size_t bit_length): _value{data}, _bit_length{bit_length} {
    if (bit_length > data.size() * 8) {
        throw std::invalid_argument("Bit length exceeds data size");
    }
}

std::string asncpp::types::bit_string_t::to_string() const {
    using namespace std::string_literals;
    return "BIT STRING: "s;
}

const std::vector<uint8_t> &asncpp::types::bit_string_t::value() const {
    return _value;
}

size_t asncpp::types::bit_string_t::bit_length() const {
    return _bit_length;
}

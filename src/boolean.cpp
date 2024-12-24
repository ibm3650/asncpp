//
// Created by kandu on 24.12.2024.
//
#include "asncpp/boolean.h"

#include <stdexcept>

void asncpp::types::boolean_t::decode(std::span<const uint8_t> /*data*/) {
    if (_data.size() != 1) {
        throw std::runtime_error("Invalid length for BOOLEAN value");
    }
    _value = _data[0] != 0x00;
}

asncpp::base::dynamic_array_t asncpp::types::boolean_t::encode() {
    _data.push_back(_value ? 0xFF : 0x00);
    return _data;
}

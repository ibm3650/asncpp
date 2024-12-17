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

auto asncpp::base::asn1_basic::encode() -> asncpp::base::dynamic_array_t {
    dynamic_array_t output = _data;
    output.insert_range(output.begin(), asn1_basic::encode_length(_data.size()));
    output.insert_range(output.begin(), encode_type());
    return output;
}

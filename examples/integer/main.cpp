//
// Created by kandu on 13.03.2025.
//

#include <iostream>
#include <asncpp/integer.h>


int main() {
    std::vector<uint8_t> encoded{0x02, 0x03, 0x01, 0xE2, 0x40};
    auto deserialized{ asncpp::base::deserialize_v(encoded)};
    const auto* ptr{dynamic_cast<asncpp::types::integer_t*>(deserialized.get())};
    auto tmp = 10 + *ptr;
    std::cout << tmp << std::endl;
    return 0;
}

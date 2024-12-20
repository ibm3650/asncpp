//
// Created by kandu on 22.11.2024.
//

#ifndef ASNCPP_INTEGER_H
#define ASNCPP_INTEGER_H
#include <cstdint>
#include <string>
#include "asn1_basic.h"
#include "common.h"

template<std::integral T , asncpp::base::asn1_tag type>
class integer_basic : public asncpp::base::asn1_basic {
public:
    using value_type = T;
    using const_reference = const T &;

    integer_basic() noexcept = default;

    integer_basic(integer_basic &&) noexcept = default;

    integer_basic(const integer_basic &) noexcept = default;

    integer_basic &operator=(integer_basic &&) noexcept = default;

    integer_basic &operator=(const integer_basic &) noexcept = default;

    ~integer_basic() noexcept final = default;

    explicit integer_basic(value_type val) noexcept: _decoded{val} {
    }

    asncpp::base::dynamic_array_t encode() final;

    void decode(std::span<const uint8_t> data) final;

    constexpr uintmax_t get_tag() const noexcept final {
        return static_cast<uintmax_t>(type);
    }

    explicit operator value_type() const noexcept {
        return _decoded;
    }


    const_reference get_value() const noexcept {
        return _decoded;
    }

    std::string to_string() const final {
        return std::to_string(_decoded);
    }

private:
    value_type _decoded{};
};


#endif //ASNCPP_INTEGER_H

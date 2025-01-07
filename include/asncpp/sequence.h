//
// Created by kandu on 07.01.2025.
//

#ifndef SEQUENCE_H
#define SEQUENCE_H
#include "base/asn1_basic.h"


class sequence_t : public asncpp::base::asn1_basic {
public:
    sequence_t() noexcept = default;

    explicit sequence_t(asncpp::base::asn1_basic &&base) : asncpp::base::asn1_basic(std::move(base)) {
    }




    constexpr uintmax_t get_tag() const noexcept override {
        return static_cast<uintmax_t>(asncpp::base::asn1_tag::SEQUENCE);
    }

protected:
    void decode(std::span<const uint8_t> data) override {
    }

    asncpp::base::dynamic_array_t encode() override {
        return _data;
    }

    // asncpp::base::dynamic_array_t encode() override {
    //     std::ranges::upper_bound(_children, [](const auto &lhs, const auto &rhs) {
    //         return lhs->get_tag() < rhs->get_tag();
    //     });
    //     std::ranges::sort(_children, [](const auto &lhs, const auto &rhs) {
    //         return lhs->get_tag() < rhs->get_tag();
    //     });
    //     return {};
    // }
};
#endif //SEQUENCE_H

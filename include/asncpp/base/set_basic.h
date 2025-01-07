//
// Created by kandu on 07.01.2025.
//

#ifndef SET_BASIC_H
#define SET_BASIC_H
#include "asn1_basic.h"

template<bool any_type_constraints = false>
class set_basic_t : public asncpp::base::asn1_basic {
public:
    set_basic_t() noexcept = default;

    explicit set_basic_t(asncpp::base::asn1_basic &&base) : asncpp::base::asn1_basic(std::move(base)) {
    }

    void append_child(std::unique_ptr<asn1_basic> child) override {
        auto it = _children.begin();
        if (_children.empty()) {
            _children.insert(it, std::move(child));
            return;
        }


        if constexpr (any_type_constraints) {
            if (child->get_tag() != _children[0]->get_tag()) {
                throw std::invalid_argument("Invalid child type");
            }

            //_children.emplace_back(std::move(child));
        } else {
            it = std::ranges::upper_bound(_children, child, [](const auto &lhs, const auto &rhs) {
                return lhs->get_tag() < rhs->get_tag();
            });
        }

        _children.insert(it, std::move(child));

        // _children.emplace_back(std::move(child));
    }


    constexpr uintmax_t get_tag() const noexcept override {
        return static_cast<uintmax_t>(asncpp::base::asn1_tag::SET);
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
#endif //SET_BASIC_H

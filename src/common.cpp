//
// Created by kandu on 17.12.2024.
//
#include "asncpp/base/common.h"

#include <iostream>

#include "asncpp/types.h"

std::unique_ptr<asncpp::base::asn1_basic> asncpp::base::deserialize_v(std::span<const uint8_t> data) {
    asn1_basic base(data);
    // const tag_t type = asn1_basic::extract_type(data).first;
    if (!std::holds_alternative<asn1_tag>(base._type)) {
        throw std::runtime_error("Tag is not an ASN.1 tag");
        // std::cerr << "Tag is not an ASN.1 tag. Current type state: ";
        // if (std::holds_alternative<std::monostate>(type)) {
        //     std::cerr << "std::monostate";
        // } else if (std::holds_alternative<uintmax_t>(type)) {
        //     std::cerr << "uintmax_t: " << std::get<uintmax_t>(type);
        // }
        // std::cerr << '\n';
        // return nullptr;
    }
    if (base.is_constructed()) {
        while (!base._data.empty()) {
            auto child = deserialize_v(base._data);
            if (!child) {
                throw std::runtime_error("Failed to deserialize child object");
            }
            base.truncate_data(child->_raw_length);
            base._children.emplace_back(std::move(child));
        }
    }
    auto create_object = [base = std::move(base)](const asn1_tag tag) mutable -> std::unique_ptr<asn1_basic> {
        using enum asn1_tag;
        switch (tag) {
            // case BOOLEAN: return std::make_unique<types::boolean_t>{std::move(base)};
            // case INTEGER: return std::make_unique<types::integer_t>{std::move(base)};
            case BIT_STRING: return std::make_unique<bit_string_t>(std::move(base));
            case OBJECT_IDENTIFIER: return std::make_unique<object_identifier_t>();
            case ENUMERATED: return std::make_unique<enumerated_t>();
            case RELATIVE_OID: return std::make_unique<relative_oid_t>();
            case NUMERIC_STRING: return std::make_unique<numeric_string_t>();
            case PRINTABLE_STRING: return std::make_unique<printable_string_t>();
            case IA5_STRING: return std::make_unique<ia5_string_t>();
            case VISIBLE_STRING: return std::make_unique<visible_string_t>();
            case UNIVERSAL_STRING: return std::make_unique<universal_string_t>();
            case BMP_STRING: return std::make_unique<bmp_string_t>();
            case DATE: return std::make_unique<date_t>();
            default: return nullptr;
        }
    };
    auto ptr = create_object(std::get<asn1_tag>(base._type));
    if (!ptr) {
        throw std::runtime_error("Unsupported ASN.1 tag type");
    }

    //ptr->asn1_basic::decode(data);
    ptr->decode(data);
    return ptr;
}


std::vector<uint8_t> asncpp::base::serialize(asn1_basic *block) {
    for (size_t i = 0; i < block->children_count(); ++i) {
       // if (block->is_constructed())
            block->_data.append_range(serialize(block->_children[i].get()));

       // block->_data.append_range(serialize(block->_children[i].get()));
    }
    (void) block->encode();
    return block->asn1_basic::encode();
}

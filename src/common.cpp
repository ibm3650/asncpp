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
            case BOOLEAN: return std::make_unique<types::boolean_t>(std::move(base));
            case INTEGER: return std::make_unique<types::integer_t>(std::move(base));
            case BIT_STRING: return std::make_unique<types::bit_string_t>(std::move(base));
            case OCTET_STRING: return std::make_unique<octet_string_t>(std::move(base));
            case OBJECT_IDENTIFIER: return std::make_unique<object_identifier_t>(std::move(base));
            case OBJECT_DESCRIPTOR: return std::make_unique<object_descriptor_t>(std::move(base));
            case ENUMERATED: return std::make_unique<enumerated_t>(std::move(base));
            case RELATIVE_OID: return std::make_unique<relative_oid_t>(std::move(base));
            case NUMERIC_STRING: return std::make_unique<numeric_string_t>(std::move(base));
            case PRINTABLE_STRING: return std::make_unique<printable_string_t>(std::move(base));
            case IA5_STRING: return std::make_unique<ia5_string_t>(std::move(base));
            case VISIBLE_STRING: return std::make_unique<visible_string_t>(std::move(base));
            case UNIVERSAL_STRING: return std::make_unique<universal_string_t>(std::move(base));
            case BMP_STRING: return std::make_unique<bmp_string_t>(std::move(base));
            case UTF8_STRING: return std::make_unique<utf8_string_t>(std::move(base));
            case DATE: return std::make_unique<date_t>(std::move(base));
            case DATE_TIME: return std::make_unique<date_time_t>(std::move(base));
            case TIME_OF_DAY: return std::make_unique<time_of_day_t>(std::move(base));
            case REAL: return std::make_unique<real_t>(std::move(base));
            case UTC_TIME: return std::make_unique<utc_time_t>(std::move(base));
            case GENERALIZED_TIME: return std::make_unique<generalized_time_t>(std::move(base));
            case DURATION: return std::make_unique<duration_t>(std::move(base));
            case Null: return std::make_unique<null_t>(std::move(base));
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
            block->_data.append_range(serialize(block->_children[i].get()));
    }
    (void) block->encode();
    return block->asn1_basic::encode();
}

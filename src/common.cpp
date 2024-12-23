//
// Created by kandu on 17.12.2024.
//
#include "asncpp/base/common.h"

#include <iostream>

#include "asncpp/types.h"

std::unique_ptr<asncpp::base::asn1_basic> asncpp::base::deserialize_v(std::span<const uint8_t> data) {
    const tag_t type = asn1_basic::extract_type(data).first;
    if (!std::holds_alternative<asn1_tag>(type)) {
        std::cerr << "Tag is not an ASN.1 tag. Current type state: ";
        if (std::holds_alternative<std::monostate>(type)) {
            std::cerr << "std::monostate";
        } else if (std::holds_alternative<uintmax_t>(type)) {
            std::cerr << "uintmax_t: " << std::get<uintmax_t>(type);
        }
        std::cerr << std::endl;
        return nullptr;
    }
    auto create_object = [](const asn1_tag tag) -> std::unique_ptr<asn1_basic> {
        using enum asn1_tag;
        switch (tag) {
            case INTEGER: return std::make_unique<asncpp::types::integer_t>();
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
    auto ptr = create_object(std::get<asn1_tag>(type));
    if (!ptr) {
        throw std::runtime_error("Unsupported ASN.1 tag type");
    }

    ptr->asn1_basic::decode(data);
    ptr->decode(data);
    return ptr;
}


std::vector<uint8_t> asncpp::base::serialize(asn1_basic *block) {
    (void) block->encode();
    return block->asn1_basic::encode();
}

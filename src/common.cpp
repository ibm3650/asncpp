//
// Created by kandu on 17.12.2024.
//
#include "asncpp/base/common.h"
#include "asncpp/types.h"

std::unique_ptr<asncpp::base::asn1_basic> asncpp::base::deserialize_v(std::span<const uint8_t> data)  {
    const tag_t type = asn1_basic::extract_type(data).first;
    if (!std::holds_alternative<asn1_tag>(type)) {
        return nullptr;
    }

    std::unique_ptr<asn1_basic> ptr;
    switch (std::get<asn1_tag>(type)) {
        using enum asn1_tag;
        case INTEGER:
            ptr = std::make_unique<integer_t>();
            break;
        case OBJECT_IDENTIFIER:
            ptr = std::make_unique<object_identifier_t>();
            break;
        case ENUMERATED:
            ptr = std::make_unique<enumerated_t>();
            break;
        case RELATIVE_OID:
            ptr = std::make_unique<relative_oid_t>();
            break;
        case NUMERIC_STRING:
            ptr = std::make_unique<numeric_string_t>();
            break;
        case PRINTABLE_STRING:
            ptr = std::make_unique<printable_string_t>();
            break;
        case IA5_STRING:
            ptr = std::make_unique<ia5_string_t>();
            break;
        case VISIBLE_STRING:
            ptr = std::make_unique<visible_string_t>();
            break;
        case UNIVERSAL_STRING:
            ptr = std::make_unique<universal_string_t>();
            break;
        case BMP_STRING:
            ptr = std::make_unique<bmp_string_t>();
            break;
        case DATE:
            ptr = std::make_unique<date_t>();
            break;
        default:
            return nullptr;
    }
    ptr->asn1_basic::decode(data);
    ptr->decode(data);
    return ptr;
}


std::vector<uint8_t> asncpp::base::serialize(asn1_basic *block) {
    (void) block->encode();
    return block->asn1_basic::encode();
}

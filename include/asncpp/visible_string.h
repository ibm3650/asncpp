//
// Created by kandu on 12.12.2024.
//

#ifndef VISIBLE_STRING_H
#define VISIBLE_STRING_H
#include "base/string_base.h"
#include "base/asn_base.h"

using visible_string_t [[deprecated]] = string_base_t<char, asn1_tag::VISIBLE_STRING, [](char c) { return std::isprint(static_cast<uint8_t>(c)); }>;

#endif //VISIBLE_STRING_H

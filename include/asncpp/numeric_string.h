//
// Created by kandu on 29.11.2024.
//

#ifndef ASNCPP_NUMERIC_STRING_H
#define ASNCPP_NUMERIC_STRING_H
#include "base/string_base.h"
#include "base/asn_base.h"

using numeric_string_t [[deprecated]] = string_base_t<char, asn1_tag::NUMERIC_STRING,
                                                      [](char c) { return std::isdigit(static_cast<uint8_t>(c)) || c == ' '; }>;

#endif //ASNCPP_NUMERIC_STRING_H

//
// Created by kandu on 02.12.2024.
//

#ifndef ASNCPP_IA5_STRING_H
#define ASNCPP_IA5_STRING_H
#include "base/asn_base.h"
#include "base/string_base.h"
using ia5_string_t = string_base_t<char, asn1_tag::IA5_STRING, [](char c){return static_cast<uint8_t>(c) <= 0x7F;}>;
#endif //ASNCPP_IA5_STRING_H

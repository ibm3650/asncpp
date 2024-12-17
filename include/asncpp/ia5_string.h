//
// Created by kandu on 02.12.2024.
//

#ifndef ASNCPP_IA5_STRING_H
#define ASNCPP_IA5_STRING_H
#include "base/common.h"
#include "base/string_base.h"
//TODO: Так как могут встречаться нуль-терминальные символы, конструктор должен быть специализированным, с явным указанием длинны
using ia5_string_t = string_base_t<char, asncpp::base::asn1_tag::IA5_STRING, [](char c){return static_cast<uint8_t>(c) <= 0x7F;}>;
#endif //ASNCPP_IA5_STRING_H

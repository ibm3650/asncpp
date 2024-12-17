//
// Created by kandu on 29.11.2024.
//

#ifndef ASNCPP_PRINTABLE_STRING_H
#define ASNCPP_PRINTABLE_STRING_H
#include "base/string_base.h"
#include "base/common.h"

using printable_string_t = string_base_t<char, asncpp::base::asn1_tag::PRINTABLE_STRING,
                                                      [](char c) {
                                                          using namespace std::string_view_literals;
                                                          const auto allowed_specs = " '()+,-./:=?"sv;
                                                        return std::isalnum(c) || allowed_specs.contains(c); }>;



#endif //ASNCPP_PRINTABLE_STRING_H

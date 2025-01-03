//
// Created by kandu on 28.11.2024.
//

#ifndef ASNCPP_OBJECT_DESCRIPTOR_H
#define ASNCPP_OBJECT_DESCRIPTOR_H
#include "base/string_base.h"
#include "base/common.h"
using object_descriptor_t  = string_base_t<char, asncpp::base::asn1_tag::OBJECT_DESCRIPTOR,
                                                      [](char c) { return c >= 0x20 && c <= 0x7E; }>;

#endif //ASNCPP_OBJECT_DESCRIPTOR_H

//
// Created by kandu on 13.12.2024.
//

#ifndef BMP_STRING_H
#define BMP_STRING_H
#include "base/string_base.h"
#include "base/asn_base.h"
using bmp_string_t = string_base_t<char16_t, asn1_tag::BMP_STRING>;
#endif //BMP_STRING_H

//
// Created by kandu on 03.12.2024.
//

#ifndef ASNCPP_DATE_H
#define ASNCPP_DATE_H

#include "base/iso8601_base.h"
constexpr char date_format[] = "%Y-%m-%d";
using date_t = iso8601_base<date_format, asncpp::base::asn1_tag::DATE>;

#endif //ASNCPP_DATE_H

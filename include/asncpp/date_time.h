//
// Created by kandu on 06.01.2025.
//

#ifndef DATE_TIME_H
#define DATE_TIME_H
#include "base/iso8601_base.h"
constexpr char date_time_format[] = "%FT%T";
using date_time_t = iso8601_base<date_time_format, asncpp::base::asn1_tag::DATE_TIME>;

#endif //DATE_TIME_H

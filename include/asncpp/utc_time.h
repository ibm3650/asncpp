//
// Created by kandu on 03.12.2024.
//

#ifndef ASNCPP_UTC_TIME_H
#define ASNCPP_UTC_TIME_H
#include "base/iso8601_base.h"
constexpr char utc_time_format[] = "%y%m%d%H%M%SZ";
using utc_time_t = iso8601_base<utc_time_format, asncpp::base::asn1_tag::UTC_TIME>;

#endif //ASNCPP_UTC_TIME_H

//
// Created by kandu on 06.01.2025.
//

#ifndef TIME_OF_DAY_H
#define TIME_OF_DAY_H
#include "base/iso8601_base.h"
constexpr char time_of_day_format[] = "%T";
using time_of_day_t = iso8601_base<time_of_day_format, asncpp::base::asn1_tag::TIME_OF_DAY>;
#endif //TIME_OF_DAY_H

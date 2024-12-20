//
// Created by kandu on 10.12.2024.
//

#ifndef ENUMERATED_H
#define ENUMERATED_H
#include "base/integer_base.h"
//FIXME: сделать чтобы вывод to_string был уникальным для каждого типа
using enumerated_t = asncpp::base::integer_basic<intmax_t, asncpp::base::asn1_tag::ENUMERATED>;
#endif //ENUMERATED_H

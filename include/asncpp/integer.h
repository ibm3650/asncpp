//
// Created by kandu on 10.12.2024.
//

#ifndef INTEGER_H
#define INTEGER_H
#include "base/integer_base.h"
#include "base/common.h"
//FIXME: сделать чтобы вывод to_string был уникальным для каждого типа
using integer_t = integer_base<intmax_t, asncpp::base::asn1_tag::INTEGER>;
#endif //INTEGER_H

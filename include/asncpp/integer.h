//
// Created by kandu on 10.12.2024.
//

#ifndef INTEGER_H
#define INTEGER_H
#include "base/integer_base.h"
#include "base/common.h"
//FIXME: сделать чтобы вывод to_string был уникальным для каждого типа
namespace asncpp::types {
    using integer_t = asncpp::base::integer_basic<intmax_t, asncpp::base::asn1_tag::INTEGER>;
}   // namespace asncpp::types
#endif //INTEGER_H

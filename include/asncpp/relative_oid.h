//
// Created by kandu on 27.11.2024.
//

#ifndef ASNCPP_RELATIVE_OID_H
#define ASNCPP_RELATIVE_OID_H

#include <string>
#include <stdexcept>
#include "asncpp/base/common.h"
#include "asncpp/base/oid_base.h"
using relative_oid_t = object_identifier_base<true, asncpp::base::asn1_tag::RELATIVE_OID>;
#endif //ASNCPP_RELATIVE_OID_H

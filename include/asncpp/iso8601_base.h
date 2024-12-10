//
// Created by kandu on 10.12.2024.
//

#ifndef ISO8601_BASE_H
#define ISO8601_BASE_H


#include <stdexcept>
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>
#include "asncpp/asn_base.h"

template<const char* placeholder, asn1_tag type>
class iso8601_base
        : public asn1_base {
public:
    //TODO: Add support for fractional seconds
    //TODO: Default constructor set local time
    iso8601_base() {
        _type = type;
    }

    explicit iso8601_base(std::tm data) {
        _type = type;
        this->_decoded = data;
    }

    //TODO: Use std::chrono::parse
    void decode(std::span<const uint8_t>  /*data*/) final {
        const std::string iso8601{this->_data.begin(), this->_data.begin() + this->get_length()};
        std::tm tm = {};
        std::istringstream input_stream(iso8601);
        input_stream >> std::get_time(&tm, placeholder);
        if (input_stream.fail()) {
            throw std::invalid_argument("Invalid ISO 8601 format: " + iso8601);
        }
        this->_decoded = tm;
    }

    [[nodiscard]] auto encode() -> std::vector<uint8_t> final {
//        this->_data.assign_range(std::put_time(&this->_decoded, placeholder));
        return this->_data;
    }

    [[nodiscard]] std::string to_string() const {
        std::ostringstream oss;
        oss << std::put_time(&_decoded, placeholder);
        return oss.str();
    }

private:
    std::tm _decoded{};

    [[nodiscard]] auto _to_string() const -> std::string {
        return std::put_time(&this->_decoded, placeholder);
    }
};

constexpr char date_format[] = "%Y%m%d";
constexpr char date_time_format[] = "%FT%T";
constexpr char time_of_day_format[] = "%T";

using date_t = iso8601_base<date_format, asn1_tag::DATE>;
using date_time_t = iso8601_base<date_time_format, asn1_tag::DATE_TIME>;
using time_of_day_t = iso8601_base<time_of_day_format, asn1_tag::TIME_OF_DAY>;
#endif //ISO8601_BASE_H

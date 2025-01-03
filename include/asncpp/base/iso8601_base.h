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

#include "asncpp/base/common.h"
#include "asncpp/base/asn1_basic.h"


template<const char * placeholder, asncpp::base::asn1_tag type>
class iso8601_base : public asncpp::base::asn1_basic {
public:
    //TODO: Add support for fractional seconds
    //TODO: Default constructor set local time

    iso8601_base() noexcept = default;

    explicit iso8601_base(asn1_basic &&base) : asncpp::base::asn1_basic(std::move(base)) {
    }
    explicit iso8601_base(std::tm data) noexcept: _decoded{data} {
    }

    [[nodiscard]] const std::tm &get_value() const noexcept {
        return _decoded;
    }

    //TODO: Use std::chrono::parse
    void decode(std::span<const uint8_t> /*data*/) final {
        const std::string iso8601{this->_data.begin(), this->_data.begin() + this->get_length()};
        std::tm tm = {};
        std::istringstream input_stream(iso8601);
        input_stream >> std::get_time(&tm, _placeholder_without_delim().c_str());
        if (input_stream.fail()) {
            throw std::invalid_argument("Invalid ISO 8601 format: " + iso8601);
        }
        this->_decoded = tm;
    }

    [[nodiscard]] auto encode() -> std::vector<uint8_t> final {
        std::string output = to_string();
        const auto result = std::ranges::remove_if(output,
                                                   [](char c) { return !std::isdigit(c); });
        this->_data.assign(output.begin(), result.begin());
        return this->_data;
    }

    [[nodiscard]] auto to_string() const -> std::string final {
        std::ostringstream oss;
        oss << std::put_time(&_decoded, placeholder);
        return oss.str();
    }

    [[nodiscard]] constexpr uintmax_t get_tag() const noexcept final {
        return static_cast<uintmax_t>(type);
    }

private:
    std::tm _decoded{};

    [[nodiscard]] std::string _placeholder_without_delim() const {
        std::string tmp;
        std::ranges::copy_if(std::string_view{placeholder}, std::back_inserter(tmp),
                             [](char c) {
                                 return c != '-' &&
                                        c != ':';
                             });
        return tmp;
    }
};


//constexpr char date_time_format[] = "%FT%T";
//constexpr char time_of_day_format[] = "%T";

//using date_time_t = iso8601_base<date_time_format, asn1_tag::DATE_TIME>;
//using time_of_day_t = iso8601_base<time_of_day_format, asn1_tag::TIME_OF_DAY>;
#endif //ISO8601_BASE_H

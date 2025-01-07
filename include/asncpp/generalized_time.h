//
// Created by kandu on 03.12.2024.
//

#ifndef ASNCPP_GENERALIZED_TIME_H
#define ASNCPP_GENERALIZED_TIME_H

#include "asncpp/base/asn1_basic.h"
#include <stdexcept>
#include <string>
#include <string_view>
#include <ctime>
#include <iomanip>
#include <sstream>


class generalized_time_t
        : public asncpp::base::asn1_basic {
public:
    //TODO: Add support for fractional seconds
    //TODO: Default constructor set local time
    generalized_time_t() = default;

    explicit generalized_time_t(asncpp::base::asn1_basic &&base) : asncpp::base::asn1_basic(std::move(base)) {}
    explicit generalized_time_t(std::tm data, int8_t timezone = 0) {
        this->_value = std::make_pair(data, timezone);
    }

    //TODO: Use std::chrono::parse
    void decode(std::span<const uint8_t>  /*data*/) final {
        //const asn1_base raw = asn1_base({buffer, 0xFFFFFFFF});
        const std::string iso8601{_data.data(), _data.data() + _data.size()};
        std::tm tm = {};
        std::istringstream ss(iso8601);
        //std::istringstream ss(raw.get_data().data());

        // Парсим дату-время в формате ISO 8601 (без проверки часового пояса)
        ss >> std::get_time(&tm, "%Y%m%d%H%M%S");
        if (ss.fail()) {
            throw std::invalid_argument("Invalid ISO 8601 format: " + iso8601);
        }
        if (iso8601.contains('Z')) {
            this->_value = std::make_pair(tm, 0);
            return;
        }
        std::string tz_str;
        ss >> tz_str;
        tz_str = tz_str.substr(tz_str.length() - 5, 3);
        this->_value = std::make_pair(tm, std::stoi(tz_str));
    }

    [[nodiscard]] std::vector<uint8_t> encode() final {
       // std::vector<uint8_t> result;
        const std::string str = _to_string();
       _data.insert_range(_data.end(), str);
       // result.insert(result.begin(), result.size());
       // result.insert(result.begin(), static_cast<uint8_t>(asn1_tag::GENERALIZED_TIME));
        return _data;
    }

    [[nodiscard]] std::string to_string() const final {
        return "Visible String: " + _to_string();
    }

    [[nodiscard]] constexpr uintmax_t get_tag() const noexcept final {
        return static_cast<uintmax_t>(asncpp::base::asn1_tag::GENERALIZED_TIME);
    }

private:
    [[nodiscard]] std::string _to_string() const {
        constexpr size_t buffer_size = std::size("YYYYMMDDHHMMSS.000+HHMM");
        char buffer[buffer_size];
        const size_t out_length = std::strftime(buffer,
                                                buffer_size,
                                                "%Y%m%d%H%M%S.000",
                                                &this->_value.first);
        if (this->_value.second == 0) {
            std::strcat(buffer, "Z");
        } else {
            std::sprintf(buffer + out_length, "%+03d00", this->_value.second);
        }
        return {buffer};
    }
    std::pair<std::tm, int8_t> _value{};
};

#endif //ASNCPP_GENERALIZED_TIME_H

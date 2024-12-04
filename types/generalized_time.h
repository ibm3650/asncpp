//
// Created by kandu on 03.12.2024.
//

#ifndef ASNCPP_GENERALIZED_TIME_H
#define ASNCPP_GENERALIZED_TIME_H

#include "asncpp/asn_base.h"
#include <stdexcept>
#include <string>
#include <string_view>
#include <ctime>
#include <iomanip>
#include <sstream>


class generalized_time_t
        : public asn1_type<std::pair<std::tm, int8_t>, static_cast<uintmax_t>(asn1_tag::GENERALIZED_TIME)> {
public:
    //TODO: Add support for fractional seconds
    //TODO: Default constructor set local time
    generalized_time_t() = default;

    explicit generalized_time_t(std::tm data, int8_t timezone = 0) {
        this->_value = std::make_pair(data, timezone);
    }

    //TODO: Use std::chrono::parse
    void decode(const uint8_t *buffer) final {
        const asn1_base raw = asn1_base({buffer, 0xFFFFFFFF});
        const std::string iso8601{raw.get_data().data(), raw.get_data().data() + raw.get_length()};
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

    [[nodiscard]] std::vector<uint8_t> encode() const final {
        std::vector<uint8_t> result;
        const std::string str = _to_string();
        result.insert_range(result.end(), str);
        result.insert(result.begin(), result.size());
        result.insert(result.begin(), static_cast<uint8_t>(asn1_tag::GENERALIZED_TIME));
        return result;
    }

    [[nodiscard]] std::string to_string() const final {
        return "Visible String: " + _to_string();
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
};

#endif //ASNCPP_GENERALIZED_TIME_H

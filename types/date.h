//
// Created by kandu on 03.12.2024.
//

#ifndef ASNCPP_DATE_H
#define ASNCPP_DATE_H

#include "../asn1/base.h"
#include <stdexcept>
#include <string>
#include <string_view>
#include <ctime>
#include <iomanip>
#include <sstream>


class date_t
        : public asn1_type<std::tm, static_cast<uintmax_t>(asn1_tag::DATE)> {
public:
    //TODO: Add support for fractional seconds
    //TODO: Default constructor set local time
    date_t() = default;

    explicit date_t(std::tm data) {
        this->_value = data;
    }

    //TODO: Use std::chrono::parse
    void decode(const uint8_t *buffer) final {
        const asn1_block raw = asn1_block(buffer);
        const std::string iso8601{raw.get_data().data(), raw.get_data().data() + raw.get_length()};
        std::tm tm = {};
        std::istringstream ss(iso8601);
        //std::istringstream ss(raw.get_data().data());

        // Парсим дату-время в формате ISO 8601 (без проверки часового пояса)
        ss >> std::get_time(&tm, "%Y%m%d");
        if (ss.fail()) {
            throw std::invalid_argument("Invalid ISO 8601 format: " + iso8601);
        }

        this->_value = tm;
    }

    [[nodiscard]] std::vector<uint8_t> encode() const final {
        std::vector<uint8_t> result;
        const std::string str = _to_string();
        result.insert_range(result.end(), str);
        result.insert(result.begin(), result.size());
        result.insert(result.begin(), static_cast<uint8_t>(asn1_tag::DATE));
        return result;
    }

    [[nodiscard]] std::string to_string() const final {
        return "Visible String: " + _to_string();
    }

private:
    [[nodiscard]] std::string _to_string() const {
        constexpr size_t buffer_size = std::size("YYMMDDHHMMSSZ");
        char buffer[buffer_size];
        std::strftime(buffer,
                      buffer_size,
                      "%Y%m%d",
                      &this->_value);
        return {buffer};
    }
};
#endif //ASNCPP_DATE_H

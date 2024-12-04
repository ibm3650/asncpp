//
// Created by kandu on 03.12.2024.
//

#ifndef ASNCPP_DURATION_H
#define ASNCPP_DURATION_H

#include "asncpp/asn_base.h"
#include <stdexcept>
#include <string>
#include <string_view>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <regex>


class duration_t
        : public asn1_type<std::time_t, static_cast<uintmax_t>(asn1_tag::DURATION)> {
public:
    //TODO: Add support for fractional seconds
    //TODO: Default constructor set local time
    //TODO: Constructor from std::tm
    duration_t() = default;

    explicit duration_t(std::time_t data) {
        this->_value = data;
    }

    //TODO: Use std::chrono::parse
    void decode(const uint8_t *buffer) final {
        const asn1_base raw = asn1_base({buffer, 0xFFFFFFFF});
        const std::string iso8601{raw.get_data().data(), raw.get_data().data() + raw.get_length()};
        std::regex iso8601Regex(
                R"(^P(?:(\d+)Y)?(?:(\d+)M)?(?:(\d+)D)?(?:T(?:(\d+)H)?(?:(\d+)M)?(?:(\d+(?:\.\d*)?)S)?)?$)"
        );
        std::smatch match;

        if (!std::regex_match(iso8601, match, iso8601Regex)) {
            throw std::invalid_argument("Invalid ISO 8601 duration format: " + iso8601);
        }

        // Парсим компоненты
        int years = match[1].matched ? std::stoi(match[1]) : 0;
        int months = match[2].matched ? std::stoi(match[2]) : 0;
        int days = match[3].matched ? std::stoi(match[3]) : 0;
        int hours = match[4].matched ? std::stoi(match[4]) : 0;
        int minutes = match[5].matched ? std::stoi(match[5]) : 0;
        double seconds = match[6].matched ? std::stod(match[6]) : 0.0;

        // Преобразование компонентов
        const int secondsPerMinute = 60;
        const int secondsPerHour = 60 * secondsPerMinute;
        const int secondsPerDay = 24 * secondsPerHour;

        // Годы и месяцы — общее приближение (1 месяц = 30 дней, 1 год = 365 дней)
        const int secondsPerMonth = 30 * secondsPerDay;
        const int secondsPerYear = 365 * secondsPerDay;

        // Рассчитываем общее количество секунд
        std::chrono::seconds totalSeconds = std::chrono::seconds(
                static_cast<int>(years * secondsPerYear) +
                static_cast<int>(months * secondsPerMonth) +
                static_cast<int>(days * secondsPerDay) +
                static_cast<int>(hours * secondsPerHour) +
                static_cast<int>(minutes * secondsPerMinute) +
                static_cast<int>(seconds)
        );
        this->_value = totalSeconds.count();
    }

    [[nodiscard]] std::vector<uint8_t> encode() const final {
        std::vector<uint8_t> result;
        const std::string str = _to_string();
        result.insert_range(result.end(), str);
        result.insert(result.begin(), result.size());
        result.insert(result.begin(), static_cast<uint8_t>(asn1_tag::DURATION));
        return result;
    }

    [[nodiscard]] std::string to_string() const final {
        return "Visible String: " + _to_string();
    }

private:
    [[nodiscard]] std::string _to_string() const {
        std::time_t t = this->_value;
        int year = t / 31536000;
        t %= 31536000;
        int month = t / 2628000;
        t %= 2628000;
        int day = t / 86400;
        t %= 86400;
        int hour = t / 3600;
        t %= 3600;
        int minute = t / 60;
        t %= 60;
        int second = t;
        return std::format("P{}Y{}M{}DT{}H{}M{}S",
                           year,
                           month,
                           day,
                           hour,
                           minute,
                           second);
        constexpr size_t buffer_size = std::size("P1Y2M3DT4H5M6S");
        char buffer[buffer_size];
//        std::strftime(buffer,
//                      buffer_size,
//                      "%T",
//                      &this->_value);
        return {buffer};
    }
};

#endif //ASNCPP_DURATION_H

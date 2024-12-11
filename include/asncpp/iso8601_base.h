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

//template<auto& placeholder, asn1_tag type>
template<const char * placeholder, asn1_tag type>
//template<const char * placeholder, size_t N,asn1_tag type>
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

    [[nodiscard]] auto get_value() const -> const std::tm & {
        return this->_decoded;
    }

    //TODO: Use std::chrono::parse
    void decode(std::span<const uint8_t> /*data*/) final {
        const std::string iso8601{this->_data.begin(), this->_data.begin() + this->get_length()};
        std::tm tm = {};
        std::istringstream input_stream(iso8601);
        //input_stream >> std::get_time(&tm, placeholder);
       input_stream >> std::get_time(&tm, _placeholder_without_delim().c_str());
       // input_stream >> std::get_time(&tm, _placeholder_without_delim());
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

private:
    std::tm _decoded{};

   //  [[nodiscard]] constexpr const char* _placeholder_without_delim() const {
   // // [[nodiscard]]  std::string _placeholder_without_delim() const {
   //     // std::array<char, std::size(placeholder)> tmp;
   //
   //      char tmp[std::size(placeholder)]{};
   //      size_t index = 0;
   //
   //      for (char c : std::string_view{placeholder}) {
   //          if (c != '-' && c != ':') {
   //              tmp[index++] = c;
   //          }
   //      }
   //
   //      tmp[index] = '\0'; // Добавляем завершающий нуль-символ
   //      return tmp;
   //      //std::string tmp;
   //     // std::ranges::copy_if(std::string_view{placeholder}, std::back_inserter(tmp),
   //      //                     [](char c) { return c != '-' &&
   //      //                                        c!= ':'; });
   //     // return tmp;
   //  }

    //  [[nodiscard]] constexpr const char* _placeholder_without_delim() const {
   [[nodiscard]]  std::string _placeholder_without_delim() const {
    //     // std::array<char, std::size(placeholder)> tmp;
    //
    //      char tmp[std::size(placeholder)]{};
    //      size_t index = 0;
    //
    //      for (char c : std::string_view{placeholder}) {
    //          if (c != '-' && c != ':') {
    //              tmp[index++] = c;
    //          }
    //      }
    //
    //      tmp[index] = '\0'; // Добавляем завершающий нуль-символ
    //      return tmp;
    std::string tmp;
    std::ranges::copy_if(std::string_view{placeholder}, std::back_inserter(tmp),
                       [](char c) { return c != '-' &&
                                           c!= ':'; });
     return tmp;
     }
};

constexpr char date_format[] = "%Y-%m-%d";
constexpr char date_time_format[] = "%FT%T";
constexpr char time_of_day_format[] = "%T";
using date_t = iso8601_base<date_format, asn1_tag::DATE>;
//using date_time_t = iso8601_base<date_time_format, asn1_tag::DATE_TIME>;
//using time_of_day_t = iso8601_base<time_of_day_format, asn1_tag::TIME_OF_DAY>;
#endif //ISO8601_BASE_H

//
// Created by kandu on 29.11.2024.
//

#ifndef ASNCPP_UTF_STRING_H
#define ASNCPP_UTF_STRING_H
#include <string>
#include <stdexcept>
#include "asncpp/common.h"

class utf8_string_t : public asn1_type<std::vector<uint8_t >, static_cast<uintmax_t>(asn1_tag::UTF8_STRING)> {
public:
    utf8_string_t() = default;

    explicit utf8_string_t(const std::string &data) {
       // this->_value = data;
    }


    explicit utf8_string_t(const std::wstring &data) {
        // this->_value = data;
    }
    explicit utf8_string_t(const std::vector<uint8_t> &data) {
        this->_value = data;
    }

    explicit utf8_string_t(const std::vector<uint32_t> &data) {
       // this->_value = data;
    }

    explicit utf8_string_t(uint32_t codepoint) {
        // this->_value = data;
    }

    explicit utf8_string_t(std::u32string_view data) {
        // this->_value = data;
    }

    explicit utf8_string_t(std::u8string  data) {
        // this->_value = data;
    }
    void decode(const uint8_t *buffer) final {
        const asn1_base raw = asn1_base({buffer, 0xFFFFFFFF});
        //TODO: Добавить обработку ошибок
        //TODO: Обработка UTF-8
        this->_value.clear();
        this->_value.insert(this->_value.end(), raw.get_data().begin(), raw.get_data().end());
    }

    [[nodiscard]] std::vector<uint8_t> encode() const final {
        std::vector<uint8_t> result;
        result.insert(result.end(), this->_value.begin(), this->_value.end());
        result.insert(result.begin(), result.size());
        result.insert(result.begin(), static_cast<uint8_t>(asn1_tag::UTF8_STRING));
        return result;
    }

    [[nodiscard]] std::string to_string() const override {
//        return "UTF8 STRING: " + this->_value;
        return "UTF8 STRING: " + "this->_value";
    }
};
#endif //ASNCPP_UTF_STRING_H

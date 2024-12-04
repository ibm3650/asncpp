//
// Created by kandu on 22.11.2024.
//

#ifndef ASNCPP_INTEGER_H
#define ASNCPP_INTEGER_H


#include <cstdint>
#include <stdexcept>
#include <string>
#include "asncpp/asn_base.h"

using namespace std::string_literals;

template<std::integral T = intmax_t>
class integer_t : public asn1_base {
public:

    integer_t(){
        _type = asn1_tag::INTEGER;
    };

//    integer_t(std::span<const uint8_t> data) : asn1_base{data} { // NOLINT(*-explicit-constructor)
//        decode();
//    }

    explicit integer_t(T val) : _decoded(val) {
        _type = asn1_tag::INTEGER;
    }

    [[nodiscard]] explicit operator T() const noexcept {
        return this->_decoded;
    }

    explicit operator T() {
        return this->_decoded;
    }

    T get_value() const {
        return this->_decoded;
    }

    virtual std::vector<uint8_t> encode() final {
        T temp = this->_decoded;

        //Число уже хранится в формате дополнения до двух, нет необходимости в дополнительном кодировании
        // Кодирование значения в big-endian формате
        do {
            this->_data.insert(this->_data.begin(), static_cast<uint8_t>(temp & 0xFFU));
            temp >>= 8U;
        } while (temp != 0 && temp != -1); // Учитываем дополнение для отрицательных чисел

        // Добавление ведущего байта для отрицательных чисел, если требуется
        if (this->_decoded < 0 && !(this->_data[0] & 0x80)) {
            this->_data.insert(this->_data.begin(), 0xFF);
        } else if (this->_decoded >= 0 && (this->_data[0] & 0x80)) {
            this->_data.insert(this->_data.begin(), 0x00);
        }
        return this->_data;
       // return asn1_base<T, U>::encode();
    }

    [[nodiscard]] virtual std::string to_string() const final {
        return "INTEGER: "s + std::to_string(this->_decoded);
    }

    virtual void decode(std::span<const uint8_t> /*data*/) final {
        if (this->_data.size() > sizeof(T)) {
            throw std::runtime_error("Integer value is too large");
        }

        this->_decoded = 0; // Обнуление перед началом декодирования

        // Собираем значение из байтов
        for (size_t i = 0; i < this->get_length(); ++i) {
            this->_decoded = (this->_decoded << 8) | this->_data[i];
        }


        // Проверяем знак
        if (this->_data[0] & 0x80) { // Отрицательное число
            //Для того, чтобы, 16-ти битное число, корректно отображалось в 32-х битном числе,
            //При учете, что оно отрицательное, необходимо установить все биты в 1, кроме 16-ти младших
            const T mask = static_cast<T>(-1) << (8 * this->get_length());
            this->_decoded |= mask;
        }
    }
private:
    T _decoded{};
};


#endif //ASNCPP_INTEGER_H

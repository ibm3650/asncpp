//
// Created by kandu on 22.11.2024.
//

#ifndef ASNCPP_INTEGER_H
#define ASNCPP_INTEGER_H


#include <cstdint>
#include <stdexcept>
#include <string>
#include "../include/asn_base.h"

using namespace std::string_literals;

template<std::integral T = intmax_t, uintmax_t U = static_cast<uintmax_t>(asn1_tag::INTEGER)>
class integer_t : public asn1_base<T, U> {
public:

    integer_t() = default;

    explicit integer_t(T val) {
        this->_decoded = val;
    }

    [[nodiscard]] explicit operator T() const noexcept {
        return this->_value;
    }

    [[nodiscard]] virtual asn1_base<T, U>::dynamic_data_t encode() final {
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
        return asn1_base<T, U>::encode();
    }

    [[nodiscard]] virtual std::string to_string() const final {
        return "INTEGER: "s + std::to_string(this->_value);
    }

//    void decode(const uint8_t *buffer) final {
//        const asn1_base raw = asn1_base({buffer, 0xFFFFFFFF});
//
//        if (raw.get_length() > sizeof(T)) {
//            throw std::runtime_error("Integer value is too large");
//        }
//
//        this->_value = 0; // Обнуление перед началом декодирования
//
//        // Собираем значение из байтов
//        for (size_t i = 0; i < raw.get_length(); ++i) {
//            this->_value = (this->_value << 8) | raw.get_data()[i];
//        }
//
//
//        // Проверяем знак
//        if (raw.get_data()[0] & 0x80) { // Отрицательное число
//            //Для того, чтобы, 16-ти битное число, корректно отображалось в 32-х битном числе,
//            //При учете, что оно отрицательное, необходимо установить все биты в 1, кроме 16-ти младших
//            const T mask = static_cast<T>(-1) << (8 * raw.get_length());
//            this->_value |= mask;
//        }
//    }
//




};


#endif //ASNCPP_INTEGER_H

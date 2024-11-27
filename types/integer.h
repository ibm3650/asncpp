//
// Created by kandu on 22.11.2024.
//

#ifndef ASNCPP_INTEGER_H
#define ASNCPP_INTEGER_H


#include <cstdint>
#include <stdexcept>
#include <string>
#include "../asn1/base.h"

using namespace std::string_literals;

template <std::integral T = intmax_t>
class integer_t : public asn1_type<T, static_cast<uintmax_t>(asn1_tag::INTEGER)> {
public:
    explicit integer_t(T val) {
        this->_value = val;
    }

    void decode(const uint8_t *buffer) final {
        const asn1_block raw = asn1_block(buffer);

        if (raw.get_length() > sizeof(T)) {
            throw std::runtime_error("Integer value is too large");
        }

        this->_value = 0; // Обнуление перед началом декодирования

        // Собираем значение из байтов
        for (size_t i = 0; i < raw.get_length(); ++i) {
            this->_value = (this->_value << 8) | raw.get_data()[i];
        }


        // Проверяем знак
        if (raw.get_data()[0] & 0x80) { // Отрицательное число
            //Для того, чтобы, 16-ти битное число, корректно отображалось в 32-х битном числе,
            //При учете, что оно отрицательное, необходимо установить все биты в 1, кроме 16-ти младших
            const T mask = static_cast<T>(-1) << (8 * raw.get_length());
            this->_value |= mask;
        }
    }

    [[nodiscard]] explicit operator T() const noexcept{
        return this->_value;
    }

    [[nodiscard]] std::vector<uint8_t> encode() const final {
        std::vector<uint8_t> output;
        T temp = this->_value;

        //Число уже хранится в формате дополнения до двух, нет необходимости в дополнительном кодировании
        // Кодирование значения в big-endian формате
        do {
            output.insert(output.begin(), static_cast<uint8_t>(temp & 0xFF));
            temp >>= 8;
        } while (temp != 0 && temp != -1); // Учитываем дополнение для отрицательных чисел

        // Добавление ведущего байта для отрицательных чисел, если требуется
        if (this->_value < 0 && !(output[0] & 0x80)) {
            output.insert(output.begin(), 0xFF);
        }
        else if (this->_value >= 0 && (output[0] & 0x80)) {
            output.insert(output.begin(), 0x00);
        }

        const auto length = asn1_block::encode_length(output.size());
        output.insert(output.begin(), length.begin(), length.end());
        output.insert(output.begin(), static_cast<uint8_t>(asn1_tag::INTEGER));
        return output;
    }

    [[nodiscard]] std::string to_string() const {
        return "INTEGER: "s + std::to_string(this->_value) + "\n";
    }
};


#endif //ASNCPP_INTEGER_H

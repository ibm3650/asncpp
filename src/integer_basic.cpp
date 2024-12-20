//
// Created by kandu on 20.12.2024.
//
#include <stdexcept>

#include "asncpp/base/integer_base.h"


template<std::integral T, asncpp::base::asn1_tag type>
std::vector<uint8_t> integer_basic<T, type>::encode() {
    /*Число уже хранится в формате дополнения до двух, нет необходимости в дополнительном кодировании
     * Кодирование значения в big-endian формате
     * */
    T temp = _decoded;
    do {
        _data.insert(_data.begin(), static_cast<uint8_t>(temp & 0xFFU));
        temp >>= 8U;
    } while (temp != 0 && temp != -1); // Учитываем дополнение для отрицательных чисел

    // Добавление ведущего байта для отрицательных чисел, если требуется
    if (_decoded < 0 && !(_data[0] & 0x80U)) {
        _data.insert(_data.begin(), 0xFFU);
    }
    // Добавление ведущего байта для положительных чисел, у которых старший бит установлен
    else if (_decoded >= 0 && (_data[0] & 0x80U)) {
        _data.insert(_data.begin(), 0x00U);
    }
    return _data;
}

template<std::integral T, asncpp::base::asn1_tag type>
void integer_basic<T, type>::decode(std::span<const uint8_t> /*data*/) {
    if (_data.size() > sizeof(T)) {
        throw std::runtime_error("Integer value is too large");
    }
    _decoded = 0; // Обнуление перед началом декодирования
    const size_t length = _data.size();
    // Собираем значение из байтов
    for (size_t i = 0; i < length; ++i) {
        _decoded = (this->_decoded << 8U) | _data[i];
    }


    /* Проверяем знак. Не делаем второе дополнение, если длинна значения равна размеру типа
     * Потому как при сдвиге влево на равное размеру число бит или большее, происходит UB
     * */
    if ((_data[0] & 0x80U) && length != sizeof(T)) {
        // Отрицательное число
        /* Для того чтобы 16-ти битное число, корректно отображалось в 32-х битном типе,
         * При учете, что оно отрицательное, необходимо установить все биты в 1, кроме 16-ти младших
         * */
        const T mask = static_cast<T>(-1) << (8U * length);
        _decoded |= mask;
    }
}

template class integer_basic<intmax_t, asncpp::base::asn1_tag::INTEGER>;
template class integer_basic<intmax_t, asncpp::base::asn1_tag::ENUMERATED>;

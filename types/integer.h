//
// Created by kandu on 22.11.2024.
//

#ifndef ASNCPP_INTEGER_H
#define ASNCPP_INTEGER_H


#include <cstdint>
#include <stdexcept>
#include <string>
#include "asncpp/asn_base.h"

//TODO: Добавить поддержку чисел, размер  которых превышает размер типа T
/**
 * @class integer_base
 * @brief ASN.1 INTEGER тип с поддержкой кодирования и декодирования.
 * @tparam T Тип данных для хранения значения INTEGER (например, intmax_t).
 */
template<std::integral T>
class integer_base: public asn1_base {
public:
    using value_t = T;

    /**
     * @brief Конструктор по умолчанию.
     */
    integer_base() noexcept {
        _type = asn1_tag::INTEGER;
    };


    /**
     * @brief Конструктор с инициализацией значением.
     * @param val Значение для инициализации INTEGER.
     */
    explicit integer_base(T val) noexcept: _decoded{val} {
        _type = asn1_tag::INTEGER;
    }

    /**
     * @brief Явное преобразование к типу T.
     * @return Декодированное значение.
     */
    [[nodiscard]] operator T() const noexcept { // NOLINT(*-explicit-constructor)
        return this->_decoded;
    }

    /**
     * @brief Получить декодированное значение INTEGER.
     * @return Ссылка на декодированное значение.
     */
    [[maybe_unused]] auto get_value() const noexcept -> value_t& {
        return this->_decoded;
    }

    /**
     * @brief Кодирование INTEGER в DER.
     * @return Вектор байтов, представляющий закодированное значение.
     */
    auto encode() -> std::vector<uint8_t> final  {
        /*Число уже хранится в формате дополнения до двух, нет необходимости в дополнительном кодировании
         * Кодирование значения в big-endian формате
         * */
        T temp = this->_decoded;
        do {
            this->_data.insert(this->_data.begin(), static_cast<uint8_t>(temp & 0xFFU));
            temp >>= 8U;
        } while (temp != 0 && temp != -1); // Учитываем дополнение для отрицательных чисел

        // Добавление ведущего байта для отрицательных чисел, если требуется
        if (this->_decoded < 0 && !(this->_data[0] & 0x80U)) {
            this->_data.insert(this->_data.begin(), 0xFFU);
        }
        // Добавление ведущего байта для положительных чисел, у которых старший бит установлен
        else if (this->_decoded >= 0 && (this->_data[0] & 0x80U)) {
            this->_data.insert(this->_data.begin(), 0x00U);
        }
        return this->_data;
    }

    /**
     * @brief Преобразование INTEGER в строку.
     * @return Строка, представляющая значение INTEGER.
     */

    [[nodiscard]] auto to_string() const -> std::string final {
        return "INTEGER: " + std::to_string(this->_decoded);
    }

    /**
     * @brief Декодирование INTEGER из DER.
     * @param data Байтовый диапазон, представляющий закодированное значение.
     * @throws std::runtime_error Если значение слишком велико для типа T.
     */
    void decode(std::span<const uint8_t> /*data*/) final {
        if (this->_data.size() > sizeof(T)) {
            throw std::runtime_error("Integer value is too large");
        }
        this->_decoded = 0; // Обнуление перед началом декодирования
        const size_t length = this->get_length();
        // Собираем значение из байтов
        for (size_t i = 0; i < length; ++i) {
            this->_decoded = (this->_decoded << 8U) | this->_data[i];
        }


        /* Проверяем знак. Не делаем второе дополнение, если длинна значения равна размеру типа
         * Потому как при сдвиге влево на равное размеру число бит или большее, происходит UB
         * */
        if ((this->_data[0] & 0x80U) && length != sizeof(T)) { // Отрицательное число
            /* Для того чтобы 16-ти битное число, корректно отображалось в 32-х битном типе,
             * При учете, что оно отрицательное, необходимо установить все биты в 1, кроме 16-ти младших
             * */
            const T mask = static_cast<T>(-1) << (8U * length);
            this->_decoded |= mask;
        }
    }

private:
    T _decoded{}; ///< Хранит декодированное значение INTEGER.
};

using integer_t = integer_base<intmax_t>;

#endif //ASNCPP_INTEGER_H

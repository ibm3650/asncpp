//
// Created by kandu on 22.11.2024.
//

#ifndef ASNCPP_INTEGER_H
#define ASNCPP_INTEGER_H
#include <cstdint>
#include <string>
#include <stdexcept>
#include "asn1_basic.h"
#include "common.h"

namespace asncpp::base {
    /**
 * @class integer_basic
 * @brief Базовый класс для хранения целочисленных значений и членов перечислений в формате ASN.1.
 *
 * Этот шаблонный класс используется для хранения целочисленных значений и значений из перечислений в формате ASN.1.
 * Класс поддерживает кодирование и декодирование целочисленных значений, а также работу с перечислениями.
 *
 * @tparam T Тип данных, используемый для хранения целочисленного значения в разобранном виде.
 * @tparam type Тег ASN.1 типа (например, INTEGER или ENUMERATED).
 *
 * @details
 * Класс предоставляет методы для кодирования и декодирования целочисленных значений в формат ASN.1.
 * Он параметризован целочисленным типом и тегом ASN.1 типа, что позволяет использовать любые целочисленные типы.
 * В контексте ASN.1, данный класс используется для типов INTEGER и ENUMERATED, которые отличаются только тегом.
 *
 * @todo Реализовать методы для работы с перечислениями (ENUMERATED).
 * @todo Реализовать методы для работы с целыми числами.
 * @todo Реализовать операторы приведения типов для работы с целыми числами.
 * @todo Добавить тесты для функции `to_string`.
 * @todo Добавить тесты для случая переполнения.
 * @todo Добавить конструктор, принимающий строковое представление числа.
 * @todo Добавить поддержку длинных целых чисел, длина которых превышает 64 бита.
 */
    template<std::integral T, asn1_tag type>
    class integer_basic : public asn1_basic {
    public:
        using asn1_basic::operator=; ///< Оператор присваивания базового класса.
        using value_type = T; ///< Тип данных для хранения целочисленного значения.
        using const_reference = const T &; ///< Константная ссылка на целочисленное значение.


        /**
 * @brief Конструктор перемещения из базового класса ASN.1.
 *
 * @details
 * Этот конструктор позволяет создать объект integer_basic путём перемещения данных из существующего
 * объекта базового класса `asn1_basic`. Это необходимо при десериализации объектов с помощью @ref deserialize_v и
 * обязательно для оптимизации производительности и не может применяться для других целей.
 *
 * @param[in] base Объект базового класса @ref asn1_basic, данные которого будут перемещены в текущий объект.
 * После вызова конструктора объект `base` перейдёт в неопределённое состояние и не должен использоваться.
 *
        * @note Этот конструктор предназначен для десериализации объектов с помощью @ref deserialize_v и
 * обязательно для оптимизации производительности и не может применяться для других целей.
 */
        explicit integer_basic(asn1_basic &&base) noexcept : asn1_basic(std::move(base)) {};


        /**
         * @brief Конструктор по умолчанию.
         */
        integer_basic() noexcept = default;

        /**
         * @brief Конструктор перемещения по умолчанию.
         * @param[in] other Объект, данные которого будут перемещены в текущий объект.
         */
        integer_basic(integer_basic &&) noexcept = default;


        /**
         * @brief Конструктор копирования по умолчанию.
         * @param[in] other Объект, данные которого будут скопированы в текущий объект.
         */
        integer_basic(const integer_basic &) noexcept = default;

        /**
         * @brief Оператор присваивания перемещения по умолчанию.
         * @param[in] other Объект, данные которого будут перемещены в текущий объект.
         * @return Ссылка на текущий объект.
         */
        integer_basic &operator=(integer_basic &&) noexcept = default;

        /**
         * @brief Оператор копирования по умолчанию.
         * @param[in] other Объект, данные которого будут скопированы в текущий объект.
         * @return Ссылка на текущий объект.
         */
        integer_basic &operator=(const integer_basic &) noexcept = default;


        /**
         * @brief Конструктор, принимающий целочисленное значение.
         * @param[in] val Целочисленное значение для хранения.
         */
        integer_basic(const value_type val) noexcept : _decoded{val} {
        }

        /**
         * @brief Оператор присваивания перемещения по умолчанию.
         * @param[in] val Целочисленное значение для присваивания.
         * @return Ссылка на текущий объект.
         */
        integer_basic &operator=(const value_type val) noexcept {
            _decoded = val;
            return *this;
        }

        /**
         * @brief Деструктор по умолчанию.
         */
        ~integer_basic() noexcept final = default;

        /**
         * @brief Кодирует целочисленное значение в ASN.1 байтовый массив.
         * @details
         * Функция кодирует целочисленное значение в формате ASN.1, используя правила кодирования DER и возвращает полный
         * TLV-блок, содержащий закодированное значение. Функция используется исключительно с @ref serialize_v
         * @return Байтовый массив, содержащий закодированное ASN.1 значение.
         */
        dynamic_array_t encode() final;

        /**
         * @brief Декодирует целочисленное значение из ASN.1 байтового массива.
         * @details
         * Функция декодирует целочисленное значение из байтового массива, содержащего закодированное значение в формате
            * ASN.1. Функция используется исключительно с @ref deserialize_v.
         * @param[in] data Байтовый массив, содержащий закодированное ASN.1 значение.
         * @details Функция используется исключительно с @ref serialize_v
         * @throws std::runtime_error Если длина значения превышает размер типа.
         */
        void decode(std::span<const uint8_t> data) final;

        /**
         * @brief Возвращает тег ASN.1 типа.
         * @details Функция возвращает тег ASN.1 типа, используемый для кодирования и декодирования значения. Тег зависит от
         * типа данных, хранящихся в объекте. Определение данного метода обязательно для всех классов, наследующихся от
         * @ref asn1_basic.
         * @return Тег ASN.1 типа.
         */
        constexpr uintmax_t get_tag() const noexcept final {
            return static_cast<uintmax_t>(type);
        }

        /**
         *  @brief Возвращает целочисленное значение.
         *  @return Константная ссылка на целочисленное значение.
         */
        constexpr const_reference get_value() const noexcept {
            return _decoded;
        }

        /**
         * @brief Converts the integer value to a string representation.
         * @return String representation of the integer value.
         */
        std::string to_string() const final {
            return std::to_string(_decoded);
        }

        /**
         * @brief Converts the integer value to an integral type.
         * @return Converted integer value.
         */
        operator value_type() const noexcept {
            return _decoded;
        }

    private:
        value_type _decoded{}; ///< Decoded integer value.
    };
} // namespace asncpp::base

#endif //ASNCPP_INTEGER_H

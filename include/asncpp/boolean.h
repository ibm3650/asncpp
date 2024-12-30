//
// Created by kandu on 26.11.2024.
//

#ifndef ASNCPP_BOOLEAN_H
#define ASNCPP_BOOLEAN_H

#include "base/asn1_basic.h"
namespace asncpp::types {
    /**
     * @class boolean_t
     * @brief Класс для представления типа BOOLEAN в ASN.1.
     */
    class boolean_t : public asncpp::base::asn1_basic {
    public:
        using value_type = bool;

        explicit boolean_t(asn1_basic &&base) : asn1_basic(std::move(base)) {
        };

        /**
         * @brief Конструктор по умолчанию.
         */
        boolean_t() noexcept = default;

        /**
         * @brief Конструктор перемещения.
         */
        boolean_t(boolean_t &&) noexcept = default;

        /**
         * @brief Конструктор копирования.
         */
        boolean_t(const boolean_t &) noexcept = default;

        /**
         * @brief Оператор перемещения.
         */
        boolean_t &operator=(boolean_t &&) noexcept = default;

        /**
         * @brief Оператор присваивания.
         */
        boolean_t &operator=(const boolean_t &) noexcept = default;

        /**
         * @brief Конструктор с инициализацией значением.
         * @param val Значение типа BOOLEAN.
         */
        explicit boolean_t(bool val) noexcept : _value{val} {
        }

        /**
         * @brief Деструктор.
         */
        ~boolean_t() final = default;

        /**
         * @brief Преобразование в тип bool.
         * @return Значение BOOLEAN.
         */
        constexpr operator value_type() const noexcept {
            return _value;
        }

        /**
         * @brief Получение тега типа BOOLEAN.
         * @return ASN.1 тег.
         */
        constexpr uintmax_t get_tag() const noexcept final {
            return static_cast<uintmax_t>(asncpp::base::asn1_tag::BOOLEAN);
        }

        /**
         * @brief Декодирование значения BOOLEAN из данных.
         * @param data Буфер данных.
         * @throws std::runtime_error В случае некорректной длины данных.
         */
        void decode(std::span<const uint8_t> data) final;

        /**
         * @brief Кодирование значения BOOLEAN в массив байт.
         * @return Закодированные данные.
         */
        asncpp::base::dynamic_array_t encode() final;

        /**
         * @brief Преобразование значения BOOLEAN в строку.
         * @return "TRUE" или "FALSE".
         */
        [[nodiscard]] std::string to_string() const final {
            return _value ? "TRUE" : "FALSE";
        }

    private:
        bool _value{}; /**< Значение BOOLEAN. */
    };
}


#endif // ASNCPP_BOOLEAN_H

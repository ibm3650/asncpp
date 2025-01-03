//
// Created by kandu on 12.12.2024.
//

#ifndef STRING_BASE_H
#define STRING_BASE_H

#include "asn1_basic.h"
#include <string>
#include <string_view>
#include <stdexcept>

/**
 * @class string_base_t
 * @brief Базовый класс для представления строковых ASN.1 типов.
 *
 * Этот класс реализует кодирование и декодирование строковых типов ASN.1,
 * таких как VISIBLE_STRING, BMP_STRING, UNIVERSAL_STRING и другие.
 * Поддерживает ограничения символов с помощью функции limitations.
 *
 * @tparam T Тип символов строки (например, char, char16_t, char32_t).
 * @tparam type ASN.1 тег строкового типа.
 * @tparam limitations Функция или указатель на функцию для проверки допустимости символов.
 */
template<class T, asncpp::base::asn1_tag type, auto limitations = nullptr>
class string_base_t : public asncpp::base::asn1_basic {
public:
    /**
     * @brief Конструктор перемещения для базового класса.
     * @param base Экземпляр базового класса asn1_basic.
     */
    explicit string_base_t(asn1_basic &&base) : asncpp::base::asn1_basic(std::move(base)) {
    }

    /**
        * @brief Получение значения строки.
        * @return Константная ссылка на строку.
        */
    [[nodiscard]] const std::basic_string<T>& value() const {
        return _value;
    }

    using value_t = std::basic_string<T>;


    string_base_t() noexcept = default;

    string_base_t(string_base_t &&) noexcept = default;

    string_base_t(const string_base_t &) noexcept = default;

    string_base_t &operator=(string_base_t &&) noexcept = default;

    string_base_t &operator=(const string_base_t &) noexcept = default;

    ~string_base_t() noexcept final = default;

    /**
      * @brief Конструктор с инициализацией строки.
      * @param data Входная строка.
      * @throws std::invalid_argument Если строка содержит недопустимые символы.
      */
    explicit string_base_t(std::basic_string_view<T> data) {
        if constexpr (limitations != nullptr) {
            for (const T symbol: data) {
                if (!limitations(symbol)) {
                    throw std::invalid_argument("Invalid character in VISIBLE_STRING");
                }
            }
        }

        this->_value = data;
    }

    //TODO: Reserve memory
    /**
   * @brief Кодирует строку в формат ASN.1.
   * @return Вектор байтов, представляющий строку в формате ASN.1.
   */
    [[nodiscard]] auto encode() -> std::vector<uint8_t> final {
        if constexpr (sizeof(T) == 1) {
            _data.append_range(_value);
            return _data;
        }
        for (auto symbol: _value) {
            std::vector<uint8_t> tmp;
            for (size_t i = 0; i < sizeof(T); ++i) {
                tmp.insert(tmp.begin(), static_cast<uint8_t>(symbol & 0xFFU));
                symbol >>= 8U;
            }
            _data.append_range(tmp);
        }
        return _data;
    }


    //TODO: Implement long tag encoding
    //TODO: REserve space for length
    /**
    * @brief Декодирует данные из формата ASN.1 в строку.
    * @param data Данные в формате ASN.1.
    * @throws std::invalid_argument Если размер данных некорректен для строки.
    */
    void decode(std::span<const uint8_t> /*data*/) final {
        for (size_t i = 0; i < _data.size(); i += sizeof(T)) {
            T symbol{};
            for (size_t j = 0; j < sizeof(T); ++j) {
                symbol = (symbol << 8U) | static_cast<T>(_data[i + j]);
            }
            _value += symbol;
        }
    }

protected:
    /**
    * @brief Возвращает ASN.1 тег строки.
    * @return ASN.1 тег.
    */
    [[nodiscard]] constexpr uintmax_t get_tag() const noexcept override {
        return static_cast<uintmax_t>(type);
    }

private:
    value_t _value{};
};

#endif //STRING_BASE_H

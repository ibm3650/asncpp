//
// Created by kandu on 13.12.2024.
//

#ifndef OID_BASE_H
#define OID_BASE_H
#include <algorithm>
#include <vector>
#include <string>
#include <stdexcept>
#include <span>
#include <sstream>
#include <ranges>
#include "asncpp/base/asn_base.h"

//TODO: Implement long sid encoding
//TODO: Инициализация строкой
//TODO: Ошибка, если при декодировании bse128 не хватает байтов
//TODO: Проверка на длину массива
//TODO: Оптимизация кода
//FIXME: сделать чтобы вывод to_string был уникальным для каждого типа
/**
 * @class object_identifier_base
 * @brief Реализация ASN.1 OBJECT IDENTIFIER с поддержкой кодирования и декодирования.
 */
template<bool relative, asn1_tag type>
class object_identifier_base : public asncpp::base::asn1_base {
public:
    using sid_t = uint32_t;

    /**
     * @brief Конструктор по умолчанию.
     */
    object_identifier_base() {
        _type = type;
    }

    /**
     * @brief Конструктор с инициализацией значением.
     * @param data Последовательность идентификаторов.
     */
    object_identifier_base(std::span<const sid_t> data) {
        _type = type;
        _value.assign(data.begin(), data.end());
    }

    /**
     * @brief Получение строки, представляющей OBJECT IDENTIFIER.
     * @return Строковое представление.
     */
    [[nodiscard]] auto to_string() const -> std::string final {
        std::ostringstream oss;
        oss << "OBJECT IDENTIFIER: ";
        for (size_t i = 0; i < _value.size(); ++i) {
            oss << _value[i];
            if (i != _value.size() - 1) {
                oss << ".";
            }
        }
        return oss.str();
    }

    /**
     * @brief Получение значения OBJECT IDENTIFIER.
     * @return Значения идентификаторов.
     */
    [[nodiscard]] auto get_value() const -> std::vector<sid_t> {
        return _value;
    }

protected:
    /**
     * @brief Кодирование OBJECT IDENTIFIER в DER.
     * @return Вектор байтов, представляющий закодированное значение.
     */
    [[nodiscard]] auto encode() -> std::vector<uint8_t> final {
        auto to_b128 = [](sid_t value) -> std::vector<uint8_t> {
            std::vector<uint8_t> result;
            do {
                result.insert(result.begin(), static_cast<uint8_t>(value & 0x7FU));
                value >>= 7U;
            } while (value > 0);

            std::transform(result.begin(),
                           std::prev(result.end()),
                           result.begin(),
                           [](const uint8_t byte) {
                               return byte | 0x80U;
                           });
            return result;
        };
        if constexpr (relative) {
            if (_value.empty()) {
                throw std::runtime_error("Relative OID must have at least one SID");
            }
        } else {
            if (_value.size() < 2) {
                throw std::runtime_error("OID must have at least two SIDs");
            }
            if (_value[0] > 2) {
                throw std::runtime_error("First SID must be in range [0, 2]");
            }
            if (_value[0] != 2 && _value[1] > 39) {
                throw std::runtime_error("Second SID must be in range [0, 39]");
            }

            this->_data = to_b128((40U * _value[0]) + _value[1]);
        }
        for (const auto &sid: _value | std::views::drop(2)) {
            this->_data.append_range(to_b128(sid));
        }

        return this->_data;
    }

    /**
     * @brief Декодирование OBJECT IDENTIFIER из DER.
     * @param data Байтовый диапазон.
     */
    void decode(std::span<const uint8_t> /*data*/) final {
        if (this->_data.empty()) {
            throw std::runtime_error("OID must have at least one byte");
        }

        auto from_b128 = [&](auto &iter) -> sid_t {
            sid_t result = 0;
            while (iter != this->_data.end()) {
                result = (result << 7U) | (*iter & 0x7FU);
                if (!(*iter++ & 0x80U)) {
                    break;
                }
            }
            return result;
        };

        auto it = this->_data.begin();
        if constexpr (!relative) {
            const sid_t first_sid = from_b128(it);

            if (first_sid > 80) {
                _value = {2, first_sid - 80};
            } else {
                _value = {first_sid / 40, first_sid % 40};
            }
        }
        while (it < this->_data.end()) {
            _value.push_back(from_b128(it));
        }
    }

private:
    std::vector<sid_t> _value; ///< Хранение идентификаторов.
};

#endif //OID_BASE_H

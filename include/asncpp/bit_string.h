//
// Created by kandu on 28.11.2024.
//

#ifndef ASNCPP_BIT_STRING_H
#define ASNCPP_BIT_STRING_H

#include <ranges>
#include <bitset>
#include <numeric>

#include "asncpp/base/asn1_basic.h"

class bit_string_t : public asncpp::base::asn1_basic {
public:
    //template<size_t N>
    //bit_string_t(const std::bitset<N>& data);


    /**
     * @brief Конструктор из вектора булевых значений.
     * @param data Вектор значений `bool`.
     */
    bit_string_t(const std::vector<bool>& data) : _bit_length{data.size()} {
        for (const auto& chunk : data | std::views::chunk(8) ) {
            uint8_t byte{0};
            for (const bool bit : chunk) {
                byte <<= 1U;
                byte |= static_cast<uint8_t>(bit);
            }
            if (chunk.size() < 8) {
                byte <<= 8 - chunk.size();
            }
            _value.push_back(byte);
        }
    }



    /**
     * @brief Конструктор из вектора байтов и длины в битах.
     * @param data Вектор байтов.
     * @param bit_length Длина в битах.
     */
    bit_string_t(const std::vector<uint8_t>& data, size_t bit_length): _value{data}, _bit_length{bit_length} {
        if (bit_length > data.size() * 8) {
            throw std::invalid_argument("Bit length exceeds data size");
        }
    }



    /**
     * @brief Конструктор из строкового представления битов.
     * @param data Строка, где '1' и '0' представляют биты.
     */
    bit_string_t(std::string_view data) : _bit_length{data.size()} {
        for (const auto& chunk : data | std::views::chunk(8) ) {
            uint8_t byte{0};
            for (const char bit : chunk) {
                if (bit != '0' && bit != '1') {
                    throw std::invalid_argument("Invalid character in bit string");
                }
                byte <<= 1U;
                byte |= static_cast<uint8_t>(bit == '1');
            }
            if (chunk.size() < 8) {
                byte <<= 8 - chunk.size();
            }
            _value.push_back(byte);
        }
    }

    /**
     * @brief Конструктор из числа и длины в битах.
     * @param data Число, которое нужно преобразовать в битовую строку.
     * @param bit_length Длина в битах.
     */
    bit_string_t(uint64_t data, size_t bit_length) : _bit_length{bit_length} {
        if (bit_length > 64) {
            throw std::invalid_argument("Bit length exceeds 64 bits");
        }
        while (bit_length > 0) {
            size_t chunk = std::min<size_t>(bit_length, 8);
            _value.insert(_value.begin(), static_cast<uint8_t>(data & 0xFFU));
            data >>= chunk;
            bit_length -= chunk;
        }
    }




    bit_string_t() = default;

    ~bit_string_t() final = default;

    constexpr uintmax_t get_tag() const noexcept final {
        return static_cast<uintmax_t>(asncpp::base::asn1_tag::BIT_STRING);
    }


    [[nodiscard]] const std::vector<uint8_t> & value() const {
        return _value;
    }

    [[nodiscard]] size_t bit_length() const {
        return _bit_length;
    }

    void decode(std::span<const uint8_t>  /*data*/) final {
        if (_data.empty()) {
            throw std::invalid_argument("Invalid BIT STRING: empty data");
        }

        uint8_t unused_bits = _data[0];
        if (unused_bits > 7) {
            throw std::invalid_argument("Invalid BIT STRING: unused bits exceed 7");
        }
        _bit_length = (_data.size() - 1) * 8 - unused_bits;
        _value.assign(_data.begin() + 1, _data.end());
        // _bit_length = _data.size() * 8 - _data[0];
        // _value.assign(_data.begin() + 1, _data.end());
    }

    asncpp::base::dynamic_array_t encode() final {
        _data.clear();
        _data.push_back(static_cast<uint8_t>((8 - (_bit_length % 8)) % 8)); // Неиспользуемые биты.
        _data.insert(_data.end(), _value.begin(), _value.end());
        // _data.insert(_data.begin(), static_cast<uint8_t>(_bit_length % 8));
        // _data.append_range( _value);
        return _data;
    }

    [[nodiscard]] std::string to_string() const override {
        return "BIT STRING: "
        // + std::accumulate(_value.begin(), _value.end(), std::string{},
        //                                         [](std::string& acc, uint8_t byte) {
        //                                             for (size_t i = 0; i < 8; ++i) {
        //                                                 acc += ((byte & 0x80) == 0x80) + '0';
        //                                                 byte <<= 1;
        //                                             }
        //                                             return acc;
        //                                         })
        ;
    }


private:
    std::vector<uint8_t> _value;
    size_t _bit_length = 0;
};

// bit_string_t(uint64_t data, size_t bit_length) : _bit_length{bit_length} {
//     if (bit_length > 64) {
//         throw std::invalid_argument("Bit length exceeds 64 bits");
//     }
//     for (size_t i = 0; i < bit_length; ++i) {
//         uint8_t byte{0};
//         for (size_t j = 0; j < (bit_length - i >= 8 ? 8 : bit_length - i); ++j) {
//             byte <<= 1U;
//             byte |= (data & 1U);
//             data >>= 1U;
//         }
//         _value.push_back(byte);
//     }
// }


//  explicit bit_string_t(const std::vector<bool> &data) : _value{data} {
//  }
//
//
//  template<std::integral T>
//  explicit bit_string_t(const std::vector<T> &data) {
//      for (T value: data) {
//          const auto bits_view = std::bitset<sizeof(T)>(value) |
//                                 std::ranges::views::take_while([](bool bit) { return bit; });
//          for (const bool bit: bits_view) {
//              _value.push_back(bit);
//          }
//      }
//  }
//ОТДЕЛЬНЫЙ БАЗОВЫЙ АБСТРАКТНЫЙ КЛАСС ДЛЯ КОНСТРУКЦИОНЫЫХ ТИПОВЫХ КЛАССОВ
//
//class bit_string_t : public asn1_type<std::vector<uint8_t>, static_cast<uintmax_t>(asn1_tag::BIT_STRING)> {
//public:
//    bit_string_t() = default;
//
////    explicit bit_string_t(const std::vector<uint8_t> &data) {
////        this->_value = data;
////    }
////
////    template <size_t N>
////    explicit bit_string_t(const std::bitset<N>& data) {
////        this->_value = data;
////    }
////
////    explicit bit_string_t(std::string_view data) {
////        this->_value = std::vector<uint8_t>(data.begin(), data.end());
////    }
////
////    explicit bit_string_t(const std::vector<uint8_t> &data, size_t unused_bits) {
////        this->_value = data;
////        this->_value.insert(this->_value.begin(), static_cast<uint8_t>(unused_bits));
////    }
////
////    explicit bit_string_t(const std::vector<uint8_t> &data, size_t unused_bits, size_t length) {
////        this->_value = data;
////        this->_value.insert(this->_value.begin(), static_cast<uint8_t>(unused_bits));
////        this->_value.insert(this->_value.begin(), static_cast<uint8_t>(length));
////    }
////
////    explicit bit_string_t(const std::vector<uint8_t> &data, size_t unused_bits, size_t length, bool is_last_byte) {
////        this->_value = data;
////        this->_value.insert(this->_value.begin(), static_cast<uint8_t>(unused_bits));
////        this->_value.insert(this->_value.begin(), static_cast<uint8_t>(length));
////        this->_value.insert(this->_value.begin(), static_cast<uint8_t>(is_last_byte));
////    }
////    void encode(uint64_t data) {
//////    bit_string_t(uint64_t data, size_t unused_bits) {
////        size_t length = 0;
////        while (data) {
////            this->_value.insert(this->_value.begin(), static_cast<uint8_t>(data & 0xFF));
////            data >>= 8;
////        }
////        uint8_t last_byte = this->_value[0];
////        while (last_byte) {
////            last_byte >>= 1;
////            ++length;
////        }
////        this->_value.insert(this->_value.begin(), static_cast<uint8_t>(8 - length));
////        this->_value.insert(this->_value.begin(), static_cast<uint8_t>(this->_value.size()));
////        this->_value.insert(this->_value.begin(), static_cast<uint8_t>(asn1_tag::BIT_STRING));
//////        this->_value = std::vector<uint8_t>(sizeof(data));
//////        for (size_t i = 0; i < sizeof(data); ++i) {
//////            this->_value[i] = (data >> (8 * i)) & 0xFF;
//////        }
//////        this->_value.insert(this->_value.begin(), static_cast<uint8_t>(unused_bits));
////    }
//
//    bit_string_t(uint64_t data, size_t payload_bits) {
//        _value.clear();
//        while (data) {
//            this->_value.insert(this->_value.begin(), static_cast<uint8_t>(data & 0xFF));
//            data >>= 8;
//        }
//        this->_value.insert(this->_value.begin(), static_cast<uint8_t>(8 - (payload_bits % 8)));
//    }
//
//    void decode(const uint8_t *buffer) final {
////        const asn1_base raw = asn1_base({buffer, 0xFFFFFFFF});
////
////        if (raw.get_length() < 1) {
////            throw std::runtime_error("Invalid length for BIT STRING value");
////        }
////
////        this->_value.clear();
////        this->_value.insert(this->_value.end(), raw.get_data(), raw.get_data() + raw.get_length());
//    }
//
//    [[nodiscard]] std::vector<uint8_t> encode() const final {
//        std::vector<uint8_t> result;
//        result.insert(result.end(), this->_value.begin(), this->_value.end());
//        result.insert(result.begin(), result.size());
//        result.insert(result.begin(), static_cast<uint8_t>(asn1_tag::BIT_STRING));
//        return result;
//    }
//    //TODO: хранить число фактически занимаемых битов
//    [[nodiscard]] std::string to_string() const override {
//        std::string result = "BIT STRING: ";
//        for (size_t i = 1; i < this->_value.size(); ++i) {
//            uint8_t value = this->_value[i];
//            for (size_t j = 0; j < 8; ++j) {
//                result += ((value & 0x80) == 0x80) + '0';
//                value <<= 1;
//            }
//        }
//        return result;
//    }
//};

#endif //ASNCPP_BIT_STRING_H

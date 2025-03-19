/**
* @file asn1_basic.cpp
 * @brief Definition of the `asn1_basic` class.
 * @author Nikita Kanduba
 * @date 17.12.2024
 */

#include "asncpp/base/asn1_basic.h"

#include <bit>
#include <ranges>
#include <stdexcept>

namespace views = std::ranges::views;


asncpp::base::asn1_basic::asn1_basic(const std::span<const uint8_t> data) {
    //Вызов виртуального метода decode для разбора данных в данном участке безопасен,
    //так как он должен вызывать реализацию именно этого класса, а не дочернего
    asn1_basic::decode(data);
}


void asncpp::base::asn1_basic::append_child(std::shared_ptr<asn1_basic> child) {
    if (!constructed()) {
        throw std::runtime_error("Cannot append child to a non-constructed object");
    }
    _children.emplace_back(std::move(child));
}

std::shared_ptr<asncpp::base::asn1_basic> asncpp::base::asn1_basic::get_child(const size_t index) const {
    if (!constructed()) {
        throw std::runtime_error("Cannot get child from a non-constructed object");
    }
    return _children.at(index);
}

bool asncpp::base::asn1_basic::is_have_children() const {
    return number_of_children() != 0;
}

size_t asncpp::base::asn1_basic::number_of_children() const {
    return _children.size();
}

void asncpp::base::asn1_basic::decode(std::span<const uint8_t> data) {
    if (data.size_bytes() < 2) {
        throw std::invalid_argument("Invalid ASN.1 data. Length is too short");
    }

    //Класс  и конструктивность всегда находится в первом байте
    _cls = extract_class(data[0]);
    _constructed = extract_is_constructed(data[0]);
    //Получение типа, длинны  и срез массива сырых данных. Тип/тег и длинна может занимать более одного байта
    const auto [type, type_length]{extract_type(data)};
    _type = type;
    data = data.subspan(type_length);
    const auto [length, length_length]{extract_length(data)};
    data = data.subspan(length_length);
    _length = length;

    //Проверка на то, что длинна не превышает фактический размер буфера
    if (std::distance(data.cbegin() + static_cast<std::ptrdiff_t>(length), data.end()) < 0) {
        throw std::invalid_argument("Invalid ASN.1 data. Length exceeds buffer size");
    }
    _raw_length = type_length + length_length + length;
    //Сохранение данных в буфере без тега и длинны
    _data.assign(data.cbegin(), data.cbegin() + static_cast<std::ptrdiff_t>(length));
}


asncpp::base::dynamic_array_t asncpp::base::asn1_basic::encode() {
    dynamic_array_t output{encode_type()};
    output.append_range(encode_length(_data.size()));
    output.append_range(_data);
    return output;
}

std::string asncpp::base::asn1_basic::to_string() const {
    return "ASN.1 basic";
}


void asncpp::base::asn1_basic::truncate_data(const size_t length) {
    if (length > _data.size()) {
        throw std::invalid_argument("Length exceeds buffer size");
    }
    //Перемещение незадействованных данных в начало буфера
    std::move(_data.begin() + static_cast<std::ptrdiff_t>(length), _data.end(), _data.begin());
    //Уменьшение размера буфера c учётом обрезанных данных
    _data.resize(_data.size() - length);
}

std::pair<asncpp::base::tag_t, size_t> asncpp::base::asn1_basic::extract_type(std::span<const uint8_t> buffer) {
    if (buffer.empty()) {
        throw std::runtime_error("Data is too short for the long encoding");
    }
    size_t count{};
    //Получение тега. Если тег меньше 0x1F, то он занимает один байт
    const uint8_t tag_decoded = buffer[count++] & 0x1FU;
    if (tag_decoded < 0x1FU) {
        return std::make_pair(static_cast<asn1_tag>(tag_decoded), 1ULL);
    }

    if (buffer.size_bytes() < 2) {
        throw std::runtime_error("Data is too short for the long encoding");
    }

    uintmax_t type_decoded{};
    //Генератор для получения байтов типа. Пока байт имеет 0x80 в старшем бите, продолжаем считывать
    const auto type_view{
        buffer | views::drop(count) | views::take_while(
            [](const uint8_t byte) { return byte & 0x80U; })
    };


    for (const uint8_t byte: type_view) {
        //Добавление 7 младших битов к числу(тегу)
        type_decoded = (type_decoded << 7U) | (byte & 0x7FU);
        count++;
    }

    if (count == 0) {
        throw std::runtime_error("Data is too short for the long encoding");
    }
    if (count == buffer.size_bytes()) {
        throw std::runtime_error("Data is too short for the long encoding");
    }
    //Добавление последнего байта к числу(тегу). Старший бит равен 0, так как это финальный байт
    type_decoded = (type_decoded << 7U) | (buffer[count++] & 0x7FU);

    //Проверка на допустимость тега. Если тег больше 0x22, то он расширенный
    if (type_decoded <= 0x22U) {
        return std::make_pair(static_cast<asn1_tag>(type_decoded), count);
    }
    //Если тег больше 0x22, то он расширенный и возвращается в виде сырого числа
    return std::make_pair(type_decoded, count);
}


asncpp::base::dynamic_array_t asncpp::base::asn1_basic::encode_length(size_t length) {
    if (length <= 127) {
        return {static_cast<uint8_t>(length)};
    }
    std::vector<uint8_t> output;
    while (length > 0) {
        output.emplace(output.begin(), static_cast<uint8_t>(length & 0xFFU));
        length >>= 8U;
    }
    output.emplace(output.begin(), static_cast<uint8_t>(output.size() | 0x80U));
    return output;
}


asncpp::base::dynamic_array_t asncpp::base::asn1_basic::encode_type() const {
    uintmax_t raw_type{0};
    std::visit([&](auto &&arg) {
                   using T = std::decay_t<decltype(arg)>;
                   if constexpr (std::is_same_v<T, std::monostate>) {
                       raw_type = get_tag();
                   } else if constexpr (std::is_same_v<T, asn1_tag>) {
                       raw_type = static_cast<uintmax_t>(arg);
                   } else if constexpr (std::is_same_v<T, uintmax_t>) {
                       raw_type = arg;
                   }
               },
               _type);
    const uint8_t base = (static_cast<uint8_t>(get_class()) << 6U) |
                         (static_cast<uint8_t>(constructed()) << 5U);

    if (raw_type == 0) {
        throw std::runtime_error("Tag is not set");
    }
    if (raw_type < 0x1FU) {
        return {static_cast<unsigned char>(base | static_cast<uint8_t>(raw_type))};
    }

    std::vector<uint8_t> result;
    do {
        result.insert(result.begin(), static_cast<uint8_t>(raw_type & 0x7FU));
        raw_type >>= 7U;
    } while (raw_type > 0);
    result.insert(result.begin(), static_cast<uint8_t>(base | static_cast<uint8_t>(0x1FU)));

    // const auto out = result | views::drop(1) |
    //     views::take(result.size() - 2) |
    //         views::transform([](uint8_t byte) { return byte | 0x80U; });
    // return std::ranges::to<std::vector<uint8_t>>(out);
    std::transform(std::next(result.cbegin()),
                   std::prev(result.cend()),
                   std::next(result.begin()),
                   [](const uint8_t byte) { return byte | 0x80U; });
    return result;
}


std::pair<size_t, size_t> asncpp::base::asn1_basic::extract_length(const std::span<const uint8_t> buffer) {
    if (buffer.empty()) {
        throw std::runtime_error("Buffer is empty");
    }

    // Если длина закодирована в одном байте
    if (!(buffer[0] & 0x80U)) {
        return std::make_pair(buffer[0], 1ULL);
    }

    // Определяем количество байт, использованных для длины
    size_t length{0};
    const uint8_t num_octets = buffer[0] & 0x7FU;

    // Проверка на слишком большую длину
    if (num_octets > sizeof(size_t)) {
        throw std::runtime_error("Length is too long for this platform");
    }

    // Проверка на доступность байт для длины
    if (num_octets > buffer.size_bytes() - 1) {
        throw std::runtime_error("Data is too short for the specified length encoding");
    }

    // Проверяем, что длина не равна нулю
    if (num_octets == 0) {
        throw std::runtime_error("Length encoding specifies zero octets");
    }

    // Собираем длину из октетов
    for (size_t i = 1; i <= num_octets; ++i) {
        length = (length << 8U) | buffer[i];
    }

    // Проверяем, что длина больше нуля (дополнительная проверка)
    if (length == 0) {
        throw std::runtime_error("Length cannot be zero");
    }

    return std::make_pair(length, num_octets + 1);
}

constexpr bool asncpp::base::asn1_basic::constructed() const noexcept {
    return _constructed || is_have_children();
}

constexpr asncpp::base::asn1_class asncpp::base::asn1_basic::get_class() const noexcept {
    return _cls;
}

constexpr uintmax_t asncpp::base::asn1_basic::get_tag() const noexcept {
    return static_cast<uintmax_t>(asn1_tag::Reserved);
}

constexpr bool asncpp::base::asn1_basic::extract_is_constructed(const uint8_t tag) noexcept {
    return (tag & 0x20U) >> 5U;
}

constexpr asncpp::base::asn1_class asncpp::base::asn1_basic::extract_class(const uint8_t tag) noexcept {
    return static_cast<asn1_class>((tag & 0xC0U) >> 6U);
}

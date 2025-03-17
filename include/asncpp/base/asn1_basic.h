//
// Created by kandu on 17.12.2024.
//

#ifndef ASN1_BASIC_H
#define ASN1_BASIC_H

#include <stdexcept>
#include <string>
#include "common.h"

/**
 * @brief Forward declarations for test classes.
 * These declarations allow specific test cases to access private members of the `asn1_basic` class.
 */
class TestASN1Basic;
class asn_basic_test_encode_short_tag_Test;
class asn_basic_test_encode_long_tag_Test;
class asn_basic_test_encode_short_length_Test;
class asn_basic_test_encode_long_length_Test;
class asn_basic_test_decode_short_length_Test;
class asn_basic_test_decode_long_length_Test;
class asn_basic_test_decode_invalid_length_Test;
class asn_basic_test_encode_type_invalid_state_Test;
class asn_basic_test_encode_invalid_tag_Test;
class common_test_deserialize_Test;

namespace asncpp::base {

    class asn1_basic {
        //TODO: Переосмыслить инкапсуляцию, чтобы не было необходимости в дружественных классах и чтобы обезопасить доступ к членам класса
        //TODO: Навести порядок с указателями, заменить на shared_ptr
    public:
        /** @brief Конструктор TLV-объекта ASN.1 из буфера данных.
         *  @details Конструктор декодирует объект ASN.1 из буфера данных, разделяя его на тег, длину и данные.
         *  При этом конструктор не разбирает данные, оставляя их в закодированном виде, и не создаёт дочерних объектов.
         *  Для этого используется функция deserialize_v.
         *  Сам конструктор вызывает функцию decode, которая разбирает данные объекта.
         * @param[in] data Буфер данных, содержащий закодированный объект ASN.1.
         * @throws std::invalid_argument Если буфер не содержит корректных данных ASN.1.
         */
        asn1_basic(std::span<const uint8_t> data) {
            //TODO: Is secure to use virtual of base class in base class method in constructor?
            asn1_basic::decode(data);
        }

        /**
         * @brief Метод для получения информации о конструктивности объекта ASN.1.
         * @return Конструктивный ли объект ASN.1.
         * @retval true Объект ASN.1 конструктивный или составной (содержит дочерние объекты).
         * @retval false Объект ASN.1 примитивный.
         */
        [[nodiscard]] constexpr bool constructed() const noexcept {
            //FIXME: Переделать на проверку по тегу. Для типов-коллекций результат неверен. Они конструкционные по определнию.
            return _constructed || is_have_children();
        }

        /**
         * @brief Метод для получения класса тега ASN.1.
         * @return Класс тега ASN.1.
         * @retval asn1_class::UNIVERSAL Универсальный класс тега.
         * @retval asn1_class::APPLICATION Прикладной класс тега.
         * @retval asn1_class::CONTEXT_SPECIFIC Контекстно-специфический класс тега.
         * @retval asn1_class::PRIVATE Частный класс тега.
         */
        [[nodiscard]] constexpr asn1_class get_class() const noexcept {
            return _cls;
        }

        /**
         * @brief Метод для получения длины закодированных данных объекта ASN.1.
         * @return Длина закодированных данных уже без учёта тега и длины.
         */
        [[nodiscard]] constexpr size_t length() const noexcept {
            return _length;
        }

        /**
         * @brief Метод для получения данных объекта ASN.1.
         * @return Константная ссылка на буфер данных объекта ASN.1. Уже без тега и длины.
         */
        [[nodiscard]] constexpr const dynamic_array_t &data() const noexcept {
            return _data;
        }

        /**
         * @brief Виртуальный метод для добавления дочернего объекта в составной объект ASN.1.
         * @details Метод виртуальный, так как каждый составной объект ASN.1 может иметь свои правила добавления дочерних объектов.
         * @param[in] child Умный указатель на дочерний объект ASN.1.
         */
        virtual void append_child(std::unique_ptr<asn1_basic> child) {
            //TODO: Переделать на shared_ptr?
            //TODO: Кинуть  исключение, если объект не конструктивный
            _children.emplace_back(std::move(child));
        }


        /**
         * @brief Метод для получения дочернего объекта по индексу.
         * @param[in] index Индекс дочернего объекта.
         * @return Указатель на дочерний объект ASN.1.
         * @throws std::out_of_range Если индекс выходит за пределы массива дочерних объектов.
         */
        [[nodiscard]] auto get_child(size_t index) const {
            //TODO: Переделать на shared_ptr?
            //TODO: Может, стоит вернуть константную ссылку?
            //TODO: Кинуть исключение, если индекс выходит за пределы массива?
            //TODO: Кинуть исключение, если объект не конструктивный?
            return _children.at(index).get();
        }

        /**
         * @brief Метод возвращает значение тега объекта ASN.1.
         * @details Метод виртуальный, так как каждый объект ASN.1 может иметь свой тег. Каждый дочерний метод должен в обязательном порядке переопределить этот метод.
         * @return Значение тега объекта ASN.1 в виде числа.
         * Какждый тип объекта ASN.1 имеет свой уникальный тег и возвращать его как обычное число, это важно для корректной сериализации и десериализации.
         */
        [[nodiscard]] constexpr virtual uintmax_t get_tag() const noexcept {
            return static_cast<uintmax_t>(asn1_tag::Reserved);
        }

        //TODO: Инкапсулировать данную переменную от дочерних класов?
        dynamic_array_t _data; /**< Внутренний буффер для хранения данных в сыром виде. Только данные. */
        //TODO: Инкапсулировать данную переменную от дочерних класов?
        std::vector<std::unique_ptr<asn1_basic> > _children; /**< Массив дочерних объектов для конструкционных типов или сотавных верий типов. */

        asn1_basic() noexcept = default;
        asn1_basic(asn1_basic &&) = default;
        asn1_basic(const asn1_basic &) = default;
        asn1_basic &operator=(asn1_basic &&) = default;
        asn1_basic &operator=(const asn1_basic &) = default;
        virtual ~asn1_basic() = default;

        // template<class T>
        // T *get() {
        //     uintmax_t raw_type{0};
        //     std::visit([&](auto &&arg) {
        //                    using T = std::decay_t<decltype(arg)>;
        //                    if constexpr (std::is_same_v<T, std::monostate>) {
        //                        raw_type = get_tag();
        //                    } else if constexpr (std::is_same_v<T, asn1_tag>) {
        //                        raw_type = static_cast<uintmax_t>(arg);
        //                    } else if constexpr (std::is_same_v<T, uintmax_t>) {
        //                        raw_type = arg;
        //                    }
        //                },
        //                _type);
        //     if (T().get_tag() != raw_type)
        //         return nullptr;
        //     return static_cast<T *>(this);
        // }
    protected:
        /**
         * @brief Decodes an ASN.1 object from a byte buffer.
         * @param data The buffer containing ASN.1 encoded data.
         * @throws std::invalid_argument If the buffer does not contain valid ASN.1 data.
         */
        virtual void decode(std::span<const uint8_t> data);

        /**
         * @brief Encodes the ASN.1 object into a byte buffer.
         * @return A byte array containing the encoded data.
         */
        virtual dynamic_array_t encode();

        /**
         * @brief Converts the ASN.1 object to a string representation.
         * @return A string describing the ASN.1 object.
         */
        [[nodiscard, maybe_unused]] virtual std::string to_string() const {
            return "ASN.1 basic";
        }


        [[nodiscard]] bool is_have_children() const {
            return !_children.empty();
        }

        [[nodiscard]] size_t children_count() const {
            return _children.size();
        }

        void truncate_data(size_t length) {
            // _data.erase(_data.begin(), _data.begin() + length);
            //size_t const n = 3;
            if (length > _data.size()) {
                throw std::invalid_argument("Length exceeds buffer size");
            }


            std::move(_data.begin() + length, _data.end(), _data.begin());
            _data.resize(_data.size() - length);
            //}
        }

        void append_data(std::span<const uint8_t> data) {
            _data.insert(_data.end(), data.begin(), data.end());
        }

    private:
        bool _constructed{}; /**< Флаг конструктивности объекта ASN.1. */
        asn1_class _cls{}; /**< Класс объекта ASN.1. */
        size_t _length{}; /**< Длинна исключительно данных */
        size_t _raw_length{}; /**< Длинна полного, сырого TLV-пакета */
        tag_t _type; /**< Тип объекта ASN.1. Может быть как стандартным, так и пользовательским. Значение по умолчанию - std::monostate */

        /**
         * @brief Сериализует объект ASN.1 в байтовый массив.
         * @details Дружественная функция, для доступа к закрытым членам класса.
         * @param[in, out] block Объект ASN.1 для сериализации. Сырой блок данны c базовым классом @ref asn1_basic.
         * @return Байтовый массив, представляющий сериализованный объект ASN.1. Полный TLV-блок.
         */
        friend dynamic_array_t serialize(asn1_basic* block);

        /**
         * @brief Десериализует байтовый поток в объект ASN.1.
         * @details Дружественная функция, для доступа к закрытым членам класса.
         * @param[in] data Байтовый поток для десериализации.
         * @return Указатель на десериализованный объект ASN.1.
         * @throws std::runtime_error Если данные не могут быть разобраны.
         */
        friend std::unique_ptr<asn1_basic> deserialize_v(std::span<const uint8_t> data);


        /**
 * @brief Extracts the tag type from the given data buffer.
 * @param buffer The buffer containing ASN.1 encoded data.
 * @return A pair containing the tag type and the number of bytes used by the tag.
 * @throws std::runtime_error If the tag type cannot be determined from the buffer.
 */
        static std::pair<tag_t, size_t> extract_type(std::span<const uint8_t> buffer);

        /**
         * @brief Encodes the tag value of the ASN.1 object.
         * @return A byte array representing the encoded tag.
         */
        [[nodiscard]] dynamic_array_t encode_type() const;

        /**
         * @brief Encodes the length value of the ASN.1 object.
         * @param length The length to encode.
         * @return A byte array representing the encoded length.
         */
        [[nodiscard]] static dynamic_array_t encode_length(size_t length);

        /**
         * @brief Extracts the length value from a byte buffer.
         * @param buffer The buffer containing ASN.1 encoded data.
         * @return A pair containing the length value and the number of bytes used by the length.
         * @throws std::runtime_error If the length cannot be determined from the buffer.
         */
        [[nodiscard]] static std::pair<size_t, size_t> extract_length(std::span<const uint8_t> buffer);

        /**
         * @brief Determines whether the ASN.1 object is constructed.
         * @param tag The tag byte.
         * @return `true` if the object is constructed, `false` otherwise.
         */
        [[nodiscard]] constexpr static bool extract_is_constructed(uint8_t tag) noexcept;

        /**
         * @brief Extracts the class of the ASN.1 object.
         * @param tag The tag byte.
         * @return The class of the ASN.1 object.
         */
        [[nodiscard]] constexpr static asn1_class extract_class(uint8_t tag) noexcept;

        // Test friend declarations
        friend class TestASN1Basic;
        friend class asn_basic_test_encode_short_tag_Test;
        friend class asn_basic_test_encode_long_tag_Test;
        friend class asn_basic_test_encode_short_length_Test;
        friend class asn_basic_test_encode_long_length_Test;
        friend class asn_basic_test_decode_short_length_Test;
        friend class asn_basic_test_decode_long_length_Test;
        friend class asn_basic_test_decode_invalid_length_Test;
        friend class asn_basic_test_encode_type_invalid_state_Test;
        friend class asn_basic_test_encode_invalid_tag_Test;
        friend class common_test_deserialize_Test;
    };
} // namespace asncpp::base

#endif //ASN1_BASIC_H

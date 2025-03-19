/**
* @file asn1_basic.h
 * @brief Declaration of the `asn1_basic` class.
 * @author Nikita Kanduba
 * @date 17.12.2024
 */
#ifndef ASN1_BASIC_H
#define ASN1_BASIC_H

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
    public:
        /** @brief Конструктор TLV-объекта ASN.1 из буфера данных.
         *  @details Конструктор декодирует объект ASN.1 из буфера данных, разделяя его на тег, длину и данные.
         *  При этом конструктор не разбирает данные, оставляя их в закодированном виде, и не создаёт дочерних объектов.
         *  Для этого используется функция deserialize_v.
         *  Сам конструктор вызывает функцию decode, которая разбирает данные объекта.
         * @param[in] data Буфер данных, содержащий закодированный объект ASN.1.
         * @throws std::invalid_argument Если буфер не содержит корректных данных ASN.1.
         */
        asn1_basic(std::span<const uint8_t> data);

        /**
         * @brief Метод для получения информации о конструктивности объекта ASN.1.
         * @return Конструктивный ли объект ASN.1.
         * @retval true Объект ASN.1 конструктивный или составной (содержит дочерние объекты).
         * @retval false Объект ASN.1 примитивный.
         * @bug Метод вероятно дает ложный результат для типов-коллекций, особенно если они пустые.
         */
        [[nodiscard]] constexpr bool constructed() const noexcept;

        /**
         * @brief Метод для получения класса тега ASN.1.
         * @return Класс тега ASN.1.
         * @retval asn1_class::UNIVERSAL Универсальный класс тега.
         * @retval asn1_class::APPLICATION Прикладной класс тега.
         * @retval asn1_class::CONTEXT_SPECIFIC Контекстно-специфический класс тега.
         * @retval asn1_class::PRIVATE Частный класс тега.
         */
        [[nodiscard]] constexpr asn1_class get_class() const noexcept;



        /**
         * @brief Виртуальный метод для добавления дочернего объекта в составной объект ASN.1.
         * @details Метод виртуальный, так как каждый составной объект ASN.1 может иметь свои правила добавления дочерних объектов.
         * Дочерний объект должен быть создан заранее и передан в метод в виде умного указателя @ref std::shared_ptr.
         * @throws std::runtime_error Если объект не конструктивный. Объекты, не являющиеся конструктивными, не могут содержать дочерние объекты.
         * @param[in] child Умный указатель на дочерний объект ASN.1.
         * @par Returns
         *  None.
         */
        virtual void append_child(std::shared_ptr<asn1_basic> child);

        /**
         * @brief Метод для получения дочернего объекта по индексу.
         * @param[in] index Индекс дочернего объекта.
         * @return Указатель на дочерний объект ASN.1.
         * @throws std::out_of_range Если индекс выходит за пределы массива дочерних объектов.
         * @throws std::runtime_error Если объект не конструктивный. Объекты, не являющиеся конструктивными, не могут содержать дочерние объекты.
         */
        [[nodiscard]] std::shared_ptr<asn1_basic> get_child(const size_t index) const;

        /**
         * @brief Метод возвращает значение тега объекта ASN.1.
         * @details Метод виртуальный, так как каждый объект ASN.1 может иметь свой тег. Каждый дочерний метод должен в обязательном порядке переопределить этот метод.
         * @return Значение тега объекта ASN.1 в виде числа.
         * Каждый тип объекта ASN.1 имеет свой уникальный тег и возвращать его как обычное число, это важно для корректной сериализации и десериализации.
         */
        [[nodiscard]] constexpr virtual uintmax_t get_tag() const noexcept;

        /**
         * @brief Метод для проверки наличия дочерних объектов.
         * @return Наличие дочерних объектов.
         * @retval true Объект ASN.1 имеет дочерние объекты.
         * @retval false Объект ASN.1 не имеет дочерних объектов.
         */
        [[nodiscard]] bool is_have_children() const;

        /**
         * @brief Метод для получения количества дочерних объектов.
         * @return Количество дочерних объектов.
         */
        [[nodiscard]] size_t number_of_children() const;


        /**
 * @brief Преобразует объект ASN.1 в строку.
 * @details Метод виртуальный, так как каждый объект ASN.1 может иметь своё представление в виде строки.
 * Рекомендуется в каждом классе-наследнике переопределить этот метод для корректного отображения объекта.
 * @return A строковое представление объекта ASN.1.
 */
        [[nodiscard, maybe_unused]] virtual std::string to_string() const;

        asn1_basic() noexcept = default;

        asn1_basic(asn1_basic &&) = default;

        asn1_basic(const asn1_basic &) = default;

        asn1_basic &operator=(asn1_basic &&) = default;

        asn1_basic &operator=(const asn1_basic &) = default;

        virtual ~asn1_basic() = default;


    protected:
        /**
         * @brief Разбирает байтовый поток в объект ASN.1.
         * @details Метод виртуальный, так как каждый объект ASN.1 может иметь свои правила декодирования.
         * Его переопределение обязательно для каждого класса-наследника.
         * Подразумевается, что переопределённый метод будет разбирать данные из буфера и заполнять внутренние поля объекта.
         * Крайне не рекомендуется его использовать без необходимости или в обход функций сериализации и десериализации.
         * @param[in] data The buffer containing ASN.1 encoded data.
         * @throws std::invalid_argument If the buffer does not contain valid ASN.1 data.
         */
        virtual void decode(std::span<const uint8_t> data);

        /**
         * @brief Сериализует объект ASN.1 в байтовый массив.
         * @details Метод виртуальный, так как каждый объект ASN.1 может иметь свои правила сериализации.
         * Его переопределение обязательно для каждого класса-наследника. Переопределенный метод кодировать данные специфическим образом
         * для каждого типа объекта ASN.1. и заполнять внутренние поля объекта.
         * Крайне не рекомендуется его использовать без необходимости или в обход функций сериализации и десериализации.
         * @return Байтовый массив, представляющий сериализованный объект ASN.1. Полный TLV-блок.
         */
        virtual dynamic_array_t encode();


        dynamic_array_t _data; /**< Внутренний буфер для хранения данных в сыром виде. Только данные. */
    private:
        bool _constructed{}; /**< Флаг конструктивности объекта ASN.1. */
        asn1_class _cls{}; /**< Класс объекта ASN.1. */
        size_t _length{}; /**< Длинна исключительно данных */
        size_t _raw_length{}; /**< Длинна полного, сырого TLV-пакета */
        tag_t _type; /**< Тип объекта ASN.1. Может быть как стандартным, так и пользовательским. Значение по умолчанию - std::monostate */
        std::vector<std::shared_ptr<asn1_basic> > _children; /**< Массив дочерних объектов для конструкционных типов или составных вариаций типов. */


        /**
         * @brief Применяется для вложенных или конструкционных типов. Используется исключительно с deserialize_v.
        * @details Нужно для того, чтобы из родительского объекта удалять данные, скопированные во вложенные объекты. Вызывается до тех пор, пока все объекты не будут извлечены.
         * @param[in] length Длина данных, которые необходимо удалить из исходного массива сырых данных.
         * @throws std::invalid_argument Если длина превышает размер буфера.
         * @par Returns
         *  None.
         */
        void truncate_data(const size_t length);


        /**
         * @brief Сериализует объект ASN.1 в байтовый массив.
         * @details Дружественная функция, для доступа к закрытым членам класса.
         * @param[in, out] block Объект ASN.1 для сериализации. Сырой блок данных c базовым классом @ref asn1_basic.
         * @return Байтовый массив, представляющий сериализованный объект ASN.1. Полный TLV-блок.
         */
        friend dynamic_array_t serialize(asn1_basic *block);

        /**
         * @brief Десериализует байтовый поток в объект ASN.1.
         * @details Дружественная функция, для доступа к закрытым членам класса.
         * @param[in] data Байтовый поток для десериализации.
         * @return Указатель на десериализованный объект ASN.1.
         * @throws std::runtime_error Если данные не могут быть разобраны.
         */
        friend std::shared_ptr<asn1_basic> deserialize_v(std::span<const uint8_t> data);


        /**
         * @brief Extracts the tag value from a byte buffer.
         * @param[in] buffer The buffer containing ASN.1 encoded data.
         * @return A pair containing the tag value and the number of bytes used by the tag.
         * @throws std::runtime_error If the tag cannot be determined from the buffer.
         */
        [[nodiscard]] static std::pair<tag_t, size_t> extract_type(std::span<const uint8_t> buffer);

        /**
         * @brief Encodes the tag value of the ASN.1 object.
         * @return A byte array representing the encoded tag.
         */
        [[nodiscard]] dynamic_array_t encode_type() const;

        /**
         * @brief Encodes the length value of the ASN.1 object.
         * @param[in] length The length to encode.
         * @return A byte array representing the encoded length.
         */
        [[nodiscard]] static dynamic_array_t encode_length(size_t length);

        /**
         * @brief Extracts the length value from a byte buffer.
         * @param[in] buffer The buffer containing ASN.1 encoded data.
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

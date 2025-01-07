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
    /**
     * @class asn1_basic
     * @brief Base class for ASN.1 data representation and manipulation.
     *
     * This class provides the core functionality for encoding, decoding, and managing
     * ASN.1 data types. Derived classes should implement the `get_tag` method to define
     * the specific tag associated with their ASN.1 type.
     */
    //TODO: Переосмысдить инкапсуляцию, чтобы не было необходимости в дружественных классах и чтобы обезопасить достуап к членам класса
    //TODO: Навести порядок с указателями, заменить на shared_ptr
    class asn1_basic {
    public:
        /**
         * @brief Default constructor.
         */
        asn1_basic() noexcept = default;

        /**
         * @brief Move constructor.
         */
        asn1_basic(asn1_basic &&) = default;

        /**
         * @brief Copy constructor.
         */
        asn1_basic(const asn1_basic &) = default;

        /**
         * @brief Move assignment operator.
         */
        asn1_basic &operator=(asn1_basic &&) = default;

        /**
         * @brief Copy assignment operator.
         */
        asn1_basic &operator=(const asn1_basic &) = default;

        /**
         * @brief Virtual destructor.
         */
        virtual ~asn1_basic() = default;

        /**
         * @brief Constructor to initialize an `asn1_basic` object from a data buffer.
         * @param data The buffer containing ASN.1 encoded data.
         * @throws std::invalid_argument If the buffer does not contain valid ASN.1 data.
         */
        asn1_basic(std::span<const uint8_t> data) {
            // NOLINT(*-explicit-constructor)
            //TODO: Is secure to use virtual of base class in base class method in constructor?
            asn1_basic::decode(data);
        }

        /**
         * @brief Checks whether the ASN.1 object is constructed or primitive.
         * @return `true` if the object is constructed, `false` otherwise.
         */
        constexpr bool is_constructed() const noexcept {
            return _constructed || is_have_children();
        }

        /**
         * @brief Retrieves the class of the ASN.1 tag.
         * @return The class of the ASN.1 tag (`UNIVERSAL`, `APPLICATION`, etc.).
         */
        constexpr asn1_class get_cls() const noexcept {
            return _cls;
        }

        /**
         * @brief Retrieves the length of the ASN.1 data.
         * @return The length of the encoded data.
         */
        constexpr size_t get_length() const noexcept {
            return _length;
        }

        /**
         * @brief Retrieves the raw data of the ASN.1 object.
         * @return A reference to the internal dynamic array containing the raw data.
         */
        constexpr const dynamic_array_t &get_data() const noexcept {
            return _data;
        }

        /**
         * @brief Extracts the tag type from the given data buffer.
         * @param buffer The buffer containing ASN.1 encoded data.
         * @return A pair containing the tag type and the number of bytes used by the tag.
         * @throws std::runtime_error If the tag type cannot be determined from the buffer.
         */
        static std::pair<tag_t, size_t> extract_type(std::span<const uint8_t> buffer);

        /**
         * @brief Serializes the ASN.1 object into a byte array.
         * @param block The ASN.1 object to serialize.
         * @return A byte array representing the serialized object.
         */
        friend dynamic_array_t serialize(asn1_basic *block);

        /**
         * @brief Deserializes a byte array into an ASN.1 object.
         * @param data The byte array to deserialize.
         * @return A unique pointer to the deserialized object.
         */
        friend std::unique_ptr<asn1_basic> deserialize_v(std::span<const uint8_t> data);

        void append_child(std::unique_ptr<asn1_basic> child) {
            _children.emplace_back(std::move(child));
        }

        const auto get_children(size_t index) const {
            return _children.at(index).get();
        }

        template<class T>
        T *get() {
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
            if (T().get_tag() != raw_type)
                return nullptr;
            return static_cast<T *>(this);
        }

    protected:
        /**
         * @brief Retrieves the tag value of the ASN.1 object.
         * Derived classes must implement this method to define their specific tag value.
         * @return The tag value of the object.
         */
        constexpr virtual uintmax_t get_tag() const noexcept {
            return 0;
        }

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

        dynamic_array_t _data; /**< Internal buffer storing the ASN.1 object's raw data. */
        unsigned long long _raw_length;
        std::vector<std::unique_ptr<asn1_basic> > _children; /**< Child ASN.1 objects for constructed types. */
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
        bool _constructed{}; /**< Indicates whether the ASN.1 object is constructed. */
        asn1_class _cls{}; /**< The class of the ASN.1 tag (`UNIVERSAL`, `APPLICATION`, etc.). */
        size_t _length{}; /**< The length of the ASN.1 object's encoded data. */
        tag_t _type; /**< The tag type of the ASN.1 object. */


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

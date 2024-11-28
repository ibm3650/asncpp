#include <iostream>
#include <vector>
#include <numeric>
#include <iomanip>
#include "types/integer.h"
#include "types/object_identifier.h"
#include "types/enumerated.h"
#include "types/relative_oid.h"
#include "types/bit_string.h"

//TODO: Endianess check
//TODO: Tag check
//TODO: Length check
//TODO:Valid _data check
//TODO: Add support for large numbers
//DER encoding
//https://www.itu.int/ITU-T/studygroups/com17/languages/X.690-0207.pdf



inline std::ostream &operator<<(std::ostream &os, asn1_tag tag) {
    switch (tag) {
        case asn1_tag::Reserved:
            os << "Reserved";
            break;
        case asn1_tag::BOOLEAN:
            os << "BOOLEAN";
            break;
        case asn1_tag::INTEGER:
            os << "INTEGER";
            break;
        case asn1_tag::BIT_STRING:
            os << "BIT_STRING";
            break;
        case asn1_tag::OCTET_STRING:
            os << "OCTET_STRING";
            break;
        case asn1_tag::Null:
            os << "Null";
            break;
        case asn1_tag::OBJECT_IDENTIFIER:
            os << "OBJECT_IDENTIFIER";
            break;
        case asn1_tag::OBJECT_DE:
            os << "OBJECT_DE";
            break;
        case asn1_tag::External:
            os << "External";
            break;
        case asn1_tag::Real:
            os << "Real";
            break;
        case asn1_tag::ENUMERATED:
            os << "ENUMERATED";
            break;
        case asn1_tag::EmbeddedPDV:
            os << "EmbeddedPDV";
            break;
        case asn1_tag::UTF8String:
            os << "UTF8String";
            break;
        case asn1_tag::Sequence:
            os << "Sequence";
            break;
        case asn1_tag::Set:
            os << "Set";
            break;
        case asn1_tag::NumericString:
            os << "NumericString";
            break;
        case asn1_tag::PrintableString:
            os << "PrintableString";
            break;
        case asn1_tag::T61String:
            os << "T61String";
            break;
        case asn1_tag::VideotexString:
            os << "VideotexString";
            break;
        case asn1_tag::IA5String:
            os << "IA5String";
            break;
        case asn1_tag::UTCTime:
            os << "UTCTime";
            break;
        case asn1_tag::GeneralizedTime:
            os << "GeneralizedTime";
            break;
        case asn1_tag::GraphicString:
            os << "GraphicString";
            break;
        case asn1_tag::VisibleString:
            os << "VisibleString";
            break;
        case asn1_tag::GeneralString:
            os << "GeneralString";
            break;
        case asn1_tag::UniversalString:
            break;
        case asn1_tag::BMPString:
            break;
    }
    return os;
}


//void decode_octet_string(const uint8_t* _data, size_t length, std::vector<uint8_t>& output) {
//    size_t index = 0;
//
//    while (index < length) {
//        if (index + 2 > length) { // Проверяем наличие тега и длины
//            throw std::runtime_error("Invalid _data: insufficient bytes for tag and length");
//        }
//
//        uint8_t tag = _data[index++];
//        uint8_t len = _data[index++];
//
//        // Обработка длины
//        size_t lengthValue = 0;
//        if (len & 0x80) { // Длинная форма
//            uint8_t numLengthBytes = len & 0x7F;
//            if (index + numLengthBytes > length) {
//                throw std::runtime_error("Invalid _data: insufficient bytes for length");
//            }
//            for (uint8_t i = 0; i < numLengthBytes; ++i) {
//                lengthValue = (lengthValue << 8) | _data[index++];
//            }
//        } else { // Короткая форма
//            lengthValue = len;
//        }
//
//        if (index + lengthValue > length) { // Проверяем длину данных
//            throw std::runtime_error("Invalid _data: length exceeds available bytes");
//        }
//
//        // Проверяем тип
//        if (extract_type(tag) != asn1_tag::OCTET_STRING) {
//            throw std::runtime_error("Unexpected tag: not an OCTET STRING");
//        }
//
//        if (extract_is_constructed(tag)) {
//            // Рекурсивный вызов для вложенной конструкторной формы
//            decode_octet_string(_data + index, lengthValue, output);
//        } else {
//            // Добавляем данные в выходной вектор
//            output.insert(output.end(), _data + index, _data + index + lengthValue);
//        }
//
//        index += lengthValue;
//    }
//}



//IEEE-754 binary64 format
inline std::vector<uint8_t> mantissa_bits(double mantissa) {
    // Умножаем мантиссу на 2^53 и отбрасываем дробную часть
    auto bits = static_cast<uint64_t>(mantissa * (1ULL << 53));
    std::vector<uint8_t> result(8, 0);// Массив для результата (53 бита (56))

    for (int i = 7; i >= 0; --i) {
        result[i] = static_cast<uint8_t>(bits & 0xFF); // Извлекаем младший байт
        bits >>= 8;                                   // Сдвигаем число вправо
    }

    return result; // Возвращаем big-endian представление
}


std::vector<uint8_t> removeLeadingZeros(const std::vector<uint8_t> &bytes) {
    size_t startIndex = 0;

    // Найдём первый ненулевой байт
    while (startIndex < bytes.size() && bytes[startIndex] == 0x00) {
        startIndex++;
    }

    // Если всё число состоит из нулей, возвращаем один нулевой байт
    if (startIndex == bytes.size()) {
        return {0x00};
    }

    // Возвращаем массив без ведущих нулей
    return std::vector<uint8_t>(bytes.begin() + startIndex, bytes.end());
}

std::vector<uint8_t> minimizeMantissa(const std::vector<uint8_t> &bytes) {
    size_t startIndex = 0;
    size_t endIndex = bytes.size();

    // Удаляем ведущие нули
    while (startIndex < bytes.size() && bytes[startIndex] == 0x00) {
        startIndex++;
    }

    // Удаляем хвостовые нули
    while (endIndex > startIndex && bytes[endIndex - 1] == 0x00) {
        endIndex--;
    }

    // Возвращаем минимизированный массив
    return std::vector<uint8_t>(bytes.begin() + startIndex, bytes.begin() + endIndex);
}


double decodeReal(const uint8_t *data, size_t length) {
    if (length == 0) {
        return 0.0; // Ноль представляется пустым содержимым
    }

    // Разбираем флаги
    uint8_t flags = data[0];
    bool isNegative = (flags & 0x20) != 0;
    uint8_t baseFlag = (flags & 0xC0) >> 6;
    uint8_t exponentLength = flags & 0x1F;

    // Определяем основание
    int base = 2; // Основание по умолчанию
    if (baseFlag == 1) {
        base = 8;
    } else if (baseFlag == 2) {
        base = 16;
    }

    // Извлекаем экспоненту
    int exponent = 0;
    size_t index = 1; // Индекс после флагов
    for (size_t i = 0; i < exponentLength; ++i) {
        exponent = (exponent << 8) | data[index++];
    }

    // Учёт знака экспоненты (если используется дополнение до двух)
    if (exponent & (1 << ((exponentLength * 8) - 1))) {
        exponent -= (1 << (exponentLength * 8));
    }

    // Извлекаем мантиссу
    uint64_t mantissa = 0;
    for (; index < length; ++index) {
        mantissa = (mantissa << 8) | data[index];
    }

    // Восстанавливаем вещественное число
    double result = static_cast<double>(mantissa) * std::pow(base, exponent);
    if (isNegative) {
        result = -result;
    }

    return result;
}
std::vector<uint8_t> encodeBase128(uint32_t value) {
    std::vector<uint8_t> result;

    do {
        uint8_t byte = value & 0x7F; // Младшие 7 бит
        value >>= 7;

        if (!result.empty()) {
            byte |= 0x80; // Устанавливаем старший бит для продолжения
        }

        result.insert(result.begin(), byte);
    } while (value > 0);

    return result;
}

std::vector<uint8_t> encodeRelativeOid(const std::vector<uint32_t>& values) {
    std::vector<uint8_t> result;

    // Добавляем тег
    result.push_back(0x0D);

    // Кодируем значения
    std::vector<uint8_t> encodedValues;
    for (uint32_t value : values) {
        auto encoded = encodeBase128(value);
        encodedValues.insert(encodedValues.end(), encoded.begin(), encoded.end());
    }

    // Добавляем длину
    result.push_back(static_cast<uint8_t>(encodedValues.size()));

    // Добавляем закодированные значения
    result.insert(result.end(), encodedValues.begin(), encodedValues.end());

    return result;
}



//
//
//class Parent {
//public:
//    virtual void say() { // Виртуальный метод
//        std::cout << "Parent says hello!" << std::endl;
//    }
//};
//
//class Child : public Parent {
//public:
//    void say() override { // Переопределяем метод
//        std::cout << "Child says hello!" << std::endl;
//    }
//};
//
//int main() {
//    Parent* pParent = new Child();
//
//    // Вызов переопределённого метода через виртуальный механизм
//    pParent->say(); // Вызовет Child::say
//
//    // Явный вызов Parent::say
//    static_cast<Parent*>(pParent)->Parent::say();
//
//    delete pParent;
//    return 0;
//}
int main() {
    //const auto d = "\uD800\uDF48"
    bit_string_t bit_string(8564743, 24);
    std::cout << bit_string.to_string() << std::endl;
    std::vector<uint32_t> relativeOid = {123, 456};

    auto encoded2 = encodeRelativeOid(relativeOid);

    std::cout << "Encoded RELATIVE-OID: ";
    for (uint8_t byte : encoded2) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte) << " ";
    }
    std::cout << std::endl;
    const uint8_t values[] = {0x0a ,0x02 ,0x01 ,0x00};
    const uint8_t values1[] = {0x0D, 0x03, 0x7B, 0x83, 0x48};
    relative_identifier_t relative_oid;
    relative_oid.decode(values1);
    std::cout << relative_oid.to_string() << std::endl;
    enumerated_t enumerated;
    enumerated.decode(values);
    std::cout << static_cast<uintmax_t>(enumerated) << std::endl;
    std::cout << enumerated.to_string() << std::endl;




    const int mask = static_cast<int>(-1) << (8 * 2);
    std::cout << std::hex << mask << std::endl;
    std::vector<uint32_t> data = {1, 2, 840, 113549};
    std::vector<uint8_t> result;

    if (data.size() >= 2) {
        result.push_back(40 * data[0] + data[1]);
    }

    // Кодируем остальные идентификаторы (c, d, ...)
    for (size_t i = 2; i < data.size(); ++i) {
        uint32_t value = data[i];
        std::vector<uint8_t> temp;

        // Кодируем value в base-128
        do {
            temp.insert(temp.begin(), static_cast<uint8_t>(value & 0x7F));
            value >>= 7;
        } while (value > 0);

        // Устанавливаем MSB для всех байтов, кроме последнего
        for (size_t j = 0; j < temp.size() - 1; ++j) {
            temp[j] |= 0x80;
        }

        // Добавляем закодированное значение в результат
        result.insert(result.end(), temp.begin(), temp.end());
    }


    for (const auto byte: result) {
        std::cout << std::hex << static_cast<uint32_t>(byte) << ' ';
    }

    const uint8_t data1[] = {
            0x06, 0x06, 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D
    };
    object_identifier_t oid;
    oid.decode(data1);
    std::cout << oid.to_string() << std::endl;
    return 0;
//    const uint8_t buffer[] = {0x1F, 0x81, 0x2A, 0x82, 0x01, 0x2C}; // Длинный тег: 0x12A
//    asn1_block block(buffer);
//    std::cout << "Type: " << block.get_type() << std::endl;
//    std::cout << "Class: " << static_cast<int>(block.get_cls()) << std::endl;
//    std::cout << "Constructed: " << std::boolalpha << block.is_constructed() << std::endl;
//    std::cout << "Length: " << block.get_length() << std::endl;
//    std::cout << "Data: ";
//    const uint8_t integerData[] = { //1
//            0x02,       // Тег INTEGER
//            0x01,       // Длина: 1 байт
//            0x2A        // Значение: 42 (в шестнадцатеричной системе)
//    };
//
    const uint8_t largeIntegerData[] = {
            0x02,
            0x04,
            0xF8, 0xA4, 0x32, 0xEB
    };//-123456789
//    integer_t integer;
//    integer.decode(largeIntegerData);
//    auto value = static_cast<intmax_t>(integer);
//    std::cout << value << std::endl;
//    integer_t i(5);
//    const auto tl =  asn1_block::encode_length(30);
//    std::cout << "Length: " << asn1_block::extract_length(tl.data()).first << std::endl;
//    return 0;

    const int64_t test_nums[] = {0, -0,
                                 1, -1,
                                 127, -127,
                                 128, -128,
                                 255, -255,
                                 256, -256,
                                 32767, -32767,
                                 32768, -32768,
                                 65535, -65535,
                                 65536, -65536,
                                 2147483647, -2147483647,
                                 2147483648, -2147483648,
                                 4294967295, -4294967295,
                                 4294967296, -4294967296,
                                 9223372036854775807, -9223372036854775807,
                                 9223372036854775808, -9223372036854775808};
    for (const auto &num: test_nums) {
        integer_t integer(num);
        const auto encoded = integer.encode();
        integer_t decoded(int64_t{0});
        decoded.decode(encoded.data());
        const auto decoded_num = static_cast<int64_t>(decoded);
        std::cout << "Original: " << num << ' ' << decoded.to_string() << std::endl;
    }
//
//
//    const uint8_t sequenceData[] = { //4
//            0x30,       // Тег SEQUENCE (конструкторный)
//            0x06,       // Длина: 6 байт
//            0x02, 0x01, 0x7B, // INTEGER: Тег (0x02), длина (0x01), значение (123 = 0x7B)
//            0x01, 0x01, 0xFF  // BOOLEAN: Тег (0x01), длина (0x01), значение (TRUE = 0xFF)
//    };
//
//    const uint8_t booleanData[] = {
//            0x01,       // Тег BOOLEAN
//            0x01,       // Длина: 1 байт
//            0xFF        // Значение: TRUE (0xFF)
//    };
//
//
//    const uint8_t octetStringData[] = {
//            0x04,       // Тег OCTET STRING
//            0x05,       // Длина: 5 байт
//            0x48, 0x65, 0x6C, 0x6C, 0x6F // Значение: "Hello"
//    };
//
//    const uint8_t constructedOctetStringData[] = {
//            0x24,       // Тег OCTET STRING (конструкторная форма)
//            14,       // Длина: 12 байт (2 вложенных элемента по 6 байт каждый)
//
//            // Первый вложенный OCTET STRING ("Hello")
//            0x04,       // Тег OCTET STRING (примитивная форма)
//            0x05,       // Длина: 5 байт
//            0x48, 0x65, 0x6C, 0x6C, 0x6F, // "Hello"
//
//            // Второй вложенный OCTET STRING ("World")
//            0x04,       // Тег OCTET STRING (примитивная форма)
//            0x05,       // Длина: 5 байт
//            0x57, 0x6F, 0x72, 0x6C, 0x64 // "World"
//    };
//
//
    // const uint8_t *ptr = largeIntegerData;
//    //const uint8_t *ptr = constructedOctetStringData;
//
//    std::cout <<
//              "Type: " << extract_type(ptr[0]) << std::endl <<
//              "Constructed: " << std::boolalpha << extract_is_constructed(ptr[0]) << std::endl <<
//              "Class: " << static_cast<int>(extract_class(ptr[0])) << std::endl;
//
//    const size_t length = ptr[1];
//    const uint8_t *_data = ptr + 2;
//    //const asn1_tag type = extract_type(ptr[0]);
////    if (type == asn1_tag::INTEGER) {
//        int32_t value = 0;
//        int sign = 1;
//        if (*_data == 0x00)
//            _data++;
//        else if (*_data == 0x80) {
//            sign = -1;
//            _data++;
//        }
//        for (size_t i = 0; i < length; i++) {
//            value = value << 8;
//            value += *_data;
//            _data++;
//        }
//        value *= sign;
//        std::cout << std::dec << "Value: " << value << std::endl;
//    }
//
//    if (type == asn1_tag::BOOLEAN) {
//        bool value = *_data != 0x00;
//        std::cout << "Value: " << std::boolalpha << value << std::endl;
//    }
//    if (type == asn1_tag::OCTET_STRING) {
//        std::vector<uint8_t> output;
//        decode_octet_string(ptr, sizeof octetStringData, output);
//        std::cout << "Value: " << std::string{reinterpret_cast<char *>(output._data()), output.size()} << std::endl;
//    }
    return 0;
}

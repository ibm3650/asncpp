#include <iostream>
#include <vector>

//TODO: Endianess check
//TODO: Tag check
//TODO: Length check
//TODO:Valid data check
//TODO: Add support for large numbers
//DER encoding
//https://www.itu.int/ITU-T/studygroups/com17/languages/X.690-0207.pdf
enum class asn1_tag : uint8_t {
    Reserved = 0x00,
    BOOLEAN = 0x01,
    INTEGER = 0x02,
    BitString = 0x03,
    OCTET_STRING = 0x04,
    Null = 0x05,
    ObjectIdentifier = 0x06,
    ObjectDescriptor = 0x07,
    External = 0x08,
    Real = 0x09,
    Enumerated = 0x0A,
    EmbeddedPDV = 0x0B,
    UTF8String = 0x0C,
    Sequence = 0x10,
    Set = 0x11,
    NumericString = 0x12,
    PrintableString = 0x13,
    T61String = 0x14,
    VideotexString = 0x15,
    IA5String = 0x16,
    UTCTime = 0x17,
    GeneralizedTime = 0x18,
    GraphicString = 0x19,
    VisibleString = 0x1A,
    GeneralString = 0x1B,
    UniversalString = 0x1C,
    BMPString = 0x1E
};

enum class asn1_class : uint8_t {
    UNIVERSAL = 0x00,
    APPLICATION = 0x01,
    CONTEXT_SPECIFIC = 0x02,
    PRIVATE = 0x03
};

//inline asn1_tag operator|(asn1_tag lhs, asn1_tag rhs) {
//    return static_cast<asn1_tag>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
//}


inline asn1_tag get_type(uint8_t tag) {
    return static_cast<asn1_tag>(tag & 0b00011111);
}

inline bool is_constructed(uint8_t tag) {
    return (tag & 0b00100000);
}

inline asn1_class get_class(uint8_t tag) {
    return static_cast<asn1_class>(tag & 0b11000000);
}

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
        case asn1_tag::BitString:
            os << "BitString";
            break;
        case asn1_tag::OCTET_STRING:
            os << "OCTET_STRING";
            break;
        case asn1_tag::Null:
            os << "Null";
            break;
        case asn1_tag::ObjectIdentifier:
            os << "ObjectIdentifier";
            break;
        case asn1_tag::ObjectDescriptor:
            os << "ObjectDescriptor";
            break;
        case asn1_tag::External:
            os << "External";
            break;
        case asn1_tag::Real:
            os << "Real";
            break;
        case asn1_tag::Enumerated:
            os << "Enumerated";
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


void decode_octet_string(const uint8_t* data, size_t length, std::vector<uint8_t>& output) {
    size_t index = 0;

    while (index < length) {
        if (index + 2 > length) { // Проверяем наличие тега и длины
            throw std::runtime_error("Invalid data: insufficient bytes for tag and length");
        }

        uint8_t tag = data[index++];
        uint8_t len = data[index++];

        // Обработка длины
        size_t lengthValue = 0;
        if (len & 0x80) { // Длинная форма
            uint8_t numLengthBytes = len & 0x7F;
            if (index + numLengthBytes > length) {
                throw std::runtime_error("Invalid data: insufficient bytes for length");
            }
            for (uint8_t i = 0; i < numLengthBytes; ++i) {
                lengthValue = (lengthValue << 8) | data[index++];
            }
        } else { // Короткая форма
            lengthValue = len;
        }

        if (index + lengthValue > length) { // Проверяем длину данных
            throw std::runtime_error("Invalid data: length exceeds available bytes");
        }

        // Проверяем тип
        if (get_type(tag) != asn1_tag::OCTET_STRING) {
            throw std::runtime_error("Unexpected tag: not an OCTET STRING");
        }

        if (is_constructed(tag)) {
            // Рекурсивный вызов для вложенной конструкторной формы
            decode_octet_string(data + index, lengthValue, output);
        } else {
            // Добавляем данные в выходной вектор
            output.insert(output.end(), data + index, data + index + lengthValue);
        }

        index += lengthValue;
    }
}


int main() {
    const uint8_t integerData[] = { //1
            0x02,       // Тег INTEGER
            0x01,       // Длина: 1 байт
            0x2A        // Значение: 42 (в шестнадцатеричной системе)
    };

    const uint8_t largeIntegerData[] = {
            0x02,
            0x04,
            0xF8, 0xA4, 0x32, 0xEB
    };//-123456789


    const uint8_t sequenceData[] = { //4
            0x30,       // Тег SEQUENCE (конструкторный)
            0x06,       // Длина: 6 байт
            0x02, 0x01, 0x7B, // INTEGER: Тег (0x02), длина (0x01), значение (123 = 0x7B)
            0x01, 0x01, 0xFF  // BOOLEAN: Тег (0x01), длина (0x01), значение (TRUE = 0xFF)
    };

    const uint8_t booleanData[] = {
            0x01,       // Тег BOOLEAN
            0x01,       // Длина: 1 байт
            0xFF        // Значение: TRUE (0xFF)
    };


    const uint8_t octetStringData[] = {
            0x04,       // Тег OCTET STRING
            0x05,       // Длина: 5 байт
            0x48, 0x65, 0x6C, 0x6C, 0x6F // Значение: "Hello"
    };

    const uint8_t constructedOctetStringData[] = {
            0x24,       // Тег OCTET STRING (конструкторная форма)
            14,       // Длина: 12 байт (2 вложенных элемента по 6 байт каждый)

            // Первый вложенный OCTET STRING ("Hello")
            0x04,       // Тег OCTET STRING (примитивная форма)
            0x05,       // Длина: 5 байт
            0x48, 0x65, 0x6C, 0x6C, 0x6F, // "Hello"

            // Второй вложенный OCTET STRING ("World")
            0x04,       // Тег OCTET STRING (примитивная форма)
            0x05,       // Длина: 5 байт
            0x57, 0x6F, 0x72, 0x6C, 0x64 // "World"
    };


    const uint8_t *ptr = octetStringData;
    //const uint8_t *ptr = constructedOctetStringData;

    std::cout <<
              "Type: " << get_type(ptr[0]) << std::endl <<
              "Constructed: " << std::boolalpha << is_constructed(ptr[0]) << std::endl <<
              "Class: " << static_cast<int>(get_class(ptr[0])) << std::endl;

    const size_t length = ptr[1];
    const uint8_t *data = ptr + 2;
    const asn1_tag type = get_type(ptr[0]);
    if (type == asn1_tag::INTEGER) {
        int value = 0;
        int sign = 1;
        if (*data == 0x00)
            data++;
        if (*data == 0x80) {
            sign = -1;
            data++;
        }
        for (size_t i = 0; i < length; i++) {
            value = value << 8;
            value += *data;
            data++;
        }
        value *= sign;
        std::cout << std::dec << "Value: " << value << std::endl;
    }

    if (type == asn1_tag::BOOLEAN) {
        bool value = *data != 0x00;
        std::cout << "Value: " << std::boolalpha << value << std::endl;
    }
    if (type == asn1_tag::OCTET_STRING) {
        std::vector<uint8_t> output;
        decode_octet_string(ptr, sizeof octetStringData, output);
        std::cout << "Value: " << std::string{reinterpret_cast<char *>(output.data()), output.size()} << std::endl;
    }
    return 0;
}

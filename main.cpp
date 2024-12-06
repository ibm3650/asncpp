#include <iostream>
#include <vector>
#include <numeric>
#include <iomanip>
#include <sstream>
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
        case asn1_tag::OBJECT_DESCRIPTOR:
            os << "OBJECT_DE";
            break;
        case asn1_tag::EXTERNAL:
            os << "EXTERNAL";
            break;
        case asn1_tag::REAL:
            os << "REAL";
            break;
        case asn1_tag::ENUMERATED:
            os << "ENUMERATED";
            break;
        case asn1_tag::EMBEDDED_PDV:
            os << "EMBEDDED_PDV";
            break;
        case asn1_tag::UTF8_STRING:
            os << "UTF8_STRING";
            break;
        case asn1_tag::SEQUENCE:
            os << "SEQUENCE";
            break;
        case asn1_tag::SET:
            os << "Set";
            break;
        case asn1_tag::NUMERIC_STRING:
            os << "NUMERIC_STRING";
            break;
        case asn1_tag::PRINTABLE_STRING:
            os << "PRINTABLE_STRING";
            break;
        case asn1_tag::T61_STRING:
            os << "T61String";
            break;
        case asn1_tag::VIDEOTEX_STRING:
            os << "VideotexString";
            break;
        case asn1_tag::IA5_STRING:
            os << "IA5_STRING";
            break;
        case asn1_tag::UTC_TIME:
            os << "UTC_TIME";
            break;
        case asn1_tag::GENERALIZED_TIME:
            os << "GENERALIZED_TIME";
            break;
        case asn1_tag::GRAPHIC_STRING:
            os << "GRA";
            break;
        case asn1_tag::VISIBLE_STRING:
            os << "VISIBLE_STRING";
            break;
        case asn1_tag::GENERAL_STRING:
            os << "GeneralString";
            break;
        case asn1_tag::UNIVERSAL_STRING:
            break;
        case asn1_tag::BMP_STRING:
            break;
        default:
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

std::vector<uint8_t> encodeRelativeOid(const std::vector<uint32_t> &values) {
    std::vector<uint8_t> result;

    // Добавляем тег
    result.push_back(0x0D);

    // Кодируем значения
    std::vector<uint8_t> encodedValues;
    for (uint32_t value: values) {
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

std::string generalized(std::tm datetime, int8_t timezone = 0) {
    constexpr size_t buffer_size = std::size("YYYYMMDDHHMMSS.000+HHMM");
    char buffer[buffer_size];
    std::strftime(buffer,
                  buffer_size,
                  "%Y%m%d%H%M%S.000",
                  &datetime);
    if (timezone == 0) {
        return std::string(buffer) + "Z";
    }

    sprintf(buffer + 15, "%+03d00", timezone);
    return {buffer};
}

//std::vector<uint8_t> serialize(){
//
//}

//template<typename T, uintmax_t U>
//std::vector<uint8_t> serialize(const asn1_base<T,U> *block){
//    block->encode();
//    return static_cast<asn1_base<T,U>*>(block)->asn1_base<T,U>::encode();
//}

//template<typename T1, uintmax_t U1>
//std::vector<uint8_t> serialize(asn1_base<T1, U1> *block) {
//    (void)block->encode();
//    return static_cast<asn1_base<T1, U1> *>(block)->asn1_base<T1, U1>::encode();
//}

std::vector<uint8_t> serialize(asn1_base *block) {
    (void) block->encode();
    return static_cast<asn1_base *>(block)->asn1_base::encode();
}




std::unique_ptr<asn1_base> deserialize_v(std::span<const uint8_t> data) {
    if (data.empty()) {
        throw std::invalid_argument("Invalid ASN.1 data");
    }
    const auto type = asn1_base::extract_type(data).first;
    if (!std::get_if<asn1_tag>(&type)) {
        return nullptr;
    }
    std::unique_ptr<asn1_base> ptr;
    switch (*std::get_if<asn1_tag>(&type)) {

        case asn1_tag::Reserved:
            break;
        case asn1_tag::BOOLEAN:
            break;
        case asn1_tag::INTEGER:
            ptr = std::make_unique<integer_t>();
            break;
        case asn1_tag::BIT_STRING:
            break;
        case asn1_tag::OCTET_STRING:
            break;
        case asn1_tag::Null:
            break;
        case asn1_tag::OBJECT_IDENTIFIER:
            ptr = std::make_unique<object_identifier_t>();
            break;
        case asn1_tag::OBJECT_DESCRIPTOR:
            break;
        case asn1_tag::EXTERNAL:
            break;
        case asn1_tag::REAL:
            break;
        case asn1_tag::ENUMERATED:
            break;
        case asn1_tag::EMBEDDED_PDV:
            break;
        case asn1_tag::UTF8_STRING:
            break;
        case asn1_tag::RELATIVE_OID:
            break;
        case asn1_tag::SEQUENCE:
            break;
        case asn1_tag::SET:
            break;
        case asn1_tag::NUMERIC_STRING:
            break;
        case asn1_tag::PRINTABLE_STRING:
            break;
        case asn1_tag::T61_STRING:
            break;
        case asn1_tag::VIDEOTEX_STRING:
            break;
        case asn1_tag::IA5_STRING:
            break;
        case asn1_tag::UTC_TIME:
            break;
        case asn1_tag::GENERALIZED_TIME:
            break;
        case asn1_tag::GRAPHIC_STRING:
            break;
        case asn1_tag::VISIBLE_STRING:
            break;
        case asn1_tag::GENERAL_STRING:
            break;
        case asn1_tag::UNIVERSAL_STRING:
            break;
        case asn1_tag::BMP_STRING:
            break;
        case asn1_tag::DATE:
            break;
        case asn1_tag::TIME_OF_DAY:
            break;
        case asn1_tag::DATE_TIME:
            break;
        case asn1_tag::DURATION:
            break;
    }
    (void)static_cast<asn1_base*>(ptr.get())->asn1_base::decode(data);
    ptr->decode(data);
    return ptr;
}
//#include <variant>
//auto deserialize_v(std::span<const uint8_t> data) {
//    if (data.empty()) {
//        throw std::invalid_argument("Invalid ASN.1 data");
//    }
//    const auto type = asn1_base<char, 0>::extract_type(data).first;
//    if(!std::get_if<asn1_tag>(&type)){
//        return std::variant{asn1_base<char, 0>::extract_type(data)};
//    }
//    switch (*std::get_if<asn1_tag>(&type)) {
//        case asn1_tag::INTEGER: {
//            integer_base integer;
//            integer.decode(data.subspan(asn1_base<char, 0>::extract_type(data).second));
//            return std::variant{integer};
//        }
////        case asn1_tag::OBJECT_IDENTIFIER: {
////            object_identifier_t oid;
////            oid.decode(data);
////            return oid;
////        }
////        case asn1_tag::ENUMERATED: {
////            enumerated_t enumerated;
////            enumerated.decode(data);
////            return enumerated;
////        }
////        case asn1_tag::RELATIVE_OID: {
////            relative_identifier_t relative_oid;
////            relative_oid.decode(data);
////            return relative_oid;
////        }
////        case asn1_tag::BIT_STRING: {
////            bit_string_t bit_string;
////            bit_string.decode(data);
////            return bit_string;
////        }
//        default:
//            throw std::invalid_argument("Unsupported ASN.1 type");
//    }
//    return std::variant{0};
//}


int main111() {
    integer_base inr(6564564);
    // serialize(&inr);
    //using Type = typename std::remove_reference_t<decltype(inr)>::value_t;
    //using Type1 = typename inr::type;
    // asn1_base<Type, inr.type> *base = &inr;
    // base->encode();
    const auto fff = serialize(&inr);
    for (uint8_t byte: fff) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte) << " ";
    }
    const auto fffffd = deserialize_v(fff);
    std::cout << fffffd->to_string() << std::endl;
    //const auto fff = inr.encode();

    return 0;
//    // Example of the very popular RFC 3339 format UTC time
//    struct tm tt = {0};
//    const std::string rfc3339 = "20241203105321.000-1100";
////    const std::string rfc3339 = "20241203105321";
////    const std::string rfc3339 = "20241203105321.000Z";
//    std::istringstream ss(rfc3339);
//    //std::istringstream ss(raw.get_data().data());
//
//    // Парсим дату-время в формате ISO 8601 (без проверки часового пояса)
//    ss >> std::get_time(&tt, "%Y%m%d%H%M%S");
//    if (ss.fail()) {
//        throw std::invalid_argument("Invalid ISO 8601 format: " + rfc3339);
//    }
//    std::string str;
//    ss >> str;
//    str = str.substr(str.length() - 5, 3);
//    std::cout << "Stream" << str << '\n';
//    int res{};
//    res = std::stoi(str);
////    int timezone = 0;
////    if (rfc3339.size() > 15) {
////        ss >> timezone;
////    }
//    std::time_t time = std::time({});
//    std::timespec ts{};
//    std::timespec_get(&ts, TIME_UTC);
//    char timeString[std::size("yyyy-mm-ddThh:mm:ssZ")+ 100];
//    std::strftime(std::data(timeString),
//                  std::size(timeString),
//                  "%y %m %d %H %M %S Z",
//                  std::gmtime(&ts.tv_sec));
//    std::cout << timeString << '\n'
//            << "Raw timespec.tv_nsec: " << (ts.tv_nsec % 1000000) / 1000 << '\n';
//
//
//
//    double seconds;
//    if (sscanf(generalized(*std::gmtime(&ts.tv_sec)).c_str(), "%04d%02d%02d%02d%02d%lfZ",
//               &tt.tm_year, &tt.tm_mon, &tt.tm_mday,
//               &tt.tm_hour, &tt.tm_min, &seconds) != 6)
//    tt.tm_sec   = (int) seconds;
//    tt.tm_mon  -= 1;
//    tt.tm_year -= 1900;
//    tt.tm_isdst =-1;
//    const auto ff = mktime(&tt);
//    std::strftime(std::data(timeString),
//                  std::size(timeString),
//                  "%Y %m %d %H %M %S",
//                  std::gmtime(&ff));
//    std::cout << timeString << '\n'
//              << "Raw timespec.tv_nsec: " << (ts.tv_nsec % 1000000) / 1000 << '\n';
//;
////    const auto d = u8"\uD800\uDF48"
//    bit_string_t bit_string(8564743, 24);
//    std::cout << bit_string.to_string() << std::endl;
//    std::vector<uint32_t> relativeOid = {123, 456};
//
//    auto encoded2 = encodeRelativeOid(relativeOid);
//
//    std::cout << "Encoded RELATIVE-OID: ";
//    for (uint8_t byte: encoded2) {
//        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte) << " ";
//    }
//    std::cout << std::endl;
//    const uint8_t values[] = {0x0a, 0x02, 0x01, 0x00};
//    const uint8_t values1[] = {0x0D, 0x03, 0x7B, 0x83, 0x48};
//    relative_identifier_t relative_oid;
//    relative_oid.decode(std::span{values1});
//    std::cout << relative_oid.to_string() << std::endl;
//    enumerated_t enumerated;
//    enumerated.decode(values);
//    std::cout << static_cast<uintmax_t>(enumerated) << std::endl;
//    std::cout << enumerated.to_string() << std::endl;
//
//
//    const int mask = static_cast<int>(-1) << (8 * 2);
//    std::cout << std::hex << mask << std::endl;
//    std::vector<uint32_t> data = {1, 2, 840, 113549};
//    std::vector<uint8_t> result;
//
//    if (data.size() >= 2) {
//        result.push_back(40 * data[0] + data[1]);
//    }
//
//    // Кодируем остальные идентификаторы (c, d, ...)
//    for (size_t i = 2; i < data.size(); ++i) {
//        uint32_t value = data[i];
//        std::vector<uint8_t> temp;
//
//        // Кодируем value в base-128
//        do {
//            temp.insert(temp.begin(), static_cast<uint8_t>(value & 0x7F));
//            value >>= 7;
//        } while (value > 0);
//
//        // Устанавливаем MSB для всех байтов, кроме последнего
//        for (size_t j = 0; j < temp.size() - 1; ++j) {
//            temp[j] |= 0x80;
//        }
//
//        // Добавляем закодированное значение в результат
//        result.insert(result.end(), temp.begin(), temp.end());
//    }
//
//
//    for (const auto byte: result) {
//        std::cout << std::hex << static_cast<uint32_t>(byte) << ' ';
//    }
//
//    const uint8_t data1[] = {
//            0x06, 0x06, 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D
//    };
//    object_identifier_t oid;
//    oid.decode(data1);
//    std::cout << oid.to_string() << std::endl;
//    return 0;
////    const uint8_t buffer[] = {0x1F, 0x81, 0x2A, 0x82, 0x01, 0x2C}; // Длинный тег: 0x12A
////    asn1_base block(buffer);
////    std::cout << "Type: " << block.get_type() << std::endl;
////    std::cout << "Class: " << static_cast<int>(block.get_cls()) << std::endl;
////    std::cout << "Constructed: " << std::boolalpha << block.is_constructed() << std::endl;
////    std::cout << "Length: " << block.get_length() << std::endl;
////    std::cout << "Data: ";
////    const uint8_t integerData[] = { //1
////            0x02,       // Тег INTEGER
////            0x01,       // Длина: 1 байт
////            0x2A        // Значение: 42 (в шестнадцатеричной системе)
////    };
////
//    const uint8_t largeIntegerData[] = {
//            0x02,
//            0x04,
//            0xF8, 0xA4, 0x32, 0xEB
//    };//-123456789
////    integer_base integer;
////    integer.decode(largeIntegerData);
////    auto value = static_cast<intmax_t>(integer);
////    std::cout << value << std::endl;
////    integer_base i(5);
////    const auto tl =  asn1_base::encode_length(30);
////    std::cout << "Length: " << asn1_base::extract_length(tl.data()).first << std::endl;
////    return 0;
//
//    const int64_t test_nums[] = {0, -0,
//                                 1, -1,
//                                 127, -127,
//                                 128, -128,
//                                 255, -255,
//                                 256, -256,
//                                 32767, -32767,
//                                 32768, -32768,
//                                 65535, -65535,
//                                 65536, -65536,
//                                 2147483647, -2147483647,
//                                 2147483648, -2147483648,
//                                 4294967295, -4294967295,
//                                 4294967296, -4294967296,
//                                 9223372036854775807, -9223372036854775807,
//                                 9223372036854775808, -9223372036854775808};
//    for (const auto &num: test_nums) {
//        integer_base integer(num);
//        const auto encoded = integer.encode();
//        integer_base decoded(int64_t{0});
//        decoded.decode(encoded.data());
//        const auto decoded_num = static_cast<int64_t>(decoded);
//        std::cout << "Original: " << num << ' ' << decoded.to_string() << std::endl;
//    }
////
////
////    const uint8_t sequenceData[] = { //4
////            0x30,       // Тег SEQUENCE (конструкторный)
////            0x06,       // Длина: 6 байт
////            0x02, 0x01, 0x7B, // INTEGER: Тег (0x02), длина (0x01), значение (123 = 0x7B)
////            0x01, 0x01, 0xFF  // BOOLEAN: Тег (0x01), длина (0x01), значение (TRUE = 0xFF)
////    };
////
////    const uint8_t booleanData[] = {
////            0x01,       // Тег BOOLEAN
////            0x01,       // Длина: 1 байт
////            0xFF        // Значение: TRUE (0xFF)
////    };
////
////
////    const uint8_t octetStringData[] = {
////            0x04,       // Тег OCTET STRING
////            0x05,       // Длина: 5 байт
////            0x48, 0x65, 0x6C, 0x6C, 0x6F // Значение: "Hello"
////    };
////
////    const uint8_t constructedOctetStringData[] = {
////            0x24,       // Тег OCTET STRING (конструкторная форма)
////            14,       // Длина: 12 байт (2 вложенных элемента по 6 байт каждый)
////
////            // Первый вложенный OCTET STRING ("Hello")
////            0x04,       // Тег OCTET STRING (примитивная форма)
////            0x05,       // Длина: 5 байт
////            0x48, 0x65, 0x6C, 0x6C, 0x6F, // "Hello"
////
////            // Второй вложенный OCTET STRING ("World")
////            0x04,       // Тег OCTET STRING (примитивная форма)
////            0x05,       // Длина: 5 байт
////            0x57, 0x6F, 0x72, 0x6C, 0x64 // "World"
////    };
////
////
//    // const uint8_t *ptr = largeIntegerData;
////    //const uint8_t *ptr = constructedOctetStringData;
////
////    std::cout <<
////              "Type: " << extract_type(ptr[0]) << std::endl <<
////              "Constructed: " << std::boolalpha << extract_is_constructed(ptr[0]) << std::endl <<
////              "Class: " << static_cast<int>(extract_class(ptr[0])) << std::endl;
////
////    const size_t length = ptr[1];
////    const uint8_t *_data = ptr + 2;
////    //const asn1_tag type = extract_type(ptr[0]);
//////    if (type == asn1_tag::INTEGER) {
////        int32_t value = 0;
////        int sign = 1;
////        if (*_data == 0x00)
////            _data++;
////        else if (*_data == 0x80) {
////            sign = -1;
////            _data++;
////        }
////        for (size_t i = 0; i < length; i++) {
////            value = value << 8;
////            value += *_data;
////            _data++;
////        }
////        value *= sign;
////        std::cout << std::dec << "Value: " << value << std::endl;
////    }
////
////    if (type == asn1_tag::BOOLEAN) {
////        bool value = *_data != 0x00;
////        std::cout << "Value: " << std::boolalpha << value << std::endl;
////    }
////    if (type == asn1_tag::OCTET_STRING) {
////        std::vector<uint8_t> output;
////        decode_octet_string(ptr, sizeof octetStringData, output);
////        std::cout << "Value: " << std::string{reinterpret_cast<char *>(output._data()), output.size()} << std::endl;
////    }
//    return 0;
}

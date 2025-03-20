/**
* @file common.cpp
 * @brief Common declaration for the ASN.1 encoding and decoding library.
 * @author Nikita Kanduba
 * @date 17.12.2024
 */
#include <asncpp/base/common.h>

#include <asncpp/types.h>

std::shared_ptr<asncpp::base::asn1_basic> asncpp::base::deserialize_v(std::span<const uint8_t> data) {
    asn1_basic base(data);
    //Проверка, что тип являются допустимым тегом для ASN.1-объекта
    if (!std::holds_alternative<asn1_tag>(base._type)) {
        throw std::runtime_error("Tag is not an ASN.1 tag");
    }
    //Проверка, что объект конструктивный
    if (base.constructed()) {
        /* Пока не кончатся данные извлекаются вложенные данные или просто коллекции, после каждого извлеченного объекта,
         * данные удаляются из буфера его родителя, чтобы не было дублирования данных
         */
        while (!base._data.empty()) {
            //Рекурсия, так как объект конструктивный и может иметь несколько ступеней вложенности.
            auto child = deserialize_v(base._data);
            if (!child) {
                throw std::runtime_error("Failed to deserialize child object");
            }
            //Удаление данных из буфера родителя
            base.truncate_data(child->_raw_length);
            //Добавление дочернего объекта в родительский
            base._children.emplace_back(std::move(child));
        }
    }
    const auto type = std::get<asn1_tag>(base._type);
    //Функция для создания объекта по тегу
    //В списке захваливается base, так как он необходим для дальнейшего декодирования,
    //он перемещается во избежания избыточного копирования
    auto create_object = [base = std::move(base)](const asn1_tag tag) mutable -> std::shared_ptr<asn1_basic> {
        using enum asn1_tag;
        switch (tag) {
            case BOOLEAN: return std::make_shared<types::boolean_t>(std::move(base));
            case INTEGER: return std::make_shared<types::integer_t>(std::move(base));
            case BIT_STRING: return std::make_shared<types::bit_string_t>(std::move(base));
            case OCTET_STRING: return std::make_shared<octet_string_t>(std::move(base));
            case OBJECT_IDENTIFIER: return std::make_shared<object_identifier_t>(std::move(base));
            case OBJECT_DESCRIPTOR: return std::make_shared<object_descriptor_t>(std::move(base));
            case ENUMERATED: return std::make_shared<enumerated_t>(std::move(base));
            case RELATIVE_OID: return std::make_shared<relative_oid_t>(std::move(base));
            case NUMERIC_STRING: return std::make_shared<numeric_string_t>(std::move(base));
            case PRINTABLE_STRING: return std::make_shared<printable_string_t>(std::move(base));
            case IA5_STRING: return std::make_shared<ia5_string_t>(std::move(base));
            case VISIBLE_STRING: return std::make_shared<visible_string_t>(std::move(base));
            case UNIVERSAL_STRING: return std::make_shared<universal_string_t>(std::move(base));
            case BMP_STRING: return std::make_shared<bmp_string_t>(std::move(base));
            case UTF8_STRING: return std::make_shared<utf8_string_t>(std::move(base));
            case DATE: return std::make_shared<date_t>(std::move(base));
            case DATE_TIME: return std::make_shared<date_time_t>(std::move(base));
            case TIME_OF_DAY: return std::make_shared<time_of_day_t>(std::move(base));
            case REAL: return std::make_shared<real_t>(std::move(base));
            case UTC_TIME: return std::make_shared<utc_time_t>(std::move(base));
            case GENERALIZED_TIME: return std::make_shared<generalized_time_t>(std::move(base));
            case DURATION: return std::make_shared<duration_t>(std::move(base));
            case Null: return std::make_shared<null_t>(std::move(base));
            //FIXME: SET_OF is have same type as SET. But we ned to check> is all types ov elements are same
            case SET: return std::make_shared<set_t>(std::move(base));
            case SEQUENCE: return std::make_shared<sequence_t>(std::move(base));
            // case SET_OF: return std::make_shared<set_of_t>(std::move(base));
            default: return nullptr;
        }
    };
    auto ptr = create_object(type);
    if (!ptr) {
        throw std::runtime_error("Unsupported ASN.1 tag type");
    }
    //Разбор данных объекта, так как он уже создан, и фактические данные уже извлечены и теперь вызывается
    //метод decode класса-наследника asn_basic1 для заполнения внутренних полей объекта.
    ptr->decode(data);
    return ptr;
}


std::vector<uint8_t> asncpp::base::serialize(asn1_basic *block) {
    /*
     *Рекурсивно вызывается для всех дочерних объектов, если они есть.
     *Функция применяется для кадого вложенного объекта изнутри-наружу.
     */
    for (size_t i = 0; i < block->number_of_children(); ++i) {
        block->_data.append_range(serialize(block->_children[i].get()));
    }
    //Кодирование объекта. Данный метод вызывается для каждого объекта.
    (void) block->encode();
    //Кодирование в финальный TLV-блок. Данный метод вызывается для каждого объекта. Но в этот раз радительского класса
    return block->asn1_basic::encode();
}

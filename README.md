> [!NOTE]
> Уже сейчас библиотека отлично протестирована и в полной мере выполняет свои функции. 
> Однако, я активно продолжаю развивать проект, к чему хочу привлечь и вас ;) 
> Свои планы и идеи я описал в разделе TODO's and ideas. 
> Если у вас есть свои идеи, предложения или пожелания, то я рад буду из принять и развивать.
> Сообщайте о найденных ошибках, пожеланиях и предложениях в разделе Issues.


- [Summary](#summary)
- [Achievements and advantages](#achievements-and-advantages)
- [Realization progress](#realization-progress)
- [TODO's and ideas](#todos-and-ideas)
- [Examples and quick start](#examples-and-quick-start)
- [Copyright](#copyright)


# Summary
**asncpp** - Полностью объектно-ориентированная библиотека для работы с ASN.1 стандарта. \
Поддерживает все базовые типы данных ASN.1 и позволяет создавать свои типы данных.\
Выгодно отличается от конкурентов своей простотой и удобством применения.\
Имеет возможность как cериализации, так и десериализации. \

# Achievements and advantages
- Простота использования. Для того, чтобы создать объект желаемого типа вам достаточно одной, интуитивно понятной строки кода.
- Полная поддержка всех базовых типов данных ASN.1 стандарта.
- Все типы приводятся и инициализируются в стандартные типы С++. Например, целые числа приводятся к типу int, строки к std::string, контейнерные типы к std::vector объектов и т.д.
- Покрытие тестами всех типов данных и всех возможных исключений.
- Простота интеграции в проект, используется вся мощь CMake для автоматизации сборки и установки.
- Поддержка Doxygen для документации всех классов и методов.
- Поддержка всех стандартов C++17 и выше.


# Examples and quick start
- Документация по всем классам и методам доступна в разделе Doxygen.ССЫЛКА
- Примеры использования библиотеки доступны в разделе Examples.ССЫЛКА

1. скачать
2. установить
3. подключить в проект
4. использовать

<details>
<summary>Целые числа</summary>

```cpp
#include <asncpp/base/common.h>
#include <asncpp/integer.h>

int main() {
    //Прото инициализируется любым целочиленным значением
    asncpp::types::integer_t obj{123456};
    
    //Сериализация объекта. После сериализации объект можно передать по сети или сохранить в файл
    //Возвращает массив байтов, полный пакет данныз в формате ASN.1-DER
    //Содержит {0x02, 0x03, 0x01, 0xE2, 0x40}
    const std::vector<uint8_t> serialized{asncpp::base::serialize(&obj)};
    
    //Десериализация объекта. После десериализации объект можно использовать в программе
    
    //Для получения объекта из массива байтов, необходимо использовать функцию deserialize_v
    //Возвращает указатель на объект типа asn1_basic, с полностью расшифорованным обЪектом
    auto deserialized{ asncpp::base::deserialize_v(encoded)};
    // Проверка на корректность десериализации, если объект не был десериализован, то вернет nullptr
    //const auto* ptr{dynamic_cast<asncpp::types::integer_t*>(deserialized.get())};
    // Печать значения объекта, будет выведено 123456
    std::cout << deserialized.get()->get<asncpp::types::integer_t>()->get_value();
    
    return 0;
}
```

</details>

# TODO's and ideas
### Common:

- [ ] библиотека, вместе с которой идёт модуль Cmake для её использования в других проектах.
  Т.е. скачали исходники библиотеки, добавили к себе через add_subdirectory - и вуаля, у вас появился таргет yourlib::yourlib.
  Который я могу прилинковать к себе, и всё просто заработает. Ключевая фраза для гугла "cmake export".
- [ ] Звать вручную make, make install - это, конечно, дело вкуса... Особенно если только-только из автотулзов перешёл на cmake...
  но если вместо make вы позовёте `cmake --build .` - это чуть больше букв печатать, зато нет привязки к make. cmake сам позовёт нужную тулзу (это может быть ninja, а под виндой вообще msbuild). А вам не нужно помнить детали.
  То же самое про установку. Это всего лишь одна из целей. `cmake --build . --target install`, возможно с sudo (если ставите в системные пути). И ещё это обычно можно делать без предварительной сборки (поскольку эта одна из целей проекта - она сама подтянет сборку перед установкой как зависимость).
- [ ] Поддержка стандартных утилит сборки (make, ninja, msbuild) и установки (make install, ninja install, msbuild install) через cmake.
-  [ ] Использовать glob в cmake для добавления исходников в общем случае не стоит. Один-два раза собрать пойдёт, но лучше в конце концов заменить на явное перечисление файлов. Причину расскажет любой поисковик по фразе "cmake glob evil"
- [ ] Тогда лучше сделать CmakeLists.txt для каждой директории, как вам уже сказали выше. И добавить cmake модули. Это файлы .cmake если что, их потом можно будет включать-выключать при необходимости прямо из ide.

```cmake
option(${PROJECT_NAME}_SANITIZE "Сборка приложения в режиме поиска утечек памяти" OFF)
if (${PROJECT_NAME}_SANITIZE)
include(Sanitize)
endif()
```
- [ ] Add tests for basic class asn1_basic
- [ ] Add Endian-independent class for the integer type
- [ ] Using std::bit_cast
- [ ] Use compile-time functions
- [ ] Documentations for all classes
- [ ] Decoding and encoding make as coroutine
- [ ] Check type of appended child in constructed types
- [ ] Constructor as initializer lists in format like json
- [ ] Add support of std::get analog on sets/sequences/sequence of/sets of/choice/asn1_basic
- [ ] Add support of std::visit analog on sets/sequences/sequence of/sets of/choice/asn1_basic
- [ ] Add tests for all exceptions
- [ ] Realize deprecated VideotexString/GraphicString/GeneralString/T61String
- [ ] Make encode const, and fields mutable. It will allow to change fields after encoding. Skip encoding if data
  encoded and not modified
- [ ] Separate interface for service derived classes encode_children/decode_chidlren
- [ ] Add check in asn1_basic::append_child for constructed types. Is type of child allowed for this type

### Integer type:

- [ ] Add tests for `to_string`
- [ ] Add tests for overflow case
- [ ] Add constructor from string representation
- [ ] Add support of long integers, who has length more then 64 bits.
- [ ] Add support of arithmetic operations

### Boolean type:

- [ ] Add tests for `to_string`
- [ ] Add constructor from string representation
- [ ] Add support of logical operations

### Bit string type:

- [ ] Add tests for `to_string`
- [ ] Add tests for exception cases
- [ ] Add support of comparison operations
- [ ] Add support of bitwise operations
- [ ] Add support of conversations operations
- [ ] Add support of [] operator for children
- [ ] Add support of [] operator for bits
- [ ] Add support of getting full constructed structure
- [ ] Add exception on try to get value of not constructed structure and not constructed bit

### Octet string type:

- [ ] Add tests for `to_string`
- [ ] Add concatenation operator

### OID/Relative OID type:

- [ ] Add tests for `to_string`
- [ ] Add support of [] operator for SID`s

### Time types:

- [ ] Add tests for `to_string`
- [ ] Add support of converting to std::chrono::time_point
- [ ] Add support of converting from std::chrono::time_point
- [ ] Add support of converting to std::time_t
- [ ] Add Default constructor set local time

### Collection types:

- [ ] Add tests for `to_string`
- [ ] Add support of [] operator for children
- [ ] Add support of layout map for sequence for control order of children

# Realization progress

| Type              | Primitive | Constructed | Tests | Doxygen | Additional |
|-------------------|-----------|-------------|-------|---------|------------|
| Boolean           | ✅         | 🔒          | ✅     | ✅       | Released   |
| Integer           | ✅         | 🔒          | ✅     | ✅       | Released   |
| Bit String        | ✅         | ✅           | ✅     | ✅       | Released   |
| Octet String      | ✅         | ✅           | ✅     | ✅       | Released   |
| Object identifier | ✅         | 🔒          | ✅     | ✅       | Released   |
| Object descriptor | ✅         | ✅           | ✅     | ✅       | Released   |
| Enumerated        | ✅         | 🔒          | ✅     | ✅       | Released   |
| UTF-8 string      | ✅         | ✅           | ✅     | ✅       | Released   |
| Relative OID      | ✅         | 🔒          | ✅     | ✅       | Released   |
| Numeric String    | ✅         | ✅           | ✅     | ✅       | Released   |
| Printable String  | ✅         | ✅           | ✅     | ✅       | Released   |
| IA5 String        | ✅         | ✅           | ✅     | ✅       | Released   |
| Visible String    | ✅         | ✅           | ✅     | ✅       | Released   |
| Universal String  | ✅         | ✅           | ✅     | ✅       | Released   |
| BMP String        | ✅         | ✅           | ✅     | ✅       | Released   |
| DATE              | ✅         | 🔒          | ✅     | ✅       | Released   |
| TIME-OF-DAY       | ✅         | 🔒          | ✅     | ❌       | Released   |
| DATE-TIME         | ✅         | 🔒          | ✅     | ❌       | Released   |
| UTC Time          | ✅         | 🔒          | ✅     | ❌       | Released   |
| Generalized Time  | ✅         | 🔒          | ✅     | ❌       | Released   |
| DURATION          | ✅         | 🔒          | ✅     | ❌       | Released   |
| NULL              | ✅         | 🔒          | ❌     | ❌       | Released   |
| Set               | 🔒        | ✅           | ❌     | ❌       | Released   |
| Set of            | 🔒        | ✅           | ❌     | ❌       | Released   |
| Sequence          | 🔒        | ❌           | ❌     | ✅       | Released   |
| Sequence of       | 🔒        | ❌           | ❌     | ✅       | Released   |
| Time              | ❌         | 🔒          | ❌     | ❌       | 🔄         |
| Real              | 🚧        | 🔒          | 🚧    | 🚧      | 🔄         |
| External          | 🔒        | ❌           | ❌     | ❌       | 🔄         |
| Embedded PDV      | 🔒        | ❌           | ❌     | ❌       | 🔄         |
| Character String  | 🔒        | ❌           | ❌     | ❌       | 🔄         |

# Copyright
asncpp is released under the [MIT](https://github.com/ibm3650/asncpp/blob/master/LICENSE).

Copyright © 2025 Kanduba Nikita. All rights reversed.


# Принципы и идеология

## Мои основные принципы
- George Washington
* John Adams
+ Thomas Jefferson

## Установка

### Сборка из исходников

## Clang-Tidy and Clang-Format | Стиль кода и форматирование

## Сотрудничество

## Контакты

## Архитектура

## формат коммитов








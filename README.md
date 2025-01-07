## TODO's and ideas

Common:

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
- [ ] Make encode const, and fields mutable. It will allow to change fields after encoding. Skip encoding if data encoded and not modified

Integer type:

- [ ] Add tests for `to_string`
- [ ] Add tests for overflow case
- [ ] Add constructor from string representation
- [ ] Add support of long integers, who has length more then 64 bits.
- [ ] Add support of arithmetic operations

Boolean type:

- [ ] Add tests for `to_string`
- [ ] Add constructor from string representation
- [ ] Add support of logical operations

Bit string type:

- [ ] Add tests for `to_string`
- [ ] Add tests for exception cases
- [ ] Add support of comparison operations
- [ ] Add support of bitwise operations
- [ ] Add support of conversations operations
- [ ] Add support of [] operator for children
- [ ] Add support of [] operator for bits
- [ ] Add support of getting full constructed structure
- [ ] Add exception on try to get value of not constructed structure and not constructed bit

Octet string type:

- [ ] Add tests for `to_string`
- [ ] Add concatenation operator

OID/Relative OID type:

- [ ] Add tests for `to_string`
- [ ] Add support of [] operator for SID`s

Time types:

- [ ] Add tests for `to_string`
- [ ] Add support of converting to std::chrono::time_point
- [ ] Add support of converting from std::chrono::time_point
- [ ] Add support of converting to std::time_t
- [ ] Add Default constructor set local time

## Realization progress 🤔

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
| NULL              | ✅        | 🔒          | ❌     | ❌       | Released           |
| Time              | ❌         | 🔒          | ❌     | ❌       | 🔄         |
| Real              | 🚧        | 🔒          | 🚧    | 🚧      | 🔄         |
| External          | 🔒        | ❌           | ❌     | ❌       | 🔄         |
| Embedded PDV      | 🔒        | ❌           | ❌     | ❌       | 🔄         |
| Sequence          | 🔒        | ❌           | ❌     | ❌       | 🔄         |
| Sequence of       | 🔒        | ❌           | ❌     | ❌       | 🔄         |
| Set               | 🔒        | ❌           | ❌     | ❌       | 🔄         |
| Set of            | 🔒        | ❌           | ❌     | ❌       | 🔄         |
| Character String  | 🔒        | ❌           | ❌     | ❌       | 🔄         |





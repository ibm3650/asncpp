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
- [ ] Add support of comparsion operations
- [ ] Add support of bitwise operations
- [ ] Add support of conversations operations
- [ ] Add support of [] operator for childs
- [ ] Add support of [] operator for bits
- [ ] Add support of getting full constructed structure
- [ ] Add exception on try to get value of not constructed structure and not constructed bit

Octet string type:

- [ ] Add tests for `to_string`
- [ ] Add concatenation operator

OID/Relative OID type:

- [ ] Add tests for `to_string`
- [ ] Add support of [] operator for SID`s

## Realization progress 🤔

| Type              | Primitive | Constructed | Tests | Doxygen | Additional |
|-------------------|-----------|-------------|-------|---------|------------|
| Boolean           | ✅         | 🔒          | ✅     | ✅       | Released   |
| Integer           | ✅         | 🔒          | ✅     | ✅       | Released   |
| Bit String        | ✅         | ✅           | ✅     | ✅       | Released   |
| Octet String      | ✅         | ✅           | ✅     | ✅       | Released   |
| NULL              | 🚧        | 🔒          | ❌     | ❌       | 🔄         |
| Object identifier | ✅         | 🔒          | ✅     | ✅       | Released   |
| Object descriptor | ✅         | ✅           | ✅     | ✅       | Released   |
| External          | 🔒        | ❌           | ❌     | ❌       | 🔄         |
| Real              | ❌         | 🔒          | ❌     | ❌       | 🔄         |
| Enumerated        | ✅         | 🔒          | ✅     | ✅       | Released   |
| Embedded PDV      | 🔒        | ❌           | ❌     | ❌       | 🔄         |
| UTF-8 string      | 🚧        | ❌           | ❌     | ❌       | 🔄         |
| Relative OID      | ✅         | 🔒          | ✅     | ✅       | Released   |
| Time              | ❌         | 🔒          | ❌     | ❌       | 🔄         |
| Sequence          | 🔒        | ❌           | ❌     | ❌       | 🔄         |
| Sequence of       | 🔒        | ❌           | ❌     | ❌       | 🔄         |
| Set               | 🔒        | ❌           | ❌     | ❌       | 🔄         |
| Set of            | 🔒        | ❌           | ❌     | ❌       | 🔄         |
| Numeric String    | ✅         | ✅           | ✅     | ✅       | Released   |
| Printable String  | ✅         | ✅           | ✅     | ✅       | Released   |
| T61 String        | ❌         | ❌           | ❌     | ❌       | 🔄         |
| Videotex String   | ❌         | ❌           | ❌     | ❌       | 🔄         |
| IA5 String        | ✅         | ✅           | ✅     | ✅       | Released   |
| UTC Time          | 🚧        | ❌           | ❌     | ❌       | 🔄         |
| Generalized Time  | 🚧        | ❌           | ❌     | ❌       | 🔄         |
| Graphic string    | ❌         | ❌           | ❌     | ❌       | 🔄         |
| Visible String    | ✅         | ✅           | ✅     | ✅       | Released   |
| General String    | ❌         | ❌           | ❌     | ❌       | 🔄         |
| Universal String  | ✅         | ✅           | ✅     | ✅       | Released   |
| Character String  | 🔒        | ❌           | ❌     | ❌       | 🔄         |
| BMP String        | ✅         | ✅           | ✅     | ✅       | Released   |
| DATE              | ✅         | 🔒          | ✅     | ✅       | Released   |
| TIME-OF-DAY       | 🚧        | 🔒          | ❌     | ❌       | 🔄         |
| DATE-TIME         | 🚧        | 🔒          | ❌     | ❌       | 🔄         |
| DURATION          | 🚧        | 🔒          | ❌     | ❌       | 🔄         |




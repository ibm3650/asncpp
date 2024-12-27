## TODO's and ideas

Common:

- [ ] Add tests for basic class asn1_basic
- [ ] Add Endian-independent class for the integer type
- [ ] Using std::bit_cast
- [ ] Use compile-time functions
- [ ] Documentations for all classes
- [ ] Decoding and encoding make as coroutine

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

## Realization progress 🤔

| Type              | Primitive | Constructed | Tests | Doxygen | Additional |
|-------------------|-----------|-------------|-------|---------|------------|
| Boolean           | ✅         | 🔒          | ✅     | ✅       | Released   |
| Integer           | ✅         | 🔒          | ✅     | ✅       | Released   |
| Bit String        | ✅         | ✅           | ✅     | ✅       | Released   |
| Octet String      | ✅         | ❌           | 🚧    | ❌       | 🔄         |
| NULL              | 🚧        | 🔒          | ❌     | ❌       | 🔄         |
| Object identifier | ✅         | 🔒          | ✅     | ❌       | 🔄         |
| Object descriptor | 🚧        | ❌           | ❌     | ❌       | 🔄         |
| External          | 🔒        | ❌           | ❌     | ❌       | 🔄         |
| Real              | ❌         | 🔒          | ❌     | ❌       | 🔄         |
| Enumerated        | ✅         | 🔒          | ✅     | ❌       | 🔄         |
| Embedded PDV      | 🔒        | ❌           | ❌     | ❌       | 🔄         |
| UTF-8 string      | 🚧        | ❌           | ❌     | ❌       | 🔄         |
| Relative OID      | ✅         | 🔒          | ✅     | ❌       | 🔄         |
| Time              | ❌         | 🔒          | ❌     | ❌       | 🔄         |
| Sequence          | 🔒        | ❌           | ❌     | ❌       | 🔄         |
| Sequence of       | 🔒        | ❌           | ❌     | ❌       | 🔄         |
| Set               | 🔒        | ❌           | ❌     | ❌       | 🔄         |
| Set of            | 🔒        | ❌           | ❌     | ❌       | 🔄         |
| Numeric String    | ✅         | ❌           | 🚧    | ❌       | 🔄         |
| Printable String  | ✅         | ❌           | 🚧    | ❌       | 🔄         |
| T61 String        | ❌         | ❌           | ❌     | ❌       | 🔄         |
| Videotex String   | ❌         | ❌           | ❌     | ❌       | 🔄         |
| IA5 String        | ✅         | ❌           | ✅     | ❌       | 🔄         |
| UTC Time          | 🚧        | ❌           | ❌     | ❌       | 🔄         |
| Generalized Time  | 🚧        | ❌           | ❌     | ❌       | 🔄         |
| Graphic string    | ❌         | ❌           | ❌     | ❌       | 🔄         |
| Visible String    | ✅         | ❌           | 🚧    | ❌       | 🔄         |
| General String    | ❌         | ❌           | ❌     | ❌       | 🔄         |
| Universal String  | ✅         | ❌           | 🚧    | ❌       | 🔄         |
| Character String  | 🔒        | ❌           | ❌     | ❌       | 🔄         |
| BMP String        | ✅         | ❌           | 🚧    | ❌       | 🔄         |
| DATE              | ✅         | 🔒          | ✅     | ❌       | 🔄         |
| TIME-OF-DAY       | 🚧        | 🔒          | ❌     | ❌       | 🔄         |
| DATE-TIME         | 🚧        | 🔒          | ❌     | ❌       | 🔄         |
| DURATION          | 🚧        | 🔒          | ❌     | ❌       | 🔄         |




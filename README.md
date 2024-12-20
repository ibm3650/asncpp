## TODO's and ideas

Common:
- [ ] Add tests for basic class asn1_basic
- [ ] Add Endian-independent class for the integer type

Integer type:
- [ ] Add tests for `to_string`
- [ ] Add tests for overflow case
- [ ] Add constructor from string representation
- [ ] Add support of long integers, who has length more then 64 bits.
- [ ] Add support of arithmetic operations

## Realization progress 🤔


| Type              | Primitive | Constructed | Tests | Doxygen | Additional         |
|-------------------|-----------|-------------|-------|---------|--------------------|
| EOC               | ✅         | 🔒          | ❌     | ❌       | 🔄                 |
| Boolean           | 🚧        | 🔒          | ❌     | ❌       | 🔄                 |
| Integer           | ✅         | 🔒          | ✅    | ❌       | documentation req. |
| Bit String        | 🚧        | ❌           | ❌     | ❌       | 🔄                 |
| Octet String      | ✅         | ❌           | 🚧    | ❌       | 🔄                 |
| NULL              | 🚧        | 🔒          | ❌     | ❌       | 🔄                 |
| Object identifier | ✅         | 🔒          | ✅     | ❌       | 🔄                 |
| Object descriptor | 🚧        | ❌           | ❌     | ❌       | 🔄                 |
| External          | 🔒        | ❌           | ❌     | ❌       | 🔄                 |
| Real              | ❌         | 🔒          | ❌     | ❌       | 🔄                 |
| Enumerated        | ✅         | 🔒          | ✅     | ❌       | 🔄                 |
| Embedded PDV      | 🔒        | ❌           | ❌     | ❌       | 🔄                 |
| UTF-8 string      | 🚧        | ❌           | ❌     | ❌       | 🔄                 |
| Relative OID      | ✅         | 🔒          | ✅     | ❌       | 🔄                 |
| Time              | ❌         | 🔒          | ❌     | ❌       | 🔄                 |
| Sequence          | 🔒        | ❌           | ❌     | ❌       | 🔄                 |
| Sequence of       | 🔒        | ❌           | ❌     | ❌       | 🔄                 |
| Set               | 🔒        | ❌           | ❌     | ❌       | 🔄                 |
| Set of            | 🔒        | ❌           | ❌     | ❌       | 🔄                 |
| Numeric String    | ✅         | ❌           | 🚧    | ❌       | 🔄                 |
| Printable String  | ✅         | ❌           | 🚧    | ❌       | 🔄                 |
| T61 String        | ❌         | ❌           | ❌     | ❌       | 🔄                 |
| Videotex String   | ❌         | ❌           | ❌     | ❌       | 🔄                 |
| IA5 String        | ✅         | ❌           | ✅     | ❌       | 🔄                 |
| UTC Time          | 🚧        | ❌           | ❌     | ❌       | 🔄                 |
| Generalized Time  | 🚧        | ❌           | ❌     | ❌       | 🔄                 |
| Graphic string    | ❌         | ❌           | ❌     | ❌       | 🔄                 |
| Visible String    | ✅         | ❌           | 🚧    | ❌       | 🔄                 |
| General String    | ❌         | ❌           | ❌     | ❌       | 🔄                 |
| Universal String  | ✅         | ❌           | 🚧    | ❌       | 🔄                 |
| Character String  | 🔒        | ❌           | ❌     | ❌       | 🔄                 |
| BMP String        | ✅         | ❌           | 🚧    | ❌       | 🔄                 |
| DATE              | ✅         | 🔒          | ✅     | ❌       | 🔄                 |
| TIME-OF-DAY       | 🚧        | 🔒          | ❌     | ❌       | 🔄                 |
| DATE-TIME         | 🚧        | 🔒          | ❌     | ❌       | 🔄                 |
| DURATION          | 🚧        | 🔒          | ❌     | ❌       | 🔄                 |




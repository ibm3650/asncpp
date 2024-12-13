//
// Created by kandu on 06.12.2024.
//
#include <gtest/gtest.h>
#include "asncpp/object_identifier.h"

std::vector<std::pair<std::vector<uint32_t>, std::vector<uint8_t> > > oid_test_cases = {
    // Простой OID: 0.0
    {{0, 0}, {0x06, 0x01, 0x00}},

    // Простой OID: 1.0
    {{1, 0}, {0x06, 0x01, 0x28}},

    // Простой OID: 2.0
    {{2, 0}, {0x06, 0x01, 0x50}},

    // Простой OID: 2.2
    {{2, 2}, {0x06, 0x01, 0x52}},

    // Более сложный OID: 1.3.6.1.4.1
    {{1, 3, 6, 1, 4, 1}, {0x06, 0x05, 0x2B, 0x06, 0x01, 0x04, 0x01}},

    // OID с большим значением: 2.319
    {{2, 319}, {0x06, 0x02, 0x83, 0x0F}},

    // OID с большим количеством уровней: 1.3.6.1.4.1.311
    {{1, 3, 6, 1, 4, 1, 311}, {0x06, 0x07, 0x2B, 0x06, 0x01, 0x04, 0x01, 0x82, 0x37}},

    // OID с минимальными значениями: 0.0.0
    {{0, 0, 0}, {0x06, 0x02, 0x00, 0x00}},

    // OID с максимальным значением второго SID для первой области: 2.39
    {{2, 39}, {0x06, 0x01, 0x77}},

    // OID с большим количеством SID: 2.1.128.42.65536
    {{2, 1, 128, 42, 65536}, {0x06, 0x07, 0x51, 0x81, 0x00, 0x2A, 0x84, 0x80, 0x00}},
    //100 0000000 0000000
    // OID с большим значением: 1.2.840.113549
    {{1, 2, 840, 113549}, {0x06, 0x06, 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D}},

    // OID для уникальной идентификации: 1.2.840.10045.4.3.2
    {{1, 2, 840, 10045, 4, 3, 2}, {0x06, 0x08, 0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x04, 0x03, 0x02}}
};


TEST(objext_relative_identifier_test, serialize) {
    for (const auto &test_case: oid_test_cases) {
        object_identifier_t oid(test_case.first);
        const auto encoded = serialize(&oid);
        EXPECT_EQ(encoded, test_case.second);
    }
}


TEST(objext_relative_identifier_test, deserialize) {
    for (const auto &[expected, encoded]: oid_test_cases) {
        auto deserialized = deserialize_v(encoded);
        const object_identifier_t *ptr = dynamic_cast<object_identifier_t *>(deserialized.get());
        EXPECT_EQ(ptr->get_value(), expected);
    }
}


TEST(objext_relative_identifier_test, invalid_first_sid) {
    const auto invalid_oid = std::vector<uint32_t>{3, 0};
    object_identifier_t oid(invalid_oid);
    EXPECT_THROW(serialize(&oid), std::runtime_error);
}

TEST(objext_relative_identifier_test, not_enough_data) {
    const auto invalid_oid = std::vector<uint32_t>{0};
    object_identifier_t oid(invalid_oid);
    EXPECT_THROW(serialize(&oid), std::runtime_error);
}

TEST(objext_relative_identifier_test, invalid_second_sid) {
    const auto invalid_oid = std::vector<uint32_t>{1, 50};
    object_identifier_t oid(invalid_oid);
    EXPECT_THROW(serialize(&oid), std::runtime_error);
}

TEST(objext_relative_identifier_test, empty_oid) {
    const auto invalid_oid = std::vector<uint32_t>{0x06, 0x00};
    object_identifier_t oid(invalid_oid);
    EXPECT_THROW(serialize(&oid), std::runtime_error);
}

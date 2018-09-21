/*!
 * \file Integer.cpp
 * \author ichramm
 *
 * \date 2017-08-17
 */

#include <gtest/gtest.h>

#include <json.hpp>

TEST(JSON_Integer, IntConstructor) {
    Json i(10);
    ASSERT_TRUE(i.is_integer());
    ASSERT_EQ(10, i.to_int());
    ASSERT_STREQ("10", i.stringify().c_str());
}

TEST(JSON_Integer, CopyConstructor) {
    Json i(1024);
    ASSERT_TRUE(i.is_integer());
    ASSERT_EQ(1024, i.to_int());

    Json clone(i);
    ASSERT_EQ(1024, clone.to_int());
}

TEST(JSON_Integer, GetValue) {
    Json  i(10);
    ASSERT_THROW(i.to_bool(), json::bad_json_type);
    ASSERT_NO_THROW(i.to_int());
    ASSERT_NO_THROW(i.to_double());
    ASSERT_THROW(i.to_string(), json::bad_json_type);
    ASSERT_THROW(i.to_array(), json::bad_json_type);
    ASSERT_THROW(i.to_map(), json::bad_json_type);

    ASSERT_NO_THROW(i.to_string(json::option_force));
    ASSERT_EQ("10", i.to_string(json::option_force));
}

TEST(JSON_Integer, AssignmentOperator) {
    Json i(256);
    ASSERT_TRUE(i.is_integer());
    ASSERT_EQ(256, i.to_int());

    Json clone;
    clone = i;
    ASSERT_TRUE(clone.is_integer());
    ASSERT_EQ(256, clone.to_int());

    int16_t s = 256;
    ASSERT_EQ(s, clone.to_intx<int16_t>());

    uint16_t us = 256u;
    ASSERT_EQ(us, clone.to_intx<uint16_t>());
}

TEST(JSON_Integer, IntAssignmentOperator) {
    int i = 2048;
    Json clone;
    clone = i;
    ASSERT_TRUE(clone.is_integer());
    ASSERT_EQ(2048, clone.to_int());
}

TEST(JSON_Integer, EqualOperator) {
    Json i1(256);
    Json i2(256);
    Json i3(512);

    ASSERT_TRUE(i1 == i2);
    ASSERT_FALSE(i1 == i3);
}

TEST(JSON_Integer, UnequalOperator) {
    Json i1(256);
    Json i2(256);
    Json i3(512);

    ASSERT_TRUE(i1 != i3);
    ASSERT_FALSE(i1 != i2);
}

TEST(JSON_Integer, Value) {
    Json i(4096);
    ASSERT_EQ(4096, i.to_int());

    i = Json(8192);
    ASSERT_EQ(8192, i.to_int());
}

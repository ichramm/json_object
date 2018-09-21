/*!
 * \file Bool.cpp
 * \author ichramm
 *
 * \date 2017-08-17
 */

#include <gtest/gtest.h>

#include <json.hpp>

TEST(JSON_Bool, BoolConstructor) {
    Json  b(true);
    ASSERT_TRUE(b.is_boolean());
    ASSERT_TRUE(b.to_bool());
    ASSERT_STREQ("true", b.stringify().c_str());

    Json  c(false);
    ASSERT_TRUE(c.is_boolean());
    ASSERT_FALSE(c.to_bool());
    ASSERT_STREQ("false", c.stringify().c_str());
}

TEST(JSON_Bool, CopyConstructor) {
    Json  b(true);
    ASSERT_TRUE(b.is_boolean());
    ASSERT_TRUE(b.to_bool());

    Json clone(b);
    ASSERT_TRUE(clone.is_boolean());
    ASSERT_TRUE(clone.to_bool());
}

TEST(JSON_Bool, GetValue) {
    Json  b(true);
    ASSERT_NO_THROW(b.to_bool());
    ASSERT_THROW(b.to_int(), json::bad_json_type);
    ASSERT_THROW(b.to_double(), json::bad_json_type);
    ASSERT_THROW(b.to_string(), json::bad_json_type);
    ASSERT_THROW(b.to_array(), json::bad_json_type);
    ASSERT_THROW(b.to_map(), json::bad_json_type);

    ASSERT_NO_THROW(b.to_string(json::option_force));
    ASSERT_EQ("true", b.to_string(json::option_force));
}

TEST(JSON_Bool, AssignmentOperator) {
    Json  b(true);
    ASSERT_TRUE(b.is_boolean());
    ASSERT_TRUE(b.to_bool());

    Json c;
    c = b;
    ASSERT_TRUE(c.is_boolean());
    ASSERT_TRUE(c.to_bool());

    b = Json(false);
    ASSERT_TRUE(b.is_boolean());
    ASSERT_FALSE(b.to_bool());
    ASSERT_EQ("false", b.to_string(json::option_force));
}

TEST(JSON_Bool, EqualOperator) {
    Json b1(true);
    Json b2(true);
    Json b3(false);

    ASSERT_TRUE(b1 == b2);
    ASSERT_FALSE(b1 == b3);
}

TEST(JSON_Bool, UnequalOperator) {
    Json b1(true);
    Json b2(true);
    Json b3(false);

    ASSERT_FALSE(b1 != b2);
    ASSERT_TRUE(b1 != b3);
}

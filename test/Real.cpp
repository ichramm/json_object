#include <string>
#include <sstream>

#include <gtest/gtest.h>

#include <json.hpp>

TEST(JSON_Real, DefaultConstructor) {
    Json r(double(0));
    ASSERT_DOUBLE_EQ(0, r.to_double());
}

TEST(JSON_Real, IntConstructor) {
    Json r(0.675);
    ASSERT_TRUE(r.is_double());
    ASSERT_DOUBLE_EQ(0.675, r.to_double());
    ASSERT_STREQ("0.675", r.stringify().c_str());
}

TEST(JSON_Real, CopyConstructor) {
    Json r(0.8);
    ASSERT_DOUBLE_EQ(0.8, r.to_double());

    Json clone(r);
    ASSERT_TRUE(clone.is_double());
    ASSERT_DOUBLE_EQ(0.8, clone.to_double());
}

TEST(JSON_Real, GetValue) {
    Json  r(0.8);
    ASSERT_THROW(r.to_bool(), json::bad_json_type);
    ASSERT_THROW(r.to_int(), json::bad_json_type);
    ASSERT_NO_THROW(r.to_double());
    ASSERT_THROW(r.to_string(), json::bad_json_type);
    ASSERT_THROW(r.to_array(), json::bad_json_type);
    ASSERT_THROW(r.to_map(), json::bad_json_type);

    ASSERT_NO_THROW(r.to_string(json::option_force));
    ASSERT_EQ("0.8", r.to_string(json::option_force));
}

TEST(JSON_Real, AssignmentOperator) {
    Json r(1.75);
    ASSERT_DOUBLE_EQ(1.75, r.to_double());

    Json clone;
    clone = r;
    ASSERT_TRUE(clone.is_double());
    ASSERT_DOUBLE_EQ(1.75, clone.to_double());
}

TEST(JSON_Real, DoubleAssignmentOperator) {
    double r = 0.83;
    ASSERT_DOUBLE_EQ(0.83, r);

    Json clone;
    clone = Json(r); // cannot assign to real unless in the constructor
    ASSERT_DOUBLE_EQ(0.83, clone.to_double());
}

TEST(JSON_Real, ImplicitConstructor) {
    Json r = 0.675;
    ASSERT_TRUE(r.is_double());
    ASSERT_DOUBLE_EQ(0.675, r.to_double());
}

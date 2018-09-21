/*!
 * \file Array.cpp
 * \author ichramm
 *
 * \date 2017-08-17
 */

#include <gtest/gtest.h>

#include <json.hpp>

TEST(JSON_Array, Empty) {
    Json array = Json(Json::array_type());
    ASSERT_TRUE(array.is_array());
    ASSERT_TRUE(array.empty());
    ASSERT_EQ(0, array.size());
}

TEST(JSON_Array, GetValue) {
    Json array = Json(Json::array_type());
    ASSERT_TRUE(array.is_array());
    ASSERT_TRUE(array.empty());
    ASSERT_EQ(0, array.size());

    ASSERT_THROW(array.to_bool(), json::bad_json_type);
    ASSERT_THROW(array.to_int(), json::bad_json_type);
    ASSERT_THROW(array.to_double(), json::bad_json_type);
    ASSERT_THROW(array.to_string(), json::bad_json_type);
    ASSERT_THROW(array.to_string(json::option_force), json::bad_json_type);
    ASSERT_NO_THROW(array.to_array());
    ASSERT_NO_THROW(array.to_map());
    array.push_back(Json(0));
    ASSERT_THROW(array.to_map(), json::bad_json_type);
}

TEST(JSON_Array, HomogeneousContent) {
    Json array = Json(Json::array_type());
    ASSERT_TRUE(array.is_array());

    array.push_back(Json(0));
    array.push_back(Json(1));
    array.push_back(Json(2));

    ASSERT_FALSE(array.empty());
    ASSERT_EQ(3, array.size());

    ASSERT_STREQ("[0,1,2]", array.stringify().c_str());
}

TEST(JSON_Array, BuildNested) {
    Json root;
    ASSERT_NO_THROW(root["array"].push_back(1));
    ASSERT_TRUE(root["array"].is_array());
    ASSERT_NO_THROW(root["array"].push_back(2));
}

TEST(JSON_Array, HeterogeneousContent) {
    Json array = Json(Json::array_type());
    ASSERT_TRUE(array.is_array());

    array.push_back(Json(true));
    array.push_back(Json(1));
    array.push_back(Json(2.5));
    array.push_back(Json("String"));
    array.push_back(Json::Null);

    ASSERT_FALSE(array.empty());
    ASSERT_EQ(5, array.size());

    ASSERT_STREQ("[true,1,2.5,\"String\",null]",  array.stringify().c_str());
}

TEST(JSON_Array, ExtractValues) {
    Json array = Json(Json::array_type());
    ASSERT_TRUE(array.is_array());

    array.push_back(Json(true));
    array.push_back(Json(1));
    array.push_back(Json(2.5));
    array.push_back(Json("String"));

    ASSERT_FALSE(array.empty());
    ASSERT_EQ(4, array.size());

    Json val = array.at(0);
    ASSERT_TRUE(val.is_boolean());
    ASSERT_TRUE(val.to_bool());
    val = array.at(1);
    ASSERT_TRUE(val.is_integer());
    ASSERT_EQ(1, val.to_int());
    val = array.at(2);
    ASSERT_TRUE(val.is_double());
    ASSERT_EQ(2.5, val.to_double());
    val = array.at(3);
    ASSERT_TRUE(val.is_string());
    ASSERT_EQ("String", val.to_string());
}

TEST(JSON_Array, CopyConstructor) {
    Json array = Json(Json::array_type());
    ASSERT_TRUE(array.is_array());

    array.push_back(Json(true));
    array.push_back(Json(1));
    array.push_back(Json(2.5));
    array.push_back(Json("String"));

    ASSERT_FALSE(array.empty());
    ASSERT_EQ(4, array.size());

    Json clone(array);

    ASSERT_TRUE(clone.is_array());
    ASSERT_FALSE(clone.empty());
    ASSERT_EQ(4, clone.size());

    Json val = array.at(0);
    ASSERT_TRUE(val.is_boolean());
    ASSERT_TRUE(val.to_bool());
    val = array.at(1);
    ASSERT_TRUE(val.is_integer());
    ASSERT_EQ(1, val.to_int());
    val = array.at(2);
    ASSERT_TRUE(val.is_double());
    ASSERT_EQ(2.5, val.to_double());
    val = array.at(3);
    ASSERT_TRUE(val.is_string());
    ASSERT_EQ("String", val.to_string());
}

TEST(JSON_Array, VectorConstructor) {
    Json array = Json(Json::array_type());
    ASSERT_TRUE(array.is_array());

    array.push_back(Json(true));
    array.push_back(Json(1));
    array.push_back(Json(2.5));
    array.push_back(Json("String"));

    ASSERT_FALSE(array.empty());
    ASSERT_EQ(4, array.size());

    Json clone(array.to_array());

    ASSERT_TRUE(clone.is_array());
    ASSERT_FALSE(clone.empty());
    ASSERT_EQ(4, clone.size());

    Json val = array.at(0);
    ASSERT_TRUE(val.is_boolean());
    ASSERT_TRUE(val.to_bool());
    val = array.at(1);
    ASSERT_TRUE(val.is_integer());
    ASSERT_EQ(1, val.to_int());
    val = array.at(2);
    ASSERT_TRUE(val.is_double());
    ASSERT_EQ(2.5, val.to_double());
    val = array.at(3);
    ASSERT_TRUE(val.is_string());
    ASSERT_EQ("String", val.to_string());
}


TEST(JSON_Array, AssignmentOperator) {
    Json array = Json(Json::array_type());
    ASSERT_TRUE(array.is_array());

    array.push_back(Json(true));
    array.push_back(Json(1));
    array.push_back(Json(2.5));
    array.push_back(Json("String"));

    ASSERT_FALSE(array.empty());
    ASSERT_EQ(4, array.size());

    Json clone;
    clone = array;

    ASSERT_TRUE(clone.is_array());
    ASSERT_FALSE(clone.empty());
    ASSERT_EQ(4, clone.size());

    Json val = array.at(0);
    ASSERT_TRUE(val.is_boolean());
    ASSERT_TRUE(val.to_bool());
    val = array.at(1);
    ASSERT_TRUE(val.is_integer());
    ASSERT_EQ(1, val.to_int());
    val = array.at(2);
    ASSERT_TRUE(val.is_double());
    ASSERT_EQ(2.5, val.to_double());
    val = array.at(3);
    ASSERT_TRUE(val.is_string());
    ASSERT_EQ("String", val.to_string());
}

TEST(JSON_Array, VectorAssignmentOperator) {
    Json array = Json(Json::array_type());
    ASSERT_TRUE(array.is_array());

    array.push_back(Json(true));
    array.push_back(Json(1));
    array.push_back(Json(2.5));
    array.push_back(Json("String"));

    ASSERT_FALSE(array.empty());
    ASSERT_EQ(4, array.size());

    Json clone;
    clone = array.to_array();

    ASSERT_TRUE(clone.is_array());
    ASSERT_FALSE(clone.empty());
    ASSERT_EQ(4, clone.size());

    Json val = array.at(0);
    ASSERT_TRUE(val.is_boolean());
    ASSERT_TRUE(val.to_bool());
    val = array.at(1);
    ASSERT_TRUE(val.is_integer());
    ASSERT_EQ(1, val.to_int());
    val = array.at(2);
    ASSERT_TRUE(val.is_double());
    ASSERT_EQ(2.5, val.to_double());
    val = array.at(3);
    ASSERT_TRUE(val.is_string());
    ASSERT_EQ("String", val.to_string());
}

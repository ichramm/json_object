/*!
 * \file Object.cpp
 * \author ichramm
 *
 * \date 2017-08-18
 */

#include <gtest/gtest.h>

#include <json.hpp>

TEST(JSON_Object, Empty) {
    Json obj;
    ASSERT_TRUE(obj.is_object());
    ASSERT_TRUE(obj.empty());
    ASSERT_EQ(0, obj.size());
}

TEST(JSON_Object, GetValue) {
    Json obj;
    ASSERT_THROW(obj.to_bool(), json::bad_json_type);
    ASSERT_THROW(obj.to_int(), json::bad_json_type);
    ASSERT_THROW(obj.to_double(), json::bad_json_type);
    ASSERT_THROW(obj.to_string(), json::bad_json_type);
    ASSERT_THROW(obj.to_string(json::option_force), json::bad_json_type);
    ASSERT_NO_THROW(obj.to_array());
    ASSERT_NO_THROW(obj.to_map());
    obj["hello"] = "world";
    ASSERT_THROW(obj.to_array(), json::bad_json_type);
}

TEST(JSON_Object, HomogeneousContent) {
    Json obj;
    ASSERT_TRUE(obj.is_object());

    obj["1"] = Json(0);
    obj["2"] = Json(1);
    obj["3"] = Json(2);

    ASSERT_FALSE(obj.empty());
    ASSERT_EQ(3, obj.size());

    ASSERT_STREQ("{\"1\":0,\"2\":1,\"3\":2}", obj.stringify().c_str());
}

TEST(JSON_Object, HeterogeneousContent) {
    Json obj;

    obj["1"] = Json(true);
    obj["2"] = Json(1);
    obj["3"] = Json(2.5);
    obj["4"] = Json("String");
    obj["5"] = Json::Null;

    ASSERT_FALSE(obj.empty());
    ASSERT_EQ(5, obj.size());

    ASSERT_STREQ("{\"1\":true,\"2\":1,\"3\":2.5,\"4\":\"String\",\"5\":null}", obj.stringify().c_str());
}

TEST(JSON_Object, ExtractValues) {
    Json obj;

    obj["1"] = Json(true);
    obj["2"] = Json(1);
    obj["3"] = Json(2.5);
    obj["4"] = Json("String");

    ASSERT_FALSE(obj.empty());
    ASSERT_EQ(4, obj.size());

    Json val_bool = obj["1"];
    ASSERT_TRUE(val_bool.is_boolean());
    ASSERT_TRUE(val_bool.to_bool());

    Json val_int = obj["2"];
    ASSERT_TRUE(val_int.is_integer());
    ASSERT_EQ(1, val_int.to_int());

    Json val_real = obj["3"];
    ASSERT_TRUE(val_real.is_double());
    ASSERT_EQ(2.5, val_real.to_double());

    Json val_str = obj["4"];
    ASSERT_TRUE(val_str.is_string());
    ASSERT_STREQ("String", val_str.to_string().c_str());
}

TEST(JSON_Object, InnerImplicitConstructors) {
    Json obj;

    obj["1"] = true;
    obj["2"] = 1;
    obj["3"] = 2.5;
    obj["4"] = "String";

    ASSERT_FALSE(obj.empty());
    ASSERT_EQ(4, obj.size());

    Json val_bool = obj["1"];
    ASSERT_TRUE(val_bool.is_boolean());
    ASSERT_TRUE(val_bool.to_bool());

    Json val_int = obj["2"];
    ASSERT_TRUE(val_int.is_integer());
    ASSERT_EQ(1, val_int.to_int());

    Json val_real = obj["3"];
    ASSERT_TRUE(val_real.is_double());
    ASSERT_EQ(2.5, val_real.to_double());

    Json val_str = obj["4"];
    ASSERT_TRUE(val_str.is_string());
    ASSERT_STREQ("String", val_str.to_string().c_str());
}

TEST(JSON_Object, MapConstructor) {
    std::map<std::string, Json> map;

    map["1"] = Json(true);
    map["2"] = Json(1);
    map["3"] = Json(2.5);
    map["4"] = Json("String");

    ASSERT_FALSE(map.empty());
    ASSERT_EQ(4, map.size());

    Json obj(map);

    ASSERT_FALSE(obj.empty());
    ASSERT_EQ(4, obj.size());

    Json val_bool = obj["1"];
    ASSERT_TRUE(val_bool.is_boolean());
    ASSERT_TRUE(val_bool.to_bool());

    Json val_int = obj["2"];
    ASSERT_TRUE(val_int.is_integer());
    ASSERT_EQ(1, val_int.to_int());

    Json val_real = obj["3"];
    ASSERT_TRUE(val_real.is_double());
    ASSERT_EQ(2.5, val_real.to_double());

    Json val_str = obj["4"];
    ASSERT_TRUE(val_str.is_string());
    ASSERT_STREQ("String", val_str.to_string().c_str());
}

TEST(JSON_Object, CopyConstructor) {
    Json obj;

    obj["1"] = Json(true);
    obj["2"] = Json(1);
    obj["3"] = Json(2.5);
    obj["4"] = Json("String");

    ASSERT_FALSE(obj.empty());
    ASSERT_EQ(4, obj.size());

    Json clone(obj);

    ASSERT_FALSE(clone.empty());
    ASSERT_EQ(4, clone.size());

    Json val_bool = clone["1"];
    ASSERT_TRUE(val_bool.is_boolean());
    ASSERT_TRUE(val_bool.to_bool());

    Json val_int = clone["2"];
    ASSERT_TRUE(val_int.is_integer());
    ASSERT_EQ(1, val_int.to_int());

    Json val_real = clone["3"];
    ASSERT_TRUE(val_real.is_double());
    ASSERT_EQ(2.5, val_real.to_double());

    Json val_str = clone["4"];
    ASSERT_TRUE(val_str.is_string());
    ASSERT_STREQ("String", val_str.to_string().c_str());
}

TEST(JSON_Object, AssignmentOperator) {
    Json obj;

    obj["1"] = Json(true);
    obj["2"] = Json(1);
    obj["3"] = Json(2.5);
    obj["4"] = Json("String");

    ASSERT_FALSE(obj.empty());
    ASSERT_EQ(4, obj.size());

    Json clone;
    ASSERT_TRUE(clone.empty());
    clone = obj;

    ASSERT_FALSE(clone.empty());
    ASSERT_EQ(4, clone.size());

    Json val_bool = clone["1"];
    ASSERT_TRUE(val_bool.is_boolean());
    ASSERT_TRUE(val_bool.to_bool());

    Json val_int = clone["2"];
    ASSERT_TRUE(val_int.is_integer());
    ASSERT_EQ(1, val_int.to_int());

    Json val_real = clone["3"];
    ASSERT_TRUE(val_real.is_double());
    ASSERT_EQ(2.5, val_real.to_double());

    Json val_str = clone["4"];
    ASSERT_TRUE(val_str.is_string());
    ASSERT_STREQ("String", val_str.to_string().c_str());
}

TEST(JSON_Object, KeyNotFound) {
    Json obj;

    obj["1"] = Json(true);
    obj["2"] = Json(1);
    obj["3"] = Json(2.5);
    obj["4"] = Json("String");

    ASSERT_FALSE(obj.empty());
    ASSERT_EQ(4, obj.size());

    ASSERT_TRUE(obj["1"].to_bool());
    ASSERT_EQ(1, obj["2"].to_int());
    ASSERT_DOUBLE_EQ(2.5, obj["3"].to_double());
    ASSERT_STREQ("String", obj["4"].to_string().c_str());

    ASSERT_THROW(obj["one"].type(), json::key_not_found);
    ASSERT_THROW(obj["one"].to_bool(), json::key_not_found);
    ASSERT_THROW(obj["one"].to_int(), json::key_not_found);
    ASSERT_THROW(obj["one"].to_double(), json::key_not_found);
    ASSERT_THROW(obj["one"].to_string(), json::key_not_found);
    ASSERT_THROW(obj["one"].to_array(), json::key_not_found);
    ASSERT_THROW(obj["one"].to_map(), json::key_not_found);
    obj["one"] = true;
    ASSERT_TRUE(obj["one"].to_bool());
}

TEST(JSON_Object, GetOptional) {
    Json obj;
    obj["abool"] = Json(true);
    obj["anint"] = Json(3);
    obj["afloat"] = Json(2.5);
    obj["astring"] = Json("String");
    obj["anull"] = Json::Null;

    ASSERT_TRUE(obj.get_optional<bool>("abool"));
    ASSERT_FALSE(obj.get_optional<bool>("notabool"));
    ASSERT_TRUE(obj.get_optional<bool>("notabool", true));

    ASSERT_EQ(3, obj.get_optional<int16_t>("anint"));
    ASSERT_EQ(3, obj.get_optional<uint16_t>("anint"));
    ASSERT_EQ(3, obj.get_optional<int32_t>("anint"));
    ASSERT_EQ(3, obj.get_optional<uint32_t>("anint"));
    ASSERT_EQ(3, obj.get_optional<int64_t>("anint"));
    ASSERT_EQ(3, obj.get_optional<uint64_t>("anint"));
    ASSERT_EQ(0, obj.get_optional<int16_t>("noint"));
    ASSERT_EQ(0, obj.get_optional<uint16_t>("noint"));
    ASSERT_EQ(0, obj.get_optional<int32_t>("noint"));
    ASSERT_EQ(0, obj.get_optional<uint32_t>("noint"));
    ASSERT_EQ(0, obj.get_optional<int64_t>("noint"));
    ASSERT_EQ(0, obj.get_optional<uint64_t>("noint"));
    ASSERT_EQ(20, obj.get_optional<int16_t>("noint", 20));
    ASSERT_EQ(20, obj.get_optional<uint16_t>("noint", 20));
    ASSERT_EQ(20, obj.get_optional<int32_t>("noint", 20));
    ASSERT_EQ(20, obj.get_optional<uint32_t>("noint", 20));
    ASSERT_EQ(20, obj.get_optional<int64_t>("noint", 20));
    ASSERT_EQ(20, obj.get_optional<uint64_t>("noint", 20));

    ASSERT_EQ(2.5, obj.get_optional<float>("afloat"));
    ASSERT_EQ(2.5, obj.get_optional<double>("afloat"));
    ASSERT_EQ(0.0, obj.get_optional<float>("notafloat"));
    ASSERT_EQ(0.0, obj.get_optional<double>("notafloat"));
    ASSERT_EQ(20.0, obj.get_optional<float>("notafloat", 20.0));
    ASSERT_EQ(20.0, obj.get_optional<double>("notafloat", 20.0));

    ASSERT_STREQ("String", obj.get_optional<const char *>("astring"));
    ASSERT_EQ("String", obj.get_optional<std::string>("astring"));
    ASSERT_EQ(NULL, obj.get_optional<const char *>("notastring"));
    ASSERT_EQ("", obj.get_optional<std::string>("notastring"));
    ASSERT_STREQ("Other String", obj.get_optional<const char *>("notastring", "Other String"));
    ASSERT_EQ("Other String", obj.get_optional<std::string>("notastring", "Other String"));

    ASSERT_TRUE(obj.get_optional<bool>("anull", true));
    ASSERT_FALSE(obj.get_optional<bool>("anull", false));
    ASSERT_EQ(20, obj.get_optional<int64_t>("anull", 20));
    ASSERT_EQ(20.0, obj.get_optional<float>("anull", 20.0));
    ASSERT_STREQ("A Null", obj.get_optional<const char*>("anull", "A Null"));
    ASSERT_EQ("A Null", obj.get_optional<std::string>("anull", "A Null"));

    ASSERT_THROW(obj.get_optional<int64_t>("abool"), json::bad_json_type);
    ASSERT_THROW(obj.get_optional<float>("abool"), json::bad_json_type);
    ASSERT_THROW(obj.get_optional<double>("abool"), json::bad_json_type);
    ASSERT_THROW(obj.get_optional<const char *>("abool"), json::bad_json_type);
    ASSERT_THROW(obj.get_optional<std::string>("abool"), json::bad_json_type);

    ASSERT_THROW(obj.get_optional<bool>("anint"), json::bad_json_type);
    // Note: Integer gets promoted to double so no exception would be thrown here
    ASSERT_THROW(obj.get_optional<const char *>("anint"), json::bad_json_type);
    ASSERT_THROW(obj.get_optional<std::string>("anint"), json::bad_json_type);

    ASSERT_THROW(obj.get_optional<bool>("afloat"), json::bad_json_type);
    ASSERT_THROW(obj.get_optional<int64_t>("afloat"), json::bad_json_type);
    ASSERT_THROW(obj.get_optional<const char *>("afloat"), json::bad_json_type);
    ASSERT_THROW(obj.get_optional<std::string>("afloat"), json::bad_json_type);

    ASSERT_THROW(obj.get_optional<bool>("astring"), json::bad_json_type);
    ASSERT_THROW(obj.get_optional<int64_t>("astring"), json::bad_json_type);
    ASSERT_THROW(obj.get_optional<float>("astring"), json::bad_json_type);
    ASSERT_THROW(obj.get_optional<double>("astring"), json::bad_json_type);
}
